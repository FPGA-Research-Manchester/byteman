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
inline void parseBITheader(std::ifstream& fin, Endianness e)
{
	//.bit header c0nstant:
	int headerConstTextLength = FileIO::read16(fin, e);
	std::string headerConstText = FileIO::readString(fin, headerConstTextLength, e);
	int headerConstText2Length = FileIO::read16(fin, e);
	std::string headerConstText2 = FileIO::readString(fin, headerConstText2Length, e);
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
inline Endianness parseBitstreamEndianness(std::ifstream& fin)
{
	std::streamoff fileOffset = fin.tellg();
	//Optional bitstream header
	
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those
	Endianness returnVal;
	//Find sync
	for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
		if(!fin.good())
			throw std::runtime_error("Was unable to find input bitstream's SYNC command.");
		int wordOld4 = FileIO::read32(fin, Endianness::NATIVE);
		int wordOld3 = FileIO::read32(fin, Endianness::NATIVE);
		int wordOld2 = FileIO::read32(fin, Endianness::NATIVE);
		int wordOld1 = FileIO::read32(fin, Endianness::NATIVE);
		int word = FileIO::read32(fin, Endianness::NATIVE);
		if(wordOld4 == 0x000000BB && wordOld3 == 0x11220044 && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF && word == XCAP_getSyncInstruction()){
			returnVal = Endianness::NATIVE;
			syncDetectionDone++;
		} else if(wordOld4 == (Endian::NativeToBigEndian32(0x000000BB)) && wordOld3 == (Endian::NativeToBigEndian32(0x11220044)) && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF && word == (Endian::NativeToBigEndian32(XCAP_getSyncInstruction()))){
			returnVal = Endianness::BE;
			syncDetectionDone++;
		} else if(wordOld4 == (Endian::NativeToLittleEndian32(0x000000BB)) && wordOld3 == (Endian::NativeToLittleEndian32(0x11220044)) && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF && word == (Endian::NativeToLittleEndian32(XCAP_getSyncInstruction()))){
			returnVal = Endianness::LE;
			syncDetectionDone++;
		} else if(wordOld4 == (Endian::NativeToBigEndian32(0x000000BB)) && wordOld3 == (Endian::NativeToBigEndian32(0x11220044)) && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF && word == (Endian::NativeToBigEndian32(XCAP_getSyncInstruction()))){
			returnVal = Endianness::BE_BS;
			syncDetectionDone++;
		} else if(wordOld4 == Endian::BitSwap32((Endian::NativeToLittleEndian32(0x000000BB))) && wordOld3 == Endian::BitSwap32((Endian::NativeToLittleEndian32(0x11220044))) && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF && word == Endian::BitSwap32((Endian::NativeToLittleEndian32(XCAP_getSyncInstruction())))){
			returnVal = Endianness::LE_BS;
			syncDetectionDone++;
		} else
			fin.seekg(-19, std::ios::cur);
	}
	log("Detected file endianess: " + Endian::to_string(returnVal));
	
	fin.seekg(fileOffset, fin.beg);
	return returnVal;
}

/**************************************************************************//**
 * Reads a bitstream header until and including the sync command.
 *
 * @arg @c fin input file stream. Moves the stream pointer to after the SYNC
 * command and in a word-aligned position with the following instructions.
 *****************************************************************************/

bool findBitstreamSyncWord(std::ifstream& fin, Endianness e)
{
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those
	for( ; ; ){
		if(!fin.good())
			return false;
		int word = FileIO::read32(fin, e);
		if(word == XCAP_getSyncInstruction()){
			return true;
		} else
			fin.seekg(-3, std::ios::cur);
	}
	return false; //not going to be reached.
}

/**************************************************************************//**
 * Reads a bitstream header until and including the sync sequence.
 *
 * @arg @c fin input file stream. Moves the stream pointer to after the SYNC
 * command and in a word-aligned position with the following instructions.
 *****************************************************************************/

