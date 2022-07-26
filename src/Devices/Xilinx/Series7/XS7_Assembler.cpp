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
#include<algorithm> //replace
#include<string>
#include<stdexcept>
#include<fstream>
#include<cstring>
#include<iomanip>//setfill, setw

#include "XilinxSeries7.h"
#include "../../../Common/FileIO.h"
#include "../../../Common/str.h"
#include "../../../Common/Endianness.h"

using namespace std;

void XilinxSeries7::assembler(string filenameIn, string filenameOut)
{
	enum FILEFORMAT {FILE_NULL = 0, FILE_BIT, FILE_BIN, FILE_BIT_ASM};
	FILEFORMAT fileformatIn = FILE_NULL, fileformatOut = FILE_NULL;
	
	if(str::iff::stringEndsWith(filenameIn, ".bit"))
		fileformatIn = FILE_BIT;
	if(str::iff::stringEndsWith(filenameIn, ".bin"))
		fileformatIn = FILE_BIN;
	if(str::iff::stringEndsWith(filenameIn, ".bitasm"))
		fileformatIn = FILE_BIT_ASM;
	if(str::iff::stringEndsWith(filenameOut, ".bit"))
		fileformatOut = FILE_BIT;
	if(str::iff::stringEndsWith(filenameOut, ".bin"))
		fileformatOut = FILE_BIN;
	if(str::iff::stringEndsWith(filenameOut, ".bitasm"))
		fileformatOut = FILE_BIT_ASM;
	
	ifstream fin (filenameIn, ifstream::binary);
	if(!fin.good())
		throw runtime_error(string("Could not open file: \"").append(filenameIn).append("\" .\n"));
	
	ofstream fout (filenameOut, ofstream::binary | ofstream::trunc);
	if(!fout.good())
		throw runtime_error(string("Could not open file: \"").append(filenameOut).append("\"!\n"));
	
	if(fileformatIn == FILE_BIT && fileformatOut == FILE_BIT_ASM)
		disassemblerBitToAsm(fin, fout);
	else if(fileformatIn == FILE_BIN && fileformatOut == FILE_BIT_ASM)
		disassemblerBinToAsm(filenameIn, fin, fout);
	else if(fileformatIn == FILE_BIT_ASM && fileformatOut == FILE_BIT)
		assemblerAsmToBit(fin, fout);
	else if(fileformatIn == FILE_BIT_ASM && fileformatOut == FILE_BIN)
		assemblerAsmToBin(fin, fout);
	else
		throw runtime_error(string("Unknown Xilinx Series 7 assembler operation between file formats. See \"byteman -h assembly\".\n"));
	fin.close();
	fout.close();
}

void XilinxSeries7::disassemblerBinToAsm(string filenameIn, ifstream& fin, ofstream& fout) {
	loadedBitstreamEndianness = parseBitstreamEndianness(fin);
	uint32_t idcode = parseBitstreamIDCODE(fin, loadedBitstreamEndianness);
	setDeviceByIDCODEOrThrow(idcode);
	designName = filenameIn;
	updateDateAndTime();
	
	disassemblerToAsm(fin, fout);
}

void XilinxSeries7::disassemblerBitToAsm(ifstream& fin, ofstream& fout) {
	loadedBitstreamEndianness = parseBitstreamEndianness(fin);
	parseBITheader(fin, loadedBitstreamEndianness);
	setDeviceByPartNameOrThrow();
	
	disassemblerToAsm(fin, fout);
}
void XilinxSeries7::assemblerAsmToBit(ifstream& fin, ofstream& fout){
	assemblerParseHeader(fin);
	
	setDeviceByPartNameOrThrow();
	initializeResourceStringParameters();
	outputBITheader(fout, loadedBitstreamEndianness);
	
	assemblerAsmTo(fin, fout);
}
void XilinxSeries7::assemblerAsmToBin(ifstream& fin, ofstream& fout){
	assemblerParseHeader(fin);
	
	setDeviceByPartNameOrThrow();
	initializeResourceStringParameters();
	
	assemblerAsmTo(fin, fout);
}

