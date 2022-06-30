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

/**************************************************************************//**
 * Reads and parses .BIT header until and including the sync command.
 * Updates "designName", "partName", "fileDate", "fileTime".
 * 
 * @arg @c fin input file stream, which is left at the position following the
 * located sync command.
 *****************************************************************************/
inline void parseBITheader(ifstream& fin, Endianness e)
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
	log("Name of design: " + designName);
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
				log("Date: " + fileDate);
				break;
			}
			case 'd': {
				int headerAttrLength = FileIO::read16(fin, e);
				fileTime = FileIO::readString(fin, headerAttrLength, e);
				log("Time: " + fileTime);
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
}

/**************************************************************************//**
 * Reads a bitstream header until and including the sync command and returns
 * the file's endianess.
 * 
 * @arg @c fin input file stream. Gets fixed back to the original value before
 * leaving this function!
 *****************************************************************************/
inline Endianness parseBitstreamEndianness(ifstream& fin)
{
	streamoff fileOffset = fin.tellg();
	//Optional bitstream header
	
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those
	Endianness returnVal;
	//Find sync
	for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
		if(!fin.good())
			throw runtime_error("Was unable to find input bitstream's SYNC command.");
		int word = FileIO::read32(fin, Endianness::NATIVE);
		if(XCAP_getSyncInstruction() == word){
			returnVal = Endianness::NATIVE;
			syncDetectionDone++;
		} else if(XCAP_getSyncInstruction() == (Endian::NativeToBigEndian32(word))){
			returnVal = Endianness::BE;
			syncDetectionDone++;
		} else if(XCAP_getSyncInstruction() == (Endian::NativeToLittleEndian32(word))){
			returnVal = Endianness::LE;
			syncDetectionDone++;
		} else if(XCAP_getSyncInstruction() == Endian::BitSwap32(Endian::NativeToBigEndian32(word))){
			returnVal = Endianness::BE_BS;
			syncDetectionDone++;
		} else if(XCAP_getSyncInstruction() == Endian::BitSwap32(Endian::NativeToLittleEndian32(word))){
			returnVal = Endianness::LE_BS;
			syncDetectionDone++;
		} else
			fin.seekg(-3,ios::cur);
	}
	log("Detected file endianess: " + Endian::to_string(returnVal));
	
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
inline uint32_t parseBitstreamIDCODE(ifstream& fin, Endianness e)
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

bool findBitstreamSync(ifstream& fin, Endianness e)
{
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those
	for( ; ; ){
		if(!fin.good())
			return false;
		int word = FileIO::read32(fin, e);
		if(XCAP_getSyncInstruction() == word){
			return true;
		} else
			fin.seekg(-3,ios::cur);
	}
	return true; //not going to be reached.
}

