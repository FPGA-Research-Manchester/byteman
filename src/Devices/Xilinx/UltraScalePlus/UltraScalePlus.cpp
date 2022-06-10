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
#include "UltraScalePlus.h"

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

XilinxUltraScalePlus::XilinxUltraScalePlus()
{
	initFabric();
}

XilinxUltraScalePlus::~XilinxUltraScalePlus()
{
}

void XilinxUltraScalePlus::initializeResourceStringParameters(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedResourceParamsPartName != partName){//The device is changed
		initializedResourceParamsPartName = partName;

		for((numberOfCols = 0, numberOfFramesBeforeCol[0] = 0, numberOfBRAMCols = 0) ; (uint8_t)resourceString[0][numberOfCols] ; numberOfCols++){
			numberOfFramesBeforeCol[numberOfCols+1] = numberOfFramesBeforeCol[numberOfCols] + LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[0][numberOfCols]];
			numberOfBRAMsBeforeCol[numberOfCols] = numberOfBRAMCols;
			if(str::iff::charIs(resourceString[0][numberOfCols], 'B', '1')) //B1 are "BRAM", "NULL BRAM"
				numberOfBRAMCols++;
		}
		numberOfFramesPerRow = numberOfFramesBeforeCol[numberOfCols] + XUSP_EXTRA_FRAMES_PER_ROW;
		numberOfWordsPerRow = numberOfFramesPerRow * XUSP_WORDS_PER_FRAME;
	}
}

void XilinxUltraScalePlus::ensureInitializedBitstreamArrays(){
	if(partName == "")
		throw runtime_error("The target device needs to be known, before you can manipulate any bitstream!");
	if(initializedBitstreamPartName != partName){//The device is changed
		initializedBitstreamPartName = partName;
		if(bitstreamBegin != nullptr)
			delete bitstreamBegin;

		initializeResourceStringParameters();
		//Allocate
		bitstreamBegin = new uint32_t[numberOfRows * (numberOfWordsPerRow + numberOfBRAMCols * (XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN+XUSP_EXTRA_FRAMES_PER_ROW) * XUSP_WORDS_PER_FRAME)];
		
		//Calc pointers
		int offset = 0;
		
		int srchSLRrow = 0, fromSLRrow = 0, toSLRrow = 0; // build the device bottom up (as visualized), no matter the SLR order
		for(int slrcnt = 0 ; slrcnt < numberOfSLRs ; slrcnt++){
			for(int slri = 0 ; slri < numberOfSLRs ; slri++){ // get the next SLR (bottom up as visualized)
				if(SLRinfo[slri].fromRow == srchSLRrow){
					fromSLRrow = SLRinfo[slri].fromRow;
					toSLRrow = SLRinfo[slri].toRow + 1;
				}
			}
			//build the pointers of the next SLR
			for(int r = fromSLRrow ; r < toSLRrow ; r++){
				for(int c = 0 ; c < numberOfCols ; c++){
					bitstreamCLB[r][c] = &bitstreamBegin[offset];
					offset += LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] * XUSP_WORDS_PER_FRAME;
				}
				offset += XUSP_EXTRA_FRAMES_PER_ROW * XUSP_WORDS_PER_FRAME;
			}

			for(int r = fromSLRrow ; r < toSLRrow ; r++){
				for(int c = 0 ; c < numberOfBRAMCols ; c++){
					bitstreamBRAM[r][c] = &bitstreamBegin[offset];
					offset += XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN*XUSP_WORDS_PER_FRAME;
				}
				offset += XUSP_EXTRA_FRAMES_PER_ROW * XUSP_WORDS_PER_FRAME;
			}
			
			srchSLRrow = toSLRrow;//the start row of the next SLR.
		}
		
		bitstreamEnd = &bitstreamBegin[offset];
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