void XilinxSeries7::assemblerParseHeader(ifstream& fin)
{
	for (string line; getline(fin, line); ) {
		auto firstEqPos = line.find_first_of('=');
		if(firstEqPos != string::npos)
			line.replace(firstEqPos, 1, '=', ' ');
		
		if(str::iff::stringContains(line, "Name"))designName = str::parse::lastStringWord(line);
		if(str::iff::stringContains(line, "FPGA"))partName = str::parse::lastStringWord(line);
		if(str::iff::stringContains(line, "Date"))fileDate = str::parse::lastStringWord(line);
		if(str::iff::stringContains(line, "Time"))fileTime = str::parse::lastStringWord(line);
		if(str::iff::stringContains(line, "HEADER END"))break;
	}
	log("Design name: " + designName);
	log("FPGA: " + partName);
	log("Date: " + fileDate);
	log("Time: " + fileTime);
}
void XilinxSeries7::assemblerAsmTo(ifstream& fin, ofstream& fout)
{
	outputCAPheaderConstant(fout, loadedBitstreamEndianness);
	
	XCAP::Register regAddr = XCAP::Register::UNDEFINED;
	int frameCount = 0;
	int slr = 0, b = 7, r = 0, c = 0, m = 0;
	for (string line; getline(fin, line); ) {
		if(line.at(line.find_first_not_of(" \t")) == '#')// if #, skip that line as comment
			continue;
		transform(line.begin(), line.end(),line.begin(), ::toupper);
		replace(line.begin(), line.end(), '=', ' ');
		replace(line.begin(), line.end(), '#', ' ');
		replace(line.begin(), line.end(), ',', ' ');
		if(str::iff::stringContains(line, "SYNC") && (!str::iff::stringContains(line, "DESYNC"))){
			XCAP_writeSYNQ(fout, loadedBitstreamEndianness);
		}
		else if(str::iff::stringContains(line, ".WORD")){
			uint32_t wordValue;
			if(!str::parse::multipleUints(line, wordValue))wordValue = 0;
			FileIO::write32(fout, wordValue, loadedBitstreamEndianness);
		}
		else if(str::iff::stringContains(line, "NOP")){
			int nopHiddenValue;
			if(!str::parse::multipleInts(line, nopHiddenValue))nopHiddenValue = 0;
			XCAP_writeNOP(fout, 1, nopHiddenValue, loadedBitstreamEndianness);
		}
		else if(str::iff::stringContains(line, "RESERVED")){
			int reservedHiddenValue;
			if(!str::parse::multipleInts(line, reservedHiddenValue))reservedHiddenValue = 0;
			XCAP_writeRESERVED(fout, 1, reservedHiddenValue, loadedBitstreamEndianness);
		}
		else if(str::iff::stringContains(line, "SELECT NEXT SLR")){
			slr++;
			int magic1size;
			if(!str::parse::multipleInts(line, magic1size))magic1size = 0;
			XCAP_writeSelectRegister(fout, XCAP::Register::MAGIC1, loadedBitstreamEndianness);
			XCAP_writeType2(fout, magic1size, loadedBitstreamEndianness);
		}
		else if(str::iff::stringContains(line, "@")){
			XCAP::Register newRegAddr = getXCAPregister(line);
			if(str::iff::stringContains(line, "READ REG @")){
				int readLength;
				if(!str::parse::multipleInts(line, readLength)) readLength = 1; // default read length is 1 if unspecified
				XCAP_writeReadRegister(fout, newRegAddr, readLength, loadedBitstreamEndianness);
			} else {// must be a write then ¯\_(ツ)_/¯
			
				if(newRegAddr == XCAP::Register::UNDEFINED){
					throw runtime_error(string("Couldn't parse assembly command: \"").append(line).append("\"!"));
				} else if(str::iff::stringContains(line, "SELECT REGISTER")){
					XCAP_writeSelectRegister(fout, newRegAddr, loadedBitstreamEndianness);
				} else if(newRegAddr == XCAP::Register::FDRI){
					if(!str::parse::multipleInts(line, frameCount))
						throw runtime_error(string("FDRI command needs size: \"").append(line).append("\"!"));
					int wordCount = frameCount * XS7_WORDS_PER_FRAME;
					if(regAddr == XCAP::Register::FDRI) {
						XCAP_writeType2(fout, wordCount, loadedBitstreamEndianness);
					} else {
						XCAP_writeFDRI1(fout, wordCount, loadedBitstreamEndianness);
					}
					if(frameCount > 0){
						string frameLine;
						for (string frameLine; getline(fin, frameLine); ) {
							if(frameLine.at(frameLine.find_first_not_of(" \t")) == '#')// if #, skip that line as comment
								continue;
							uint32_t frameData[XS7_WORDS_PER_FRAME];
							if(!str::parse::arrayOfUints(frameLine, XS7_WORDS_PER_FRAME, &frameData[0]))
								throw runtime_error(string("Was expecting the data for a full frame on this line: \"").append(frameLine).append("\", because I think there are ").append(to_string(frameCount)).append(" frames left."));
							//first 3 words are clk, next 90 are the 0-45 and 48-93 data words
							for (int w = XS7_WORDS_AT_CLK; w < (XS7_WORDS_AT_CLK + XS7_WORDS_BEFORE_CLK) ; w++){
								FileIO::write32(fout, frameData[w], loadedBitstreamEndianness);
							}
							for (int w = 0; w < XS7_WORDS_AT_CLK ; w++){
								FileIO::write32(fout, frameData[w], loadedBitstreamEndianness);
							}
							for (int w = (XS7_WORDS_AT_CLK + XS7_WORDS_BEFORE_CLK); w < XS7_WORDS_PER_FRAME ; w++){
								FileIO::write32(fout, frameData[w], loadedBitstreamEndianness);
							}
							
							frameCount--;
							if(frameCount == 0)
								break;
						}
						if(frameCount != 0)
							throw runtime_error(string("End of file reached while performing FDRI frame writes.!"));
					}
				} else if(newRegAddr == XCAP::Register::FAR){
					if(!str::parse::multipleInts(line, b, r, c, m))
						throw runtime_error(string("Could not parse the new FAR value: \"").append(line).append("\"!"));
					r += SLRinfo[slr].fromRow;
					uint32_t farValue = XCAP_getFAR(slr, b, r, c, m);
					XCAP_writeRegister(fout, XCAP::Register::FAR, farValue, loadedBitstreamEndianness);
				} else {
					uint32_t newValue;
					if(!str::parse::multipleUints(line, newValue))
						throw runtime_error(string("Could not parse the new register value: \"").append(line).append("\"!"));
					XCAP_writeRegister(fout, newRegAddr, newValue, loadedBitstreamEndianness);
				}
			}
			regAddr = newRegAddr;
		} else {// ! str::iff::stringContains(line, "@")
			//must have been a command then
			XCAP::Command cmdID = getXCAPcommand(line);
			if(cmdID == XCAP::Command::UNDEFINED) 
				throw runtime_error(string("Couldn't parse assembly command: \"").append(line).append("\"!"));
			XCAP_writeCommand(fout, cmdID, loadedBitstreamEndianness);
		}
	}
	outputBITheaderLengthField(fout, loadedBitstreamEndianness);
}

