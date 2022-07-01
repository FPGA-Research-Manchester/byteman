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

#ifndef XILINXSERIES7_H
#define XILINXSERIES7_H

#include<cstdint> //uint
#include<string>

#include "XS7_Fabric.h"
#include "../../CommonDevice2D.h"
#include "../XilinxConfigurationAccessPort.h"
#include "../../../Common/Endianness.h"
#include "../../../Common/Coords.h"

using namespace std;

class XilinxSeries7: virtual public CommonDevice2D, public XilinxConfigurationAccessPort<archXS7template>
{
	public:
		XilinxSeries7();
		virtual ~XilinxSeries7();
		
		void blank(string);
		
		void ensureInitializedBitstreamArrays() override;
		
		//Devices.cpp:
		int getDeviceByIDCODEorThrow(int) override;
		int getDeviceByNameOrThrow(string) override;
		int getDeviceByIDCODE(int) override;
		int getDeviceByName(string) override;
		void setDevice(int, string = "") override;
		void setDeviceByPartNameOrThrow() override;
		void setDeviceByNameOrThrow(string) override;
		void setDeviceByIDCODEOrThrow(int) override;
		
		streamoff slrMagicInstrLocation[XS7_MAX_SLRS];/// Written by outputBitstreamSLRHeaderAfterBitstreamSequence(), outputBitstreamEmptySLRHeaderSequence() and used by outputBitstreamSLRWrapUpSequence(), outputBitstreamEmptySLRWrapUpSequence()
		void outputBitstreamGlobalHeaderSequence(ofstream&, bool, Endianness) override;
		void outputBitstreamGlobalFooterSequence(ofstream&, bool, Endianness) override;
		
		void outputBitstreamSLRHeaderBitstreamSequence(ofstream&, int, bool, Endianness) override;
		void outputBitstreamSLRFooterBitstreamSequence(ofstream&, int, bool, Endianness) override;
		void outputBitstreamSLRHeaderAfterBitstreamSequence(ofstream&, int, bool, Endianness) override;
		void outputBitstreamSLRWrapUpSequence(ofstream&, int, bool, Endianness) override;
		
		void outputBitstreamEmptySLRHeaderSequence(ofstream&, int, bool, Endianness) override;
		void outputBitstreamEmptySLRWrapUpSequence(ofstream&, int, bool, Endianness) override;
		
		//Info
		void deviceHelp();
		
		//file IO
		
		//merge (relocate)
		//void ensureRowCompatibility(Coord2D, int, int, Coord2D);
		//void ensureRegionCompatibility(Rect2D, Coord2D);
		void merge(XilinxSeries7*, string, Rect2D, Coord2D);
		
		void readBitstream(string);
		//The ones below are not needed to be public, but we don't really care
		void writeBitstream(string, string, Rect2D);
		
		//resource string parameters
		void initializeResourceStringParameters() override;
		
		//assembler
		void assemblerParseHeader(ifstream&);
		void assemblerAsmToBit(ifstream&, ofstream&);
		void assemblerAsmToBin(ifstream&, ofstream&);
		void assemblerAsmTo(ifstream&, ofstream&);
		
		void disassemblerWriteHeader(ofstream&);
		void disassemblerBitToAsm(ifstream&, ofstream&);
		void disassemblerBinToAsm(string, ifstream&, ofstream&);
		void disassemblerToAsm(ifstream&, ofstream&);
		
		void assemblerHelp();
		void assembler(string, string);
		
		// Info 
		string getFrameType(int, int, int) override;
		
		//Test
		#if !defined(NDEBUG)
		void test(bool, bool, uint32_t);
		#endif
		
		void initFabric();
};

#endif //XILINXSERIES7_H
