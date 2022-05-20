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
#include "XilinxUltraScalePlusConfigurationAccessPort.h"
#include "../../../Common/FileIO.h"


inline void XilinxUltraScalePlus::CAP_IncrementFAR_BlockType0(int& blockType, int& rowAddress, int& columnAddress, int& minorAddress){
	minorAddress++;
	if((columnAddress == numberOfCols)?(minorAddress == XUSP_EXTRA_FRAMES_PER_ROW):(minorAddress == numberOfFramesPerResourceLetter[(uint8_t)resourceString[rowAddress][columnAddress]])){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == (numberOfCols+1)){
			columnAddress = 0;
			rowAddress++;
			if(rowAddress == numberOfRows){
				rowAddress = 0; 
				blockType++;
			}
		}
	}
}
inline void XilinxUltraScalePlus::CAP_IncrementFAR_BlockType1(int& blockType, int& rowAddress, int& columnAddress, int& minorAddress){
	minorAddress++;
	if((columnAddress == numberOfBRAMCols)?(minorAddress == XUSP_EXTRA_FRAMES_PER_ROW):(minorAddress == XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN)){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == (numberOfBRAMCols+1)){
			columnAddress = 0;
			rowAddress++;
			if(rowAddress == numberOfRows){
				rowAddress = 0;
				blockType++;
			}
		}
	}
}
inline void XilinxUltraScalePlus::CAP_IncrementFAR(int& blockType, int& rowAddress, int& columnAddress, int& minorAddress){
	if(CAP::BlockType::BLOCKRAM == static_cast<CAP::BlockType>(blockType))
		CAP_IncrementFAR_BlockType0(blockType, rowAddress, columnAddress, minorAddress);
	else if(CAP::BlockType::LOGIC == static_cast<CAP::BlockType>(blockType))
		CAP_IncrementFAR_BlockType1(blockType, rowAddress, columnAddress, minorAddress);
}

inline int XilinxUltraScalePlus::CAP_getInstructionType(int instruction){
	return ((instruction>>29) & 0x7);
}
inline CAP::Operation XilinxUltraScalePlus::CAP_getInstructionOperation(int instruction){
	return static_cast<CAP::Operation>((instruction>>27) & 0x3);
}
inline int XilinxUltraScalePlus::CAP_getInstructionPayload(int instruction){
	return (instruction & 0x07FFFFFF);
}
inline CAP::Register XilinxUltraScalePlus::CAP_getInstructionRegister(int instruction){
	return static_cast<CAP::Register>((instruction>>13) & 0x1F);
}
inline int XilinxUltraScalePlus::CAP_getInstructionWordCount(int instruction){
	return (instruction & 0x7FF);
}
inline void XilinxUltraScalePlus::CAP_parseFAR(int farValue, int& blockType, int& rowAddress, int& columnAddress, int& minorAddress){
	blockType = (farValue >> 24) & 0x7;
	rowAddress = (farValue >> 18) & 0x3F;
	columnAddress = (farValue >> 8) & 0x3FF;
	minorAddress = farValue & 0xFF;
}
inline uint32_t XilinxUltraScalePlus::CAP_makeFAR(int blockType, int rowAddress, int columnAddress, int minorAddress){
	return (((blockType & 0x7) << 24) | ((rowAddress & 0x3F) << 18) | ((columnAddress & 0x3FF) << 8) | (minorAddress & 0xFF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeInstruction(int type, CAP::Operation operation, CAP::Register reg, int payload){
	return (((type & 0x7) << 29) | ((static_cast<int>(operation) & 0x3) << 27) | ((static_cast<int>(reg) & 0x1F) << 13) | (payload & 0x07FFFFFF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeSyncInstruction(){
	return CAP::SYNC;
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation operation, CAP::Register reg, int payload){
	return XilinxUltraScalePlus::CAP_makeInstruction(1, operation, reg, (payload & 0x7FF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType1NopInstruction(int payload){
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::NOP, static_cast<CAP::Register>(0), payload);
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType1ReadInstruction(CAP::Register reg, int payload){
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::READ, reg, (payload & 0x7FF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType1WriteInstruction(CAP::Register reg, int payload){
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::WRITE, reg, (payload & 0x7FF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType1ReservedInstruction(int payload){
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::RESERVED, static_cast<CAP::Register>(0), payload);
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation operation, int payload){
	return XilinxUltraScalePlus::CAP_makeInstruction(2, operation, static_cast<CAP::Register>(0), payload);
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType2NopInstruction(int payload){
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::NOP, payload);
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType2ReadInstruction(int payload){
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::READ, (payload & 0x07FFFFFF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType2WriteInstruction(int payload){
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::WRITE, (payload & 0x07FFFFFF));
}
inline uint32_t XilinxUltraScalePlus::CAP_makeType2ReservedInstruction(int payload){
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::RESERVED, payload);
}
inline void XilinxUltraScalePlus::CAP_writeNOP(ofstream& fout, int cnt, int payload, Endianess e){
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1NopInstruction(payload);
	for(int i = 0 ; i < cnt ; i++)
		FileIO::write32(fout, instruction, e);
}
inline void XilinxUltraScalePlus::CAP_writeRESERVED(ofstream& fout, int cnt, int payload, Endianess e){
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1ReservedInstruction(payload);
	for(int i = 0 ; i < cnt ; i++)
		FileIO::write32(fout, instruction, e);
}
inline void XilinxUltraScalePlus::CAP_writeSelectRegister(ofstream& fout, CAP::Register reg, Endianess e){
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1WriteInstruction(reg, 0);
	FileIO::write32(fout, instruction, e);
}
inline void XilinxUltraScalePlus::CAP_writeReadRegister(ofstream& fout, CAP::Register reg, int readLength, Endianess e){
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1ReadInstruction(reg, readLength);
	FileIO::write32(fout, instruction, e);
}
inline void XilinxUltraScalePlus::CAP_writeRegister(ofstream& fout, CAP::Register reg, int writeValue, Endianess e){
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1WriteInstruction(reg, 1);
	FileIO::write32(fout, instruction, e);
	FileIO::write32(fout, writeValue, e);
}
inline void XilinxUltraScalePlus::CAP_writeCommand(ofstream& fout, CAP::Command cmd, Endianess e){
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::CMD, static_cast<int>(cmd), e);
}
inline void XilinxUltraScalePlus::CAP_writeMaskAndRegister(ofstream& fout, CAP::Register reg, int writeMask, int writeValue, Endianess e){
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::MASK, writeMask, e);
	XilinxUltraScalePlus::CAP_writeRegister(fout, reg, writeValue, e);
}
inline void XilinxUltraScalePlus::CAP_writeFDRI1(ofstream& fout, int wordCount, Endianess e){ ///< Generate and write only a type 1 FDRI command.
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1WriteInstruction(CAP::Register::FDRI, wordCount);
	FileIO::write32(fout, instruction, e);
}
inline void XilinxUltraScalePlus::CAP_writeFDRI2(ofstream& fout, int wordCount, Endianess e){ ///< Generate and write only a type 2 FDRI command.
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType2WriteInstruction(wordCount);
	FileIO::write32(fout, instruction, e);
}
inline void XilinxUltraScalePlus::CAP_writeFDRI(ofstream& fout, int wordCount, Endianess e){ ///< Generate and write an FDRI command. Always uses type 2 command for simplicity.
	XilinxUltraScalePlus::CAP_writeFDRI1(fout, 0, e);
	XilinxUltraScalePlus::CAP_writeFDRI2(fout, wordCount, e);
}