bool findBitstreamSyncSequence(std::ifstream& fin, Endianness e)
{
	//Follow some 0xFF's and the bus width detection c0nstant "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\x44\x00\x22\x11\xBB\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
	//However we don't care about reading those
	for( ; ; ){
		if(!fin.good())
			return false;
		int wordOld4 = FileIO::read32(fin, e);
		int wordOld3 = FileIO::read32(fin, e);
		int wordOld2 = FileIO::read32(fin, e);
		int wordOld1 = FileIO::read32(fin, e);
		int word = FileIO::read32(fin, e);
		if(wordOld4 == 0x000000BB && wordOld3 == 0x11220044 && wordOld2 == 0xFFFFFFFF && wordOld1 == 0xFFFFFFFF && word == XCAP_getSyncInstruction()){
			return true;
		} else
			fin.seekg(-19, std::ios::cur);
	}
	return false; //not going to be reached.
}

/**************************************************************************//**
 * Reads a bitstream until and including the first IDCODE command and returns
 * the first SLRs IDCODE.
 *
 * @arg @c fin input file stream. Gets fixed back to the original value before
 * leaving this function!
 *****************************************************************************/
inline uint32_t parseBitstreamIDCODE(std::ifstream& fin, Endianness e)
{
	std::streamoff fileOffset = fin.tellg();
	//Optional bitstream header
	findBitstreamSyncSequence(fin, e);
	uint32_t returnVal;
	//Find sync
	for(int syncDetectionDone = 0 ; !syncDetectionDone ; ){
		if(!fin.good())
			throw std::runtime_error("Was unable to find input bitstream's IDCODE command.");
		int word = FileIO::read32(fin, e);
		if(XCAP_IDCODEInstruction() == word){
			returnVal = FileIO::read32(fin, e);
			syncDetectionDone++;
		} else
			fin.seekg(-3, std::ios::cur);
	}
	
	fin.seekg(fileOffset, fin.beg);
	return returnVal;
}

