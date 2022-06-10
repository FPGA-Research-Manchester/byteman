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

#ifndef XILINXCONFIGURATIONACCESSPORT_H
#define XILINXCONFIGURATIONACCESSPORT_H

#include<iostream>
#include<cstdint> //uint
#include<fstream>
#include<algorithm> //replace
#include<string>
#include<sstream>
#include<cstring> //memset

#include "Fabric.h"
#include "XCAP.h"

#include "../CommonDevice2D.h"
#include "../../Common/Endianness.h"

#include "../../Common/FileIO.h"
#include "../../Common/str.h"
#include "../../version.h" // To get byteman version

using namespace std;

//As indicator for number and types of variables in the template
#define FABRIC_TEMPLATE(BLOCKTYPE_LOGIC, BLOCKTYPE_BLOCKRAM, WORDS_BEFORE_CLK, WORDS_AT_CLK, WORDS_AFTER_CLK, WORDS_PER_FRAME, FRAMES_PER_BRAM_CONTENT_COLUMN, CLB_PER_CLOCK_REGION, BRAM_PER_CLOCK_REGION, EXTRA_FRAMES_PER_ROW, BT_SHIFT, BT_MASK, RA_SHIFT, RA_MASK, CA_SHIFT, CA_MASK, MA_SHIFT, MA_MASK) BLOCKTYPE_LOGIC, BLOCKTYPE_BLOCKRAM, WORDS_BEFORE_CLK, WORDS_AT_CLK, WORDS_AFTER_CLK, WORDS_PER_FRAME, FRAMES_PER_BRAM_CONTENT_COLUMN, CLB_PER_CLOCK_REGION, BRAM_PER_CLOCK_REGION, EXTRA_FRAMES_PER_ROW, BT_SHIFT, BT_MASK, RA_SHIFT, RA_MASK, CA_SHIFT, CA_MASK, MA_SHIFT, MA_MASK

template <int BLOCKTYPE_LOGIC,
	int BLOCKTYPE_BLOCKRAM,
	int WORDS_BEFORE_CLK,
	int WORDS_AT_CLK,
	int WORDS_AFTER_CLK,
	int WORDS_PER_FRAME,
	int FRAMES_PER_BRAM_CONTENT_COLUMN,
	int CLB_PER_CLOCK_REGION,
	int BRAM_PER_CLOCK_REGION,
	int EXTRA_FRAMES_PER_ROW,
	int FAR_BLOCKTYPE_SHIFT,
	int FAR_BLOCKTYPE_MASK,
	int FAR_ROWADDRESS_SHIFT,
	int FAR_ROWADDRESS_MASK,
	int FAR_COLUMNADDRESS_SHIFT,
	int FAR_COLUMNADDRESS_MASK,
	int FAR_MINORADDRESS_SHIFT,
	int FAR_MINORADDRESS_MASK>
class XilinxConfigurationAccessPort: virtual public CommonDevice2D
{
	public:
//Constructor & deconstructor
		XilinxConfigurationAccessPort()
		{
			initializedBitstreamPartName = "";
			loadedBitstreamEndianness = Endianness::BE;
			SLRinfo[0] = {0, 0, 0};
			numberOfRows = 0;
		};

		virtual ~XilinxConfigurationAccessPort()
		{
			if(bitstreamBegin != nullptr)
				delete bitstreamBegin;
		};

//Type definitions
		enum class MergeOP
		{
			SET, XOR, OR, AND
		};

		struct SelectedOptions
		{
			int clk, clb, bram, blank, partial, skipUnused;
			MergeOP op;
			SelectedOptions():clk(0),clb(0),bram(0),blank(0),partial(1),op(MergeOP::SET),skipUnused(1){}
		} selectedOptions;

//Base variables
	//Output
		streamoff headerLocationOfRemainingFileLength; ///< gets set by outputBITheader() and gets used by outputBITheaderLengthField().

	//bitstream meta
		string designName;
		string partName;
		string fileDate;
		string fileTime;

