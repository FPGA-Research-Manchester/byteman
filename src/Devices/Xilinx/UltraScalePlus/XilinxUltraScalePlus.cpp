/******************************************************************************
 * Copyright 2022 Kristiyan Manev (University of Manchester)
 *
 * Licensed under the Apache License, Version 2.0(the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

#include<iostream>
#include<stdio.h>
#include<string>
#include<cstring>  //memset
#include<algorithm> //replace
#include<sstream>
#include<stdexcept>
#include<fstream>

#include "XilinxUltraScalePlus.h"

using namespace std;

XilinxUltraScalePlus::XilinxUltraScalePlus()
{
    initializedBitstreamPartName = "";
	loadedBitstreamEndianess = Endianess::BE;
    SLRinfo[0] = {0, 0, 0};
    numberOfRows = 0;
	blankFrame = calloc(XUSP_WORDS_PER_FRAME, 4);
}

XilinxUltraScalePlus::~XilinxUltraScalePlus()
{
    //dtor
	free(blankFrame);
	if(bitstreamBegin != nullptr)
		delete bitstreamBegin;
}

/**************************************************************************//**
 * Parses argument options for multiple operations.
 * @arg @c params A list of parameters to control behavior:
 * - "clock" or "clk" : Selects the clocks for operation.
 * - "logic" or "clb" : Selects the blocktype 0 for operation.
 * - "blockram" or "bram" : Selects the blockram contents for operation.
 * - If neither clock, logic, or blockram are chosen, chooses all.
 * - "blank" : Selects bitstream blanking (zeroing) operation.
 * - "full" : Disabling partial selection.
 * - "partial" : Writes only selected regions. Selected by default.
 * - "set" : Selects SET operation.
 * - "xor" : Selects bitwise XOR operation.
 * - "or" : Selects bitwise OR operation.
 * - "and" : Selects bitwise AND operation.
 *****************************************************************************/
XilinxUltraScalePlus::SelectedOptions XilinxUltraScalePlus::parseParams(string params){
	SelectedOptions options = SelectedOptions();
	replace(params.begin(), params.end(), ',', ' ');
	stringstream ss(params);
	string param;
	while (!ss.eof()) {
		ss >> param;
		if(param == "clock" || param == "clk")options.clk++;
		if(param == "logic" || param == "clb")options.clb++;
		if(param == "blockram" || param == "bram")options.bram++;
		if(param == "blank")options.blank++;
		if(param == "full")options.partial = 0;
		if(param == "partial")options.partial = 1;
		if(param == "set")options.op = MergeOP::SET;
		if(param == "xor")options.op = MergeOP::XOR;
		if(param == "or")options.op = MergeOP::OR;
		if(param == "and")options.op = MergeOP::AND;
		param.clear();
	}
	if(options.clk == 0 && options.clb == 0 && options.bram == 0){ // by default, choose all
		options.clk = options.bram = options.clb = 1;
	}
	return options;
}

void XilinxUltraScalePlus::initializeResourceStringParameters(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
    if(initializedResourceParamsPartName != partName){//The device is changed
		initializedResourceParamsPartName = partName;

        for((numberOfCols = 0, numberOfFramesBeforeCol[0] = 0, numberOfBRAMCols = 0) ; (uint8_t)resourceString[0][numberOfCols] ; numberOfCols++){
            numberOfFramesBeforeCol[numberOfCols+1] = numberOfFramesBeforeCol[numberOfCols] + numberOfFramesPerResourceLetter[(uint8_t)resourceString[0][numberOfCols]];
            numberOfBRAMsBeforeCol[numberOfCols] = numberOfBRAMCols;
            if(resourceString[0][numberOfCols] == 'A' || resourceString[0][numberOfCols] == 'B' || resourceString[0][numberOfCols] == 'C' || resourceString[0][numberOfCols] == '1') //ABC1 are "BRAM_L", "BRAM", "BRAM_R", "NULL BRAM"
                numberOfBRAMCols++;
        }
        numberOfFramesPerRow = numberOfFramesBeforeCol[numberOfCols] + XUSP_EXTRA_FRAMES_PER_ROW;
        numberOfWordsPerRow = numberOfFramesPerRow * XUSP_WORDS_PER_FRAME;
	}
}

