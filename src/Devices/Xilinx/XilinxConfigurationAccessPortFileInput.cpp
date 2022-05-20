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
void XilinxConfigurationAccessPort::parseBITheader(ifstream& fin, Endianess e){
    //.bit header c0nstant:
	int headerConstTextLength = FileIO::read16(fin, e);
	string headerConstText = FileIO::readString(fin, headerConstTextLength, e);
	int headerConstText2Length = FileIO::read16(fin, e);
	string headerConstText2 = FileIO::readString(fin, headerConstText2Length, e);
	if(warn && (!((9 == headerConstTextLength) && ("\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0" == headerConstText) && (1 == headerConstText2Length) && ("a" == headerConstText2))))
        cout << "warn: .bit c0nstant header differs from expected.\n";
	//.bit header vars:
	int headerDesignNameLength = FileIO::read16(fin, e);
	designName = FileIO::readString(fin, headerDesignNameLength, e);
	if(verbose)cout<<"\tName of design: "<<designName<<endl;
    for(int headerDone = 0 ; !headerDone ; ){
        int key = FileIO::readNative8(fin);
        switch(key){
            case 'b': {
                int headerAttrLength = FileIO::read16(fin, e);
				partName = FileIO::readString(fin, headerAttrLength, e);
                if(verbose)cout<<"\tFPGA part: "<<partName<<endl;
                break;
            }
            case 'c': {
                int headerAttrLength = FileIO::read16(fin, e);
				fileDate = FileIO::readString(fin, headerAttrLength, e);
                if(verbose)cout<<"\tdate: "<<partName<<endl;
                break;
            }
            case 'd': {
                int headerAttrLength = FileIO::read16(fin, e);
				fileTime = FileIO::readString(fin, headerAttrLength, e);
                if(verbose)cout<<"\ttime: "<<fileTime<<endl;
                break;
            }
            case 'e': {
                headerDone = 1;//last attribute in a .bit header
                int reportedRemainingFileLength = FileIO::read32(fin, e);
                int tmpPos = (int)fin.tellg();
                fin.seekg (0, fin.end);
                int fileSize = (int)fin.tellg();
                fin.seekg (tmpPos, fin.beg);
                if(warn && fileSize != (tmpPos + reportedRemainingFileLength))
                    cout << "warning: .bit header contained inaccurate file length field.\n";
                break;
            }
			default: {
				if(warn)
                    cout << "warning: .bit header contained unknown file field type.\n";
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
Endianess XilinxConfigurationAccessPort::parseBitstreamEndianess(ifstream& fin){
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
	if(verbose)
		cout << "\tDetected file endianess: " << Endian::to_string(returnVal) << endl;
	
	fin.seekg(fileOffset, fin.beg);
	return returnVal;
}