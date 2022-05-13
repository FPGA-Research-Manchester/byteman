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
#include <algorithm>
#include <string>
#include <stdexcept>
#include <fstream>
#include <time.h>
#include <cstring>
using namespace std;

void XilinxUltraScalePlus::writeBitstream(string filename, string params, int srcRow, int srcCol, int sizeRow, int sizeCol){
	size_t dotpos = filename.rfind(".");
	if(dotpos == string::npos)
        throw runtime_error(string("Invalid file name: \"").append(filename).append("\"!\n"));
	designName = filename.substr(0, dotpos);
	
    if(verbose)cout<<"Writing Xilinx UltraScale+ bitstream to file \""<<filename<<"\":\n";
	ofstream fout (filename, ofstream::binary | ofstream::trunc);
	if(!fout.good())
        throw runtime_error(string("Could not open file: \"").append(filename).append("\"!\n"));
    if(fileExtensionIs(filename, "bit"))
        writeBitstreamBIT(fout, params, srcRow, srcCol, sizeRow, sizeCol);
    else
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be written.\n"));
	fout.close();
}

/**************************************************************************//**
 * Creates a new bitstream file and writes the selected regions in it.
 * 
 *
 * @arg @c filename Name of the output file.
 * @arg @c params A list of parameters to control behavior:
 * - "logic" or "clb" : Enables bitstream generation in block type 0 (logic, routing, clocking).
 * - "blockram" or "bram" : Enables bitstream generation in block type 1 (blockram contents).
 * - "blank" : Enables bitstream blanking (zeroing) before block type 0 generation.
 * - "full" : Writes the whole chip.
 * - "partial" : Writes only selected regions.
 * @throws runtime_error if args don't define a correct region.
 *****************************************************************************/