void XilinxUltraScalePlus::ensureInitializedBitstreamArrays(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
    if(initializedBitstreamPartName != partName){//The device is changed
		initializedBitstreamPartName = partName;
        if(bitstreamBegin != nullptr)
            delete bitstreamBegin;

        XilinxUltraScalePlus::initializeResourceStringParameters();
		//Allocate
        bitstreamBegin = new uint32_t[numberOfRows * (numberOfWordsPerRow + numberOfBRAMCols * (XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN+XUSP_EXTRA_FRAMES_PER_ROW) * XUSP_WORDS_PER_FRAME)];

        //Calc pointers
        int offset = 0;
        for(int r = 0 ; r < numberOfRows ; r++){
            for(int c = 0 ; c < numberOfCols ; c++){
                bitstreamCLB[r][c] = &bitstreamBegin[offset];
                offset += numberOfFramesPerResourceLetter[(uint8_t)resourceString[r][c]] * XUSP_WORDS_PER_FRAME;
            }
			offset += XUSP_EXTRA_FRAMES_PER_ROW * XUSP_WORDS_PER_FRAME;
		}

        for(int r = 0 ; r < numberOfRows ; r++){
            for(int c = 0 ; c < numberOfBRAMCols ; c++){
                bitstreamBRAM[r][c] = &bitstreamBegin[offset];
                offset += XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN*XUSP_WORDS_PER_FRAME;
            }
			offset += XUSP_EXTRA_FRAMES_PER_ROW * XUSP_WORDS_PER_FRAME;
		}
		bitstreamEnd = &bitstreamBegin[offset];
    }
}
void XilinxUltraScalePlus::blank(string params){
	XilinxUltraScalePlus::ensureInitializedBitstreamArrays();
	int blankCLB = 0, blankBRAM = 0;
    if(params.find("logic") != string::npos || params.find("clb") != string::npos)blankCLB++;
    if(params.find("blockram") != string::npos || params.find("bram") != string::npos)blankBRAM++;
	if(regionSelection.empty()) {
		if(blankCLB)
			memset(&bitstreamCLB[0][0][0], 0, (&bitstreamBRAM[0][0][0]-&bitstreamCLB[0][0][0]));
		if(blankBRAM)
			memset(&bitstreamBRAM[0][0][0], 0, (bitstreamEnd-&bitstreamBRAM[0][0][0]));
	} else {
		if(blankCLB)
			for (Rect2D &rect : regionSelection) {
				for(int r = rect.position.row ; r < (rect.position.row + rect.size.row) ; r++){
					int blankSize = numberOfFramesBeforeCol[rect.position.col + rect.size.col] - numberOfFramesBeforeCol[rect.position.col];
					blankSize *= XUSP_WORDS_PER_FRAME * 4;
					memset(&bitstreamCLB[r][rect.position.col][0], 0, blankSize); //memset with size 0 is safe, no need to check anything
				}
			}
		if(blankBRAM)
			for (Rect2D &rect : regionSelection) {
				for(int r = rect.position.row ; r < (rect.position.row + rect.size.row) ; r++){
					int blankSize = numberOfBRAMsBeforeCol[rect.position.col + rect.size.col] - numberOfBRAMsBeforeCol[rect.position.col];
					blankSize *= XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * XUSP_WORDS_PER_FRAME * 4;
					memset(&bitstreamCLB[r][numberOfBRAMsBeforeCol[rect.position.col]][0], 0, blankSize); //memset with size 0 is safe, no need to check anything
				}
			}
	}
}
void XilinxUltraScalePlus::ensureRowCompatibility(Coord2D src, int offsetRow, int sizeCol, Coord2D dst){
	int srcRA = (src.row+offsetRow) / XUSP_CLB_PER_CLOCK_REGION;
	int dstRA = (dst.row+offsetRow) / XUSP_CLB_PER_CLOCK_REGION;
	for(int c = 0 ; c < sizeCol ; c++){
		if(resourceString[srcRA][src.col+c] != resourceString[dstRA][dst.col+c]){//if any two letters are different
			if(numberOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA][src.col+c]] != numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA][dst.col+c]]){
				//also the number of frames is different?!
				throw runtime_error(string("Tried to relocate to an incompatible location(").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA][src.col+c]]).append(" to frame type ").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA][dst.col+c]]).append(")."));
			} else {
				//okay, different col, but same width. Throw warning?
				warn("Relocating from frame type " + string(typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA][src.col+c]]) + " to frame type " + string(typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA][dst.col+c]]) + ".");
			}
		}
	}
}
void XilinxUltraScalePlus::ensureRegionCompatibility(Rect2D src, Coord2D dst){
    for(int r = 0 ; r < src.size.row ; ){ //this loop will iterate south border and all (srcRow:srcRow+sizeRow) at CLBs 0 and 59
		XilinxUltraScalePlus::ensureRowCompatibility(src.position, r, src.size.col, dst);
		if( ((src.position.row + r) % XUSP_CLB_PER_CLOCK_REGION) == (XUSP_CLB_PER_CLOCK_REGION-1) ) { // isLastCLBinRow
			r++;
		} else if( ((src.position.row + r) % XUSP_CLB_PER_CLOCK_REGION) == 0 ) { // isFirstCLBinRow
			r += (XUSP_CLB_PER_CLOCK_REGION - 2);
		} else { // get to the next {isLastCLBinRow} 
			r += (XUSP_CLB_PER_CLOCK_REGION - 1 - ((src.position.row + r) % XUSP_CLB_PER_CLOCK_REGION));
		}
    }
	ensureRowCompatibility(src.position, src.size.row-1, src.size.col, dst); //iterate north border as well
}
void XilinxUltraScalePlus::merge(XilinxUltraScalePlus* srcBitstream, string params, Rect2D src, Coord2D dst){
    //This function uses a pointer to the second XUSP instance, but this pointer should get optimized as it is statically provided in the main function
	SelectedOptions options = XilinxUltraScalePlus::parseParams(params);
	
	XilinxUltraScalePlus::ensureRegionCompatibility(src, dst);
    if(src.position.row%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
    if(src.size.row%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
    if(dst.row%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
	
	Endianess endianDifference = Endian::diff(loadedBitstreamEndianess, srcBitstream->loadedBitstreamEndianess);
	
	if((Endianess::NATIVE == endianDifference) && MergeOP::SET == options.op)
		fastMerge(srcBitstream, options, src, dst);
	else
		flexiMerge(srcBitstream, options, endianDifference, src, dst);
}

/**************************************************************************//**
 * If endianesses of the two bitstreams match and you want to do a relocate
 * operation, fastMerge is the way to go. 
 *****************************************************************************/
void XilinxUltraScalePlus::fastMerge(XilinxUltraScalePlus* srcBitstream, SelectedOptions options, Rect2D src, Coord2D dst){
    int srcRA = src.position.row  / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = src.size.row / XUSP_CLB_PER_CLOCK_REGION;
    int dstRA = dst.row  / XUSP_CLB_PER_CLOCK_REGION;

    for(int r = 0 ; r < sizeR ; r++){
        for(int c = 0 ; c < src.size.col ; c++){//relocate dst[dstRA+r][dst.col+c] <= src[srcRA+r][src.position.col+c]
            for(int m = 0 ; m < numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA+r][dst.col+c]] ; m++){
                if(options.clb && options.clk){
                    memcpy((char*)&bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME] ,(char*)&srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME], XUSP_WORDS_PER_FRAME*4);
                } else {
                    if(options.clb){
                        memcpy((char*)&bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME] ,                                        (char*)&srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME],                                         XUSP_WORDS_BEFORE_CLK*4);
                        memcpy((char*)&bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME+XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK] ,(char*)&srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME+XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK], XUSP_WORDS_AFTER_CLK*4);
					}//-if(reloCLB)
                    if(options.clk){
                        for(int w = XUSP_WORDS_BEFORE_CLK ; w < (XUSP_WORDS_BEFORE_CLK + XUSP_WORDS_AT_CLK) ; w++)
                            bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME + w] = srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME + w];
                    }//-if(reloCLK)
                }//-if(reloCLB && reloCLK)
            }
        }
    }
	if(options.bram){
		for(int r = 0 ; r < sizeR ; r++){
			int bramCols = numberOfBRAMsBeforeCol[src.position.col + src.size.col] - numberOfBRAMsBeforeCol[src.position.col];
			int srcCol = numberOfBRAMsBeforeCol[src.position.col];
			int dstCol = numberOfBRAMsBeforeCol[dst.col];
			memcpy((char*)&bitstreamBRAM[dstRA+r][dstCol][0] ,(char*)&srcBitstream->bitstreamBRAM[srcRA+r][srcCol], bramCols * XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * XUSP_WORDS_PER_FRAME * 4);
		}
	}//-if(reloBRAM)
}
void XilinxUltraScalePlus::flexiMerge(XilinxUltraScalePlus* srcBitstream, SelectedOptions options, Endianess endianConversionNeeded, Rect2D src, Coord2D dst){
    int srcRA = src.position.row  / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = src.size.row / XUSP_CLB_PER_CLOCK_REGION;
    int dstRA = dst.row  / XUSP_CLB_PER_CLOCK_REGION;

    for(int r = 0 ; r < sizeR ; r++){
        for(int c = 0 ; c < src.size.col ; c++){//relocate dst[dstRA+r][dst.col+c] <= src[srcRA+r][src.position.col+c]
            for(int m = 0 ; m < numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA+r][dst.col+c]] ; m++){
				for(int w = 0 ; w < XUSP_WORDS_PER_FRAME ; w++){
					if(XUSP_WORDS_BEFORE_CLK <= w && (XUSP_WORDS_BEFORE_CLK + XUSP_WORDS_AT_CLK) > w){
						if(!options.clk)
							continue;
					} else {
						if(!options.clb)
							continue;
					}
					switch(options.op){
						case MergeOP::SET:
							bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME + w]  = Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME + w], endianConversionNeeded);
						break;
						case MergeOP::XOR:
							bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME + w] ^= Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME + w], endianConversionNeeded);
						break;
						case MergeOP::OR:
							bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME + w] |= Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME + w], endianConversionNeeded);
						break;
						case MergeOP::AND:
							bitstreamCLB[dstRA+r][dst.col+c][m*XUSP_WORDS_PER_FRAME + w] &= Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][src.position.col+c][m*XUSP_WORDS_PER_FRAME + w], endianConversionNeeded);
						break;
					}
				}
            }
        }
    }
	if(options.bram){
		for(int r = 0 ; r < sizeR ; r++){
			int bramCols = numberOfBRAMsBeforeCol[src.position.col + src.size.col] - numberOfBRAMsBeforeCol[src.position.col];
			int srcCol = numberOfBRAMsBeforeCol[src.position.col];
			int dstCol = numberOfBRAMsBeforeCol[dst.col];
			for(int c = 0 ; c < bramCols ; c++){
				for(int w = 0 ; w < XUSP_WORDS_PER_FRAME * XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN ; w++){
					switch(options.op){
						case MergeOP::SET:
							bitstreamBRAM[dstRA+r][dstCol+c][w]  = Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][w], endianConversionNeeded);
						break;
						case MergeOP::XOR:
							bitstreamBRAM[dstRA+r][dstCol+c][w] ^= Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][w], endianConversionNeeded);
						break;
						case MergeOP::OR:
							bitstreamBRAM[dstRA+r][dstCol+c][w] |= Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][w], endianConversionNeeded);
						break;
						case MergeOP::AND:
							bitstreamBRAM[dstRA+r][dstCol+c][w] &= Endian::NativeToAnyEndianess32(srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][w], endianConversionNeeded);
						break;
					}
				}
			}
		}
	}//-if(reloBRAM)
}
string XilinxUltraScalePlus::getFrameType(int blockType, int rowAddress, int columnAddress){
	if(CAP::BlockType::BLOCKRAM == static_cast<CAP::BlockType>(blockType))
		return "BlockRAM Contents";
	else if(CAP::BlockType::LOGIC == static_cast<CAP::BlockType>(blockType))
		return typeOfFramesPerResourceLetter[(uint8_t)resourceString[rowAddress][columnAddress]];
	else
		return "Unknown";
}
