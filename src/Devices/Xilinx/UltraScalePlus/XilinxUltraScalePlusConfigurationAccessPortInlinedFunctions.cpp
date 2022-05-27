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

/// Modifies the references @c blockType, @c rowAddress, @c columnAddress, and @c minorAddress to increment FAR address in bitstream's block type 0
inline void XilinxUltraScalePlus::CAP_IncrementFAR_BlockType0(int slrID, int& blockType, int& rowAddress, int& columnAddress, int& minorAddress)
{
	minorAddress++;
	if((columnAddress == numberOfCols)?(minorAddress == XUSP_EXTRA_FRAMES_PER_ROW):(minorAddress == LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[rowAddress][columnAddress]])){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == (numberOfCols+1)){
			columnAddress = 0;
			if(rowAddress == SLRinfo[slrID].toRow){
				rowAddress = SLRinfo[slrID].fromRow;
				blockType++;
			} else {
				rowAddress++;
			}
		}
	}
}
/// Modifies the references @c blockType, @c rowAddress, @c columnAddress, and @c minorAddress to increment FAR address in bitstream's block type 1
inline void XilinxUltraScalePlus::CAP_IncrementFAR_BlockType1(int slrID, int& blockType, int& rowAddress, int& columnAddress, int& minorAddress) 
{
	minorAddress++;
	if((columnAddress == numberOfBRAMCols)?(minorAddress == XUSP_EXTRA_FRAMES_PER_ROW):(minorAddress == XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN)){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == (numberOfBRAMCols+1)){
			columnAddress = 0;
			if(rowAddress == SLRinfo[slrID].toRow){
				rowAddress = SLRinfo[slrID].fromRow;
				blockType++;
			} else {
				rowAddress++;
			}
		}
	}
}
/// Modifies the references @c blockType, @c rowAddress, @c columnAddress, and @c minorAddress to increment FAR address
inline void XilinxUltraScalePlus::CAP_IncrementFAR(int slrID, int& blockType, int& rowAddress, int& columnAddress, int& minorAddress) 
{
	if(CAP::BlockType::LOGIC == static_cast<CAP::BlockType>(blockType))
		CAP_IncrementFAR_BlockType0(slrID, blockType, rowAddress, columnAddress, minorAddress);
	else if(CAP::BlockType::BLOCKRAM == static_cast<CAP::BlockType>(blockType))
		CAP_IncrementFAR_BlockType1(slrID, blockType, rowAddress, columnAddress, minorAddress);
}