void XilinxUltraScalePlus::writeBitstreamBIT(ofstream& fout, string params, int srcRow, int srcCol, int sizeRow, int sizeCol){


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
	if(partial)
		designName.append(";PARTIAL=TRUE");
	designName.append(";bytemanVersion=").append(VERSION).append(":").append(VERSION_BUILD);
	
	{ //Update fileDate, fileTime with {now}
		time_t timestamp = time(0);
		struct tm  tstruct;
		char       buf[80];
		tstruct = *localtime(&timestamp);
		strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);
		fileDate = string(buf);
		strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
		fileTime = string(buf);
	}
	streamoff headerLocationOfRemainingFileLength = XilinxUltraScalePlus::outputBITheader(fout);
	CAPwriteNOPs(fout, 2);
	
	//TODO, add support for multi-SLR
	CAPwriteNOPs(fout, 132);
	CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_RCRC);
	CAPwriteNOPs(fout, 2);
	CAPwriteReg(fout, XUSP_CAP_REG_IDCODE, SLRinfo[0].IDCODE);
	CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_NULL);
	CAPwriteReg(fout, XUSP_CAP_REG_MASK, 0x500);
	CAPwriteReg(fout, XUSP_CAP_REG_CTRL0, 0x500);
	CAPwriteReg(fout, XUSP_CAP_REG_MASK, 0x20000);
	CAPwriteReg(fout, XUSP_CAP_REG_CTRL1, 0);
	if(partial){
		for (rect r : regionsSelected) {
			XilinxUltraScalePlus::outputBITregion(fout, outputCLB, outputBRAM, blankCLBs, r.posRow, r.posCol, r.sizeRow, r.sizeCol);
		}
		XilinxUltraScalePlus::outputBITregion(fout, outputCLB, outputBRAM, blankCLBs, srcRow, srcCol, sizeRow, sizeCol);
	} else {// full
		XilinxUltraScalePlus::outputBITregion(fout, outputCLB, outputBRAM, blankCLBs, 0, 0, numberOfRows, numberOfCols);
	}
	XilinxUltraScalePlus::outputBITfooter(fout);
	XilinxUltraScalePlus::outputBITheaderLengthField(fout, headerLocationOfRemainingFileLength);
}
streamoff XilinxUltraScalePlus::outputBITheader(ofstream& fout){
	fileWriteBE(fout, 16, 9);//16-bit BE value = 9
	fileWriteString(fout, string("\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0"));
	fileWriteBE(fout, 16, 1);//16-bit BE value = 1
	fileWriteBE(fout, 8, 'a');
	fileWriteBE(fout, 16, static_cast<uint16_t>(designName.size()+1));
	fileWriteString(fout, designName);
	fileWriteBE(fout, 8, 'b');//8-bit BE value = 'b'
	fileWriteBE(fout, 16, static_cast<uint16_t>(partName.size()+1));
	fileWriteString(fout, partName);
	fileWriteBE(fout, 8, 'c');//8-bit BE value = 'c'
	fileWriteBE(fout, 16, static_cast<uint16_t>(fileDate.size()+1));
	fileWriteString(fout, fileDate);
	fileWriteBE(fout, 8, 'd');//8-bit BE value = 'd'
	fileWriteBE(fout, 16, static_cast<uint16_t>(fileTime.size()+1));
	fileWriteString(fout, fileTime);
	fileWriteBE(fout, 8, 'e');//8-bit BE value = 'e'
	streamoff headerLocationOfRemainingFileLength = fout.tellp();
	fileWriteBE(fout, 32, 0);//32-bit BE value = 0 representing remaining file size. needs to be fixed later
	for(int i = 0 ; i < 16 ; i++) // 64 bytes of 0xFF
		fileWriteBE(fout, 32, 0xFFFFFFFF);//32-bit BE value = 0xFFFFFFFF
	fileWriteBE(fout, 32, 0x000000BB);
	fileWriteBE(fout, 32, 0x11220044);
	fileWriteBE(fout, 32, 0xFFFFFFFF);
	fileWriteBE(fout, 32, 0xFFFFFFFF);
	
	CAPwriteSync(fout);
	return headerLocationOfRemainingFileLength;
}
void XilinxUltraScalePlus::outputBITheaderLengthField(ofstream& fout, streamoff headerLocationOfRemainingFileLength){
	streamoff finalFileSize = fout.tellp();
	streamoff reportDiff = finalFileSize - (headerLocationOfRemainingFileLength + 4);
	fout.seekp(headerLocationOfRemainingFileLength);
	fileWriteBE(fout, 32, static_cast<uint32_t>(reportDiff));//32-bit BE value = reportDiff
}
void XilinxUltraScalePlus::outputBITfooter(ofstream& fout){
	
	//writeReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_GRESTORE);
	CAPwriteNOP(fout);
	CAPwriteReg(fout, XUSP_CAP_REG_MASK, 0x100);
	CAPwriteReg(fout, XUSP_CAP_REG_CTRL0, 0); // todo: writeReg(fout, XUSP_CAP_REG_CTRL0, 0x100); for ICAP instead?
	
	CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_DGHIGH);
	CAPwriteNOPs(fout, 20);
	
	CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_START);
	CAPwriteNOP(fout);
	
	CAPwriteReg(fout, XUSP_CAP_REG_FAR, 0x07FC0000);
	CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_RCRC);
	CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_DESYNC);
	
	CAPwriteNOPs(fout, 16);
}
void XilinxUltraScalePlus::outputBITregion(ofstream& fout, int outputCLB, int outputBRAM, int blankCLBs, int srcRow, int srcCol, int sizeRow, int sizeCol){
    int srcR  = srcRow  / XUSP_CLB_PER_CLOCK_REGION;
    int sizeR = sizeRow / XUSP_CLB_PER_CLOCK_REGION;
	if(outputCLB){
		for(int r = 0 ; r < sizeR ; r++){
			int framesToWrite = numberOfFramesBeforeCol[srcCol+sizeCol]-numberOfFramesBeforeCol[srcCol];
			if(blankCLBs){
				CAPwriteFAR(fout, XUSP_BLOCKTYPE_LOGIC, srcR+r, srcCol, 0);
				CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_WCFG);
				CAPwriteNOP(fout);
				CAPselectAndWriteFDRI2(fout, (framesToWrite+1)*XUSP_WORDS_PER_FRAME);
				for(int i = 0 ; i < framesToWrite ; i++)
					fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}
			CAPwriteFAR(fout, 0, srcR+r, srcCol, 0);
			CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_WCFG);
			CAPwriteNOP(fout);
			CAPselectAndWriteFDRI2(fout, (framesToWrite+1)*XUSP_WORDS_PER_FRAME);
			
			fout.write((char*)&bitstreamCLB[srcR+r][srcCol][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
			fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
		}
	}//outputCLB
	//TODO
	if(outputBRAM){
		for(int r = 0 ; r < sizeR ; r++){
			int framesToWrite = XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * (numberOfBRAMsBeforeCol[srcCol+sizeCol]-numberOfBRAMsBeforeCol[srcCol]);
			if(framesToWrite > 0) {
				CAPwriteFAR(fout, XUSP_BLOCKTYPE_BRAM, srcR+r, numberOfBRAMsBeforeCol[srcCol], 0);
				CAPwriteReg(fout, XUSP_CAP_REG_CMD, XUSP_CAP_CMD_WCFG);
				CAPwriteNOP(fout);
				CAPselectAndWriteFDRI2(fout, (framesToWrite+1)*XUSP_WORDS_PER_FRAME);
				
				fout.write((char*)&bitstreamBRAM[srcR+r][numberOfBRAMsBeforeCol[srcCol]][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}
		}
	}//outputBRAM
}
