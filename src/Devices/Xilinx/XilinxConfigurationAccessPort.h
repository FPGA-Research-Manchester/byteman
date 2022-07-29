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

#include "XCAP/XCAP.h"

#include "../CommonDevice2D.h"
#include "../../Common/Endianness.h"

#include "../../Common/assert.h"
#include "../../Common/FileIO.h"
#include "../../Common/str.h"
#include "../../../VERSION" // To get byteman version

using namespace std;

//As indicator for number and types of variables in the template
#define FABRIC_TEMPLATE(MAX_SLRS, MAX_ROWS, MAX_COLS, MAX_BRAM_COLS, BLOCKTYPE_MAX, BLOCKTYPE_LOGIC, BLOCKTYPE_BLOCKRAM, WORDS_BEFORE_CLK, WORDS_AT_CLK, WORDS_AFTER_CLK, WORDS_PER_FRAME, FRAMES_PER_BRAM_CONTENT_COLUMN, CLB_PER_CLOCK_REGION, BRAM_PER_CLOCK_REGION, EXTRA_FRAMES_PER_ROW, ENABLE_TOP_BOTTOM, TOP_BOTTOM_BIT_SHIFT, BLOCKTYPE_SHIFT, BLOCKTYPE_MASK, ROWADDRESS_SHIFT, ROWADDRESS_MASK, COLUMNADDRESS_SHIFT, COLUMNADDRESS_MASK, MINORADDRESS_SHIFT, MINORADDRESS_MASK) MAX_SLRS, MAX_ROWS, MAX_COLS, MAX_BRAM_COLS, BLOCKTYPE_MAX, BLOCKTYPE_LOGIC, BLOCKTYPE_BLOCKRAM, WORDS_BEFORE_CLK, WORDS_AT_CLK, WORDS_AFTER_CLK, WORDS_PER_FRAME, FRAMES_PER_BRAM_CONTENT_COLUMN, CLB_PER_CLOCK_REGION, BRAM_PER_CLOCK_REGION, EXTRA_FRAMES_PER_ROW, ENABLE_TOP_BOTTOM, TOP_BOTTOM_BIT_SHIFT, BLOCKTYPE_SHIFT, BLOCKTYPE_MASK, ROWADDRESS_SHIFT, ROWADDRESS_MASK, COLUMNADDRESS_SHIFT, COLUMNADDRESS_MASK, MINORADDRESS_SHIFT, MINORADDRESS_MASK

template <int MAX_SLRS,
	int MAX_ROWS,
	int MAX_COLS,
	int MAX_BRAM_COLS,
	int BLOCKTYPE_MAX,
	int BLOCKTYPE_LOGIC,
	int BLOCKTYPE_BLOCKRAM,
	int WORDS_BEFORE_CLK,
	int WORDS_AT_CLK,
	int WORDS_AFTER_CLK,
	int WORDS_PER_FRAME,
	int FRAMES_PER_BRAM_CONTENT_COLUMN,
	int CLB_PER_CLOCK_REGION,
	int BRAM_PER_CLOCK_REGION,
	int EXTRA_FRAMES_PER_ROW,
	int FAR_ENABLE_TOP_BOTTOM_BIT,
	int FAR_TOP_BOTTOM_BIT_SHIFT,
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
			bitstreamBegin = nullptr;
			initializedBitstreamShortPartName = "";
			initializedBitstreamParamsShortPartName = "";
			initializedResourceStringShortPartName = "";
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
			bool clk, clb, bram, blank, partialNotFull, skipUnused, forceEndianness;
			MergeOP op;
			int intParam;
			Endianness forcedEndianness;
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
		string initializedBitstreamShortPartName; 		///< The partName of currently initialized bitstream buffers
		Endianness loadedBitstreamEndianness;   		///< The endianess of the currently loaded bitstream.
		bool bitstreamHasValidData;
		uint32_t* bitstreamBegin;
		uint32_t* bitstreamCLB[MAX_ROWS][MAX_COLS];
		uint32_t* bitstreamBRAM[MAX_ROWS][MAX_BRAM_COLS];
		uint32_t* bitstreamEnd;

	//resource string parameters
		string initializedBitstreamParamsShortPartName; ///< the partName of currently initialized resource string parameters
		int numberOfSLRs;
		int numberOfRows;
		int numberOfBRAMCols[MAX_ROWS];
		int numberOfCols[MAX_ROWS];
		int maxNumberOfCols;
		int maxNumberOfBRAMCols;
		int numberOfFramesBeforeCol[MAX_ROWS][MAX_COLS];
		int numberOfBRAMsBeforeCol[MAX_ROWS][MAX_COLS];
		int numberOfFramesPerRow[MAX_ROWS];
		int numberOfWordsPerRow[MAX_ROWS];

	//SLR data and resource string
		string initializedResourceStringShortPartName; ///< the partName of currently initialized resource string parameters
		char resourceString[MAX_ROWS][MAX_COLS];
		struct {
			uint32_t IDCODE;
			int fromRow, toRow;
			int rowsInTopHalf, rowsInBottomHalf;//used for xilinx series 7
		}SLRinfo[MAX_SLRS];

	//constant arrays defining selected fabric
		int LUT_numberOfFramesForResourceLetter[256];
		string LUT_typeOfFrameForResourceLetter[256];
		bool LUT_isFrameUnusedForResourceLetter[256];

//Base class functions are inlined and included here (this is so to force multiple instances that get optimized for the architectures' constants)
		#include "XCAP/inlineBlank.h"
		#include "XCAP/inlineInput.h"
		#include "XCAP/inlineOutput.h"
		#include "XCAP/inlineMerge.h"
		#include "XCAP/inlineParseParams.h"
		#include "XCAP/inlineCAP.h"
		#include "XCAP/inlineFAR.h"
		#include "XCAP/inlineSetFabric.h"
		#include "XCAP/inlineTest.h"
		#include "XCAP/inlinePrintResourceStringInfo.h"
		#include "XCAP/inlineAssembly.h"
		#include "XCAP/inlineChange.h"

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