void XilinxSeries7::disassemblerWriteHeader(ofstream& fout)
{
	fout<<"--- HEADER BEGIN ---"<<endl;
	fout<<"Name = \""<<designName<<"\""<<endl;
	fout<<"FPGA = \""<<partName<<"\""<<endl;
	fout<<"Date = \""<<fileDate<<"\""<<endl;
	fout<<"Time = \""<<fileTime<<"\""<<endl;
	fout<<"--- HEADER END ---"<<endl;
}

void XilinxSeries7::disassemblerToAsm(ifstream& fin, ofstream& fout) {
	initializeResourceStringParameters();
	disassemblerWriteHeader(fout);
	
	XCAP::Register regAddr = XCAP::Register::UNDEFINED;
	bool synched = false;
	int wordCount = 0;
	int shadowFrameValid = 0;
	int slr = 0, b = 7, r = 0, c = 0, m = 0;
	//Parse bitstream
	for( ; ; ){
		uint32_t instruction = FileIO::read32(fin, loadedBitstreamEndianness);
		if(!fin.good()){
			break; // done with the bitstream
		} else {
			if(! synched){
				if(0xAA995566 == instruction){
					fin.seekg(-20,ios::cur); //sync only if we had the full sync sequence
					int wordOld4 = FileIO::read32(fin, loadedBitstreamEndianness);
					int wordOld3 = FileIO::read32(fin, loadedBitstreamEndianness);
					int wordOld2 = FileIO::read32(fin, loadedBitstreamEndianness);
					int wordOld1 = FileIO::read32(fin, loadedBitstreamEndianness);
					instruction = FileIO::read32(fin, loadedBitstreamEndianness);
					if(wordOld4 == 0x000000BB && wordOld3 == 0x11220044 && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF){
						synched = true;
						fout << "SYNC" << endl;
					} else {
						fout << ".word " <<"0x" << uppercase << hex << setw(8) << setfill('0') << instruction << endl;
					}
				} else {
					fout << ".word " <<"0x" << uppercase << hex << setw(8) << setfill('0') << instruction << endl;
				}
			} else {
				int instructionType = XCAP_getInstructionType(instruction);
				XCAP::Operation instructionOPCODE = XCAP_getInstructionOperation(instruction);
				int instructionPayload = XCAP_getInstructionPayload(instruction);
				if(instructionType == 1) {
					wordCount = XCAP_getInstructionWordCount(instruction);
					regAddr = XCAP_getInstructionRegister(instruction);
				} else if(instructionType == 2) {
					wordCount = instructionPayload;
				} else {
					fout << "0x" << uppercase << hex << setw(8) << setfill('0') << instruction << " (Invalid instruction [invalid type])" << endl;
					continue;
				}
				
				if(instructionOPCODE == XCAP::Operation::NOP) {
					if(instructionPayload != 0)
						fout << "NOP #" << instructionPayload << endl;
					else 
						fout << "NOP" << endl;
				} else if(instructionOPCODE == XCAP::Operation::RESERVED) {
					if(instructionPayload != 0)
						fout << "RESERVED #" << instructionPayload << endl;
					else 
						fout << "RESERVED" << endl;
				} else if(instructionOPCODE == XCAP::Operation::READ) {
					fout << "Read Reg @";
					writeXCAPregisterName(fout, regAddr);
					fout << " for length #" << wordCount << endl;
				} else { // XCAP::Operation::WRITE
					if((regAddr == XCAP::Register::FDRI) && (wordCount > 0) && (wordCount % XS7_WORDS_PER_FRAME == 0)) {
						if(shadowFrameValid) {
							fout << dec << "# Shadow register contents are written to frame (BlockType="<<b<<", GlobalRowAddress="<<r<<", MajorAddress="<<c<<", MinorAddress="<<m<<") (Frame type: " << getFrameType(b,r,c) << ")."<<endl;
							XCAP_IncrementFAR(slr, b, r, c, m);
						}
						shadowFrameValid = 1;
						int frameCount = (wordCount/XS7_WORDS_PER_FRAME);
						fout << dec << "@FDRI for #" << frameCount << " frames:" << endl;
						for(int i = 0 ; i < frameCount ; i++){
							fout << "# ";
							if(i == (frameCount-1)) fout << "(This frame data is written to shadow register!)";
							fout << dec << "Writing frame #" << i << " (BlockType="<<b<<", GlobalRowAddress="<<r<<", MajorAddress="<<c<<", MinorAddress="<<m<<") (Frame type: " << getFrameType(b,r,c) << ") hex data:"<<endl;
							uint32_t frameData[XS7_WORDS_PER_FRAME];
							for(int w = 0 ; w < XS7_WORDS_PER_FRAME ; w++){
								frameData[w] = FileIO::read32(fin, loadedBitstreamEndianness);
							}
							fout<<"CLOCK: ";
							for(int w = XS7_WORDS_BEFORE_CLK ; w < (XS7_WORDS_BEFORE_CLK + XS7_WORDS_AT_CLK) ; w++){
								fout<< "0x"  << uppercase << hex << setw(8) << setfill('0') << frameData[w] << " ";
							}
							fout<<" ;DATA: ";
							for(int w = 0 ; w < XS7_WORDS_BEFORE_CLK ; w++){
								fout<< "0x"  << uppercase << hex << setw(8) << setfill('0') << frameData[w] << " ";
							}
							for(int w = (XS7_WORDS_BEFORE_CLK + XS7_WORDS_AT_CLK) ; w < XS7_WORDS_PER_FRAME ; w++){
								fout<< "0x"  << uppercase << hex << setw(8) << setfill('0') << frameData[w] << " ";
							}
							fout << endl;
							XCAP_IncrementFAR(slr, b, r, c, m);
						}
					} else if(regAddr == XCAP::Register::CMD && wordCount == 1){
						uint32_t writeData = FileIO::read32(fin, loadedBitstreamEndianness);
						writeXCAPcommandName(fout, static_cast<XCAP::Command>(writeData));
						fout << ";";
						if(XCAP::Command::DESYNC == static_cast<XCAP::Command>(writeData)){
							shadowFrameValid = 0;
							synched = false;
						}
						if(XCAP::Command::WCFG == static_cast<XCAP::Command>(writeData)){
							shadowFrameValid = 0;
							fout << " (also clears shadow register)";
						}
						fout << endl;
					} else if(regAddr == XCAP::Register::MAGIC1){
						uint32_t nextInstr = FileIO::read32(fin, loadedBitstreamEndianness);
						int nextInstrType = XCAP_getInstructionType(nextInstr);
						XCAP::Operation nextInstrOP = XCAP_getInstructionOperation(nextInstr);
						int nextInstrPayload = XCAP_getInstructionPayload(nextInstr);
						if(2 == nextInstrType && XCAP::Operation::WRITE == nextInstrOP && 0 < nextInstrPayload){
							slr++;
							synched = false;
							fout << "Select next SLR for the next #"<<dec<<nextInstrPayload<<" words." << endl;
						} else {
							fout << "Bad MAGIC1 instruction" << endl;
							fin.seekg(-4,ios::cur);//rewind next instruction if not
						}
					} else if((instructionType == 1) && (wordCount == 0)){
						fout << "Select register @";
						writeXCAPregisterName(fout, regAddr);
						fout << endl;
					} else if(wordCount == 1){
						uint32_t writeData = FileIO::read32(fin, loadedBitstreamEndianness);
						fout << "@";
						writeXCAPregisterName(fout, regAddr);
						if(regAddr == XCAP::Register::FAR) {
							XCAP_parseFAR(writeData, slr, b, r, c, m);
							fout << " = BlockType=" << dec <<b<<" RowAddress="<<(r-SLRinfo[slr].fromRow)<<" MajorAddress="<<c<<" MinorAddress="<<m<< endl;
						} else {
							fout << " = 0x" << uppercase << hex << setw(8) << setfill('0') << writeData << endl;
						}
					} else {
						fout << "0x"<< uppercase << hex << setw(8) << setfill('0') << instruction << "(Bad instruction)" << endl;
					}
				} // OP_WRITE
			}// if synched
		} // if fin is still good
	} // for(;;)
}