inline void readBitstreamMain(ifstream& fin)
{
	int slr = 0;
	XCAP::Register regAddr = XCAP::Register::UNDEFINED;
	int wordCount = 0;
	char shadowFrame[WORDS_PER_FRAME*4];
	bool shadowFrameValid = false;
	int b = 7, r = 0, c = 0, m = 0;
	bool synched = false;
	//Parse bitstream
	for( ; ; ){
		if(!synched){
			if(findBitstreamSync(fin, loadedBitstreamEndianness)){
				synched = true;
			} else {
				break; // done with the bitstream
			}
		} else {
			uint32_t instruction = FileIO::read32(fin, loadedBitstreamEndianness);
			if(!fin.good()){
				break; // done with the bitstream
			} else {
				int instructionType = XCAP_getInstructionType(instruction);
				XCAP::Operation instructionOPCODE = XCAP_getInstructionOperation(instruction);
				int instructionPayload = XCAP_getInstructionPayload(instruction);
				if(instructionType == 1) {
					regAddr = XCAP_getInstructionRegister(instruction);
					wordCount = XCAP_getInstructionWordCount(instruction);
				} else if(instructionType == 2) {
					wordCount = instructionPayload;
				} else {
					throw runtime_error("Bitstream has invalid instruction ("+to_string(instruction)+" @ "+to_string(fin.tellg())+") (invalid type).");
				}
				if(regAddr == XCAP::Register::MAGIC1){
					slr++;
					wordCount = 0;
					shadowFrameValid = false;
					synched = false;	//TODO: do this recursively and not rely on large continous blocks of MAGIC1 command
				}
				if(instructionOPCODE == XCAP::Operation::WRITE){ //Write register
					for( ; wordCount > 0 ; ){
						if(regAddr == XCAP::Register::FAR){
							uint32_t farValue = FileIO::read32(fin, loadedBitstreamEndianness);
							wordCount--;
							XCAP_parseFAR(farValue, slr, b, r, c, m);
							/*
							if(shadowFrameValid){
								if(b == BLOCKTYPE_LOGIC){
									memcpy((char*)&bitstreamCLB[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								} else if(b == BLOCKTYPE_BLOCKRAM) {
									memcpy((char*)&bitstreamBRAM[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								}
								XCAP_IncrementFAR(slr, b, r, c, m);
								shadowFrameValid = false;
							}
							*/
						} else if(regAddr == XCAP::Register::FDRI){
							if(wordCount % WORDS_PER_FRAME != 0)
								throw runtime_error("FDRI write of a partial frame was detected, which is currently not supported.");
							if(shadowFrameValid){
								assert(b < BLOCKTYPE_MAX);
								assert(r < numberOfRows);
								assert(c < numberOfCols[r]);
								if(b == BLOCKTYPE_LOGIC){
									assert(m < LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]]);
									memcpy((char*)&bitstreamCLB[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								} else if(b == BLOCKTYPE_BLOCKRAM) {
									assert(m < FRAMES_PER_BRAM_CONTENT_COLUMN);
									memcpy((char*)&bitstreamBRAM[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								}
								XCAP_IncrementFAR(slr, b, r, c, m);
							}
							int forwardShadowReg = (wordCount/WORDS_PER_FRAME) - 1;//skip shadow reg for certain number of frames
							if(forwardShadowReg > 0){
								if(b == BLOCKTYPE_LOGIC){ // CLB
									fin.read((char*)&bitstreamCLB[r][c][m*WORDS_PER_FRAME], forwardShadowReg * WORDS_PER_FRAME * 4);
								} else if(b == BLOCKTYPE_BLOCKRAM){
									fin.read((char*)&bitstreamBRAM[r][c][m*WORDS_PER_FRAME], forwardShadowReg * WORDS_PER_FRAME * 4);
								}
								for(int i = 0 ; i < forwardShadowReg ; i++){
									XCAP_IncrementFAR(slr, b, r, c, m);
								}
							}
							wordCount -= forwardShadowReg * WORDS_PER_FRAME;
							shadowFrameValid = true;
							fin.read((char*)&shadowFrame, WORDS_PER_FRAME*4);
							wordCount -= WORDS_PER_FRAME;

						} else if(regAddr == XCAP::Register::CMD){
							XCAP::Command command = static_cast<XCAP::Command>(FileIO::read32(fin, loadedBitstreamEndianness));
							wordCount--;
							if(command == XCAP::Command::WCFG){
								shadowFrameValid = false;
							}
							if(command == XCAP::Command::DESYNC){
								synched = false;
							}
						} else {
							uint32_t scrap = FileIO::read32(fin, loadedBitstreamEndianness);
							wordCount--;
						}
					}//Write register count for loop
				}//Write register instruction
			}// not end of file
		}
	}// for loop parsing the rest of the bitstream
}

inline void readBitstreamBIT(ifstream& fin)
{
	parseBITheader(fin, loadedBitstreamEndianness);
	setDeviceByPartNameOrThrow();
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	readBitstreamMain(fin);
}

inline void readBitstreamBIN(ifstream& fin)
{
	uint32_t idcode = parseBitstreamIDCODE(fin, loadedBitstreamEndianness);
	setDeviceByIDCODEOrThrow(idcode);
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	readBitstreamMain(fin);
}
