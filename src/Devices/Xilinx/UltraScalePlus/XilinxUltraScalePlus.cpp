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

#include "XilinxUltraScalePlus.h"
#include "../../../Common/str.h"

using namespace std;

XilinxUltraScalePlus::XilinxUltraScalePlus()
{
	initFabric();
}

XilinxUltraScalePlus::~XilinxUltraScalePlus()
{
}

void XilinxUltraScalePlus::initializeResourceStringParameters(){
	if(initializedResourceStringShortPartName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedBitstreamParamsShortPartName != initializedResourceStringShortPartName){//The device is changed
		initializedBitstreamParamsShortPartName = initializedResourceStringShortPartName;

		for((numberOfCols[0] = 0, numberOfFramesBeforeCol[0][0] = 0, numberOfBRAMCols[0] = 0) ; (uint8_t)resourceString[0][numberOfCols[0]] ; numberOfCols[0]++){
			numberOfFramesBeforeCol[0][numberOfCols[0]+1] = numberOfFramesBeforeCol[0][numberOfCols[0]] + LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[0][numberOfCols[0]]];
			numberOfBRAMsBeforeCol[0][numberOfCols[0]] = numberOfBRAMCols[0];
			if(str::iff::charIs(resourceString[0][numberOfCols[0]], 'A', 'B', 'C', '1')) //A-C are BlockRAM columns, '1' is empty blockram column
				numberOfBRAMCols[0]++;
		}
		for(int c = numberOfCols[0] + 1 ; c < XUSP_MAX_COLS ; c++){
			numberOfFramesBeforeCol[0][c] = numberOfFramesBeforeCol[0][c - 1];
		}
		for(int c = numberOfBRAMCols[0] + 1 ; c < XUSP_MAX_BRAM_COLS ; c++){
			numberOfBRAMsBeforeCol[0][c] = numberOfBRAMsBeforeCol[0][c - 1];
		}
		numberOfFramesPerRow[0] = numberOfFramesBeforeCol[0][numberOfCols[0]];
		numberOfWordsPerRow[0] = numberOfFramesPerRow[0] * XUSP_WORDS_PER_FRAME;
		
		maxNumberOfCols = numberOfCols[0];
		maxNumberOfBRAMCols = numberOfBRAMCols[0];
		for(int r = 1 ; r < numberOfRows ; r++){
			numberOfBRAMCols[r] = numberOfBRAMCols[0];
			numberOfCols[r] = numberOfCols[0];
			numberOfFramesPerRow[r] = numberOfFramesPerRow[0];
			numberOfWordsPerRow[r] = numberOfWordsPerRow[0];
			for(int c = 0 ; c < XUSP_MAX_COLS ; c++){
				numberOfFramesBeforeCol[r][c] = numberOfFramesBeforeCol[0][c];
			}
			for(int c = 0 ; c < XUSP_MAX_BRAM_COLS ; c++){
				numberOfBRAMsBeforeCol[r][c]  = numberOfBRAMsBeforeCol[0][c];
			}
		}
	}
}

void XilinxUltraScalePlus::ensureInitializedBitstreamArrays(){
	if(initializedResourceStringShortPartName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedBitstreamShortPartName != initializedResourceStringShortPartName){//The device is changed
		initializedBitstreamShortPartName = initializedResourceStringShortPartName;
		if(bitstreamBegin != nullptr)
			delete bitstreamBegin;

		initializeResourceStringParameters();
		//Allocate
		int clbPlaneSize = numberOfRows * numberOfWordsPerRow[0];
		int bramPlaneSize = numberOfRows * XUSP_WORDS_PER_FRAME * (XUSP_EXTRA_FRAMES_PER_ROW + maxNumberOfBRAMCols * XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN);
		
		bitstreamBegin = new uint32_t[clbPlaneSize + bramPlaneSize];
		
		//Calc pointers
		int offset = 0;
		
		for(int slr = 0 ; slr < numberOfSLRs ; slr++){
			for(int r = SLRinfo[slr].fromRow ; r <= SLRinfo[slr].toRow ; r++){
				for(int c = 0 ; c < numberOfCols[r] ; c++){
					bitstreamCLB[r][c] = &bitstreamBegin[offset];
					offset += LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] * XUSP_WORDS_PER_FRAME;
				}
			}
			for(int r = SLRinfo[slr].fromRow ; r <= SLRinfo[slr].toRow ; r++){
				for(int c = 0 ; c < numberOfBRAMCols[r] ; c++){
					bitstreamBRAM[r][c] = &bitstreamBegin[offset];
					offset += XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN * XUSP_WORDS_PER_FRAME;
				}
				bitstreamBRAM[r][numberOfBRAMCols[r]] = &bitstreamBegin[offset];
				offset += XUSP_EXTRA_FRAMES_PER_ROW * XUSP_WORDS_PER_FRAME;
			}
		}
		
		bitstreamEnd = &bitstreamBegin[offset];
		log("Reserved "+to_string(clbPlaneSize + bramPlaneSize)+" words for the bitstream buffers of device \""+partName+"\"");
	}
}

string XilinxUltraScalePlus::getFrameType(int blockType, int rowAddress, int columnAddress){
	if(XUSP_BLOCKTYPE_BLOCKRAM == blockType)
		return "BlockRAM Contents";
	else if(XUSP_BLOCKTYPE_LOGIC == blockType)
		return LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[rowAddress][columnAddress]];
	else
		return "Unknown";
}
