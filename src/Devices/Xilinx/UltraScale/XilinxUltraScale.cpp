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

#include "XilinxUltraScale.h"
#include "../../../Common/str.h"

using namespace std;

XilinxUltraScale::XilinxUltraScale()
{
	initFabric();
}

XilinxUltraScale::~XilinxUltraScale()
{
}
inline bool isCharBlockRAM(char testCharacter){
	if(str::iff::charIs(testCharacter, 'A', 'B', 'C', 'D', '2')) //A-D are BlockRAM columns, '2' is empty blockram column
		return true;
	return false;
}

void XilinxUltraScale::initializeResourceStringParameters(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedResourceParamsPartName != partName){//The device is changed
		initializedResourceParamsPartName = partName;

		for((numberOfCols[0] = 0, numberOfFramesBeforeCol[0][0] = 0, numberOfBRAMCols[0] = 0) ; (uint8_t)resourceString[0][numberOfCols[0]] ; numberOfCols[0]++){
			numberOfFramesBeforeCol[0][numberOfCols[0]+1] = numberOfFramesBeforeCol[0][numberOfCols[0]] + LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[0][numberOfCols[0]]];
			numberOfBRAMsBeforeCol[0][numberOfCols[0]] = numberOfBRAMCols[0];
			if(isCharBlockRAM(resourceString[0][numberOfCols[0]]))
				numberOfBRAMCols[0]++;
		}
		for(int c = numberOfCols[0] + 1 ; c < XUS_MAX_COLS ; c++){
			numberOfFramesBeforeCol[0][c] = numberOfFramesBeforeCol[0][c - 1];
		}
		for(int c = numberOfBRAMCols[0] + 1 ; c < XUS_MAX_BRAM_COLS ; c++){
			numberOfBRAMsBeforeCol[0][c] = numberOfBRAMsBeforeCol[0][c - 1];
		}
		numberOfFramesPerRow[0] = numberOfFramesBeforeCol[0][numberOfCols[0]];
		numberOfWordsPerRow[0] = numberOfFramesPerRow[0] * XUS_WORDS_PER_FRAME;
		
		maxNumberOfCols = numberOfCols[0];
		maxNumberOfBRAMCols = numberOfBRAMCols[0];
		for(int r = 1 ; r < numberOfRows ; r++){
			numberOfBRAMCols[r] = numberOfBRAMCols[0];
			numberOfCols[r] = numberOfCols[0];
			numberOfFramesPerRow[r] = numberOfFramesPerRow[0];
			numberOfWordsPerRow[r] = numberOfWordsPerRow[0];
			for(int c = 0 ; c < XUS_MAX_COLS ; c++){
				numberOfFramesBeforeCol[r][c] = numberOfFramesBeforeCol[0][c];
			}
			for(int c = 0 ; c < XUS_MAX_BRAM_COLS ; c++){
				numberOfBRAMsBeforeCol[r][c]  = numberOfBRAMsBeforeCol[0][c];
			}
		}
	}
}

void XilinxUltraScale::ensureInitializedBitstreamArrays(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedBitstreamPartName != partName){//The device is changed
		initializedBitstreamPartName = partName;
		if(bitstreamBegin != nullptr)
			delete bitstreamBegin;

		initializeResourceStringParameters();
		//Allocate
		int clbPlaneSize = numberOfRows * numberOfWordsPerRow[0];
		int bramPlaneSize = numberOfRows * (XUS_EXTRA_FRAMES_PER_ROW + maxNumberOfBRAMCols * XUS_FRAMES_PER_BRAM_CONTENT_COLUMN * XUS_WORDS_PER_FRAME);
		
		bitstreamBegin = new uint32_t[clbPlaneSize + bramPlaneSize];
		
		//Calc pointers
		int offset = 0;
		
		for(int slr = 0 ; slr < numberOfSLRs ; slr++){
			for(int r = SLRinfo[slr].fromRow ; r <= SLRinfo[slr].toRow ; r++){
				for(int c = 0 ; c < numberOfCols[r] ; c++){
					bitstreamCLB[r][c] = &bitstreamBegin[offset];
					offset += LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] * XUS_WORDS_PER_FRAME;
				}
			}
			for(int r = SLRinfo[slr].fromRow ; r <= SLRinfo[slr].toRow ; r++){
				for(int c = 0 ; c < numberOfBRAMCols[r] ; c++){
					bitstreamBRAM[r][c] = &bitstreamBegin[offset];
					offset += XUS_FRAMES_PER_BRAM_CONTENT_COLUMN*XUS_WORDS_PER_FRAME;
				}
				bitstreamBRAM[r][numberOfBRAMCols[r]] = &bitstreamBegin[offset];
				offset += XUS_EXTRA_FRAMES_PER_ROW * XUS_WORDS_PER_FRAME;
			}
		}
		
		bitstreamEnd = &bitstreamBegin[offset];
	}
}

string XilinxUltraScale::getFrameType(int blockType, int rowAddress, int columnAddress){
	if(XUS_BLOCKTYPE_BLOCKRAM == blockType)
		return "BlockRAM Contents";
	else if(XUS_BLOCKTYPE_LOGIC == blockType)
		return LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[rowAddress][columnAddress]];
	else
		return "Unknown";
}
