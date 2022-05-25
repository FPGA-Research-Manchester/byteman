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
#include<iomanip>      // std::setfill, std::setw

#include "XilinxUltraScalePlus.h"
#include "XilinxUltraScalePlusConfigurationAccessPort.h"
#include "XilinxUltraScalePlusConfigurationAccessPortInlinedFunctions.cpp"
#include "../../../Common/FileIO.h"
#include "../../../Common/str.h"
#include "../../../Common/Endianess.h"

using namespace std;

void XilinxUltraScalePlus::assemblerHelp()
{
	cout<<"Supported file formats are \".bit\" and \".bit.asm\" at the moment." << endl;
}
void XilinxUltraScalePlus::assembler(string filenameIn, string filenameOut)
{
	enum FILEFORMAT {FILE_NULL = 0, FILE_BIT, FILE_BIT_ASM};
	FILEFORMAT fileformatIn = FILE_NULL, fileformatOut = FILE_NULL;
	
    if(str::iff::stringEndsWith(filenameIn, ".bit"))
        fileformatIn = FILE_BIT;
    if(str::iff::stringEndsWith(filenameIn, ".bitasm"))
        fileformatIn = FILE_BIT_ASM;
	
    if(str::iff::stringEndsWith(filenameOut, ".bit"))
        fileformatOut = FILE_BIT;
    if(str::iff::stringEndsWith(filenameOut, ".bitasm"))
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
CAP::Register assemblerGetRegID(string line)
{
	#define has(x) (line.find(x) != string::npos)
	if(has("CRC"))return CAP::Register::CRC;
	else if(has("FAR"))return CAP::Register::FAR;
	else if(has("FDRI"))return CAP::Register::FDRI;
	else if(has("FDRO"))return CAP::Register::FDRO;
	else if(has("CTRL0"))return CAP::Register::CTRL0;
	else if(has("MASK"))return CAP::Register::MASK;
	else if(has("STAT"))return CAP::Register::STAT;
	else if(has("LOUT"))return CAP::Register::LOUT;
	else if(has("COR0"))return CAP::Register::COR0;
	else if(has("MFWR"))return CAP::Register::MFWR;
	else if(has("CBC"))return CAP::Register::CBC;
	else if(has("IDCODE"))return CAP::Register::IDCODE;
	else if(has("AXSS"))return CAP::Register::AXSS;
	else if(has("COR1"))return CAP::Register::COR1;
	else if(has("WBSTAR"))return CAP::Register::WBSTAR;
	else if(has("TIMER"))return CAP::Register::TIMER;
	else if(has("MAGIC0"))return CAP::Register::MAGIC0;
	else if(has("BOOTSTS"))return CAP::Register::BOOTSTS;
	else if(has("CTRL1"))return CAP::Register::CTRL1;
	else if(has("MAGIC1"))return CAP::Register::MAGIC1;
	else if(has("BSPI"))return CAP::Register::BSPI;
	return CAP::Register::UNDEFINED;
	#undef has
}
CAP::Command assemblerGetCommandID(string line)
{
	#define has(x) (line.find(x) != string::npos)
	#define hOR(x,y) (has(x)||has(y))
	if(has("NULL"))return CAP::Command::NULLCMD;
	else if(hOR("WCFG", "WRITE CONFIG"))return CAP::Command::WCFG;
	else if(hOR("MFW", "MULTIPLE FRAME WRITE"))return CAP::Command::MFW;
	else if(hOR("DGHIGH", "DEASSERT GHIGH"))return CAP::Command::DGHIGH;
	else if(hOR("RCFG", "READ CONFIG"))return CAP::Command::RCFG;
	else if(has("START"))return CAP::Command::START;
	else if(hOR("URAM", "CLEAR URAM"))return CAP::Command::URAM;
	else if(hOR("RCRC", "RESET CRC"))return CAP::Command::RCRC;
	else if(hOR("AGHIGH", "ASSERT GHIGH"))return CAP::Command::AGHIGH;
	else if(has("SWITCH"))return CAP::Command::SWITCH;
	else if(has("GRESTORE"))return CAP::Command::GRESTORE;
	else if(has("SHUTDOWN"))return CAP::Command::SHUTDOWN;
	else if(has("DESYNC"))return CAP::Command::DESYNC;
	else if(has("IPROG"))return CAP::Command::IPROG;
	else if(has("CRCC"))return CAP::Command::CRCC;
	else if(has("LTIMER"))return CAP::Command::LTIMER;
	else if(has("BSPI_READ"))return CAP::Command::BSPI_READ;
	else if(has("FALL_EDGE"))return CAP::Command::FALL_EDGE;
	return CAP::Command::UNDEFINED;
	#undef has
	#undef hOR
}
void XilinxUltraScalePlus::assemblerAsmToBit(ifstream& fin, ofstream& fout)
{
	for (string line; getline(fin, line); ) {
		auto firstEqPos = line.find_first_of('=');
		if(firstEqPos != string::npos)
			line.replace(firstEqPos, 1, '=', ' ');
		#define has(x) (line.find(x) != string::npos)
		if(has("Name"))designName = str::removeExternalQuotes(str::parse::lastStringWord(line));
		if(has("FPGA"))partName = str::removeExternalQuotes(str::parse::lastStringWord(line));
		if(has("Date"))fileDate = str::removeExternalQuotes(str::parse::lastStringWord(line));
		if(has("Time"))fileTime = str::removeExternalQuotes(str::parse::lastStringWord(line));
		if(has("HEADER END"))break;
		#undef has
	}
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByNameOrThrow(partName), partName);
	XilinxUltraScalePlus::initializeResourceStringParameters();
	streamoff headerLocationOfRemainingFileLength = XilinxUltraScalePlus::outputBITheader(fout, Endianess::BE);
	
	CAP::Register regAddr = CAP::Register::UNDEFINED;
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
			if(!str::parse::multipleInts(line, nopHiddenValue))nopHiddenValue = 0;
			XilinxUltraScalePlus::CAP_writeNOP(fout, 1, nopHiddenValue, Endianess::BE);
		}
		else if(has("RESERVED")){
			int reservedHiddenValue;
			if(!str::parse::multipleInts(line, reservedHiddenValue))reservedHiddenValue = 0;
			XilinxUltraScalePlus::CAP_writeRESERVED(fout, 1, reservedHiddenValue, Endianess::BE);
		}
		else if(has("@")){
			CAP::Register newRegAddr = assemblerGetRegID(line);
			if(has("READ REG @")){
				int readLength;
				if(!str::parse::multipleInts(line, readLength)) readLength = 1; // default read length is 1 if unspecified
				XilinxUltraScalePlus::CAP_writeReadRegister(fout, newRegAddr, readLength, Endianess::BE);
			} else {// must be a write then ¯\_(ツ)_/¯
			
				if(newRegAddr == CAP::Register::UNDEFINED){
					throw runtime_error(string("Couldn't parse assembly command: \"").append(line).append("\"!"));
				} else if(has("SELECT REGISTER")){
					XilinxUltraScalePlus::CAP_writeSelectRegister(fout, newRegAddr, Endianess::BE);
				} else if(newRegAddr == CAP::Register::FDRI){
					if(!str::parse::multipleInts(line, frameCount))
						throw runtime_error(string("FDRI command needs size: \"").append(line).append("\"!"));
					int wordCount = frameCount * XUSP_WORDS_PER_FRAME;
					if(regAddr == CAP::Register::FDRI) {
						XilinxUltraScalePlus::CAP_writeFDRI2(fout, wordCount, Endianess::BE);
					} else {
						XilinxUltraScalePlus::CAP_writeFDRI1(fout, wordCount, Endianess::BE);
					}
					if(frameCount > 0){
						string frameLine;
						for (string frameLine; getline(fin, frameLine); ) {
							if(frameLine.at(frameLine.find_first_not_of(" \t")) == '#')// if #, skip that line as comment
								continue;
							uint32_t frameData[XUSP_WORDS_PER_FRAME];
							if(!str::parse::arrayOfUints(frameLine, XUSP_WORDS_PER_FRAME, &frameData[0]))
								throw runtime_error(string("Was expecting the data for a full frame on this line: \"").append(frameLine).append("\", because I think there are ").append(to_string(frameCount)).append(" frames left."));
							//first 3 words are clk, next 90 are the 0-45 and 48-93 data words
							for (int w = XUSP_WORDS_AT_CLK; w < (XUSP_WORDS_AT_CLK + XUSP_WORDS_BEFORE_CLK) ; w++){
								FileIO::write32(fout, frameData[w], Endianess::BE);
							}
							for (int w = 0; w < XUSP_WORDS_AT_CLK ; w++){
								FileIO::write32(fout, frameData[w], Endianess::BE);
							}
							for (int w = (XUSP_WORDS_AT_CLK + XUSP_WORDS_BEFORE_CLK); w < XUSP_WORDS_PER_FRAME ; w++){
								FileIO::write32(fout, frameData[w], Endianess::BE);
							}
							
							frameCount--;
							if(frameCount == 0)
								break;
						}
						if(frameCount != 0)
							throw runtime_error(string("End of file reached while performing FDRI frame writes.!"));
					}
				} else if(newRegAddr == CAP::Register::FAR){
					if(!str::parse::multipleInts(line, b, r, c, m))
						throw runtime_error(string("Could not parse the new FAR value: \"").append(line).append("\"!"));
					uint32_t farValue = XilinxUltraScalePlus::CAP_makeFAR(b, r, c, m);
					XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::FAR, farValue, Endianess::BE);
				} else {
					int newValue;
					if(!str::parse::multipleInts(line, newValue))
						throw runtime_error(string("Could not parse the new register value: \"").append(line).append("\"!"));
					XilinxUltraScalePlus::CAP_writeRegister(fout, newRegAddr, newValue, Endianess::BE);
				}
			}
			regAddr = newRegAddr;
		} else {// ! has("@")
			//must have been a command then
			CAP::Command cmdID = assemblerGetCommandID(line);
			if(cmdID == CAP::Command::UNDEFINED) 
				throw runtime_error(string("Couldn't parse assembly command: \"").append(line).append("\"!"));
			XilinxUltraScalePlus::CAP_writeCommand(fout, cmdID, Endianess::BE);
		}
		#undef has
		#undef hOR
	}
	XilinxUltraScalePlus::outputBITheaderLengthField(fout, headerLocationOfRemainingFileLength, Endianess::BE);
}
void writeCommandName(ofstream& fout, CAP::Command cmdNum)
{
	switch(cmdNum){
		case CAP::Command::NULLCMD: fout << "Null Command"; break;
		case CAP::Command::WCFG: fout << "Write Config"; break;
		case CAP::Command::MFW: fout << "Multiple Frame Write"; break;
		case CAP::Command::DGHIGH: fout << "Deassert GHIGH"; break;
		case CAP::Command::RCFG: fout << "Read Config"; break;
		case CAP::Command::START: fout << "Start"; break;
		case CAP::Command::URAM: fout << "Clear URAM"; break;
		case CAP::Command::RCRC: fout << "Reset CRC"; break;
		case CAP::Command::AGHIGH: fout << "Assert GHIGH"; break;
		case CAP::Command::SWITCH: fout << "Switch"; break;
		case CAP::Command::GRESTORE: fout << "GRestore"; break;
		case CAP::Command::SHUTDOWN: fout << "Shutdown"; break;
		case CAP::Command::DESYNC: fout << "Desync"; break;
		case CAP::Command::IPROG: fout << "IProg"; break;
		case CAP::Command::CRCC: fout << "CRCC"; break;
		case CAP::Command::LTIMER: fout << "LTimer"; break;
		case CAP::Command::BSPI_READ: fout << "BSPI_Read"; break;
		case CAP::Command::FALL_EDGE: fout << "Fall_Edge"; break;
		default: fout << "UNKNOWN"; break;
	}
}
void writeRegisterName(ofstream& fout, CAP::Register regNum)
{
	switch(regNum){
		case CAP::Register::CRC: fout << "CRC"; break;
		case CAP::Register::FAR: fout << "FAR"; break;
		case CAP::Register::FDRI: fout << "FDRI"; break;
		case CAP::Register::FDRO: fout << "FDRO"; break;
		case CAP::Register::CMD: fout << "CMD"; break;
		case CAP::Register::CTRL0: fout << "CTRL0"; break;
		case CAP::Register::MASK: fout << "MASK"; break;
		case CAP::Register::STAT: fout << "STAT"; break;
		case CAP::Register::LOUT: fout << "LOUT"; break;
		case CAP::Register::COR0: fout << "COR0"; break;
		case CAP::Register::MFWR: fout << "MFWR"; break;
		case CAP::Register::CBC: fout << "CBC"; break;
		case CAP::Register::IDCODE: fout << "IDCODE"; break;
		case CAP::Register::AXSS: fout << "AXSS"; break;
		case CAP::Register::COR1: fout << "COR1"; break;
		case CAP::Register::WBSTAR: fout << "WBSTAR"; break;
		case CAP::Register::TIMER: fout << "TIMER"; break;
		case CAP::Register::MAGIC0: fout << "MAGIC0"; break;
		case CAP::Register::BOOTSTS: fout << "BOOTSTS"; break;
		case CAP::Register::CTRL1: fout << "CTRL1"; break;
		case CAP::Register::MAGIC1: fout << "MAGIC1"; break;
		case CAP::Register::BSPI: fout << "BSPI"; break;
		default: fout << "UNKNOWN"; break;
	}
}
void XilinxUltraScalePlus::disassemblerBitToAsm(ifstream& fin, ofstream& fout)
{
	loadedBitstreamEndianess = XilinxUltraScalePlus::parseBitstreamEndianess(fin);
	XilinxUltraScalePlus::parseBITheader(fin, loadedBitstreamEndianess);
    XilinxUltraScalePlus::setDevice(XilinxUltraScalePlus::getDeviceByNameOrThrow(partName), partName);
	XilinxUltraScalePlus::initializeResourceStringParameters();
	XilinxUltraScalePlus::disassemblerWriteHeader(fout);
	
	CAP::Register regAddr = CAP::Register::UNDEFINED;
    int wordCount = 0;
    int shadowFrameValid = 0;
    int b = 7, r = 0, c = 0, m = 0;
    //Parse bitstream
    for( ; ; ){
		uint32_t instruction = FileIO::read32(fin, Endianess::BE);
        if(!fin.good()){
            break; // done with the bitstream
        } else {
			int instructionType = XilinxUltraScalePlus::CAP_getInstructionType(instruction);
			CAP::Operation instructionOPCODE = XilinxUltraScalePlus::CAP_getInstructionOperation(instruction);
			int instructionPayload = XilinxUltraScalePlus::CAP_getInstructionPayload(instruction);
			if(instructionType == 1) {
				wordCount = XilinxUltraScalePlus::CAP_getInstructionWordCount(instruction);
				regAddr = XilinxUltraScalePlus::CAP_getInstructionRegister(instruction);
			} else if(instructionType == 2) {
				wordCount = instructionPayload;
			} else {
				fout << "0x" << hex << setw(8) << setfill('0') << instruction << " (Invalid instruction [invalid type])" << endl;
				continue;
			}
			
			if(instructionOPCODE == CAP::Operation::NOP) {
				if(instructionPayload != 0)
					fout << "NOP #" << instructionPayload << endl;
				else 
					fout << "NOP" << endl;
			} else if(instructionOPCODE == CAP::Operation::RESERVED) {
				if(instructionPayload != 0)
					fout << "RESERVED #" << instructionPayload << endl;
				else 
					fout << "RESERVED" << endl;
			} else if(instructionOPCODE == CAP::Operation::READ) {
				fout << "Read Reg @";
				writeRegisterName(fout, regAddr);
				fout << " for length #" << wordCount << endl;
			} else { // CAP::Operation::WRITE
				if((regAddr == CAP::Register::FDRI) && (wordCount > 0) && (wordCount % XUSP_WORDS_PER_FRAME == 0)) {
					if(shadowFrameValid) {
						fout << "# Shadow register contents are written to frame (BlockType="<<b<<", RowAddress="<<r<<", MajorAddress="<<c<<", MinorAddress"<<m<<") (Frame type: " << XilinxUltraScalePlus::getFrameType(b,r,c) << ")."<<endl;
						XilinxUltraScalePlus::CAP_IncrementFAR(b, r, c, m);
					}
					shadowFrameValid = 1;
					int frameCount = (wordCount/XUSP_WORDS_PER_FRAME);
					fout << dec << "@FDRI for #" << frameCount << " frames:" << endl;
					for(int i = 0 ; i < frameCount ; i++){
						fout << "# ";
						if(i == (frameCount-1)) fout << "(This frame data is written to shadow register!)";
						fout << dec << "Writing frame #" << i << " (BlockType="<<b<<", RowAddress="<<r<<", MajorAddress="<<c<<", MinorAddress="<<m<<") (Frame type: " << XilinxUltraScalePlus::getFrameType(b,r,c) << ") hex data:"<<endl;
						uint32_t frameData[XUSP_WORDS_PER_FRAME];
						for(int w = 0 ; w < XUSP_WORDS_PER_FRAME ; w++){
							frameData[w] = FileIO::read32(fin, Endianess::BE);
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
						XilinxUltraScalePlus::CAP_IncrementFAR(b, r, c, m);
					}
				} else if(regAddr == CAP::Register::CMD && wordCount == 1){
					uint32_t writeData = FileIO::read32(fin, Endianess::BE);
					writeCommandName(fout, static_cast<CAP::Command>(writeData));
					fout << ";";
					if(CAP::Command::WCFG == static_cast<CAP::Command>(writeData)){
						shadowFrameValid = 0;
						fout << " (also clears shadow register)";
					}
					fout << endl;
				} else if((instructionType == 1) && (wordCount == 0)){
					fout << "Select register @";
					writeRegisterName(fout, regAddr);
					fout << endl;
				} else if(wordCount == 1){
					uint32_t writeData = FileIO::read32(fin, Endianess::BE);
					fout << "@";
					writeRegisterName(fout, regAddr);
					if(regAddr == CAP::Register::FAR) {
						XilinxUltraScalePlus::CAP_parseFAR(writeData, b, r, c, m);
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
void XilinxUltraScalePlus::disassemblerWriteHeader(ofstream& fout)
{
	fout<<"--- HEADER BEGIN ---"<<endl;
	fout<<"Name = \""<<designName<<"\""<<endl;
	fout<<"FPGA = \""<<partName<<"\""<<endl;
	fout<<"Date = \""<<fileDate<<"\""<<endl;
	fout<<"Time = \""<<fileTime<<"\""<<endl;
	fout<<"--- HEADER END ---"<<endl;
}
