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

#include "Fabric.h"
//#include "ConfigurationAccessPort.h"
#include "UltraScale.h"

#include<iostream>
#include<stdio.h>
#include<string>
#include<cstring>  //memset
#include<algorithm> //replace
#include<sstream>
#include<stdexcept>
#include<fstream>

#include "../../../Common/str.h"

using namespace std;

XilinxUltraScale::XilinxUltraScale()
{
	initFabric();
}

XilinxUltraScale::~XilinxUltraScale()
{
}

void XilinxUltraScale::initializeResourceStringParameters(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedResourceParamsPartName != partName){//The device is changed
		initializedResourceParamsPartName = partName;

		for((numberOfCols = 0, numberOfFramesBeforeCol[0] = 0, numberOfBRAMCols = 0) ; (uint8_t)resourceString[0][numberOfCols] ; numberOfCols++){
			numberOfFramesBeforeCol[numberOfCols+1] = numberOfFramesBeforeCol[numberOfCols] + LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[0][numberOfCols]];
			numberOfBRAMsBeforeCol[numberOfCols] = numberOfBRAMCols;
			if(str::iff::charIs(resourceString[0][numberOfCols], 'B')) //B are "BRAM"
				numberOfBRAMCols++;
		}
		numberOfFramesPerRow = numberOfFramesBeforeCol[numberOfCols] + XUS_EXTRA_FRAMES_PER_ROW;
		numberOfWordsPerRow = numberOfFramesPerRow * XUS_WORDS_PER_FRAME;
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
		bitstreamBegin = new uint32_t[numberOfRows * (numberOfWordsPerRow + numberOfBRAMCols * (XUS_FRAMES_PER_BRAM_CONTENT_COLUMN+XUS_EXTRA_FRAMES_PER_ROW) * XUS_WORDS_PER_FRAME)];
		
		//Calc pointers
		int offset = 0;
		
		int srchSLRrow = 0, fromSLRrow = 0, toSLRrow = 0;
		for(int slrcnt = 0 ; slrcnt < numberOfSLRs ; slrcnt++){
			for(int slri = 0 ; slri < numberOfSLRs ; slri++){
				if(SLRinfo[slri].fromRow == srchSLRrow){
					fromSLRrow = SLRinfo[slri].fromRow;
					toSLRrow = SLRinfo[slri].toRow + 1;
				}
			}
			//build the pointers of the next SLR
			for(int r = fromSLRrow ; r < toSLRrow ; r++){
				for(int c = 0 ; c < numberOfCols ; c++){
					bitstreamCLB[r][c] = &bitstreamBegin[offset];
					offset += LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] * XUS_WORDS_PER_FRAME;
				}
				offset += XUS_EXTRA_FRAMES_PER_ROW * XUS_WORDS_PER_FRAME;
			}

			for(int r = fromSLRrow ; r < toSLRrow ; r++){
				for(int c = 0 ; c < numberOfBRAMCols ; c++){
					bitstreamBRAM[r][c] = &bitstreamBegin[offset];
					offset += XUS_FRAMES_PER_BRAM_CONTENT_COLUMN*XUS_WORDS_PER_FRAME;
				}
				offset += XUS_EXTRA_FRAMES_PER_ROW * XUS_WORDS_PER_FRAME;
			}
			
			srchSLRrow = toSLRrow;//the start row of the next SLR.
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
