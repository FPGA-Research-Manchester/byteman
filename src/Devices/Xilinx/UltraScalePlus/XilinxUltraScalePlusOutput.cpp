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
#include "../../../byteman.h"
//#include "../XilinxFileIO.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstring>
using namespace std;

/**************************************************************************//**
 * Creates a new bitstream file and writes the selected regions in it.
 * 
 *
 * @arg @c filename Name of the output file.
 * @throws runtime_error if args don't define a correct region.
 *****************************************************************************/
void XilinxUltraScalePlus::writeBitstream(string filename, string params, Rect2D rect){
	size_t dotpos = filename.rfind(".");
	if(dotpos == string::npos)
        throw runtime_error(string("Invalid file name: \"").append(filename).append("\"!\n"));
	designName = filename.substr(0, dotpos);
	
    if(verbose)cout<<"Writing Xilinx UltraScale+ bitstream to file \""<<filename<<"\":\n";
	
	SelectedOptions options = XilinxUltraScalePlus::parseParams(params);
	
	if(options.partial) {
		if((rect.position.row % XUSP_CLB_PER_CLOCK_REGION != 0) || (rect.size.row % XUSP_CLB_PER_CLOCK_REGION != 0))
			throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
		if(rect.size.row <= 0 || rect.size.col <= 0)
			throw runtime_error("Invalid output size dimentions.");
	}
	
	ofstream fout (filename, ofstream::binary | ofstream::trunc);
	if(!fout.good())
        throw runtime_error(string("Could not open file: \"").append(filename).append("\"!\n"));
    if(StringFuncs::checkIf::stringEndsWith(filename, ".bit"))
        XilinxUltraScalePlus::writeBitstreamBIT(fout, params, rect, options);
    else if(StringFuncs::checkIf::stringEndsWith(filename, ".bin"))
        XilinxUltraScalePlus::writeBitstreamBIN(fout, params, rect, options);
    else
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be written.\n"));
	fout.close();
}
void XilinxUltraScalePlus::writeBitstreamBIT(ofstream& fout, string params, Rect2D rect, SelectedOptions options){

	if(options.partial)
		designName.append(";PARTIAL=TRUE");
	designName.append(";bytemanVersion=").append(VERSION).append(":").append(VERSION_BUILD);
	
	XilinxUltraScalePlus::updateDateAndTime();
	streamoff headerLocationOfRemainingFileLength = XilinxUltraScalePlus::outputBITheader(fout, Endianess::BIG_ENDIAN);//.bit always big endian
	XilinxUltraScalePlus::writeBitstreamMain(fout, params, rect, options);
	
	XilinxUltraScalePlus::outputBITheaderLengthField(fout, headerLocationOfRemainingFileLength, loadedBitstreamEndianess);
}
void XilinxUltraScalePlus::writeBitstreamBIN(ofstream& fout, string params, Rect2D rect, SelectedOptions options){
	XilinxUltraScalePlus::outputCAPheaderConstant(fout, loadedBitstreamEndianess);
	XilinxUltraScalePlus::writeBitstreamMain(fout, params, rect, options);
}
void XilinxUltraScalePlus::writeBitstreamMain(ofstream& fout, string params, Rect2D rect, SelectedOptions options){
	XilinxUltraScalePlus::CAP_writeNOP(fout, 2, 0, loadedBitstreamEndianess);
	
	//TODO, add support for multi-SLR
	XilinxUltraScalePlus::CAP_writeNOP(fout, 132, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::RCRC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 2, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::IDCODE, SLRinfo[0].IDCODE, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::NULLCMD, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeMaskAndRegister(fout, CAP::Register::CTRL0, 0x500, 0x500, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeMaskAndRegister(fout, CAP::Register::CTRL1, 0x20000, 0, loadedBitstreamEndianess);
	if(options.partial){
		for (Rect2D r : regionSelection) {
			XilinxUltraScalePlus::writeBitstreamMainSingleRegion(fout, r, options);
		}
		XilinxUltraScalePlus::writeBitstreamMainSingleRegion(fout, rect, options);
	} else {// full
		Rect2D fullDevice = {{0, 0}, {0, 0}};
		fullDevice.size.row = numberOfRows * XUSP_CLB_PER_CLOCK_REGION;
		fullDevice.size.col = numberOfCols;
		XilinxUltraScalePlus::writeBitstreamMainSingleRegion(fout, fullDevice, options);
		
		XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::GRESTORE, loadedBitstreamEndianess);
		XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	}
	
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeMaskAndRegister(fout, CAP::Register::CTRL0, 0x100, 0, loadedBitstreamEndianess);// todo: writeReg(fout, XUSP_CAP_REG_CTRL0, 0x100); for ICAP instead?
	
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::DGHIGH, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 20, 0, loadedBitstreamEndianess);
	
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::START, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, 0x07FC0000, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::RCRC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::DESYNC, loadedBitstreamEndianess);
	
	XilinxUltraScalePlus::CAP_writeNOP(fout, 16, 0, loadedBitstreamEndianess);
}
void XilinxUltraScalePlus::writeBitstreamMainSingleRegion(ofstream& fout, Rect2D rect, SelectedOptions options){
    int srcR  = rect.position.row / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = rect.size.row / XUSP_CLB_PER_CLOCK_REGION;
	if(options.clb){
		for(int r = 0 ; r < sizeR ; r++){
			int framesToWrite = numberOfFramesBeforeCol[rect.position.col+rect.size.col]-numberOfFramesBeforeCol[rect.position.col];
			uint32_t farValue = XilinxUltraScalePlus::CAP_makeFAR(static_cast<int>(CAP::BlockType::LOGIC), srcR+r, rect.position.col, 0);
			if(options.blank){
				XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, farValue, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::WCFG, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeFDRI(fout, ((framesToWrite+1)*XUSP_WORDS_PER_FRAME), loadedBitstreamEndianess);
				for(int i = 0 ; i <= framesToWrite ; i++)
					fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}//options.blank
			XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, farValue, loadedBitstreamEndianess);
			XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::WCFG, loadedBitstreamEndianess);
			XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
			XilinxUltraScalePlus::CAP_writeFDRI(fout, ((framesToWrite+1)*XUSP_WORDS_PER_FRAME), loadedBitstreamEndianess);
			
			fout.write((char*)&bitstreamCLB[srcR+r][rect.position.col][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
			fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
		}
	}//options.clb
	//TODO
	if(options.bram){
		for(int r = 0 ; r < sizeR ; r++){
			int framesToWrite = XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * (numberOfBRAMsBeforeCol[rect.position.col+rect.size.col]-numberOfBRAMsBeforeCol[rect.position.col]);
			if(framesToWrite > 0) {
				uint32_t farValue = XilinxUltraScalePlus::CAP_makeFAR(static_cast<int>(CAP::BlockType::BLOCKRAM), srcR+r, numberOfBRAMsBeforeCol[rect.position.col], 0);
				XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, farValue, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::WCFG, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeFDRI(fout, ((framesToWrite+1)*XUSP_WORDS_PER_FRAME), loadedBitstreamEndianess);
				
				fout.write((char*)&bitstreamBRAM[srcR+r][numberOfBRAMsBeforeCol[rect.position.col]][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}
		}
	}//options.blockram
}
