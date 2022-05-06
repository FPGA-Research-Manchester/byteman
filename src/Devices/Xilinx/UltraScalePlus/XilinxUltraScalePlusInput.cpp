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
#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstring>
using namespace std;

void XilinxUltraScalePlus::readBitstream(string filename)
{
	ifstream fin (filename, ifstream::binary);
	if(!fin.good())
        throw runtime_error(string("Could not open file: \"").append(filename).append("\" .\n"));
    if(verbose)cout<<"Reading Xilinx UltraScale+ bitstream from file \""<<filename<<"\":\n";
    if(fileExtensionIs(filename, "bit"))
        XilinxUltraScalePlus::readBitstreamBIT(fin);
    else
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be read.\n"));
	fin.close();
}
void XilinxUltraScalePlus::readBitstreamBIT(ifstream& fin)
{
	XilinxUltraScalePlus::readBITheader(fin);
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByNameOrThrow(partName), partName);
    XilinxUltraScalePlus::ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing

    int regAddr = 0;
    int wordCount = 0;
    char shadowFrame[XUSP_WORDS_PER_FRAME*4];
    int shadowFrameValid = 0;
    int b = 7, r = 0, c = 0, m = 0;
    //Parse bitstream
    for( ; ; ){
        fileReadAndDefineBE(instruction, fin, 32);
        if(!fin.good()){
            break; // done with the bitstream
        } else {
			int instructionType = getInstructionType(instruction);
			int instructionOPCODE = getInstructionOPCODE(instruction);
			int instructionPayload = getInstructionPayload(instruction);
			if(instructionType == 1) {
				wordCount = getInstructionType1WordCount(instruction);
				regAddr = getInstructionType1Register(instruction);
			} else if(instructionType == 2) {
				wordCount = instructionPayload;
			} else {
				throw runtime_error("Bitstream has invalid instruction (invalid type).");
			}
            if(instructionOPCODE == OP_WRITE){ //Write register
                for( ; wordCount > 0 ; ){
                    if(regAddr == XUSP_CAP_REG_FAR){
                        fileReadAndDefineBE(farValue, fin, 32);
                        wordCount--;
						parseFAR(farValue,b,r,c,m);
                    } else if(regAddr == XUSP_CAP_REG_FDRI){
						if(wordCount % 93 != 0)
                            throw runtime_error("FDRI write of a partial frame was detected, which is currently not supported.");
                        if(shadowFrameValid){
                            if(b == 0){ // CLB
                                memcpy((char*)&bitstreamCLB[r][c][m*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                            } else if(b == 1) { // bram;
                                memcpy((char*)&bitstreamBRAM[r][c][m*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                            }
							XUSP_INCR_FAR(b,r,c,m);
                        }
                        int forwardShadowReg = (wordCount/XUSP_WORDS_PER_FRAME) - 1;//skip shadow reg for certain number of frames
                        if(forwardShadowReg > 0){
                            if(b == 0){ // CLB
                                fin.read((char*)&bitstreamCLB[r][c][m*XUSP_WORDS_PER_FRAME], forwardShadowReg * XUSP_WORDS_PER_FRAME * 4);
                            } else if(b == 1){
                                fin.read((char*)&bitstreamBRAM[r][c][m*XUSP_WORDS_PER_FRAME], forwardShadowReg * XUSP_WORDS_PER_FRAME * 4);
                            }
                            for(int i = 0 ; i < forwardShadowReg ; i++){
                                XUSP_INCR_FAR(b,r,c,m);
                            }
                        }
                        wordCount -= forwardShadowReg * XUSP_WORDS_PER_FRAME;
                        shadowFrameValid = 1;
                        fin.read((char*)&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                        wordCount -= XUSP_WORDS_PER_FRAME;

                    } else if(regAddr == XUSP_CAP_REG_CMD){
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
/**************************************************************************//**
 * Reads and parses .BIT header until and including the sync command.
 * Updates "designName", "partName", "fileDate", "fileTime".
 * 
 * @arg @c fin input file stream, which is left at the position following the
 * located sync command.
 *****************************************************************************/
void XilinxUltraScalePlus::readBITheader(ifstream& fin){
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
	delete designNameChars;
    for(int headerDone = 0 ; !headerDone ; ){
        fileReadAndDefineBE(key, fin, 8);
        switch(key){
            case 'b': {
                fileReadAndDefineBE(headerAttrLength, fin, 16);
                fileReadAndDefineChars(partNameChars, fin, headerAttrLength);
                if(verbose)cout<<"\tFPGA part: "<<partNameChars<<endl;
				partName = string(partNameChars);
				delete partNameChars;
                break;
            }
            case 'c': {
                fileReadAndDefineBE(headerAttrLength, fin, 16);
                fileReadAndDefineChars(date, fin, headerAttrLength);
                if(verbose)cout<<"\tdate: "<<date<<endl;
				fileDate = string(date);
                delete date;
                break;
            }
            case 'd': {
                fileReadAndDefineBE(headerAttrLength, fin, 16);
                fileReadAndDefineChars(time, fin, headerAttrLength);
                if(verbose)cout<<"\ttime: "<<time<<endl;
				fileTime = string(time);
                delete time;
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
                    cout << "warning: .bit header contained inaccurate file length field.\n";
                break;
            }
        }
    }
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
}
