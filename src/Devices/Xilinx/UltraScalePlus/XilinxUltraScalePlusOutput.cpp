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
#include<string>
#include<stdexcept>
#include<fstream>
#include<cstring>

#include "XilinxUltraScalePlus.h"
#include "XilinxUltraScalePlusConfigurationAccessPortInlinedFunctions.cpp"
#include "../../../Common/FileIO.h"
#include "../../../Common/str.h"
#include "../../../byteman.h"

using namespace std;

/**************************************************************************//**
 * Creates a new bitstream file and writes the selected regions in it.
 * 
 *
 * @arg @c filename Name of the output file.
 * @throws runtime_error if args don't define a correct region.
 *****************************************************************************/
void XilinxUltraScalePlus::writeBitstream(string filename, string params, Rect2D cmdRect)
{
	size_t dotpos = filename.rfind(".");
	if(dotpos == string::npos)
		throw runtime_error(string("Invalid file name: \"").append(filename).append("\"!\n"));
	designName = filename.substr(0, dotpos);
	
	log("Writing Xilinx UltraScale+ bitstream to file \"" + filename + "\":");
	
	SelectedOptions options = XilinxUltraScalePlus::parseParams(params);
	
	if(options.partial) {
		if((cmdRect.position.row % XUSP_CLB_PER_CLOCK_REGION != 0) || (cmdRect.size.row % XUSP_CLB_PER_CLOCK_REGION != 0))
			throw runtime_error("Currently only full clock region height relocations are supported (use row numbers multiple of 60).");
		if(cmdRect.size.row <= 0 || cmdRect.size.col <= 0)
			throw runtime_error("Invalid output size dimentions.");
	}
	
	ofstream fout (filename, ofstream::binary | ofstream::trunc);
	if(!fout.good())
		throw runtime_error(string("Could not open file: \"").append(filename).append("\"!\n"));
	if(str::iff::stringEndsWith(filename, ".bit"))
		XilinxUltraScalePlus::writeBitstreamBIT(fout, params, cmdRect, options);
	else if(str::iff::stringEndsWith(filename, ".bin"))
		XilinxUltraScalePlus::writeBitstreamBIN(fout, params, cmdRect, options);
	else
		throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be written.\n"));
	fout.close();
}
void XilinxUltraScalePlus::writeBitstreamBIT(ofstream& fout, string params, Rect2D cmdRect, SelectedOptions options)
{
	if(options.partial)
		designName.append(";PARTIAL=TRUE");
	designName.append(";bytemanVersion=").append(VERSION).append(":").append(VERSION_BUILD);
	
	XilinxUltraScalePlus::updateDateAndTime();
	streamoff headerLocationOfRemainingFileLength = XilinxUltraScalePlus::outputBITheader(fout, Endianess::BE);//.bit always big endian
	XilinxUltraScalePlus::writeBitstreamMain(fout, params, cmdRect, options);
	
	XilinxUltraScalePlus::outputBITheaderLengthField(fout, headerLocationOfRemainingFileLength, loadedBitstreamEndianess);
}
void XilinxUltraScalePlus::writeBitstreamBIN(ofstream& fout, string params, Rect2D cmdRect, SelectedOptions options)
{
	XilinxUltraScalePlus::writeBitstreamMain(fout, params, cmdRect, options);
}
void XilinxUltraScalePlus::writeBitstreamMain(ofstream& fout, string params, Rect2D cmdRect, SelectedOptions options)
{
	streamoff slrMagicInstrLocation[XUSP_MAX_SLRS];

	for(int slr = 0 ; slr < numberOfSLRs ; slr++){
		//Check if there is overlap between selected regions and the current SLR.
		Rect2D slrCoordsRect = {{0, 0}, {0, 0}};
		slrCoordsRect.position.row = SLRinfo[slr].fromRow * XUSP_CLB_PER_CLOCK_REGION;
		slrCoordsRect.size.row = (SLRinfo[slr].toRow - SLRinfo[slr].fromRow + 1) * XUSP_CLB_PER_CLOCK_REGION;
		slrCoordsRect.size.col = numberOfCols;
		bool overlap = false;
		if(options.partial){
			for (Rect2D selRect : regionSelection) {
				if(!rect::empty(rect::getOverlap(selRect, slrCoordsRect))){
					overlap = true;
					break;
				}
			}
			if(!rect::empty(rect::getOverlap(cmdRect, slrCoordsRect)))
				overlap = true;
		} else {// full bitstream output
			overlap = true;
		}
		
		if(overlap)
			slrMagicInstrLocation[slr] = XilinxUltraScalePlus::writeBitstreamMainSingleSLR(fout, slr, cmdRect, options);
		else 
			slrMagicInstrLocation[slr] = XilinxUltraScalePlus::writeBitstreamMainEmptySLR(fout, slr, options);
	}
	for(int slr = numberOfSLRs - 2 ; slr >= 0 ; slr--){
		{ //fix magic command value
			streamoff currentStreamHead = fout.tellp();
			streamoff bypassSize = (currentStreamHead - slrMagicInstrLocation[slr])/4 - 1;
			fout.seekp(slrMagicInstrLocation[slr]);
			FileIO::write32(fout, static_cast<uint32_t>(bypassSize), loadedBitstreamEndianess);
			fout.seekp(currentStreamHead);
		}
		XilinxUltraScalePlus::writeBitstreamMainSLRfooter(fout, slr, options);
	}
	XilinxUltraScalePlus::CAP_writeNOP(fout, 400, 0, loadedBitstreamEndianess);
}
streamoff XilinxUltraScalePlus::writeBitstreamMainEmptySLR(ofstream& fout, int slr, SelectedOptions options)
{
	
	XilinxUltraScalePlus::outputCAPheaderConstant(fout, loadedBitstreamEndianess);
	
	XilinxUltraScalePlus::CAP_writeNOP(fout, 132, 0, loadedBitstreamEndianess);
	//Optional Shutdown + 1 NOP ?
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::RCRC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 2, 0, loadedBitstreamEndianess);
	
	streamoff fileLocationOfSLRmagicWrite;
	if((slr + 1) < numberOfSLRs) { // If there are more SLRs, output the magic
		XilinxUltraScalePlus::CAP_writeSelectRegister(fout, CAP::Register::MAGIC1, loadedBitstreamEndianess);
		fileLocationOfSLRmagicWrite = fout.tellp();
		XilinxUltraScalePlus::CAP_writeType2(fout, 0, loadedBitstreamEndianess);
	}
	return fileLocationOfSLRmagicWrite;
}
streamoff XilinxUltraScalePlus::writeBitstreamMainSingleSLR(ofstream& fout, int slr, Rect2D cmdRect, SelectedOptions options)
{
	Rect2D slrCoordsRect = {{0, 0}, {0, 0}};
	slrCoordsRect.position.row = SLRinfo[slr].fromRow * XUSP_CLB_PER_CLOCK_REGION;
	slrCoordsRect.size.row = (SLRinfo[slr].toRow - SLRinfo[slr].fromRow + 1) * XUSP_CLB_PER_CLOCK_REGION;
	slrCoordsRect.size.col = numberOfCols;
	
	XilinxUltraScalePlus::outputCAPheaderConstant(fout, loadedBitstreamEndianess);
	
	//XilinxUltraScalePlus::CAP_writeNOP(fout, 2, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 132, 0, loadedBitstreamEndianess);
	//Optional Shutdown + 1 NOP ?
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::RCRC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 2, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::IDCODE, SLRinfo[slr].IDCODE, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::NULLCMD, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeMaskAndRegister(fout, CAP::Register::CTRL0, 0x500, 0x500, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeMaskAndRegister(fout, CAP::Register::CTRL1, 0x20000, 0, loadedBitstreamEndianess);
	
	if(options.partial){
		for (Rect2D selRect : regionSelection) {
			XilinxUltraScalePlus::writeBitstreamMainSingleRegion(fout, slr, rect::getOverlap(selRect, slrCoordsRect), options);
		}
		XilinxUltraScalePlus::writeBitstreamMainSingleRegion(fout, slr, rect::getOverlap(cmdRect, slrCoordsRect), options);
	} else {// full
		XilinxUltraScalePlus::writeBitstreamMainSingleRegion(fout, slr, slrCoordsRect, options);
		
		// Add GRESTORE since it is full device bitstream.
		XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::GRESTORE, loadedBitstreamEndianess);
		XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	}
	
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeMaskAndRegister(fout, CAP::Register::CTRL0, 0x100, 0, loadedBitstreamEndianess);// todo: writeReg(fout, XUSP_CAP_REG_CTRL0, 0x100); for ICAP instead?
	
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::DGHIGH, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 20, 0, loadedBitstreamEndianess);
	
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::START, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, CAP::FARFAR, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::RCRC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::DESYNC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 16, 0, loadedBitstreamEndianess);
	
	streamoff fileLocationOfSLRmagicWrite;
	if((slr + 1) < numberOfSLRs) { // If there are more SLRs, output the magic
		
		XilinxUltraScalePlus::CAP_writeNOP(fout, 400, 0, loadedBitstreamEndianess);
		XilinxUltraScalePlus::CAP_writeSYNQ(fout, loadedBitstreamEndianess);
		XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
		XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::RCRC, loadedBitstreamEndianess);
		XilinxUltraScalePlus::CAP_writeNOP(fout, 2, 0, loadedBitstreamEndianess);
		
		XilinxUltraScalePlus::CAP_writeSelectRegister(fout, CAP::Register::MAGIC1, loadedBitstreamEndianess);
		fileLocationOfSLRmagicWrite = fout.tellp();
		XilinxUltraScalePlus::CAP_writeType2(fout, 0, loadedBitstreamEndianess);
	}
	return fileLocationOfSLRmagicWrite;
}
void XilinxUltraScalePlus::writeBitstreamMainSLRfooter(ofstream& fout, int slr, SelectedOptions options)
{
	XilinxUltraScalePlus::CAP_writeNOP(fout, 16, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::START, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::DESYNC, loadedBitstreamEndianess);
	XilinxUltraScalePlus::CAP_writeNOP(fout, 16, 0, loadedBitstreamEndianess);
}
void XilinxUltraScalePlus::writeBitstreamMainSingleRegion(ofstream& fout, int slr, Rect2D writeRect, SelectedOptions options)
{
	if(rect::empty(writeRect)) // If output region is empty, just return
		return;
	int srcR  = writeRect.position.row / XUSP_CLB_PER_CLOCK_REGION;
	int sizeR = writeRect.size.row / XUSP_CLB_PER_CLOCK_REGION;
	for(int r = 0 ; r < sizeR ; r++){
		int fromCol = writeRect.position.col;
		int toCol = writeRect.position.col+writeRect.size.col;
		if(options.skipUnused){
			for( ; (fromCol < numberOfCols && LUT_isFrameUnusedForResourceLetter[(uint8_t)resourceString[srcR+r][fromCol]]) ; fromCol++);
			for( ; (toCol > 0 && LUT_isFrameUnusedForResourceLetter[(uint8_t)resourceString[srcR+r][toCol]]) ; toCol--);
			if(fromCol >= toCol)
				continue;
		}
		if(options.clb){
			int framesToWrite = numberOfFramesBeforeCol[toCol]-numberOfFramesBeforeCol[fromCol];
			uint32_t farValue = XilinxUltraScalePlus::CAP_makeFAR(static_cast<int>(CAP::BlockType::LOGIC), slr, srcR+r, fromCol, 0);
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
			
			fout.write((char*)&bitstreamCLB[srcR+r][fromCol][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
			fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
		}//options.clb
		if(options.bram){
			int framesToWrite = XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * (numberOfBRAMsBeforeCol[toCol]-numberOfBRAMsBeforeCol[fromCol]);
			if(framesToWrite > 0) {
				uint32_t farValue = XilinxUltraScalePlus::CAP_makeFAR(static_cast<int>(CAP::BlockType::BLOCKRAM), slr, srcR+r, numberOfBRAMsBeforeCol[fromCol], 0);
				XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, farValue, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeCommand(fout, CAP::Command::WCFG, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeNOP(fout, 1, 0, loadedBitstreamEndianess);
				XilinxUltraScalePlus::CAP_writeFDRI(fout, ((framesToWrite+1)*XUSP_WORDS_PER_FRAME), loadedBitstreamEndianess);
				
				fout.write((char*)&bitstreamBRAM[srcR+r][numberOfBRAMsBeforeCol[fromCol]][0], framesToWrite * XUSP_WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, XUSP_WORDS_PER_FRAME * 4);
			}
		}//options.bram
	}
}
