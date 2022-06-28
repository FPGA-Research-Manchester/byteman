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

#define FABRIC_SYNC_WORD 0xAA995566

inline XCAP::Register getXCAPregister(string s)
{
	if(str::iff::stringContains(s, "CRC"))
		return XCAP::Register::CRC;
	else if(str::iff::stringContains(s, "FAR"))
		return XCAP::Register::FAR;
	else if(str::iff::stringContains(s, "FDRI"))
		return XCAP::Register::FDRI;
	else if(str::iff::stringContains(s, "FDRO"))
		return XCAP::Register::FDRO;
	else if(str::iff::stringContains(s, "CTRL0"))
		return XCAP::Register::CTRL0;
	else if(str::iff::stringContains(s, "MASK"))
		return XCAP::Register::MASK;
	else if(str::iff::stringContains(s, "STAT"))
		return XCAP::Register::STAT;
	else if(str::iff::stringContains(s, "LOUT"))
		return XCAP::Register::LOUT;
	else if(str::iff::stringContains(s, "COR0"))
		return XCAP::Register::COR0;
	else if(str::iff::stringContains(s, "MFWR"))
		return XCAP::Register::MFWR;
	else if(str::iff::stringContains(s, "CBC"))
		return XCAP::Register::CBC;
	else if(str::iff::stringContains(s, "IDCODE"))
		return XCAP::Register::IDCODE;
	else if(str::iff::stringContains(s, "AXSS"))
		return XCAP::Register::AXSS;
	else if(str::iff::stringContains(s, "COR1"))
		return XCAP::Register::COR1;
	else if(str::iff::stringContains(s, "WBSTAR"))
		return XCAP::Register::WBSTAR;
	else if(str::iff::stringContains(s, "TIMER"))
		return XCAP::Register::TIMER;
	else if(str::iff::stringContains(s, "MAGIC0"))
		return XCAP::Register::MAGIC0;
	else if(str::iff::stringContains(s, "BOOTSTS"))
		return XCAP::Register::BOOTSTS;
	else if(str::iff::stringContains(s, "CTRL1"))
		return XCAP::Register::CTRL1;
	else if(str::iff::stringContains(s, "MAGIC1"))
		return XCAP::Register::MAGIC1;
	else if(str::iff::stringContains(s, "BSPI"))
		return XCAP::Register::BSPI;

	return XCAP::Register::UNDEFINED;
}

inline XCAP::Command getXCAPcommand(string s)
{
	if(str::iff::stringContains(s, "NULL"))
		return XCAP::Command::NULLCMD;
	else if(str::iff::stringContains(s, "WCFG", "WRITE CONFIG"))
		return XCAP::Command::WCFG;
	else if(str::iff::stringContains(s, "MFW", "MULTIPLE FRAME WRITE"))
		return XCAP::Command::MFW;
	else if(str::iff::stringContains(s, "DGHIGH", "DEASSERT GHIGH"))
		return XCAP::Command::DGHIGH;
	else if(str::iff::stringContains(s, "RCFG", "READ CONFIG"))
		return XCAP::Command::RCFG;
	else if(str::iff::stringContains(s, "START"))
		return XCAP::Command::START;
	else if(str::iff::stringContains(s, "URAM", "CLEAR URAM"))
		return XCAP::Command::URAM;
	else if(str::iff::stringContains(s, "RCRC", "RESET CRC"))
		return XCAP::Command::RCRC;
	else if(str::iff::stringContains(s, "AGHIGH", "ASSERT GHIGH"))
		return XCAP::Command::AGHIGH;
	else if(str::iff::stringContains(s, "SWITCH"))
		return XCAP::Command::SWITCH;
	else if(str::iff::stringContains(s, "GRESTORE"))
		return XCAP::Command::GRESTORE;
	else if(str::iff::stringContains(s, "SHUTDOWN"))
		return XCAP::Command::SHUTDOWN;
	else if(str::iff::stringContains(s, "DESYNC"))
		return XCAP::Command::DESYNC;
	else if(str::iff::stringContains(s, "IPROG"))
		return XCAP::Command::IPROG;
	else if(str::iff::stringContains(s, "CRCC"))
		return XCAP::Command::CRCC;
	else if(str::iff::stringContains(s, "LTIMER"))
		return XCAP::Command::LTIMER;
	else if(str::iff::stringContains(s, "BSPI_READ"))
		return XCAP::Command::BSPI_READ;
	else if(str::iff::stringContains(s, "FALL_EDGE"))
		return XCAP::Command::FALL_EDGE;
	else if(str::iff::stringContains(s, "MAGIC2"))
		return XCAP::Command::MAGIC2;

	return XCAP::Command::UNDEFINED;
}