/// Parses and returns instruction type. Valid XUS+ instructions will be of types 1 and 2
inline int XilinxUltraScalePlus::CAP_getInstructionType(int instruction)
{
	return ((instruction>>29) & 0x7);
}
/// Parses and returns instruction operation. Most XUS+ instructions will NOP or write.
inline CAP::Operation XilinxUltraScalePlus::CAP_getInstructionOperation(int instruction)
{
	return static_cast<CAP::Operation>((instruction>>27) & 0x3);
}
/// Parses and returns instruction payload. This is the immediate value after instruction type and operation encodings.
inline int XilinxUltraScalePlus::CAP_getInstructionPayload(int instruction)
{
	return (instruction & 0x07FFFFFF);
}
/// Parses and returns instruction register. This is the register being addressed if the instruction is of type 1
inline CAP::Register XilinxUltraScalePlus::CAP_getInstructionRegister(int instruction)
{
	return static_cast<CAP::Register>((instruction>>13) & 0x1F);
}
/// Parses and returns instruction word count. This is the number of words to be read/written if the instruction is of type 1
inline int XilinxUltraScalePlus::CAP_getInstructionWordCount(int instruction)
{
	return (instruction & 0x7FF);
}
/// Parse the Frame Address Register @c farValue into referenced @c blockType, @c rowAddress, @c columnAddress, and @c minorAddress
inline void XilinxUltraScalePlus::CAP_parseFAR(int farValue, int slr, int& blockType, int& rowAddress, int& columnAddress, int& minorAddress)
{
	blockType = (farValue >> 24) & 0x7;
	rowAddress = (farValue >> 18) & 0x3F;
	rowAddress += SLRinfo[slr].fromRow;
	columnAddress = (farValue >> 8) & 0x3FF;
	minorAddress = farValue & 0xFF;
}
/// Parse @c blockType, @c rowAddress, @c columnAddress, and @c minorAddress. Generate and return Frame Address Register encoding value.
inline uint32_t XilinxUltraScalePlus::CAP_makeFAR(int slr, int blockType, int rowAddress, int columnAddress, int minorAddress)
{
	return (((blockType & 0x7) << 24) | (((rowAddress - SLRinfo[slr].fromRow) & 0x3F) << 18) | ((columnAddress & 0x3FF) << 8) | (minorAddress & 0xFF));
}
/// Generate and return the encoding for an instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeInstruction(int type, CAP::Operation operation, CAP::Register reg, int payload)
{
	return (((type & 0x7) << 29) | ((static_cast<int>(operation) & 0x3) << 27) | ((static_cast<int>(reg) & 0x1F) << 13) | (payload & 0x07FFFFFF));
}
/// Generate and return the encoding for a SYNC instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeSyncInstruction()
{
	return CAP::SYNC;
}
/// Generate and return the encoding for a type 1 instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation operation, CAP::Register reg, int payload)
{
	return XilinxUltraScalePlus::CAP_makeInstruction(1, operation, reg, (payload & 0x7FF));
}
/// Generate and return the encoding for a type 1 NOP instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType1NopInstruction(int payload)
{
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::NOP, static_cast<CAP::Register>(0), payload);
}
/// Generate and return the encoding for a type 1 Read instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType1ReadInstruction(CAP::Register reg, int payload)
{
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::READ, reg, (payload & 0x7FF));
}
/// Generate and return the encoding for a type 1 Write instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType1WriteInstruction(CAP::Register reg, int payload)
{
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::WRITE, reg, (payload & 0x7FF));
}
/// Generate and return the encoding for a type 1 Reserved instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType1ReservedInstruction(int payload)
{
	return XilinxUltraScalePlus::CAP_makeType1Instruction(CAP::Operation::RESERVED, static_cast<CAP::Register>(0), payload);
}
/// Generate and return the encoding for a type 2 instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation operation, int payload)
{
	return XilinxUltraScalePlus::CAP_makeInstruction(2, operation, static_cast<CAP::Register>(0), payload);
}
/// Generate and return the encoding for a type 2 NOP instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType2NopInstruction(int payload)
{
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::NOP, payload);
}
/// Generate and return the encoding for a type 2 Read instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType2ReadInstruction(int payload)
{
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::READ, (payload & 0x07FFFFFF));
}
/// Generate and return the encoding for a type 2 Write instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType2WriteInstruction(int payload)
{
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::WRITE, (payload & 0x07FFFFFF));
}
/// Generate and return the encoding for a type 2 Reserved instruction.
inline uint32_t XilinxUltraScalePlus::CAP_makeType2ReservedInstruction(int payload)
{
	return XilinxUltraScalePlus::CAP_makeType2Instruction(CAP::Operation::RESERVED, payload);
}
/// Generate the encoding for NOP instructions and write them to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeNOP(ofstream& fout, int cnt, int payload, Endianess e)
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1NopInstruction(payload);
	for(int i = 0 ; i < cnt ; i++)
		FileIO::write32(fout, instruction, e);
}
/// Generate the encoding for Reserved instructions and write them to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeRESERVED(ofstream& fout, int cnt, int payload, Endianess e)
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1ReservedInstruction(payload);
	for(int i = 0 ; i < cnt ; i++)
		FileIO::write32(fout, instruction, e);
}
/// Generate the encoding for "selecting" a CAP register and write it to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeSelectRegister(ofstream& fout, CAP::Register reg, Endianess e)
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1WriteInstruction(reg, 0);
	FileIO::write32(fout, instruction, e);
}
/// Generate the encoding for reading a CAP register and write it to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeReadRegister(ofstream& fout, CAP::Register reg, int readLength, Endianess e)
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1ReadInstruction(reg, readLength);
	FileIO::write32(fout, instruction, e);
}
/// Generate the encoding for writing a CAP register and write it to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeRegister(ofstream& fout, CAP::Register reg, int writeValue, Endianess e)
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1WriteInstruction(reg, 1);
	FileIO::write32(fout, instruction, e);
	FileIO::write32(fout, writeValue, e);
}
/// Generate the encoding for writing a CAP command and write it to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeCommand(ofstream& fout, CAP::Command cmd, Endianess e)
{
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::CMD, static_cast<int>(cmd), e);
}
/// Generate the encoding for writing a CAP register with a mask and write it to file ofstream.
inline void XilinxUltraScalePlus::CAP_writeMaskAndRegister(ofstream& fout, CAP::Register reg, int writeMask, int writeValue, Endianess e)
{
	XilinxUltraScalePlus::CAP_writeRegister(fout, CAP::Register::MASK, writeMask, e);
	XilinxUltraScalePlus::CAP_writeRegister(fout, reg, writeValue, e);
}
/// Generate and write only a type 1 FDRI command.
inline void XilinxUltraScalePlus::CAP_writeFDRI1(ofstream& fout, int wordCount, Endianess e) 
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType1WriteInstruction(CAP::Register::FDRI, wordCount);
	FileIO::write32(fout, instruction, e);
}
/// Generate and write only a type 2 FDRI command.
inline void XilinxUltraScalePlus::CAP_writeType2(ofstream& fout, int wordCount, Endianess e) 
{
	uint32_t instruction = XilinxUltraScalePlus::CAP_makeType2WriteInstruction(wordCount);
	FileIO::write32(fout, instruction, e);
}
/// Generate and write an FDRI command. Always uses type 2 command for simplicity.
inline void XilinxUltraScalePlus::CAP_writeFDRI(ofstream& fout, int wordCount, Endianess e) 
{	
	XilinxUltraScalePlus::CAP_writeFDRI1(fout, 0, e);
	XilinxUltraScalePlus::CAP_writeType2(fout, wordCount, e);
}
/// Generate and write an SYNQ command.
inline void XilinxUltraScalePlus::CAP_writeSYNQ(ofstream& fout, Endianess e) 
{	
	FileIO::write32(fout, XilinxUltraScalePlus::CAP_makeSyncInstruction(), e);
}