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
#include "XilinxUltraScalePlusConfigurationAccessPortInlinedFunctions.cpp"
#include "../../../Common/FileIO.h"
#include "../../../Common/StringFuncs.h"
//#include "../XilinxFileIO.h"
#include <iostream>
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
	
	loadedBitstreamEndianess = XilinxUltraScalePlus::parseBitstreamEndianess(fin);
	
    if(StringFuncs::checkIf::stringEndsWith(filename, ".bit"))
        XilinxUltraScalePlus::readBitstreamBIT(fin);
    if(StringFuncs::checkIf::stringEndsWith(filename, ".bin"))
        XilinxUltraScalePlus::readBitstreamBIN(fin);
    else
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be read.\n"));
	fin.close();
}
void XilinxUltraScalePlus::readBitstreamBIT(ifstream& fin)
{
	XilinxUltraScalePlus::parseBITheader(fin, loadedBitstreamEndianess);
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByNameOrThrow(partName), partName);
    XilinxUltraScalePlus::ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	XilinxUltraScalePlus::readBitstreamMain(fin);
}

void XilinxUltraScalePlus::readBitstreamBIN(ifstream& fin)
{
	uint32_t IDCODE = XilinxUltraScalePlus::parseBitstreamIDCODE(fin, loadedBitstreamEndianess);
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByIDCODEorThrow(IDCODE));
    XilinxUltraScalePlus::ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	XilinxUltraScalePlus::findBitstreamSync(fin, loadedBitstreamEndianess);
	XilinxUltraScalePlus::readBitstreamMain(fin);
}

void XilinxUltraScalePlus::readBitstreamMain(ifstream& fin)
{
    CAP::Register regAddr = CAP::Register::UNDEFINED;
    int wordCount = 0;
    char shadowFrame[XUSP_WORDS_PER_FRAME*4];
    int shadowFrameValid = 0;
    int b = 7, r = 0, c = 0, m = 0;
    //Parse bitstream
    for( ; ; ){
        uint32_t instruction = FileIO::read32(fin, loadedBitstreamEndianess);
        if(!fin.good()){
            break; // done with the bitstream
        } else {
			int instructionType = XilinxUltraScalePlus::CAP_getInstructionType(instruction);
			CAP::Operation instructionOPCODE = XilinxUltraScalePlus::CAP_getInstructionOperation(instruction);
			int instructionPayload = XilinxUltraScalePlus::CAP_getInstructionPayload(instruction);
			if(instructionType == 1) {
				regAddr = XilinxUltraScalePlus::CAP_getInstructionRegister(instruction);
				wordCount = XilinxUltraScalePlus::CAP_getInstructionWordCount(instruction);
			} else if(instructionType == 2) {
				wordCount = instructionPayload;
			} else {
				throw runtime_error("Bitstream has invalid instruction (invalid type).");
			}
            if(instructionOPCODE == CAP::Operation::WRITE){ //Write register
                for( ; wordCount > 0 ; ){
                    if(regAddr == CAP::Register::FAR){
						uint32_t farValue = FileIO::read32(fin, loadedBitstreamEndianess);
                        wordCount--;
						XilinxUltraScalePlus::CAP_parseFAR(farValue, b, r, c, m);
                    } else if(regAddr == CAP::Register::FDRI){
						if(wordCount % XUSP_WORDS_PER_FRAME != 0)
                            throw runtime_error("FDRI write of a partial frame was detected, which is currently not supported.");
                        if(shadowFrameValid){
                            if(b == 0){ // CLB
                                memcpy((char*)&bitstreamCLB[r][c][m*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                            } else if(b == 1) { // bram;
                                memcpy((char*)&bitstreamBRAM[r][c][m*XUSP_WORDS_PER_FRAME] ,&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                            }
							XilinxUltraScalePlus::CAP_IncrementFAR(b, r, c, m);
                        }
                        int forwardShadowReg = (wordCount/XUSP_WORDS_PER_FRAME) - 1;//skip shadow reg for certain number of frames
                        if(forwardShadowReg > 0){
                            if(b == 0){ // CLB
                                fin.read((char*)&bitstreamCLB[r][c][m*XUSP_WORDS_PER_FRAME], forwardShadowReg * XUSP_WORDS_PER_FRAME * 4);
                            } else if(b == 1){
                                fin.read((char*)&bitstreamBRAM[r][c][m*XUSP_WORDS_PER_FRAME], forwardShadowReg * XUSP_WORDS_PER_FRAME * 4);
                            }
                            for(int i = 0 ; i < forwardShadowReg ; i++){
                                XilinxUltraScalePlus::CAP_IncrementFAR(b, r, c, m);
                            }
                        }
                        wordCount -= forwardShadowReg * XUSP_WORDS_PER_FRAME;
                        shadowFrameValid = 1;
                        fin.read((char*)&shadowFrame, XUSP_WORDS_PER_FRAME*4);
                        wordCount -= XUSP_WORDS_PER_FRAME;

                    } else if(regAddr == CAP::Register::CMD){
						CAP::Command command = static_cast<CAP::Command>(FileIO::read32(fin, loadedBitstreamEndianess));
                        wordCount--;
                        if(command == CAP::Command::WCFG){
                            shadowFrameValid = 0;
                        }
                    } else {
						uint32_t scrap = FileIO::read32(fin, loadedBitstreamEndianess);
                        wordCount--;
                    }
                }//Write register count for loop
            }//Write register instruction
        }// not end of file
    }// for loop parsing the rest of the bitstream
}