inline void writeXCAPcommandName(ofstream& fout, XCAP::Command commandID)
{
	switch(commandID){
		case XCAP::Command::NULLCMD:
			fout << "Null Command";
			break;
		case XCAP::Command::WCFG:
			fout << "Write Config";
			break;
		case XCAP::Command::MFW:
			fout << "Multiple Frame Write";
			break;
		case XCAP::Command::DGHIGH:
			fout << "Deassert GHIGH";
			break;
		case XCAP::Command::RCFG:
			fout << "Read Config";
			break;
		case XCAP::Command::START:
			fout << "Start";
			break;
		case XCAP::Command::URAM:
			fout << "Clear URAM";
			break;
		case XCAP::Command::RCRC:
			fout << "Reset CRC";
			break;
		case XCAP::Command::AGHIGH:
			fout << "Assert GHIGH";
			break;
		case XCAP::Command::SWITCH:
			fout << "Switch";
			break;
		case XCAP::Command::GRESTORE:
			fout << "GRestore";
			break;
		case XCAP::Command::SHUTDOWN:
			fout << "Shutdown";
			break;
		case XCAP::Command::DESYNC:
			fout << "Desync";
			break;
		case XCAP::Command::IPROG:
			fout << "IProg";
			break;
		case XCAP::Command::CRCC:
			fout << "CRCC";
			break;
		case XCAP::Command::LTIMER:
			fout << "LTimer";
			break;
		case XCAP::Command::BSPI_READ:
			fout << "BSPI_Read";
			break;
		case XCAP::Command::FALL_EDGE:
			fout << "Fall_Edge";
			break;
		case XCAP::Command::MAGIC2:
			fout << "MAGIC2";
			break;
		default:
			fout << string("UNKNOWN(").append(to_string(static_cast<int>(commandID))).append(")");
			break;
	}
}

inline void writeXCAPregisterName(ofstream& fout, XCAP::Register registerID)
{
	switch(registerID){
		case XCAP::Register::CRC:
			fout << "CRC";
			break;
		case XCAP::Register::FAR:
			fout << "FAR";
			break;
		case XCAP::Register::FDRI:
			fout << "FDRI";
			break;
		case XCAP::Register::FDRO:
			fout << "FDRO";
			break;
		case XCAP::Register::CMD:
			fout << "CMD";
			break;
		case XCAP::Register::CTRL0:
			fout << "CTRL0";
			break;
		case XCAP::Register::MASK:
			fout << "MASK";
			break;
		case XCAP::Register::STAT:
			fout << "STAT";
			break;
		case XCAP::Register::LOUT:
			fout << "LOUT";
			break;
		case XCAP::Register::COR0:
			fout << "COR0";
			break;
		case XCAP::Register::MFWR:
			fout << "MFWR";
			break;
		case XCAP::Register::CBC:
			fout << "CBC";
			break;
		case XCAP::Register::IDCODE:
			fout << "IDCODE";
			break;
		case XCAP::Register::AXSS:
			fout << "AXSS";
			break;
		case XCAP::Register::COR1:
			fout << "COR1";
			break;
		case XCAP::Register::WBSTAR:
			fout << "WBSTAR";
			break;
		case XCAP::Register::TIMER:
			fout << "TIMER";
			break;
		case XCAP::Register::MAGIC0:
			fout << "MAGIC0";
			break;
		case XCAP::Register::BOOTSTS:
			fout << "BOOTSTS";
			break;
		case XCAP::Register::CTRL1:
			fout << "CTRL1";
			break;
		case XCAP::Register::MAGIC1:
			fout << "MAGIC1";
			break;
		case XCAP::Register::BSPI:
			fout << "BSPI";
			break;
		default:
			fout << string("UNKNOWN(").append(to_string(static_cast<int>(registerID))).append(")");
			break;
	}
}

/// Parses and returns instruction type. Valid Xil instructions will be of types 1 and 2
inline uint32_t XCAP_getInstructionType(uint32_t instruction)
{
	return ((instruction>>29) & 0x7); // type is @ bits [31:29]
}

/// Parses and returns instruction operation. Most Xil instructions will NOP or write.
inline XCAP::Operation XCAP_getInstructionOperation(uint32_t instruction)
{
	return static_cast<XCAP::Operation>((instruction>>27) & 0x3); //Operation is @ bits [28:27]
}

