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
#include "../../../Common/fileIO.h"
#include "../../../byteman.h"
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdexcept>
#include <fstream>
#include <time.h>
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
	delete blankFrame;
}


void XilinxUltraScalePlus::ensureInitializedBitstreamArrays(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
    if(initializedBitstreamPartName != partName){//The device is changed
		initializedBitstreamPartName = partName;
        if(bitstreamGlobal != nullptr)
            delete bitstreamGlobal;

        for((numberOfCols = 0, numberOfFramesBeforeCol[0] = 0, numberOfBRAMCols = 0) ; (uint8_t)resourceString[0][numberOfCols] ; numberOfCols++){
            numberOfFramesBeforeCol[numberOfCols+1] = numberOfFramesBeforeCol[numberOfCols] + numberOfFramesPerResourceLetter[(uint8_t)resourceString[0][numberOfCols]];
            numberOfBRAMsBeforeCol[numberOfCols] = numberOfBRAMCols;
            if(resourceString[0][numberOfCols] == 'A' || resourceString[0][numberOfCols] == 'B' || resourceString[0][numberOfCols] == 'C' || resourceString[0][numberOfCols] == '1') //ABC1 are "BRAM_L", "BRAM", "BRAM_R", "NULL BRAM"
                numberOfBRAMCols++;
        }
        numberOfFramesPerRow = numberOfFramesBeforeCol[numberOfCols];
        numberOfWordsPerRow = numberOfFramesPerRow * XUSP_WORDS_PER_FRAME;
		//Allocate
        bitstreamGlobal = new uint32_t[numberOfRows * (numberOfWordsPerRow + numberOfBRAMCols * XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * XUSP_WORDS_PER_FRAME) + 27*XUSP_WORDS_PER_FRAME];//TODO: WHY 27 ?! 27* extra frame for safety

        //Calc pointers
        int offset = 0;
        for(int r = 0 ; r < numberOfRows ; r++)
            for(int c = 0 ; c < numberOfCols ; c++){
                bitstreamCLB[r][c] = &bitstreamGlobal[offset];
                offset += numberOfFramesPerResourceLetter[(uint8_t)resourceString[r][c]] * XUSP_WORDS_PER_FRAME;
            }

        for(int r = 0 ; r < numberOfRows ; r++)
            for(int c = 0 ; c < numberOfBRAMCols ; c++){
                bitstreamBRAM[r][c] = &bitstreamGlobal[offset];
                offset += XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN*XUSP_WORDS_PER_FRAME;
            }
		bitstreamEnd = &bitstreamGlobal[offset];
    }
}
void XilinxUltraScalePlus::region(string params, int posRow, int posCol, int sizeRow, int sizeCol){
	if(params.find("clear") != string::npos) //first do the clearing
		regionsSelected.clear();
	if(params.find("add") != string::npos){ //before potential adding
		if(sizeRow <= 0 || sizeCol <= 0)
			throw runtime_error("Regions cannot be of positive size.");
		regionsSelected.push_back({posRow, posCol, sizeRow, sizeCol});
	}
}
void XilinxUltraScalePlus::blank(string params){
	ensureInitializedBitstreamArrays();
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
void XilinxUltraScalePlus::readBitstream(string filename)
{
    if(0 == filename.compare (filename.length() - 4, 4, ".bit"))
        readBitstreamBIT(filename);
    else
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be read.\n"));
}
void XilinxUltraScalePlus::readBitstreamBIT(string filename)
{
    if(verbose)cout<<"Reading Xilinx UltraScale+ bitstream from file \""<<filename<<"\":\n";
	//parse .BIT header:
	ifstream fin (filename, ifstream::binary);
	if(!fin.good())
        throw runtime_error(string("Could not open file: \"").append(filename).append("\" .\n"));
    //.bit header c0nstant:
	fileReadAndDefineBE(headerConstTextLength, fin, 16);
	fileReadAndDefineChars(headerConstText, fin, headerConstTextLength);
	fileReadAndDefineBE(headerConstText2Length, fin, 16);
	fileReadAndDefineChars(headerConstText2, fin, headerConstText2Length);
	if(warn && (headerConstTextLength != 9 || strcmp(headerConstText,"\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0\0") != 0 || headerConstText2Length != 1 || strcmp(headerConstText2,"a\0") != 0))
        cout << "warn: .bit c0nstant header differs from expected.\n";
	delete headerConstText; // we dont need that
	delete headerConstText2; // we dont need that

	//.bit header vars:
	fileReadAndDefineBE(headerDesignNameLength, fin, 16);
	fileReadAndDefineChars(designNameChars, fin, headerDesignNameLength);

	if(verbose)cout<<"\tName of design: "<<designNameChars<<endl;
	designName = string(designNameChars);
    for(int headerDone = 0 ; !headerDone ; ){
        fileReadAndDefineBE(key, fin, 8);
        switch(key){
            case 'b': {
                fileReadAndDefineBE(headerAttrLength, fin, 16);
                fileReadAndDefineChars(partNameChars, fin, headerAttrLength);
                if(verbose)cout<<"\tFPGA part: "<<partNameChars<<endl;
				partName = string(partNameChars);
                break;
            }
            case 'c': {
                fileReadAndDefineBE(headerAttrLength, fin, 16);
                fileReadAndDefineChars(date, fin, headerAttrLength);
                if(verbose)cout<<"\tdate: "<<date<<endl;
                delete date; //not needed
                break;
            }
            case 'd': {
                fileReadAndDefineBE(headerAttrLength, fin, 16);
                fileReadAndDefineChars(time, fin, headerAttrLength);
                if(verbose)cout<<"\ttime: "<<time<<endl;
                delete time; //not needed
                break;
            }
            case 'e': {
                headerDone = 1;//last attribute in a .bit header
                int reportedRemainingFileLength;
                fileReadBE(reportedRemainingFileLength, fin, 32);
                int tmpPos = (int)fin.tellg();
                fin.seekg (0, fin.end);
                int fileSize = (int)fin.tellg();
                fin.seekg (tmpPos, fin.beg);
                if(warn && fileSize != (tmpPos + reportedRemainingFileLength))
                    cout << "warn: .bit header contained inaccurate file length field.\n";
                break;
            }
        }
    }
    setDevice(getDeviceByNameOrThrow(partName));
    ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing

    //Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
    //However we don't care about reading those

    //Find sync
    for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
        fileReadAndDefineBE(word, fin, 32);
        if(word == 0xAA995566)
            syncDetectionDone++;
        else
            fin.seekg(-3,ios::cur);
    }

    int regAddr = 0;
    int wordCount = 0;
    char shadowFrame[XUSP_WORDS_PER_FRAME*4];
    int shadowFrameValid = 0;
    int b = 0, r = 0, c = 0, m = 0;
    //Parse bitstream
    for( ; ; ){
        fileReadAndDefineBE(instruction, fin, 32);
        if(!fin.good()){
            break; // done with the bitstream
        } else {
            if((instruction & 0xE0000000) == 0x20000000){ //Type 1 instruction
                regAddr = (instruction & 0x07FFE000) >> 13;
                wordCount   = (instruction & 0x7FF);
            } else if((instruction & 0xE0000000) == 0x40000000){ //Type 2 instruction
                wordCount   = (instruction & 0x07FFFFFF);
            }
            if((instruction & 0x18000000) == 0x00000000){ ;// NOP
            } else if((instruction & 0x18000000) == 0x08000000){ ;// Read register
            } else if((instruction & 0x18000000) == 0x18000000){ ;// Reserved
            } else if((instruction & 0x18000000) == 0x10000000){ //Write register
                for( ; wordCount > 0 ; ){
                    if(regAddr == XUSP_CAP_FAR){
                        fileReadAndDefineBE(farValue, fin, 32);
                        wordCount--;
                        b  = (farValue & 0x07000000) >> 24; // block type
                        r  = (farValue & 0x00FC0000) >> 18; // row address
                        c = (farValue & 0x0003FF00) >> 8; // column address
                        m = (farValue & 0x000000FF);  // minor address
                    } else if(regAddr == XUSP_CAP_FDRI){
                        if(wordCount % 93 != 0)
                            throw runtime_error("FDRI write of a partial frame was detected, which is currently not supported.");
                        if(shadowFrameValid){
                            if(b == 0){ // CLB
                                //perf test 1
                                //memcpy((char*)&bitstreamCLB[r*numberOfWordsPerRow + (numberOfFramesBeforeCol[c] + m)*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                                //perf test 2
                                //memcpy((char*)&bitstreamCLB[r][(numberOfFramesBeforeCol[c] + m)*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                                //perf test 3
                                memcpy((char*)&bitstreamCLB[r][c][m*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);

                                //increment FAR
                                m++;
                                if(m == numberOfFramesPerResourceLetter[(uint8_t)resourceString[r][c]]){
                                    m = 0;
                                    c++;
                                    if(c == numberOfCols){
                                        c = 0;
                                        r++;
                                        if(r == numberOfRows){
                                            r = 0;
                                            b++;
                                        }
                                    }
                                }
                            } else if(b == 1) { // bram;
                                memcpy((char*)&bitstreamBRAM[r][c][m*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                                //increment FAR
                                m++;
                                if(m == XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN){
                                    m = 0;
                                    c++;
                                    if(c == numberOfBRAMCols){
                                        c = 0;
                                        r++;
                                        if(r == numberOfRows){
                                            r = 0;
                                            b++;
                                        }
                                    }
                                }
                            }
                        }
                        int forwardShadowReg = (wordCount/XUSP_WORDS_PER_FRAME) - 1;//skip shadow reg for certain number of frames
                        if(forwardShadowReg > 0){
                            if(b == 0){ // CLB
                                fin.read((char*)&bitstreamCLB[r][c][m*XUSP_WORDS_PER_FRAME], forwardShadowReg * XUSP_WORDS_PER_FRAME * 4);
                            } else if(b == 1){
                                fin.read((char*)&bitstreamBRAM[r][c][m*XUSP_WORDS_PER_FRAME], forwardShadowReg * XUSP_WORDS_PER_FRAME * 4);
                            }
                            for(int i = 0 ; i < forwardShadowReg ; i++){
                                if(b == 0){
                                    m++;
                                    if(m == numberOfFramesPerResourceLetter[(uint8_t)resourceString[r][c]]){
                                        m = 0;
                                        c++;
                                        if(c == numberOfCols){
                                            c = 0;
                                            r++;
                                            if(r == numberOfRows){
                                                r = 0;
                                                b++;
                                            }
                                        }
                                    }
                                } else if(b == 1){
                                    m++;
                                    if(m == XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN){
                                        m = 0;
                                        c++;
                                        if(c == numberOfBRAMCols){
                                            c = 0;
                                            r++;
                                            if(r == numberOfRows){
                                                r = 0;
                                                b++;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                        wordCount -= forwardShadowReg * XUSP_WORDS_PER_FRAME;
                        shadowFrameValid = 1;
                        fin.read((char*)&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                        wordCount -= XUSP_WORDS_PER_FRAME;

                    } else if(regAddr == XUSP_CAP_CMD){
                        fileReadAndDefineBE(command, fin, 32);
                        wordCount--;
                        if(command == XUSP_CAP_CMD_WCFG){
                            shadowFrameValid = 0;
                        }
                    } else {
                        fileReadAndDefineBE(scrap, fin, 32);
                        wordCount--;
                    }
                }//Write register count for loop
            }//Write register instruction
        }// not end of file
    }// for loop parsing the rest of the bitstream
}

void XilinxUltraScalePlus::merge(XilinxUltraScalePlus* srcBitstream, string params, int srcRow, int srcCol, int sizeRow, int sizeCol, int dstRow, int dstCol){
    //Parse merge params
    //This function uses a pointer to the second XUSP instance, but this pointer gets optimized as it is statically provided in the main function
    int reloCLK = 0, reloCLB = 0, reloBRAM = 0;
    if(params.find("logic") != string::npos || params.find("clb") != string::npos)reloCLB++;
    if(params.find("clock") != string::npos || params.find("clk") != string::npos)reloCLK++;
    if(params.find("blockram") != string::npos || params.find("bram") != string::npos)reloBRAM++;

    if(srcRow%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
    if(sizeRow%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
    if(dstRow%XUSP_CLB_PER_CLOCK_REGION != 0)
        throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");

    int srcR  = srcRow  / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = sizeRow / XUSP_CLB_PER_CLOCK_REGION;
    int dstR  = dstRow  / XUSP_CLB_PER_CLOCK_REGION;

    for(int r = 0 ; r < sizeR ; r++){
        for(int c = 0 ; c < sizeCol ; c++){
            if(resourceString[srcR+r][srcCol+c] != resourceString[dstR+r][dstCol+c]){//if any two letters are different
                if(numberOfFramesPerResourceLetter[(uint8_t)resourceString[srcR+r][srcCol+c]] != numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstR+r][dstCol+c]]){
                    //also the number of frames is different?!
                    throw runtime_error(string("Tried to relocate to an incompatible location(").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcR+r][srcCol+c]]).append(" to frame type ").append(typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstR+r][dstCol+c]]).append(")."));
                } else {
                    //okay, different col, but same width. Throw warning?
                    if(warn)
                        cout<<"Relocating from frame type "<<typeOfFramesPerResourceLetter[(uint8_t)resourceString[srcR+r][srcCol+c]]<<" to frame type "<<typeOfFramesPerResourceLetter[(uint8_t)resourceString[dstR+r][dstCol+c]]<<".\n";
                }
            }
        }
    }

    for(int r = 0 ; r < sizeR ; r++){
        for(int c = 0 ; c < sizeCol ; c++){//relocate dst[dstR+r][dstCol+c] <= src[srcR+r][srcCol+c]
            for(int m = 0 ; m < numberOfFramesPerResourceLetter[(uint8_t)resourceString[dstR+r][dstCol+c]] ; m++){
                if(reloCLB && reloCLK){
                    memcpy((char*)&bitstreamCLB[dstR+r][dstCol+c][m*XUSP_WORDS_PER_FRAME] ,(char*)&srcBitstream->bitstreamCLB[srcR+r][srcCol+c][m*XUSP_WORDS_PER_FRAME], XUSP_WORDS_PER_FRAME*4);
                } else {
                    if(reloCLB){
                        memcpy((char*)&bitstreamCLB[dstR+r][dstCol+c][m*XUSP_WORDS_PER_FRAME] ,(char*)&srcBitstream->bitstreamCLB[srcR+r][srcCol+c][m*XUSP_WORDS_PER_FRAME], XUSP_WORDS_BEFORE_CLK*4);
                        memcpy((char*)&bitstreamCLB[dstR+r][dstCol+c][m*XUSP_WORDS_PER_FRAME+XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK] ,(char*)&srcBitstream->bitstreamCLB[srcR+r][srcCol+c][m*XUSP_WORDS_PER_FRAME+XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK], XUSP_WORDS_AFTER_CLK*4);
                    }//-if(reloCLB)
                    if(reloCLK){
                        for(int w = XUSP_WORDS_BEFORE_CLK ; w < (XUSP_WORDS_BEFORE_CLK + XUSP_WORDS_AT_CLK) ; w++)
                            bitstreamCLB[dstR+r][dstCol+c][m*XUSP_WORDS_PER_FRAME + w] = srcBitstream->bitstreamCLB[srcR+r][srcCol+c][m*XUSP_WORDS_PER_FRAME + w];
                    }//-if(reloCLK)
                }//-if(reloCLB && reloCLK)
            }
            if(reloBRAM){
                if(resourceString[dstR+r][dstCol+c] == 'A' || resourceString[dstR+r][dstCol+c] == 'B' || resourceString[dstR+r][dstCol+c] == 'C'){//If resource is BRAM, BRAM_L or BRAM_R
                    memcpy((char*)&bitstreamBRAM[dstR+r][numberOfBRAMsBeforeCol[dstCol+c]][0] ,(char*)&srcBitstream->bitstreamBRAM[srcR+r][numberOfBRAMsBeforeCol[srcCol+c]], XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * XUSP_WORDS_PER_FRAME * 4);
                }
            }//-if(reloBRAM)
        }
    }
}

void XilinxUltraScalePlus::writeBitstream(string filename, string params, int srcRow, int srcCol, int sizeRow, int sizeCol){
    if(0 == filename.compare (filename.length() - 4, 4, ".bit"))
        writeBitstreamBIT(filename, params, srcRow, srcCol, sizeRow, sizeCol);
    else
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be written.\n"));
}
string getDate(time_t timestamp) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&timestamp);
    strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);
    return string(buf);
}

string getTime(time_t timestamp) {
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&timestamp);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return string(buf);
}
streamoff headerLocationOfRemainingFileLength;
void XilinxUltraScalePlus::outputBITheader(ofstream& fout){
	fileWriteBE(fout, 16, 9);//16-bit BE value = 9
	fileWriteString(fout, string("\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0"));
	fileWriteBE(fout, 16, 1);//16-bit BE value = 1
	fileWriteString(fout, string("a"));
	fileWriteBE(fout, 16, static_cast<uint16_t>(designName.size()+1));
	fileWriteString(fout, designName);
	fileWriteBE(fout, 8, 'b');//8-bit BE value = 'b'
	fileWriteBE(fout, 16, static_cast<uint16_t>(partName.size()+1));
	fileWriteString(fout, partName);
	fileWriteBE(fout, 8, 'c');//8-bit BE value = 'c'
	time_t timestamp = time(0);
	string date = getDate(timestamp);
	fileWriteBE(fout, 16, static_cast<uint16_t>(date.size()+1));
	fileWriteString(fout, date);
	string time = getTime(timestamp);
	fileWriteBE(fout, 8, 'd');//8-bit BE value = 'd'
	fileWriteBE(fout, 16, static_cast<uint16_t>(time.size()+1));
	fileWriteString(fout, time);
	fileWriteBE(fout, 8, 'e');//8-bit BE value = 'e'
	headerLocationOfRemainingFileLength = fout.tellp();
	fileWriteBE(fout, 32, 0);//32-bit BE value = 0 representing remaining file size. needs to be fixed later
	for(int i = 0 ; i < 16 ; i++) // 64 bytes of 0xFF
		fileWriteBE(fout, 32, 0xFFFFFFFF);//32-bit BE value = 0xFFFFFFFF
	fileWriteBE(fout, 32, 0xBB);
	fileWriteBE(fout, 32, 0x11220044);
	fileWriteBE(fout, 32, 0xFFFFFFFF);
	fileWriteBE(fout, 32, 0xFFFFFFFF);
	
	writeSync(fout);
	writeNOPs(fout, 2);
	
	//TODO, add support for multi-SLR
	writeNOPs(fout, 132);
	writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_RCRC);
	writeNOPs(fout, 2);
	writeReg(fout, XUSP_CAP_IDCODE, IDCODEs[0]);
	writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_NULL);
	writeReg(fout, XUSP_CAP_MASK, 0x500);
	writeReg(fout, XUSP_CAP_CTRL0, 0x500);
	writeReg(fout, XUSP_CAP_MASK, 0x20000);
	writeReg(fout, XUSP_CAP_CTRL1, 0);
}
void XilinxUltraScalePlus::outputBITfooter(ofstream& fout){
	
	//writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_GRESTORE);
	writeNOP(fout);
	writeReg(fout, XUSP_CAP_MASK, 0x100);
	writeReg(fout, XUSP_CAP_CTRL0, 0); // todo: writeReg(fout, XUSP_CAP_CTRL0, 0x100); for ICAP instead?
	
	writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_DGHIGH);
	writeNOPs(fout, 20);
	
	writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_START);
	writeNOP(fout);
	
	writeReg(fout, XUSP_CAP_FAR, 0x07FC0000);
	writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_RCRC);
	writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_DESYNC);
	
	writeNOPs(fout, 16);
	
	//now fix file size in header
	streamoff finalFileSize = fout.tellp();
	streamoff reportDiff = finalFileSize - (headerLocationOfRemainingFileLength + 4);
	fout.seekp(headerLocationOfRemainingFileLength);
	fileWriteBE(fout, 32, static_cast<uint32_t>(reportDiff));//32-bit BE value = reportDiff
}
void XilinxUltraScalePlus::outputBITregion(ofstream& fout, int outputCLB, int outputBRAM, int blankCLBs, int srcRow, int srcCol, int sizeRow, int sizeCol){
    int srcR  = srcRow  / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = sizeRow / XUSP_CLB_PER_CLOCK_REGION;
	if(outputCLB){
		for(int r = 0 ; r < sizeR ; r++){
			int framesToWrite = numberOfFramesBeforeCol[srcCol+sizeCol]-numberOfFramesBeforeCol[srcCol];
			if(blankCLBs){
				writeFAR(fout, XUSP_BLOCKTYPE_LOGIC, srcR+r, srcCol, 0);
				writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_WCFG);
				writeNOP(fout);
				writeReg(fout, XUSP_CAP_FDRI, (framesToWrite+1)*XUSP_WORDS_PER_FRAME);
				for(int i = 0 ; i < framesToWrite ; i++)
					fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}
			writeFAR(fout, 0, srcR+r, srcCol, 0);
			writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_WCFG);
			writeNOP(fout);
			writeReg(fout, XUSP_CAP_FDRI, (framesToWrite+1)*XUSP_WORDS_PER_FRAME);
			
			fout.write((char*)&bitstreamCLB[srcR+r][srcCol][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
			fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
		}
	}//outputCLB
	//TODO
	if(outputBRAM){
		for(int r = 0 ; r < sizeR ; r++){
			int framesToWrite = XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * (numberOfBRAMsBeforeCol[srcCol+sizeCol]-numberOfBRAMsBeforeCol[srcCol]);
			if(framesToWrite > 0) {
				writeFAR(fout, XUSP_BLOCKTYPE_BRAM, srcR+r, numberOfBRAMsBeforeCol[srcCol], 0);
				writeReg(fout, XUSP_CAP_CMD, XUSP_CAP_CMD_WCFG);
				writeNOP(fout);
				writeReg(fout, XUSP_CAP_FDRI, (framesToWrite+1)*XUSP_WORDS_PER_FRAME);
				
				fout.write((char*)&bitstreamBRAM[srcR+r][numberOfBRAMsBeforeCol[srcCol]][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}
		}
	}//outputBRAM
}
void XilinxUltraScalePlus::writeBitstreamBIT(string filename, string params, int srcRow, int srcCol, int sizeRow, int sizeCol){
    if(verbose)cout<<"Writing Xilinx UltraScale+ bitstream to file \""<<filename<<"\":\n";
	//parse .BIT header:
	ofstream fout (filename, ofstream::binary | ofstream::trunc);
	if(!fout.good())
        throw invalid_argument(string("Could not open file: \"").append(filename).append("\"!\n"));


    //Parse output params
    int outputCLB = 0, outputBRAM = 0, blankCLBs = 0, partial = 1;
    if(params.find("logic") != string::npos || params.find("clb") != string::npos)outputCLB++;
    if(params.find("blockram") != string::npos || params.find("bram") != string::npos)outputBRAM++;
    if(params.find("blank") != string::npos)blankCLBs++;
    if(params.find("full") != string::npos)partial = 0;
	
	if(outputCLB == 0 && outputBRAM == 0){ // if nothing was chosen, choose both
		outputCLB = outputBRAM = 1;
	}
	if(partial) {
		if((srcRow%XUSP_CLB_PER_CLOCK_REGION != 0) || (sizeRow%XUSP_CLB_PER_CLOCK_REGION != 0))
			throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
		if(sizeRow <= 0 || sizeCol <= 0)
			throw runtime_error("Output size needs to be larger than 0.");
	}
	//for now overwrite the design name
	designName = filename.substr(0, filename.size()-4);
	if(partial)
		designName.append(";PARTIAL=TRUE");
	designName.append(";bytemanVersion=").append(VERSION).append(":").append(VERSION_BUILD);
	
	outputBITheader(fout);
	if(partial){
		for (rect r : regionsSelected) {
			outputBITregion(fout, outputCLB, outputBRAM, blankCLBs, r.posRow, r.posCol, r.sizeRow, r.sizeCol);
		}
		outputBITregion(fout, outputCLB, outputBRAM, blankCLBs, srcRow, srcCol, sizeRow, sizeCol);
	} else {// full
		outputBITregion(fout, outputCLB, outputBRAM, blankCLBs, 0, 0, numberOfRows, numberOfCols);
	}
	outputBITfooter(fout);
}
