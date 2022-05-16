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
#include "../../../Common/parseString.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <fstream>
#include <cstring>
#include <iomanip>      // std::setfill, std::setw
using namespace std;

void XilinxUltraScalePlus::assemblerHelp(){
	cout<<"Supported file formats are \".bit\" and \".bit.asm\" at the moment." << endl;
}
void XilinxUltraScalePlus::assembler(string filenameIn, string filenameOut){
	enum FILEFORMAT {FILE_NULL = 0, FILE_BIT, FILE_BIT_ASM};
	FILEFORMAT fileformatIn = FILE_NULL, fileformatOut = FILE_NULL;
	
    if(fileExtensionIs(filenameIn, "bit"))
        fileformatIn = FILE_BIT;
    if(fileExtensionIs(filenameIn, "bitasm"))
        fileformatIn = FILE_BIT_ASM;
	
    if(fileExtensionIs(filenameOut, "bit"))
        fileformatOut = FILE_BIT;
    if(fileExtensionIs(filenameOut, "bitasm"))
        fileformatOut = FILE_BIT_ASM;
	if(fileformatIn == FILE_NULL)
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be read by the assembler. See \"byteman -h assembly\".\n"));
	if(fileformatOut == FILE_NULL)
        throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be written by the assembler. See \"byteman -h assembly\".\n"));
	if(fileformatIn == fileformatOut)
        throw runtime_error(string("Unknown Xilinx UltraScale+ assembler operation between identical file formats. See \"byteman -h assembly\".\n"));
	
	
	ifstream fin (filenameIn, ifstream::binary);
	if(!fin.good())
        throw runtime_error(string("Could not open file: \"").append(filenameIn).append("\" .\n"));
	
	ofstream fout (filenameOut, ofstream::binary | ofstream::trunc);
	if(!fout.good())
        throw runtime_error(string("Could not open file: \"").append(filenameOut).append("\"!\n"));
	
	if(fileformatIn == FILE_BIT && fileformatOut == FILE_BIT_ASM)
		XilinxUltraScalePlus::disassemblerBitToAsm(fin, fout);
	if(fileformatIn == FILE_BIT_ASM && fileformatOut == FILE_BIT)
		XilinxUltraScalePlus::assemblerAsmToBit(fin, fout);
	
	fin.close();
	fout.close();
}
int assemblerGetRegID(string line){
	#define has(x) (line.find(x) != string::npos)
	if(has("CRC"))return XUSP_CAP_REG_CRC;
	else if(has("FAR"))return XUSP_CAP_REG_FAR;
	else if(has("FDRI"))return XUSP_CAP_REG_FDRI;
	else if(has("FDRO"))return XUSP_CAP_REG_FDRO;
	else if(has("CTRL0"))return XUSP_CAP_REG_CTRL0;
	else if(has("MASK"))return XUSP_CAP_REG_MASK;
	else if(has("STAT"))return XUSP_CAP_REG_STAT;
	else if(has("LOUT"))return XUSP_CAP_REG_LOUT;
	else if(has("COR0"))return XUSP_CAP_REG_COR0;
	else if(has("MFWR"))return XUSP_CAP_REG_MFWR;
	else if(has("CBC"))return XUSP_CAP_REG_CBC;
	else if(has("IDCODE"))return XUSP_CAP_REG_IDCODE;
	else if(has("AXSS"))return XUSP_CAP_REG_AXSS;
	else if(has("COR1"))return XUSP_CAP_REG_COR1;
	else if(has("WBSTAR"))return XUSP_CAP_REG_WBSTAR;
	else if(has("TIMER"))return XUSP_CAP_REG_TIMER;
	else if(has("MAGIC0"))return XUSP_CAP_REG_MAGIC0;
	else if(has("BOOTSTS"))return XUSP_CAP_REG_BOOTSTS;
	else if(has("CTRL1"))return XUSP_CAP_REG_CTRL1;
	else if(has("MAGIC1"))return XUSP_CAP_REG_MAGIC1;
	else if(has("BSPI"))return XUSP_CAP_REG_BSPI;
	return XUSP_CAP_REG_UNKNOWN;
	#undef has
}
int assemblerGetCommandID(string line){
	#define has(x) (line.find(x) != string::npos)
	#define hOR(x,y) (has(x)||has(y))
	if(has("NULL"))return XUSP_CAP_CMD_NULL;
	else if(hOR("WCFG", "WRITE CONFIG"))return XUSP_CAP_CMD_WCFG;
	else if(hOR("MFW", "MULTIPLE FRAME WRITE"))return XUSP_CAP_CMD_MFW;
	else if(hOR("DGHIGH", "DEASSERT GHIGH"))return XUSP_CAP_CMD_DGHIGH;
	else if(hOR("RCFG", "READ CONFIG"))return XUSP_CAP_CMD_RCFG;
	else if(has("START"))return XUSP_CAP_CMD_START;
	else if(hOR("URAM", "CLEAR URAM"))return XUSP_CAP_CMD_URAM;
	else if(hOR("RCRC", "RESET CRC"))return XUSP_CAP_CMD_RCRC;
	else if(hOR("AGHIGH", "ASSERT GHIGH"))return XUSP_CAP_CMD_AGHIGH;
	else if(has("SWITCH"))return XUSP_CAP_CMD_SWITCH;
	else if(has("GRESTORE"))return XUSP_CAP_CMD_GRESTORE;
	else if(has("SHUTDOWN"))return XUSP_CAP_CMD_SHUTDOWN;
	else if(has("DESYNC"))return XUSP_CAP_CMD_DESYNC;
	else if(has("IPROG"))return XUSP_CAP_CMD_IPROG;
	else if(has("CRCC"))return XUSP_CAP_CMD_CRCC;
	else if(has("LTIMER"))return XUSP_CAP_CMD_LTIMER;
	else if(has("BSPI_READ"))return XUSP_CAP_CMD_BSPI_READ;
	else if(has("FALL_EDGE"))return XUSP_CAP_CMD_FALL_EDGE;
	return XUSP_CAP_CMD_UNKNOWN;
	#undef has
	#undef hOR
}
void XilinxUltraScalePlus::assemblerAsmToBit(ifstream& fin, ofstream& fout){
	
	for (string line; getline(fin, line); ) {
		auto firstEqPos = line.find_first_of('=');
		if(firstEqPos != string::npos)
			line.replace(firstEqPos, 1, '=', ' ');
		#define has(x) (line.find(x) != string::npos)
		if(has("Name"))designName = parseString::removeQuotes(parseString::getLastWord(line));
		if(has("FPGA"))partName = parseString::removeQuotes(parseString::getLastWord(line));
		if(has("Date"))fileDate = parseString::removeQuotes(parseString::getLastWord(line));
		if(has("Time"))fileTime = parseString::removeQuotes(parseString::getLastWord(line));
		if(has("HEADER END"))break;
		#undef has
	}
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByNameOrThrow(partName), partName);
	XilinxUltraScalePlus::initializeResourceStringParameters();
	streamoff headerLocationOfRemainingFileLength = XilinxUltraScalePlus::outputBITheader(fout);
	
	int regAddr = XUSP_CAP_REG_UNKNOWN;
    int frameCount = 0;
    int b = 7, r = 0, c = 0, m = 0;
	for (string line; getline(fin, line); ) {
		#define has(x) (line.find(x) != string::npos)
		#define hOR(x,y) (has(x)||has(y))
		if(line.at(line.find_first_not_of(" \t")) == '#')// if #, skip that line as comment
			continue;
		transform(line.begin(), line.end(),line.begin(), ::toupper);
		replace(line.begin(), line.end(), '=', ' ');
		replace(line.begin(), line.end(), '#', ' ');
		replace(line.begin(), line.end(), ',', ' ');
		if(has("NOP")){
			int nopHiddenValue;
			if(!parseString::getMultipleInts(line, nopHiddenValue))nopHiddenValue = 0;
			CAPwriteNOPwData(fout, nopHiddenValue);
		}
		else if(has("RESERVED")){
			int reservedHiddenValue;
			if(!parseString::getMultipleInts(line, reservedHiddenValue))reservedHiddenValue = 0;
			CAPwriteRESERVEDwData(fout, reservedHiddenValue);
		}
		else if(has("@")){
			int newRegAddr = assemblerGetRegID(line);
			if(has("READ REG @")){
				int readLength;
				if(!parseString::getMultipleInts(line, readLength)) readLength = 1; // default read length is 1 if unspecified
				CAPreadReg(fout, newRegAddr, readLength);
			} else {// must be a write then ¯\_(ツ)_/¯
			
				if(newRegAddr == XUSP_CAP_REG_UNKNOWN){
					throw runtime_error(string("Couldn't parse assembly command: \"").append(line).append("\"!"));
				} else if(has("SELECT REGISTER")){
					CAPselectReg(fout, newRegAddr);
				} else if(newRegAddr == XUSP_CAP_REG_FDRI){
					if(!parseString::getMultipleInts(line, frameCount))
						throw runtime_error(string("FDRI command needs size: \"").append(line).append("\"!"));
					int wordCount = frameCount * XUSP_WORDS_PER_FRAME;
					if(regAddr == XUSP_CAP_REG_FDRI) {
						CAPwriteFDRI2(fout, wordCount);
					} else {
						CAPwriteFDRI1(fout, wordCount);
					}
					string frameLine;
					for (string frameLine; getline(fin, frameLine); ) {
						if(frameLine.at(frameLine.find_first_not_of(" \t")) == '#')// if #, skip that line as comment
							continue;
						uint32_t frameData[XUSP_WORDS_PER_FRAME];
						if(!parseString::getArrayOfUints(frameLine, XUSP_WORDS_PER_FRAME, &frameData[0]))
							throw runtime_error(string("Was expecting the data for a full frame on this line: \"").append(frameLine).append("\", because I think there are ").append(to_string(frameCount)).append(" frames left."));
						//first 3 words are clk, next 90 are the 0-45 and 48-93 data words
						for (int w = XUSP_WORDS_AT_CLK; w < (XUSP_WORDS_AT_CLK + XUSP_WORDS_BEFORE_CLK) ; w++){
							fileWriteBE(fout, 32, frameData[w]);
						}
						for (int w = 0; w < XUSP_WORDS_AT_CLK ; w++){
							fileWriteBE(fout, 32, frameData[w]);
						}
						for (int w = (XUSP_WORDS_AT_CLK + XUSP_WORDS_BEFORE_CLK); w < XUSP_WORDS_PER_FRAME ; w++){
							fileWriteBE(fout, 32, frameData[w]);
						}
						
						frameCount--;
						if(frameCount == 0)
							break;
					}
					if(frameCount != 0)
						throw runtime_error(string("End of file reached while performing FDRI frame writes.!"));
				} else if(newRegAddr == XUSP_CAP_REG_FAR){
					if(!parseString::getMultipleInts(line, b, r, c, m))
						throw runtime_error(string("Could not parse the new FAR value: \"").append(line).append("\"!"));
					CAPwriteFAR(fout, b, r, c, m);
				} else {
					int newValue;
					if(!parseString::getMultipleInts(line, newValue))
						throw runtime_error(string("Could not parse the new register value: \"").append(line).append("\"!"));
					CAPwriteReg(fout, newRegAddr, newValue);
				}
			}
			regAddr = newRegAddr;
		} else {// ! has("@")
			//must have been a command then
			int cmdID = assemblerGetCommandID(line);
			if(cmdID == XUSP_CAP_CMD_UNKNOWN) 
				throw runtime_error(string("Couldn't parse assembly command: \"").append(line).append("\"!"));
			CAPwriteReg(fout, XUSP_CAP_REG_CMD, cmdID);
		}
		#undef has
		#undef hOR
	}
	XilinxUltraScalePlus::outputBITheaderLengthField(fout, headerLocationOfRemainingFileLength);
}
void writeCommandName(ofstream& fout, int cmdNum){
	switch(cmdNum){
		case XUSP_CAP_CMD_NULL: fout << "Null Command"; break;
		case XUSP_CAP_CMD_WCFG: fout << "Write Config"; break;
		case XUSP_CAP_CMD_MFW: fout << "Multiple Frame Write"; break;
		case XUSP_CAP_CMD_DGHIGH: fout << "Deassert GHIGH"; break; // TODO: XUSP_CAP_CMD_DGHIGH and XUSP_CAP_CMD_LFRM are the same
		case XUSP_CAP_CMD_RCFG: fout << "Read Config"; break;
		case XUSP_CAP_CMD_START: fout << "Start"; break;
		case XUSP_CAP_CMD_URAM: fout << "Clear URAM"; break;
		case XUSP_CAP_CMD_RCRC: fout << "Reset CRC"; break;
		case XUSP_CAP_CMD_AGHIGH: fout << "Assert GHIGH"; break;
		case XUSP_CAP_CMD_SWITCH: fout << "Switch"; break;
		case XUSP_CAP_CMD_GRESTORE: fout << "GRestore"; break;
		case XUSP_CAP_CMD_SHUTDOWN: fout << "Shutdown"; break;
		case XUSP_CAP_CMD_DESYNC: fout << "Desync"; break;
		case XUSP_CAP_CMD_IPROG: fout << "IProg"; break;
		case XUSP_CAP_CMD_CRCC: fout << "CRCC"; break;
		case XUSP_CAP_CMD_LTIMER: fout << "LTimer"; break;
		case XUSP_CAP_CMD_BSPI_READ: fout << "BSPI_Read"; break;
		case XUSP_CAP_CMD_FALL_EDGE: fout << "Fall_Edge"; break;
		default: fout << "UNKNOWN"; break;
	}
}
void writeRegisterName(ofstream& fout, int regNum){
	switch(regNum){
		case XUSP_CAP_REG_CRC: fout << "CRC"; break;
		case XUSP_CAP_REG_FAR: fout << "FAR"; break;
		case XUSP_CAP_REG_FDRI: fout << "FDRI"; break;
		case XUSP_CAP_REG_FDRO: fout << "FDRO"; break;
		case XUSP_CAP_REG_CMD: fout << "CMD"; break;
		case XUSP_CAP_REG_CTRL0: fout << "CTRL0"; break;
		case XUSP_CAP_REG_MASK: fout << "MASK"; break;
		case XUSP_CAP_REG_STAT: fout << "STAT"; break;
		case XUSP_CAP_REG_LOUT: fout << "LOUT"; break;
		case XUSP_CAP_REG_COR0: fout << "COR0"; break;
		case XUSP_CAP_REG_MFWR: fout << "MFWR"; break;
		case XUSP_CAP_REG_CBC: fout << "CBC"; break;
		case XUSP_CAP_REG_IDCODE: fout << "IDCODE"; break;
		case XUSP_CAP_REG_AXSS: fout << "AXSS"; break;
		case XUSP_CAP_REG_COR1: fout << "COR1"; break;
		case XUSP_CAP_REG_WBSTAR: fout << "WBSTAR"; break;
		case XUSP_CAP_REG_TIMER: fout << "TIMER"; break;
		case XUSP_CAP_REG_MAGIC0: fout << "MAGIC0"; break;
		case XUSP_CAP_REG_BOOTSTS: fout << "BOOTSTS"; break;
		case XUSP_CAP_REG_CTRL1: fout << "CTRL1"; break;
		case XUSP_CAP_REG_MAGIC1: fout << "MAGIC1"; break;
		case XUSP_CAP_REG_BSPI: fout << "BSPI"; break;
		default: fout << "UNKNOWN"; break;
	}
}
void XilinxUltraScalePlus::disassemblerBitToAsm(ifstream& fin, ofstream& fout){
	XilinxUltraScalePlus::readBITheader(fin);
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByNameOrThrow(partName), partName);
	XilinxUltraScalePlus::initializeResourceStringParameters();
	XilinxUltraScalePlus::disassemblerWriteHeader(fout);
	
	int regAddr = XUSP_CAP_REG_UNKNOWN;
    int wordCount = 0;
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
				fout << "0x" << hex << setw(8) << setfill('0') << instruction << " (Invalid instruction [invalid type])" << endl;
				continue;
			}
			
			if(instructionOPCODE == OP_NOP) {
				if(instructionPayload != 0)
					fout << "NOP #" << instructionPayload << endl;
				else 
					fout << "NOP" << endl;
			} else if(instructionOPCODE == OP_RESERVED) {
				if(instructionPayload != 0)
					fout << "RESERVED #" << instructionPayload << endl;
				else 
					fout << "RESERVED" << endl;
			} else if(instructionOPCODE == OP_READ) {
				fout << "Read Reg @";
				writeRegisterName(fout, regAddr);
				fout << " for length #" << wordCount << endl;
			} else { // OP_WRITE
				if((regAddr == XUSP_CAP_REG_FDRI) && (wordCount > 0) && (wordCount % XUSP_WORDS_PER_FRAME == 0)) {
					if(shadowFrameValid) {
						fout << "# Shadow register contents are written to frame (BlockType="<<b<<", RowAddress="<<r<<", MajorAddress="<<c<<", MinorAddress"<<m<<") (Frame type: " << XUSP_GET_FRAMETYPE(b,r,c) << ")."<<endl;
						XUSP_INCR_FAR(b,r,c,m)
					}
					shadowFrameValid = 1;
					int frameCount = (wordCount/XUSP_WORDS_PER_FRAME);
					fout << dec << "@FDRI for #" << frameCount << " frames:" << endl;
					for(int i = 0 ; i < frameCount ; i++){
						fout << "# ";
						if(i == (frameCount-1)) fout << "(This frame data is written to shadow register!)";
						fout << dec << "Writing frame #" << i << " (BlockType="<<b<<", RowAddress="<<r<<", MajorAddress="<<c<<", MinorAddress="<<m<<") (Frame type: " << XUSP_GET_FRAMETYPE(b,r,c) << ") hex data:"<<endl;
						uint32_t frameData[XUSP_WORDS_PER_FRAME];
						for(int w = 0 ; w < XUSP_WORDS_PER_FRAME ; w++){
							fileReadBE(frameData[w], fin, 32);
						}
						fout<<"CLOCK: ";
						for(int w = XUSP_WORDS_BEFORE_CLK ; w < (XUSP_WORDS_BEFORE_CLK + XUSP_WORDS_AT_CLK) ; w++){
							fout<< "0x"  << hex << setw(8) << setfill('0') << frameData[w] << " ";
						}
						fout<<" ;DATA: ";
						for(int w = 0 ; w < XUSP_WORDS_BEFORE_CLK ; w++){
							fout<< "0x"  << hex << setw(8) << setfill('0') << frameData[w] << " ";
						}
						for(int w = (XUSP_WORDS_BEFORE_CLK + XUSP_WORDS_AT_CLK) ; w < XUSP_WORDS_PER_FRAME ; w++){
							fout<< "0x"  << hex << setw(8) << setfill('0') << frameData[w] << " ";
						}
						fout << endl;
						XUSP_INCR_FAR(b,r,c,m)
					}
				} else if(regAddr == XUSP_CAP_REG_CMD && wordCount == 1){
					fileReadAndDefineBE(writeData, fin, 32);
					writeCommandName(fout, writeData);
					fout << ";";
					if(writeData == XUSP_CAP_CMD_WCFG){
						shadowFrameValid = 0;
						fout << " (also clears shadow register)";
					}
					fout << endl;
				} else if((instructionType == 1) && (wordCount == 0)){
					fout << "Select register @";
					writeRegisterName(fout, regAddr);
					fout << endl;
				} else if(wordCount == 1){
					fileReadAndDefineBE(writeData, fin, 32);
					fout << "@";
					writeRegisterName(fout, regAddr);
					if(regAddr == XUSP_CAP_REG_FAR) {
						parseFAR(writeData,b,r,c,m);
						fout << " = BlockType=" << dec <<b<<" RowAddress="<<r<<" MajorAddress="<<c<<" MinorAddress="<<m<< endl;
					} else {
						fout << " = 0x" << hex << setw(8) << setfill('0') << writeData << endl;
					}
				} else {
					fout << "0x"<< hex << setw(8) << setfill('0') << instruction << "(Bad instruction)" << endl;
				}
			} // OP_WRITE
		} // if fin is still good
	} // for(;;)
}
void XilinxUltraScalePlus::disassemblerWriteHeader(ofstream& fout){
	fout<<"--- HEADER BEGIN ---"<<endl;
	fout<<"Name = \""<<designName<<"\""<<endl;
	fout<<"FPGA = \""<<partName<<"\""<<endl;
	fout<<"Date = \""<<fileDate<<"\""<<endl;
	fout<<"Time = \""<<fileTime<<"\""<<endl;
	fout<<"--- HEADER END ---"<<endl;
}