/// Parses and returns instruction payload. This is the immediate value after instruction type and operation encodings.
inline uint32_t XCAP_getInstructionPayload(uint32_t instruction)
{
	return (instruction & 0x07FFFFFF);	//Payload in type 2 instructions is @ bits [26:0]
}

/// Parses and returns instruction register. This is the register being addressed if the instruction is of type 1
inline XCAP::Register XCAP_getInstructionRegister(uint32_t instruction)
{
	return static_cast<XCAP::Register>((instruction>>13) & 0x1F);	//Register in type 1 instructions is @ bits [17:13]
}

/// Parses and returns instruction word count. This is the number of words to be read/written if the instruction is of type 1
inline uint32_t XCAP_getInstructionWordCount(uint32_t instruction)
{
	return (instruction & 0x7FF);	//WordCount in type 1 instructions is @ bits [10:0]
}

/// Generate and return the encoding for an instruction.
inline uint32_t XCAP_getInstruction(int type, XCAP::Operation operation, XCAP::Register reg, int payload)
{
	//type 1 instr: type@[31:29] ; operation@[28:27] ; reg@[17:13] ; payload@[10:0]
	//type 2 instr: type@[31:29] ; operation@[28:27] ; payload@[26:0]
	//This function works for both types, but is not safe for bad instructions
	return (((type & 0x7) << 29) | ((static_cast<int>(operation) & 0x3) << 27) | ((static_cast<int>(reg) & 0x1F) << 13) | (payload & 0x07FFFFFF));
}

/// Generate and return the encoding for a type 1 instruction.
inline uint32_t XCAP_getType1Instruction(XCAP::Operation operation, XCAP::Register reg, int payload)
{
	return XCAP_getInstruction(1, operation, reg, (payload & 0x7FF));
}

/// Generate and return the encoding for a type 1 NOP instruction.
inline uint32_t XCAP_getType1NopInstruction(int payload)
{
	return XCAP_getType1Instruction(XCAP::Operation::NOP, static_cast<XCAP::Register>(0), payload);
}

/// Generate and return the encoding for a type 1 Read instruction.
inline uint32_t XCAP_getType1ReadInstruction(XCAP::Register reg, int payload)
{
	return XCAP_getType1Instruction(XCAP::Operation::READ, reg, (payload & 0x7FF));
}

/// Generate and return the encoding for a type 1 Write instruction.
inline uint32_t XCAP_getType1WriteInstruction(XCAP::Register reg, int payload)
{
	return XCAP_getType1Instruction(XCAP::Operation::WRITE, reg, (payload & 0x7FF));
}

/// Generate and return the encoding for a type 1 Reserved instruction.
inline uint32_t XCAP_getType1ReservedInstruction(int payload)
{
	return XCAP_getType1Instruction(XCAP::Operation::RESERVED, static_cast<XCAP::Register>(0), payload);
}

/// Generate and return the encoding for a type 2 instruction.
inline uint32_t XCAP_getType2Instruction(XCAP::Operation operation, int payload)
{
	return XCAP_getInstruction(2, operation, static_cast<XCAP::Register>(0), payload);
}

/// Generate and return the encoding for a type 2 NOP instruction.
inline uint32_t XCAP_getType2NopInstruction(int payload)
{
	return XCAP_getType2Instruction(XCAP::Operation::NOP, payload);
}

/// Generate and return the encoding for a type 2 Read instruction.
inline uint32_t XCAP_getType2ReadInstruction(int payload)
{
	return XCAP_getType2Instruction(XCAP::Operation::READ, (payload & 0x07FFFFFF));
}

/// Generate and return the encoding for a type 2 Write instruction.
inline uint32_t XCAP_getType2WriteInstruction(int payload)
{
	return XCAP_getType2Instruction(XCAP::Operation::WRITE, (payload & 0x07FFFFFF));
}

/// Generate and return the encoding for a type 2 Reserved instruction.
inline uint32_t XCAP_getType2ReservedInstruction(int payload)
{
	return XCAP_getType2Instruction(XCAP::Operation::RESERVED, payload);
}

/// Generate the encoding for NOP instructions and write them to file ofstream.
inline void XCAP_writeNOP(ofstream& fout, int cnt, int payload, Endianness e)
{
	uint32_t instruction = XCAP_getType1NopInstruction(payload);
	for(int i = 0 ; i < cnt ; i++)
		FileIO::write32(fout, instruction, e);
}

