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
#include "XilinxUltraScalePlus.h"
#include "XilinxUltraScalePlusCAP.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <fstream>
using namespace std;

XilinxUltraScalePlus::XilinxUltraScalePlus()
{
	designName = "";
    partName = "";
    initializedBitstreamPartName = "";
    IDCODEs[0] = 0;
    numberOfRows = 0;
    verbose = 0;
    warn = 1;
	blankFrame = calloc(XUSP_WORDS_PER_FRAME, 4);
}

XilinxUltraScalePlus::~XilinxUltraScalePlus()
{
    //dtor
	free(blankFrame);
	if(bitstreamBegin != nullptr)
		delete bitstreamBegin;
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
void XilinxUltraScalePlus::region(string params, int posRow, int posCol, int sizeRow, int sizeCol){
	if(params.find("clear") != string::npos) //first do the clearing
		regionsSelected.clear();
	if(params.find("add") != string::npos){ //before potential adding
		if(sizeRow <= 0 || sizeCol <= 0)
			throw runtime_error("Regions need to be of positive size.");
		regionsSelected.push_back({posRow, posCol, sizeRow, sizeCol});
	}
}
void XilinxUltraScalePlus::blank(string params){
	XilinxUltraScalePlus::ensureInitializedBitstreamArrays();
	int blankCLB = 0, blankBRAM = 0;
    if(params.find("logic") != string::npos || params.find("clb") != string::npos)blankCLB++;
    if(params.find("blockram") != string::npos || params.find("bram") != string::npos)blankBRAM++;
	if(regionsSelected.empty()) {
		if(blankCLB)
			memset(&bitstreamCLB[0][0][0], 0, (&bitstreamBRAM[0][0][0]-&bitstreamCLB[0][0][0]));
		if(blankBRAM)
			memset(&bitstreamBRAM[0][0][0], 0, (bitstreamEnd-&bitstreamBRAM[0][0][0]));
	} else {
		if(blankCLB)
			for (auto &r : regionsSelected) {
				// TODO, blank bitstream at selected regions (r->posRow, r->posCol, r->sizeRow, r->sizeCol)
				//memset(&bitstreamCLB[0][0][0], 0, (&bitstreamBRAM[0][0][0]-&bitstreamCLB[0][0][0]));
			}
		if(blankBRAM)
			for (auto &r : regionsSelected) {
				// TODO, blank bitstream at selected regions
				//memset(&bitstreamBRAM[0][0][0], 0, (bitstreamEnd-&bitstreamBRAM[0][0][0]));
			}
	}
}
void XilinxUltraScalePlus::ensureRowCompatibility(int srcRow, int srcCol, int offsetRow, int sizeCol, int dstRow, int dstCol){
	int srcRA = (srcRow+offsetRow) / XUSP_CLB_PER_CLOCK_REGION;
	int dstRA = (dstRow+offsetRow) / XUSP_CLB_PER_CLOCK_REGION;
	for(int c = 0 ; c < sizeCol ; c++){
		if(resourceString[srcRA][srcCol+c] != resourceString[dstRA][dstCol+c]){//if any two letters are different
			if(numberOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA][srcCol+c]] != numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA][dstCol+c]]){
				//also the number of frames is different?!
				throw runtime_error(string("Tried to relocate to an incompatible location(").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA][srcCol+c]]).append(" to frame type ").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA][dstCol+c]]).append(")."));
			} else {
				//okay, different col, but same width. Throw warning?
				if(warn)
					cout<<"Relocating from frame type "<<typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA][srcCol+c]]<<" to frame type "<<typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA][dstCol+c]]<<".\n";
			}
		}
	}
}
void XilinxUltraScalePlus::ensureRegionCompatibility(int srcRow, int srcCol, int sizeRow, int sizeCol, int dstRow, int dstCol){
    for(int r = 0 ; r < sizeRow ; ){ //this loop will iterate south border and all (srcRow:srcRow+sizeRow) at CLBs 0 and 59
		XilinxUltraScalePlus::ensureRowCompatibility(srcRow, srcCol, r, sizeCol, dstRow, dstCol);
		if( ((srcRow + r) % XUSP_CLB_PER_CLOCK_REGION) == (XUSP_CLB_PER_CLOCK_REGION-1) ) { // isLastCLBinRow
			r++;
		} else if( ((srcRow + r) % XUSP_CLB_PER_CLOCK_REGION) == 0 ) { // isFirstCLBinRow
			r += (XUSP_CLB_PER_CLOCK_REGION - 2);
		} else { // get to the next {isLastCLBinRow} 
			r += (XUSP_CLB_PER_CLOCK_REGION - 1 - ((srcRow + r) % XUSP_CLB_PER_CLOCK_REGION));
		}
    }
	ensureRowCompatibility(srcRow, srcCol, sizeRow-1, sizeCol, dstRow, dstCol); //iterate north border as well
}
void XilinxUltraScalePlus::merge(XilinxUltraScalePlus* srcBitstream, string params, int srcRow, int srcCol, int sizeRow, int sizeCol, int dstRow, int dstCol){
    //Parse merge params
    //This function uses a pointer to the second XUSP instance, but this pointer gets optimized as it is statically provided in the main function
    int reloCLK = 0, reloCLB = 0, reloBRAM = 0;
    if(params.find("logic") != string::npos || params.find("clb") != string::npos)reloCLB++;
    if(params.find("clock") != string::npos || params.find("clk") != string::npos)reloCLK++;
    if(params.find("blockram") != string::npos || params.find("bram") != string::npos)reloBRAM++;
	XilinxUltraScalePlus::ensureRegionCompatibility(srcRow, srcCol, sizeRow, sizeCol, dstRow, dstCol);
    if(srcRow%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
    if(sizeRow%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
    if(dstRow%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");

    int srcRA  = srcRow  / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = sizeRow / XUSP_CLB_PER_CLOCK_REGION;
    int dstRA  = dstRow  / XUSP_CLB_PER_CLOCK_REGION;

    for(int r = 0 ; r < sizeR ; r++){
        for(int c = 0 ; c < sizeCol ; c++){
            if(resourceString[srcRA+r][srcCol+c] != resourceString[dstRA+r][dstCol+c]){//if any two letters are different
                if(numberOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA+r][srcCol+c]] != numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA+r][dstCol+c]]){
                    //also the number of frames is different?!
                    throw runtime_error(string("Tried to relocate to an incompatible location(").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA+r][srcCol+c]]).append(" to frame type ").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA+r][dstCol+c]]).append(")."));
                } else {
                    //okay, different col, but same width. Throw warning?
                    if(warn)
                        cout<<"Relocating from frame type "<<typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcRA+r][srcCol+c]]<<" to frame type "<<typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA+r][dstCol+c]]<<".\n";
                }
            }
        }
    }

    for(int r = 0 ; r < sizeR ; r++){
        for(int c = 0 ; c < sizeCol ; c++){//relocate dst[dstRA+r][dstCol+c] <= src[srcRA+r][srcCol+c]
            for(int m = 0 ; m < numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstRA+r][dstCol+c]] ; m++){
                if(reloCLB && reloCLK){
                    memcpy((char*)&bitstreamCLB[dstRA+r][dstCol+c][m*XUSP_WORDS_PER_FRAME] ,(char*)&srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][m*XUSP_WORDS_PER_FRAME], XUSP_WORDS_PER_FRAME*4);
                } else {
                    if(reloCLB){
                        memcpy((char*)&bitstreamCLB[dstRA+r][dstCol+c][m*XUSP_WORDS_PER_FRAME] ,                                        (char*)&srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][m*XUSP_WORDS_PER_FRAME],                                         XUSP_WORDS_BEFORE_CLK*4);
                        memcpy((char*)&bitstreamCLB[dstRA+r][dstCol+c][m*XUSP_WORDS_PER_FRAME+XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK] ,(char*)&srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][m*XUSP_WORDS_PER_FRAME+XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK], XUSP_WORDS_AFTER_CLK*4);
					}//-if(reloCLB)
                    if(reloCLK){
                        for(int w = XUSP_WORDS_BEFORE_CLK ; w < (XUSP_WORDS_BEFORE_CLK + XUSP_WORDS_AT_CLK) ; w++)
                            bitstreamCLB[dstRA+r][dstCol+c][m*XUSP_WORDS_PER_FRAME + w] = srcBitstream->bitstreamCLB[srcRA+r][srcCol+c][m*XUSP_WORDS_PER_FRAME + w];
                    }//-if(reloCLK)
                }//-if(reloCLB && reloCLK)
            }
            if(reloBRAM){
                if(resourceString[dstRA+r][dstCol+c] == 'A' || resourceString[dstRA+r][dstCol+c] == 'B' || resourceString[dstRA+r][dstCol+c] == 'C'){//If resource is BRAM, BRAM_L or BRAM_R
                    memcpy((char*)&bitstreamBRAM[dstRA+r][numberOfBRAMsBeforeCol[dstCol+c]][0] ,(char*)&srcBitstream->bitstreamBRAM[srcRA+r][numberOfBRAMsBeforeCol[srcCol+c]], XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * XUSP_WORDS_PER_FRAME * 4);
                }
            }//-if(reloBRAM)
        }
    }
}