inline void readBitstreamMain(std::ifstream& fin, Endianness bitstreamFileEndianness)
{
	int slr = 0;
	XCAP::Register regAddr = XCAP::Register::UNDEFINED;
	int wordCount = 0;
	char shadowFrame[WORDS_PER_FRAME*4];
	bool shadowFrameValid = false;
	int b = 7, r = 0, c = 0, m = 0;
	bool aligned = false;
	bool synched = false;
	
	if(bitstreamHasValidData){
		ensureSelectedEndianness(bitstreamFileEndianness);
	}
	loadedBitstreamEndianness = bitstreamFileEndianness;
	//Parse bitstream
	for( ; ; ){
		if(!synched){
			if(!aligned){//if not aligned, we search for the whole sync sequence, otherwise, just the sync word
				if(findBitstreamSyncSequence(fin, loadedBitstreamEndianness)){
					synched = true;
					aligned = true;
				} else {
					break;// done with the bitstream
				}
			} else {
				if(findBitstreamSyncWord(fin, loadedBitstreamEndianness)){
					synched = true;
				} else {
					break;// done with the bitstream
				}
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
					throw std::runtime_error("Bitstream has invalid instruction (" + std::to_string(instruction) + " @ " + std::to_string(fin.tellg()) + ") (invalid type).");
				}
				if(regAddr == XCAP::Register::MAGIC1){
					slr++;
					wordCount = 0;
					shadowFrameValid = false;
					synched = false;	//TODO: do this recursively and not rely on large continous blocks of MAGIC1 command
					aligned = false;
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
									memcpy(&bitstreamCLB[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								} else if(b == BLOCKTYPE_BLOCKRAM) {
									memcpy(&bitstreamBRAM[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								}
								XCAP_IncrementFAR(slr, b, r, c, m);
								shadowFrameValid = false;
							}
							*/
						} else if(regAddr == XCAP::Register::FDRI){
							if(wordCount % WORDS_PER_FRAME != 0)
								throw std::runtime_error("FDRI write of a partial frame was detected, which is currently not supported.");
							if(shadowFrameValid){
								throwingAssert(b, <, BLOCKTYPE_MAX);
								throwingAssertPrintVar_1(r, >=, SLRinfo[slr].fromRow, slr);
								throwingAssertPrintVar_1(r, <=, SLRinfo[slr].toRow, slr);
								throwingAssertPrintVar_1(c, <, numberOfCols[r], r);
								if(b == BLOCKTYPE_LOGIC){
									throwingAssertPrintVar_3(m, <, LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]], r, c, (int)resourceString[r][c]);
									memcpy(&bitstreamCLB[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								} else if(b == BLOCKTYPE_BLOCKRAM) {
									throwingAssert(m, <, FRAMES_PER_BRAM_CONTENT_COLUMN);
									memcpy(&bitstreamBRAM[r][c][m*WORDS_PER_FRAME], &shadowFrame, WORDS_PER_FRAME*4);
								} else {
									warn("Unknown BlockType(" + std::to_string(b) + ") written while reading bitstream file.");
								}
								XCAP_IncrementFAR(slr, b, r, c, m);
							}
							int forwardShadowReg = (wordCount/WORDS_PER_FRAME) - 1;//skip shadow reg for certain number of frames
							if(forwardShadowReg > 0){
								if(b == BLOCKTYPE_LOGIC){ // CLB
									fin.read((char*)&bitstreamCLB[r][c][m*WORDS_PER_FRAME], forwardShadowReg * WORDS_PER_FRAME * 4);
								} else if(b == BLOCKTYPE_BLOCKRAM){
									fin.read((char*)&bitstreamBRAM[r][c][m*WORDS_PER_FRAME], forwardShadowReg * WORDS_PER_FRAME * 4);
								} else {
									fin.ignore(forwardShadowReg * WORDS_PER_FRAME * 4);
									warn("Unknown BlockType(" + std::to_string(b) + ") written while reading bitstream file.");
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
								synched = false;//but aligned remains true
								shadowFrameValid = false;
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
	bitstreamHasValidData = true;
}

inline void readBitstreamBIT(std::ifstream& fin)
{
	Endianness bitstreamFileEndianness = parseBitstreamEndianness(fin);
	
	parseBITheader(fin, bitstreamFileEndianness);
	setDeviceByPartNameOrThrow();
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	readBitstreamMain(fin, bitstreamFileEndianness);
}


inline void readBitstreamBIN(std::ifstream& fin)
{
	Endianness bitstreamFileEndianness = parseBitstreamEndianness(fin);
	
	uint32_t idcode = parseBitstreamIDCODE(fin, bitstreamFileEndianness);
	setDeviceByIDCODEOrThrow(idcode);
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	readBitstreamMain(fin, bitstreamFileEndianness);
}

inline void readBitstreamRBDMain(std::ifstream& fin, Endianness bitstreamFileEndianness)
{
	//int slr = 0;
	XCAP::Register regAddr = XCAP::Register::UNDEFINED;
	if(bitstreamHasValidData){
		ensureSelectedEndianness(bitstreamFileEndianness);
	}
	loadedBitstreamEndianness = bitstreamFileEndianness;
	//Parse RBD
	std::string str;
	for(int i = 0 ; i < WORDS_PER_FRAME ; i++)//dump first frame.
		std::getline(fin, str);
	uint32_t* fromPtr = bitstreamBegin;
	uint32_t* toPtr = bitstreamEnd;
	for(uint32_t* bitstreamLoc = fromPtr ; bitstreamLoc < toPtr ; bitstreamLoc++){
		std::getline(fin, str);
		uint32_t val = (uint32_t)(std::stoll(str, nullptr, 2) & 0xFFFFFFFF);
		bitstreamLoc[0] = Endian::NativeToAnyEndianness32(val, loadedBitstreamEndianness);
	}
	bitstreamHasValidData = true;
}

inline void readBitstreamRBD(std::ifstream& fin)
{
	Endianness bitstreamFileEndianness = Endianness::BE;
	
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before writing
	
	readBitstreamRBDMain(fin, bitstreamFileEndianness);
}