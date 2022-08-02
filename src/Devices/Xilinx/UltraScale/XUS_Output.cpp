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
#include<string>
#include<stdexcept>
#include<fstream>
#include<cstring>

#include "XilinxUltraScale.h"
#include "../../../Common/FileIO.h"
#include "../../../Common/str.h"

using namespace std;

void XilinxUltraScale::outputBitstreamGlobalHeaderSequence(ofstream& fout, bool fullBitstream, Endianness e)
{
	
}

void XilinxUltraScale::outputBitstreamGlobalFooterSequence(ofstream& fout, bool fullBitstream, Endianness e)
{
	XCAP_writeNOP(fout, 400, 0, loadedBitstreamEndianness);
}

void XilinxUltraScale::outputBitstreamSLRHeaderBitstreamSequence(ofstream& fout, int slr, bool fullBitstream, Endianness e)
{
	XCAP_writeNOP(fout, 132, 0, e);
	XCAP_writeCommand(fout, XCAP::Command::RCRC, e);
	XCAP_writeNOP(fout, 2, 0, e);
	//XCAP_writeRegister(fout, XCAP::Register::MAGIC0, 0x00000000);
	XCAP_writeRegister(fout, XCAP::Register::IDCODE, SLRinfo[slr].IDCODE, e);
	uint32_t cor0 = XCAP_getCOR0value(7, 0, 0, 0, 0, 0, 0, DONE_CYCLE::Phase4, MATCH_CYCLE::NoWait, LOCK_CYCLE::NoWait, GTS_CYCLE::Phase5, GWE_CYCLE::Phase6); // no clue where the '7' comes from (ask Vivado)
	XCAP_writeRegister(fout, XCAP::Register::COR0, cor0, e);
	//XCAP_writeRegister(fout, XCAP::Register::COR1, 0x00400000);
	//XCAP_writeCommand(fout, XCAP::Command::SWITCH, e);
	//Optional Shutdown + 1 NOP ?
	XCAP_writeNOP(fout, 1, 0, e);
	XCAP_writeCommand(fout, XCAP::Command::RCRC, e);
	XCAP_writeNOP(fout, 20, 0, e);
	XCAP_writeCommand(fout, XCAP::Command::AGHIGH, e);
	XCAP_writeNOP(fout, 100, 0, e);
	XCAP_writeCommand(fout, XCAP::Command::NULLCMD, e);
	uint32_t ctrl0 = XCAP_getCTRL0value(0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0); //disable fallback bitstream, enable overwriting of LUTs
	XCAP_writeMaskAndRegister(fout, XCAP::Register::CTRL0, ctrl0, ctrl0, e);
	//XCAP_writeMaskAndRegister(fout, XCAP::Register::CTRL1, 0x20000, 0, e);// TODO: why does vivado do that? : bit at 0x20000 is undocumented
}

void XilinxUltraScale::outputBitstreamSLRFooterBitstreamSequence(ofstream& fout, int slr, bool fullBitstream, Endianness e)
{
	if(fullBitstream) // Add GRESTORE since it is full device bitstream.
	{
		XCAP_writeCommand(fout, XCAP::Command::GRESTORE, e);
		XCAP_writeNOP(fout, 2, 0, e);
		XCAP_writeCommand(fout, XCAP::Command::MAGIC2, e);
		XCAP_writeNOP(fout, 20, 0, e);
	}
	
	XCAP_writeNOP(fout, 1, 0, e);
	uint32_t ctrl0 = XCAP_getCTRL0value(0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0);
	XCAP_writeMaskAndRegister(fout, XCAP::Register::CTRL0, ctrl0, ctrl0, e);
	
	XCAP_writeCommand(fout, XCAP::Command::DGHIGH, e);
	XCAP_writeNOP(fout, 20, 0, e);
	
	
	XCAP_writeCommand(fout, XCAP::Command::START, e);
	XCAP_writeNOP(fout, 1, 0, e);
	
	XCAP_writeRegister(fout, XCAP::Register::FAR, XCAP_getFarFarInstruction(), e);
	XCAP_writeNOP(fout, 1, 0, e);
	XCAP_writeCommand(fout, XCAP::Command::RCRC, e);
	XCAP_writeNOP(fout, 2, 0, e);
	XCAP_writeCommand(fout, XCAP::Command::DESYNC, e);
	XCAP_writeNOP(fout, 16, 0, e);
}

void XilinxUltraScale::outputBitstreamSLRHeaderAfterBitstreamSequence(ofstream& fout, int slr, bool fullBitstream, Endianness e)
{
	if((slr + 1) < numberOfSLRs) { // If there are more SLRs, output the magic
		XCAP_writeNOP(fout, 400, 0, e);
		XCAP_writeSYNQ(fout, e);
		XCAP_writeNOP(fout, 1, 0, e);
		//Optional Shutdown + 1 NOP ?
		XCAP_writeCommand(fout, XCAP::Command::RCRC, e);
		XCAP_writeNOP(fout, 2, 0, e);
		
		XCAP_writeSelectRegister(fout, XCAP::Register::MAGIC1, e);
		slrMagicInstrLocation[slr] = fout.tellp();
		XCAP_writeType2(fout, 0, e);
	}
}

