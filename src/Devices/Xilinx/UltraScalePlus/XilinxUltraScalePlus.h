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

#ifndef XILINXULTRASCALEPLUS_H
#define XILINXULTRASCALEPLUS_H

#include<cstdint> //uint
#include<string>

#include "XUSP_Fabric.h"
#include "../../CommonDevice2D.h"
#include "../XilinxConfigurationAccessPort.h"
#include "../../../Common/Endianness.h"
#include "../../../Common/Coords.h"



class XilinxUltraScalePlus: virtual public CommonDevice2D, public XilinxConfigurationAccessPort<archXUSPtemplate>
{
	public:
		XilinxUltraScalePlus();
		virtual ~XilinxUltraScalePlus();
		
		void blank(std::string);
		
		void ensureInitializedBitstreamArrays() override;
		
		//Devices.cpp:
		int getDeviceByIDCODEorThrow(int) override;
		int getDeviceByNameOrThrow(std::string) override;
		int getDeviceByIDCODE(int) override;
		int getDeviceByName(std::string) override;
		void setDevice(int, std::string = "") override;
		void setDeviceByPartNameOrThrow() override;
		void setDeviceByNameOrThrow(std::string) override;
		void setDeviceByIDCODEOrThrow(int) override;
		
		std::streamoff slrMagicInstrLocation[XUSP_MAX_SLRS];/// Written by outputBitstreamSLRHeaderAfterBitstreamSequence(), outputBitstreamEmptySLRHeaderSequence() and used by outputBitstreamSLRWrapUpSequence(), outputBitstreamEmptySLRWrapUpSequence()
		void outputBitstreamGlobalHeaderSequence(std::ofstream&, bool, Endianness) override;
		void outputBitstreamGlobalFooterSequence(std::ofstream&, bool, Endianness) override;
		
		void outputBitstreamSLRHeaderBitstreamSequence(std::ofstream&, int, bool, Endianness) override;
		void outputBitstreamSLRFooterBitstreamSequence(std::ofstream&, int, bool, Endianness) override;
		void outputBitstreamSLRHeaderAfterBitstreamSequence(std::ofstream&, int, bool, Endianness) override;
		void outputBitstreamSLRWrapUpSequence(std::ofstream&, int, bool, Endianness) override;
		
		void outputBitstreamEmptySLRHeaderSequence(std::ofstream&, int, bool, Endianness) override;
		void outputBitstreamEmptySLRWrapUpSequence(std::ofstream&, int, bool, Endianness) override;
		
		//Info
		void deviceHelp();
		
		//file IO
		
		//merge (relocate)
		//void ensureRowCompatibility(Coord2D, int, int, Coord2D);
		//void ensureRegionCompatibility(Rect2D, Coord2D);
		void merge(XilinxUltraScalePlus*, std::string, Rect2D, Coord2D);
		
		void readBitstream(std::string);
		//The ones below are not needed to be public, but we don't really care
		void writeBitstream(std::string, std::string, Rect2D);
		
		//resource std::string parameters
		void initializeResourceStringParameters() override;
		
		//assembler
		void assemblerParseHeader(std::ifstream&);
		void assemblerAsmToBit(std::ifstream&, std::ofstream&);
		void assemblerAsmToBin(std::ifstream&, std::ofstream&);
		void assemblerAsmTo(std::ifstream&, std::ofstream&);
		
		void disassemblerWriteHeader(std::ofstream&);
		void disassemblerBitToAsm(std::ifstream&, std::ofstream&);
		void disassemblerBinToAsm(std::string, std::ifstream&, std::ofstream&);
		void disassemblerToAsm(std::ifstream&, std::ofstream&);
		
		void assembler(std::string, std::string);
		
		// Info 
		std::string getFrameType(int, int, int) override;
		
		//Test
		#if !defined(NDEBUG)
		void test(bool, bool, uint32_t);
		#endif
		//change
		void change(std::string);
		
		void initFabric();
};

#endif // XILINXULTRASCALEPLUS_H