/// Generate the encoding for Reserved instructions and write them to file ofstream.
inline void XCAP_writeRESERVED(ofstream& fout, int cnt, int payload, Endianness e)
{
	uint32_t instruction = XCAP_getType1ReservedInstruction(payload);
	for(int i = 0 ; i < cnt ; i++)
		FileIO::write32(fout, instruction, e);
}

/// Generate the encoding for "selecting" a CAP register and write it to file ofstream.
inline void XCAP_writeSelectRegister(ofstream& fout, XCAP::Register reg, Endianness e)
{
	uint32_t instruction = XCAP_getType1WriteInstruction(reg, 0);
	FileIO::write32(fout, instruction, e);
}

/// Generate the encoding for reading a CAP register and write it to file ofstream.
inline void XCAP_writeReadRegister(ofstream& fout, XCAP::Register reg, int readLength, Endianness e)
{
	uint32_t instruction = XCAP_getType1ReadInstruction(reg, readLength);
	FileIO::write32(fout, instruction, e);
}

/// Generate the encoding for writing a CAP register and write it to file ofstream.
inline void XCAP_writeRegister(ofstream& fout, XCAP::Register reg, int writeValue, Endianness e)
{
	uint32_t instruction = XCAP_getType1WriteInstruction(reg, 1);
	FileIO::write32(fout, instruction, e);
	FileIO::write32(fout, writeValue, e);
}

/// Generate the encoding for writing a CAP command and write it to file ofstream.
inline void XCAP_writeCommand(ofstream& fout, XCAP::Command cmd, Endianness e)
{
	XCAP_writeRegister(fout, XCAP::Register::CMD, static_cast<int>(cmd), e);
}

/// Generate the encoding for writing a CAP register with a mask and write it to file ofstream.
inline void XCAP_writeMaskAndRegister(ofstream& fout, XCAP::Register reg, int writeMask, int writeValue, Endianness e)
{
	XCAP_writeRegister(fout, XCAP::Register::MASK, writeMask, e);
	XCAP_writeRegister(fout, reg, writeValue, e);
}

/// Generate and write only a type 1 FDRI command.
inline void XCAP_writeFDRI1(ofstream& fout, int wordCount, Endianness e)
{
	uint32_t instruction = XCAP_getType1WriteInstruction(XCAP::Register::FDRI, wordCount);
	FileIO::write32(fout, instruction, e);
}

/// Generate and write only a type 2 FDRI command.
inline void XCAP_writeType2(ofstream& fout, int wordCount, Endianness e)
{
	uint32_t instruction = XCAP_getType2WriteInstruction(wordCount);
	FileIO::write32(fout, instruction, e);
}

/// Generate and write an FDRI command. Always uses type 2 command for simplicity.
inline void XCAP_writeFDRI(ofstream& fout, int wordCount, Endianness e)
{
	XCAP_writeFDRI1(fout, 0, e);
	XCAP_writeType2(fout, wordCount, e);
}

/// Generate and return the encoding for a IDCODE writing instruction.
inline uint32_t XCAP_IDCODEInstruction()
{
	return XCAP_getType1WriteInstruction(XCAP::Register::IDCODE, 1);
}

/// Generate and return the encoding for a SYNC instruction.
inline uint32_t XCAP_getSyncInstruction()
{
	return FABRIC_SYNC_WORD;
}

/// Generate and write an SYNQ command.
inline void XCAP_writeSYNQ(ofstream& fout, Endianness e)
{
	FileIO::write32(fout, XCAP_getSyncInstruction(), e);
}