void XilinxUltraScale::outputBitstreamSLRWrapUpSequence(ofstream& fout, int slr, bool fullBitstream, Endianness e)
{
	streamoff currentStreamHead = fout.tellp();
	streamoff bypassSize = (currentStreamHead - slrMagicInstrLocation[slr])/4 - 1;
	fout.seekp(slrMagicInstrLocation[slr]);
	FileIO::write32(fout, static_cast<uint32_t>(bypassSize), e);
	fout.seekp(currentStreamHead);
	XCAP_writeNOP(fout, 16, 0, loadedBitstreamEndianness);
	XCAP_writeCommand(fout, XCAP::Command::START, loadedBitstreamEndianness);
	XCAP_writeNOP(fout, 1, 0, loadedBitstreamEndianness);
	XCAP_writeCommand(fout, XCAP::Command::DESYNC, loadedBitstreamEndianness);
	XCAP_writeNOP(fout, 16, 0, loadedBitstreamEndianness);
}

void XilinxUltraScale::outputBitstreamEmptySLRHeaderSequence(ofstream& fout, int slr, bool fullBitstream, Endianness e)
{
	XCAP_writeNOP(fout, 132, 0, e);
	uint32_t cor0 = XCAP_getCOR0value(7, 0, 0, 0, 0, 0, 0, DONE_CYCLE::Phase4, MATCH_CYCLE::NoWait, LOCK_CYCLE::NoWait, GTS_CYCLE::Phase5, GWE_CYCLE::Phase6); // no clue where the '7' comes from (ask Vivado)
	XCAP_writeRegister(fout, XCAP::Register::COR0, cor0, e);
	//Optional Shutdown + 1 NOP ?
	XCAP_writeCommand(fout, XCAP::Command::RCRC, e);
	XCAP_writeNOP(fout, 2, 0, e);
	XCAP_writeRegister(fout, XCAP::Register::IDCODE, SLRinfo[slr].IDCODE, e);
	XCAP_writeCommand(fout, XCAP::Command::NULLCMD, e);
	
	if((slr + 1) < numberOfSLRs) { // If there are more SLRs, output the magic
		XCAP_writeSelectRegister(fout, XCAP::Register::MAGIC1, e);
		slrMagicInstrLocation[slr] = fout.tellp();
		XCAP_writeType2(fout, 0, e);
	}
}

void XilinxUltraScale::outputBitstreamEmptySLRWrapUpSequence(ofstream& fout, int slr, bool fullBitstream, Endianness e)
{
	streamoff currentStreamHead = fout.tellp();
	streamoff bypassSize = (currentStreamHead - slrMagicInstrLocation[slr])/4 - 1;
	fout.seekp(slrMagicInstrLocation[slr]);
	FileIO::write32(fout, static_cast<uint32_t>(bypassSize), e);
	fout.seekp(currentStreamHead);
	XCAP_writeNOP(fout, 16, 0, loadedBitstreamEndianness);
	XCAP_writeCommand(fout, XCAP::Command::START, loadedBitstreamEndianness);
	XCAP_writeNOP(fout, 1, 0, loadedBitstreamEndianness);
	XCAP_writeCommand(fout, XCAP::Command::DESYNC, loadedBitstreamEndianness);
	XCAP_writeNOP(fout, 16, 0, loadedBitstreamEndianness);
}

/**************************************************************************//**
 * Creates a new bitstream file and writes the selected regions in it.
 *
 *
 * @arg @c filename Name of the output file.
 * @throws runtime_error if args don't define a correct region.
 *****************************************************************************/
void XilinxUltraScale::writeBitstream(string filename, string params, Rect2D cmdRect)
{
	size_t dotpos = filename.rfind(".");
	if(dotpos == string::npos)
		throw runtime_error(string("Invalid file name: \"").append(filename).append("\"!\n"));
	designName = filename.substr(0, dotpos);
	
	log("Writing Xilinx UltraScale bitstream to file \"" + filename + "\":");
	
	parseParams(params);
	
	if(selectedOptions.partialNotFull) {
		if((cmdRect.position.row % XUS_CLB_PER_CLOCK_REGION != 0) || (cmdRect.size.row % XUS_CLB_PER_CLOCK_REGION != 0))
			throw runtime_error(string("Currently only full clock region height relocations are supported (use row numbers multiple of ").append(to_string(XUS_CLB_PER_CLOCK_REGION)).append("."));
		if(cmdRect.size.row <= 0 || cmdRect.size.col <= 0)
			throw runtime_error("Invalid output size dimentions.");
	}
	
	ofstream fout (filename, ofstream::binary | ofstream::trunc);
	if(!fout.good())
		throw runtime_error(string("Could not open file: \"").append(filename).append("\"!\n"));
	if(str::iff::stringEndsWith(filename, ".bit"))
		writeBitstreamBIT(fout, cmdRect);
	else if(str::iff::stringEndsWith(filename, ".bin"))
		writeBitstreamBIN(fout, cmdRect);
	else
		throw runtime_error(string("Unknown Xilinx UltraScale file format tried to be written.\n"));
	fout.close();
	log("Xilinx UltraScale bitstream file \"" + filename + "\" written successfully.");
}
