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

#include "XilinxConfigurationAccessPort.h"
#include "../../Common/FileIO.h"

/**************************************************************************//**
 * Reads and parses .BIT header until and including the sync command.
 * Updates "designName", "partName", "fileDate", "fileTime".
 * 
 * @arg @c fin input file stream, which is left at the position following the
 * located sync command.
 *****************************************************************************/
void XilinxConfigurationAccessPort::parseBITheader(ifstream& fin, Endianess e)
{
	//.bit header c0nstant:
	int headerConstTextLength = FileIO::read16(fin, e);
	string headerConstText = FileIO::readString(fin, headerConstTextLength, e);
	int headerConstText2Length = FileIO::read16(fin, e);
	string headerConstText2 = FileIO::readString(fin, headerConstText2Length, e);
	if((!((9 == headerConstTextLength) && ("\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0" == headerConstText) && (1 == headerConstText2Length) && ("a" == headerConstText2))))
		warn(".bit c0nstant header differs from expected.");
	//.bit header vars:
	int headerDesignNameLength = FileIO::read16(fin, e);
	designName = FileIO::readString(fin, headerDesignNameLength, e);
	log("\tName of design: " + designName);
	for(int headerDone = 0 ; !headerDone ; ){
		int key = FileIO::readNative8(fin);
		switch(key){
			case 'b': {
				int headerAttrLength = FileIO::read16(fin, e);
				partName = FileIO::readString(fin, headerAttrLength, e);
				log("FPGA part: " + partName);
				break;
			}
			case 'c': {
				int headerAttrLength = FileIO::read16(fin, e);
				fileDate = FileIO::readString(fin, headerAttrLength, e);
				log("\tDate: " + fileDate);
				break;
			}
			case 'd': {
				int headerAttrLength = FileIO::read16(fin, e);
				fileTime = FileIO::readString(fin, headerAttrLength, e);
				log("\tTime: " + fileTime);
				break;
			}
			case 'e': {
				headerDone = 1;//last attribute in a .bit header
				int reportedRemainingFileLength = FileIO::read32(fin, e);
				int tmpPos = (int)fin.tellg();
				fin.seekg (0, fin.end);
				int fileSize = (int)fin.tellg();
				fin.seekg (tmpPos, fin.beg);
				if(fileSize != (tmpPos + reportedRemainingFileLength))
					warn(".bit header contained inaccurate file length field.");
				break;
			}
			default: {
				warn(".bit header contained unknown file field type.");
			}
		}
	}
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those

	//Find sync
	for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
		int word = FileIO::read32(fin, e);
		if(XCAP_SyncInstruction() == word)
			syncDetectionDone++;
		else
			fin.seekg(-3,ios::cur);
	}
}
/**************************************************************************//**
 * Reads a bitstream header until and including the sync command and returns
 * the file's endianess.
 * 
 * @arg @c fin input file stream. Gets fixed back to the original value before
 * leaving this function!
 *****************************************************************************/
Endianess XilinxConfigurationAccessPort::parseBitstreamEndianess(ifstream& fin)
{
	streamoff fileOffset = fin.tellg();
	//Optional bitstream header
	
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those
	Endianess returnVal;
	//Find sync
	for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
		if(!fin.good())
			throw runtime_error("Was unable to find input bitstream's SYNC command.");
		int word = FileIO::read32(fin, Endianess::NATIVE);
		if(XCAP_SyncInstruction() == word){
			returnVal = Endianess::NATIVE;
			syncDetectionDone++;
		} else if(XCAP_SyncInstruction() == (Endian::NativeToBigEndian32(word))){
			returnVal = Endianess::BE;
			syncDetectionDone++;
		} else if(XCAP_SyncInstruction() == (Endian::NativeToLittleEndian32(word))){
			returnVal = Endianess::LE;
			syncDetectionDone++;
		} else if(XCAP_SyncInstruction() == Endian::BitSwap32(Endian::NativeToBigEndian32(word))){
			returnVal = Endianess::BE_BS;
			syncDetectionDone++;
		} else if(XCAP_SyncInstruction() == Endian::BitSwap32(Endian::NativeToLittleEndian32(word))){
			returnVal = Endianess::LE_BS;
			syncDetectionDone++;
		} else
			fin.seekg(-3,ios::cur);
	}
	log("\tDetected file endianess: " + Endian::to_string(returnVal));
	
	fin.seekg(fileOffset, fin.beg);
	return returnVal;
}
/**************************************************************************//**
 * Reads a bitstream until and including the first IDCODE command and returns
 * the first SLRs IDCODE.
 * 
 * @arg @c fin input file stream. Gets fixed back to the original value before
 * leaving this function!
 *****************************************************************************/
uint32_t XilinxConfigurationAccessPort::parseBitstreamIDCODE(ifstream& fin, Endianess e)
{
	streamoff fileOffset = fin.tellg();
	//Optional bitstream header
	uint32_t returnVal;
	//Find sync
	for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
		if(!fin.good())
			throw runtime_error("Was unable to find input bitstream's IDCODE command.");
		int word = FileIO::read32(fin, e);
		if(XCAP_IDCODEInstruction() == word){
			returnVal = FileIO::read32(fin, e);
			syncDetectionDone++;
		} else
			fin.seekg(-3,ios::cur);
	}
	
	fin.seekg(fileOffset, fin.beg);
	return returnVal;
}
/**************************************************************************//**
 * Reads a bitstream header until and including the sync command.
 * 
 * @arg @c fin input file stream. Moves the stream pointer to after the SYNC
 * command and in a word-aligned position with the following instructions.
 *****************************************************************************/
void XilinxConfigurationAccessPort::findBitstreamSync(ifstream& fin, Endianess e)
{
	for( ; ; ){
		if(!fin.good())
			throw runtime_error("Was unable to find input bitstream's SYNC command.");
		int word = FileIO::read32(fin, e);
		if(XCAP_SyncInstruction() == word){
			return;
		} else
			fin.seekg(-3,ios::cur);
	}
}