enum class DONE_CYCLE{
	//Start-up cycle to release the DONE pin:
	Phase1 = 0,
	Phase2 = 1,
	Phase3 = 2,
	Phase4 = 3,
	Phase5 = 4,
	Phase6 = 5
};
enum class MATCH_CYCLE{
	//Start-up cycle to stall in until DCI matches:
	Phase0 = 0,
	Phase1 = 1,
	Phase2 = 2,
	Phase3 = 3,
	Phase4 = 4,
	Phase5 = 5,
	Phase6 = 6,
	NoWait = 7
};
enum class LOCK_CYCLE{
	//Start-up cycle to stall in until MMCMs lock:
	Phase0 = 0,
	Phase1 = 1,
	Phase2 = 2,
	Phase3 = 3,
	Phase4 = 4,
	Phase5 = 5,
	Phase6 = 6,
	NoWait = 7
};
enum class GTS_CYCLE{
	//Start-up cycle to deassert the Global 3-State (GTS) signal:
	Phase1 = 0,
	Phase2 = 1,
	Phase3 = 2,
	Phase4 = 3,
	Phase5 = 4,
	Phase6 = 5,
	GTS_DONE = 6 ///<GTS tracks DONE pin. Bitstream property: GTS_CYCLE:DONE.
};
enum class GWE_CYCLE{
	//Start-up phase to deassert the Global Write Enable (GWE) signal:
	Phase1 = 0,
	Phase2 = 1,
	Phase3 = 2,
	Phase4 = 3,
	Phase5 = 4,
	Phase6 = 5,
	GWE_DONE = 6 ///<GWE tracks DONE pin. Bitstream property: GWE_CYCLE:DONE
};
/// Generate COR0 register write value
inline uint32_t XCAP_getCOR0value(int Reserved_31_27, int ECLK_EN, int Reserved_25, int DRIVE_DONE, int Reserved_23, int OSCFSEL, int Reserved_16_15, DONE_CYCLE selDONE_CYCLE, MATCH_CYCLE selMATCH_CYCLE, LOCK_CYCLE selLOCK_CYCLE, GTS_CYCLE selGTS_CYCLE, GWE_CYCLE selGWE_CYCLE)
{
	uint32_t retValue = 0;
	retValue |= Reserved_31_27<<27;
	retValue |= ECLK_EN<<26;
	retValue |= Reserved_25<<25;
	retValue |= DRIVE_DONE<<24;
	retValue |= Reserved_23<<23;
	retValue |= OSCFSEL<<17;
	retValue |= Reserved_16_15<<15;
	retValue |= static_cast<int>(selDONE_CYCLE)<<12;
	retValue |= static_cast<int>(selMATCH_CYCLE)<<9;
	retValue |= static_cast<int>(selLOCK_CYCLE)<<6;
	retValue |= static_cast<int>(selGTS_CYCLE)<<3;
	retValue |= static_cast<int>(selGWE_CYCLE);
	return retValue;
}


/// Generate CTRL0 register write value
inline uint32_t XCAP_getCTRL0value(int EFUSE_KEY, int ICAP_SELECT, int Reserved_29_13, int OverTempShutDown, int Reserved_11, int ConfigFallback, int Reserved_9, int GLUTMASK_B, int Reserved_7, int DEC, int SBITS, int PERSIST, int Reserved_2_1, int GTS_USR_B)
{
	uint32_t retValue = 0;
	retValue |= EFUSE_KEY<<31; //Selects the AES key source: 0: Battery-backed RAM (default) 1: eFUSE
	retValue |= ICAP_SELECT<<30; //ICAPE3 port select: 0: Top ICAPE3 port enabled (default) 1: Bottom ICAPE3 port enabled
	retValue |= Reserved_29_13<<13;
	retValue |= OverTempShutDown<<12; //high-active: enables over-temperature shutdown (default:0)
	retValue |= Reserved_11<<11;
	retValue |= ConfigFallback<<10; //Stops when configuration fails and disables fallback to the default bitstream. 0: Enables fallback (default)
	retValue |= Reserved_9<<9;
	retValue |= GLUTMASK_B<<8; //Global LUT mask signal. Masks any changeable memory cell readback value. 0: Masks changeable memory cell readback value, such as distributed RAM or SRL 1: Does not mask changeable memory cell readback values(default)
	retValue |= Reserved_7<<7;
	retValue |= DEC<<6; //AES decryptor enable bit: 0: Decryptor disabled (default) 1: Decryptor enabled
	retValue |= SBITS<<4; //Security level. The FPGA security level is extended to encrypted bitstreams. It is applicable to the configuration port, not to ICAPE3. The security level takes affect at the end of the encrypted bitstream or after EOS for an unencrypted bitstream. 00: Read/write OK (default) 01: Readback disabled 1x: Both writes and reads disabled Only FAR and FDRI allow encrypt write access for security levels 00 and 01.
	retValue |= PERSIST<<3; //The configuration interface defined by M2:M0 remains after configuration. Typically used only with the SelectMAP interface to allow reconfiguration and readback. See Chapter 5, SelectMAP Configuration Modes. 0: No (default) 1: Yes
	retValue |= Reserved_2_1<<1;
	retValue |= GTS_USR_B; //Active-Low global 3-state I/Os. Turns off pull-ups if GTS_CFG_B is also asserted. 0: I/Os 3-stated 1: I/Os active (default)
	return retValue;
}