	//bitstream structure
		string initializedBitstreamPartName; 		///< The partName of currently initialized bitstream buffers
		Endianness loadedBitstreamEndianness;   		///< The endianess of the currently loaded bitstream.
		uint32_t* bitstreamBegin;
		uint32_t* bitstreamCLB[FABRIC_MAX_ROWS][FABRIC_MAX_COLS];
		uint32_t* bitstreamBRAM[FABRIC_MAX_ROWS][FABRIC_MAX_BRAM_COLS];
		uint32_t* bitstreamEnd;

	//resource string parameters
		string initializedResourceParamsPartName; ///< the partName of currently initialized resource string parameters
		int numberOfSLRs;
		int numberOfRows;
		int numberOfBRAMCols;
		int numberOfCols;
		int numberOfFramesBeforeCol[FABRIC_MAX_COLS];
		int numberOfBRAMsBeforeCol[FABRIC_MAX_COLS];
		int numberOfFramesPerRow;
		int numberOfWordsPerRow;

	//SLR data and resource string
		char resourceString[FABRIC_MAX_ROWS][FABRIC_MAX_COLS];
		struct {
			uint32_t IDCODE;
			int fromRow, toRow; 
		}SLRinfo[FABRIC_MAX_SLRS];

	//constant arrays defining selected fabric
		int LUT_numberOfFramesForResourceLetter[256] = DEFAULT_LOOKUP_FRAME_SIZES;
		string LUT_typeOfFrameForResourceLetter[256] = DEFAULT_LOOKUP_FRAME_NAMES;
		bool LUT_isFrameUnusedForResourceLetter[256] = DEFAULT_LOOKUP_FRAME_UNUSED;

//Base class functions are inlined and included here (this is so to force multiple instances that get optimized for the architectures' constants)
		#include "XCAP_inline_Blank.h"
		#include "XCAP_inline_Input.h"
		#include "XCAP_inline_Output.h"
		#include "XCAP_inline_Merge.h"
		#include "XCAP_inline_ParseParams.h"
		#include "XCAP_inline_CAP.h"

//Some functions that will need to be implemented by each architecture
	//get/set device type
		virtual int getDeviceByIDCODEorThrow(int) = 0;
		virtual int getDeviceByNameOrThrow(string) = 0;
		virtual int getDeviceByIDCODE(int) = 0;
		virtual int getDeviceByName(string) = 0;
		virtual void setDevice(int, string = "") = 0;
		virtual void setDeviceByPartNameOrThrow() = 0;
		virtual void setDeviceByNameOrThrow(string) = 0;
		virtual void setDeviceByIDCODEOrThrow(int) = 0;

	//initialize resource strings and bitstream buffers
		virtual void ensureInitializedBitstreamArrays() = 0;
		virtual void initializeResourceStringParameters() = 0;

	// output bitstream
		virtual void outputBitstreamGlobalHeaderSequence(ofstream&, bool, Endianness) = 0;
		virtual void outputBitstreamGlobalFooterSequence(ofstream&, bool, Endianness) = 0;
		
		virtual void outputBitstreamSLRHeaderBitstreamSequence(ofstream&, int, bool, Endianness) = 0;
		virtual void outputBitstreamSLRFooterBitstreamSequence(ofstream&, int, bool, Endianness) = 0;
		virtual void outputBitstreamSLRHeaderAfterBitstreamSequence(ofstream&, int, bool, Endianness) = 0;
		virtual void outputBitstreamSLRWrapUpSequence(ofstream&, int, bool, Endianness) = 0;
		
		virtual void outputBitstreamEmptySLRHeaderSequence(ofstream&, int, bool, Endianness) = 0;
		virtual void outputBitstreamEmptySLRWrapUpSequence(ofstream&, int, bool, Endianness) = 0;

	// Info 
		virtual string getFrameType(int, int, int) = 0;

	protected:
	private:
};

#endif //XILINXCONFIGURATIONACCESSPORT_H