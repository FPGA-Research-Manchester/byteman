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

#include "../../CommonDevice2D.h"
#include "../XilinxConfigurationAccessPort.h"
#include "XilinxUltraScalePlusConfigurationAccessPort.h"
#include "XilinxUltraScalePlusFabric.h"
#include "../../../Common/Endianess.h"
#include "../../../Common/Coords.h"

using namespace std;

class XilinxUltraScalePlus: public CommonDevice2D, public XilinxConfigurationAccessPort
{
    public:
        XilinxUltraScalePlus();
        virtual ~XilinxUltraScalePlus();
		
		enum class MergeOP
		{
			SET,
			XOR,
			OR,
			AND
		};
		struct SelectedOptions
		{
			int clk, clb, bram, blank, partial;
			MergeOP op;
			SelectedOptions():clk(0),clb(0),bram(0),blank(0),partial(1),op(MergeOP::SET){}
		};
		
		//XilinxUltraScalePlusDevices.cpp:
		int getDeviceByIDCODEorThrow(int);
		int getDeviceByNameOrThrow(string);
		int getDeviceByIDCODE(int);
		int getDeviceByName(string);
		void setDevice(int, string = "");
		
		//parse
		SelectedOptions parseParams(string);
		//Info
		static void deviceHelp();
		
		//file IO
        void readBitstream(string);
        void readBitstreamBIT(ifstream&);
        void readBitstreamBIN(ifstream&);
        void readBitstreamMain(ifstream&);
		
        //merge (relocate)
		void ensureRowCompatibility(Coord2D, int, int, Coord2D);
		void ensureRegionCompatibility(Rect2D, Coord2D);
        void merge(XilinxUltraScalePlus*, string, Rect2D, Coord2D);
		void fastMerge(XilinxUltraScalePlus*,  SelectedOptions, Rect2D, Coord2D);
		void flexiMerge(XilinxUltraScalePlus*, SelectedOptions, Endianess, Rect2D, Coord2D);
		//
		void blank(string);
		
		//The ones below are not needed to be public, but we don't really care
        void writeBitstream(string, string, Rect2D);
        void writeBitstreamBIT(ofstream&, string, Rect2D, SelectedOptions);
        void writeBitstreamBIN(ofstream&, string, Rect2D, SelectedOptions);
		void writeBitstreamMainSingleRegion(ofstream&, Rect2D, SelectedOptions);
		void writeBitstreamMain(ofstream&, string, Rect2D, SelectedOptions);
		
		
		
		//helper frame
		void* blankFrame;
		//bitstream
        string initializedBitstreamPartName; 		///< The partName of currently initialized bitstream buffers
		Endianess loadedBitstreamEndianess;   		///< The endianess of the currently loaded bitstream.
        void ensureInitializedBitstreamArrays();
        uint32_t* bitstreamBegin;
        uint32_t* bitstreamCLB[XUSP_MAX_ROWS][XUSP_MAX_COLS];
        uint32_t* bitstreamBRAM[XUSP_MAX_ROWS][XUSP_MAX_BRAM_COLS];
		uint32_t* bitstreamEnd;
		
		//device data and resource string
        char resourceString[XUSP_MAX_ROWS][XUSP_MAX_COLS];
		struct {
			uint32_t IDCODE;
			int fromRow, toRow; 
		}SLRinfo[XUSP_MAX_SLRS];
		
		//resource string parameters
        string initializedResourceParamsPartName; ///< the partName of currently initialized resource string parameters
		void initializeResourceStringParameters();
        int numberOfSLRs;
        int numberOfRows;
        int numberOfBRAMCols;
        int numberOfCols;
        int numberOfFramesBeforeCol[XUSP_MAX_COLS];
        int numberOfBRAMsBeforeCol[XUSP_MAX_COLS];
        int numberOfFramesPerRow;
        int numberOfWordsPerRow;
		
		//assembler
		void assemblerAsmToBit(ifstream&, ofstream&);
		void disassemblerWriteHeader(ofstream&);
		void disassemblerBitToAsm(ifstream&, ofstream&);
		static void assemblerHelp();
		void assembler(string, string);
		// Info 
		string getFrameType(int, int, int);
		
		// CAP FAR
		inline void CAP_IncrementFAR_BlockType0(int&, int&, int&, int&);
		inline void CAP_IncrementFAR_BlockType1(int&, int&, int&, int&);
		inline void CAP_IncrementFAR(int&, int&, int&, int&);
		
		// XCAP: override functions for superclass XilinxConfigurationAccessPort
		uint32_t XCAP_SyncInstruction() override;
		uint32_t XCAP_IDCODEInstruction() override;
		
		
		// CAP: local inline alternatives
		inline uint32_t CAP_makeSyncInstruction();
		
		inline uint32_t CAP_makeInstruction(int, CAP::Operation, CAP::Register, int);
		inline uint32_t CAP_makeType1Instruction(CAP::Operation, CAP::Register, int);
		inline uint32_t CAP_makeType1NopInstruction(int=0);
		inline uint32_t CAP_makeType1ReadInstruction(CAP::Register, int);
		inline uint32_t CAP_makeType1WriteInstruction(CAP::Register, int);
		inline uint32_t CAP_makeType1ReservedInstruction(int=0);
		
		inline uint32_t CAP_makeType2Instruction(CAP::Operation, int);
		inline uint32_t CAP_makeType2NopInstruction(int=0);
		inline uint32_t CAP_makeType2ReadInstruction(int);
		inline uint32_t CAP_makeType2WriteInstruction(int);
		inline uint32_t CAP_makeType2ReservedInstruction(int=0);
		
		inline int CAP_getInstructionType(int);
		inline CAP::Operation CAP_getInstructionOperation(int);
		inline int CAP_getInstructionPayload(int);
		inline CAP::Register CAP_getInstructionRegister(int);
		inline int CAP_getInstructionWordCount(int);
		inline void CAP_parseFAR(int, int&, int&, int&, int&);
		inline uint32_t CAP_makeFAR(int, int, int, int);
		
		//CAP write
		inline void CAP_writeSelectRegister(ofstream&, CAP::Register, Endianess=Endianess::NATIVE);
		inline void CAP_writeReadRegister(ofstream&, CAP::Register, int, Endianess=Endianess::NATIVE);
		inline void CAP_writeNOP(ofstream&, int=0, int=0, Endianess=Endianess::NATIVE);
		inline void CAP_writeRESERVED(ofstream&, int=0, int=0, Endianess=Endianess::NATIVE);
		inline void CAP_writeRegister(ofstream&, CAP::Register, int, Endianess=Endianess::NATIVE);
		inline void CAP_writeMaskAndRegister(ofstream&, CAP::Register, int, int, Endianess=Endianess::NATIVE);
		inline void CAP_writeCommand(ofstream&, CAP::Command, Endianess=Endianess::NATIVE);
		inline void CAP_writeFDRI1(ofstream&, int, Endianess=Endianess::NATIVE);
		inline void CAP_writeFDRI2(ofstream&, int, Endianess=Endianess::NATIVE);
		inline void CAP_writeFDRI(ofstream&, int, Endianess=Endianess::NATIVE);
		
    protected:
    private:
		//constant arrays defining US+ fabric
        int const numberOfFramesPerResourceLetter[256] = XUSP_LOOKUP_FRAME_SIZES;
        char const *typeOfFramesPerResourceLetter[256] = XUSP_LOOKUP_FRAME_NAMES;
};

#endif // XILINXULTRASCALEPLUS_H
