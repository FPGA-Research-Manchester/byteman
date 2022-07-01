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
#include<stdio.h>
#include<string>
#include<cstring>  //memset
#include<algorithm> //replace
#include<sstream>
#include<stdexcept>
#include<fstream>

#include "XilinxSeries7.h"
#include "../../../Common/str.h"

using namespace std;

XilinxSeries7::XilinxSeries7()
{
	initFabric();
}

XilinxSeries7::~XilinxSeries7()
{
}

void XilinxSeries7::initializeResourceStringParameters(){
	if(initializedResourceStringShortPartName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedBitstreamParamsShortPartName != initializedResourceStringShortPartName){//The device is changed
		initializedBitstreamParamsShortPartName = initializedResourceStringShortPartName;
		for(int r = 0 ; r < numberOfRows ; r++){
			for((numberOfCols[r] = 0, numberOfFramesBeforeCol[r][0] = 0, numberOfBRAMCols[r] = 0) ; (uint8_t)resourceString[r][numberOfCols[r]] ; numberOfCols[r]++){
				numberOfFramesBeforeCol[r][numberOfCols[r]+1] = numberOfFramesBeforeCol[r][numberOfCols[r]] + LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][numberOfCols[r]]];
				numberOfBRAMsBeforeCol[r][numberOfCols[r]] = numberOfBRAMCols[r];
				if(str::iff::charIs(resourceString[r][numberOfCols[r]], 'A', 'B', 'C', 'D', 'E', 'F', 'G', '4')) //A-G are BlockRAM columns, '4' is empty blockram column
					numberOfBRAMCols[r]++;
			}
			numberOfFramesPerRow[r] = numberOfFramesBeforeCol[r][numberOfCols[r]];
			numberOfWordsPerRow[r] = numberOfFramesPerRow[r] * XS7_WORDS_PER_FRAME;
			for(int c = numberOfCols[r] + 1 ; c < XS7_MAX_COLS ; c++){
				numberOfFramesBeforeCol[r][c] = numberOfFramesBeforeCol[r][c - 1];
			}
			for(int c = numberOfCols[r] ; c < XS7_MAX_COLS ; c++){
				numberOfBRAMsBeforeCol[r][c] = numberOfBRAMsBeforeCol[r][c - 1];
			}
		}
		
		
		maxNumberOfCols = numberOfCols[0];
		maxNumberOfBRAMCols = numberOfBRAMCols[0];
		for(int r = 1 ; r < numberOfRows ; r++){
			if(maxNumberOfCols < numberOfCols[r])
				maxNumberOfCols = numberOfCols[r];
			if(maxNumberOfBRAMCols < numberOfBRAMCols[r])
				maxNumberOfBRAMCols = numberOfBRAMCols[r];
		}
	}
}

void XilinxSeries7::ensureInitializedBitstreamArrays(){
	if(initializedResourceStringShortPartName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedBitstreamShortPartName != initializedResourceStringShortPartName){//The device is changed
		initializedBitstreamShortPartName = initializedResourceStringShortPartName;
		if(bitstreamBegin != nullptr)
			delete bitstreamBegin;

		initializeResourceStringParameters();
		//Allocate
		int bramPlaneSize = 0;
		int clbPlaneSize = 0;
		for(int r = 0 ; r < numberOfRows ; r++){
			bramPlaneSize += XS7_WORDS_PER_FRAME * (XS7_EXTRA_FRAMES_PER_ROW + numberOfBRAMCols[r] * XS7_FRAMES_PER_BRAM_CONTENT_COLUMN);
			clbPlaneSize += numberOfWordsPerRow[r];
		}
		bitstreamBegin = new uint32_t[clbPlaneSize + bramPlaneSize];
		
		//Calc pointers
		int offset = 0;
		
		for(int slr = 0 ; slr < numberOfSLRs ; slr++){
			int fromGlobalRowTop = SLRinfo[slr].fromRow + SLRinfo[slr].rowsInBottomHalf;
			int toGlobalRowTop = SLRinfo[slr].toRow;
			int fromGlobalRowBottom = SLRinfo[slr].fromRow + SLRinfo[slr].rowsInBottomHalf - 1;
			int toGlobalRowBottom = SLRinfo[slr].fromRow;
			for(int r = fromGlobalRowTop ; r <= toGlobalRowTop ; r++){
				for(int c = 0 ; c < numberOfCols[r] ; c++){
					bitstreamCLB[r][c] = &bitstreamBegin[offset];
					offset += LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] * XS7_WORDS_PER_FRAME;
				}
			}
			for(int r = fromGlobalRowBottom ; r >= toGlobalRowBottom ; r--){
				for(int c = 0 ; c < numberOfCols[r] ; c++){
					bitstreamCLB[r][c] = &bitstreamBegin[offset];
					offset += LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] * XS7_WORDS_PER_FRAME;
				}
			}
			for(int r = fromGlobalRowTop ; r <= toGlobalRowTop ; r++){
				for(int c = 0 ; c < numberOfBRAMCols[r] ; c++){
					bitstreamBRAM[r][c] = &bitstreamBegin[offset];
					offset += XS7_FRAMES_PER_BRAM_CONTENT_COLUMN*XS7_WORDS_PER_FRAME;
				}
				bitstreamBRAM[r][numberOfBRAMCols[r]] = &bitstreamBegin[offset];
				offset += XS7_EXTRA_FRAMES_PER_ROW * XS7_WORDS_PER_FRAME;
			}
			for(int r = fromGlobalRowBottom ; r >= toGlobalRowBottom ; r--){
				for(int c = 0 ; c < numberOfBRAMCols[r] ; c++){
					bitstreamBRAM[r][c] = &bitstreamBegin[offset];
					offset += XS7_FRAMES_PER_BRAM_CONTENT_COLUMN*XS7_WORDS_PER_FRAME;
				}
				bitstreamBRAM[r][numberOfBRAMCols[r]] = &bitstreamBegin[offset];
				offset += XS7_EXTRA_FRAMES_PER_ROW * XS7_WORDS_PER_FRAME;
			}
		}
		
		bitstreamEnd = &bitstreamBegin[offset];
		log("Reserved "+to_string(clbPlaneSize + bramPlaneSize)+" words for the bitstream buffers of device \""+partName+"\"");
	}
}

string XilinxSeries7::getFrameType(int blockType, int rowAddress, int columnAddress){
	if(XS7_BLOCKTYPE_BLOCKRAM == blockType)
		return "BlockRAM Contents";
	else if(XS7_BLOCKTYPE_LOGIC == blockType)
		return LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[rowAddress][columnAddress]];
	else
		return "Unknown";
}
