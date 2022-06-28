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
inline void XCAP_IncrementFAR_BlockType0_new(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	uint8_t resourceLetter = (uint8_t)resourceString[globalRowAddress][columnAddress];
	minorAddress++;
	if(minorAddress == LUT_numberOfFramesForResourceLetter[resourceLetter]){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == numberOfCols[0]){
			columnAddress = 0;
			if(globalRowAddress == SLRinfo[slrID].toRow){
				globalRowAddress = SLRinfo[slrID].fromRow;
				blockType++;
			} else {
				globalRowAddress++;
			}
		}
	}
}

inline void XCAP_IncrementFAR_BlockType0_old(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	uint8_t resourceLetter = (uint8_t)resourceString[globalRowAddress][columnAddress];
	minorAddress++;
	if(minorAddress == LUT_numberOfFramesForResourceLetter[resourceLetter]){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == numberOfCols[globalRowAddress]){
			columnAddress = 0;
			bool isInTopHalf = (globalRowAddress >= (SLRinfo[slrID].fromRow + SLRinfo[slrID].rowsInBottomHalf));
			if(isInTopHalf){
				bool isAtTheTopTop = (globalRowAddress == (SLRinfo[slrID].toRow));
				if(isAtTheTopTop){
					bool hasNoBottomHalf = (SLRinfo[slrID].rowsInBottomHalf == 0);
					if(hasNoBottomHalf){ // go to blockram contents
						globalRowAddress = SLRinfo[slrID].fromRow;
						blockType++;
					} else { //go to bottom half
						globalRowAddress = SLRinfo[slrID].fromRow + SLRinfo[slrID].rowsInBottomHalf - 1;
					}
				} else {
					globalRowAddress++;
				}
			} else { //is in bottom half
				bool isAtTheBottomBottom = (globalRowAddress == (SLRinfo[slrID].fromRow));
				if(isAtTheBottomBottom){
					globalRowAddress = SLRinfo[slrID].fromRow + SLRinfo[slrID].rowsInBottomHalf;
					blockType++;
				} else {
					globalRowAddress--;
				}
			}
		}
	}
}

inline void XCAP_IncrementFAR_BlockType0(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	if(FAR_ENABLE_TOP_BOTTOM_BIT == 0){// US and newer devices
		XCAP_IncrementFAR_BlockType0_new(slrID, blockType, globalRowAddress, columnAddress, minorAddress);
	} else {// Series 7 and older devices
		XCAP_IncrementFAR_BlockType0_old(slrID, blockType, globalRowAddress, columnAddress, minorAddress);
	}
}

inline void XCAP_IncrementFAR_BlockType1_new(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	minorAddress++;
	if((columnAddress == numberOfBRAMCols[0])?(minorAddress == EXTRA_FRAMES_PER_ROW):(minorAddress == FRAMES_PER_BRAM_CONTENT_COLUMN)){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == (numberOfBRAMCols[0]+1)){
			columnAddress = 0;
			if(globalRowAddress == SLRinfo[slrID].toRow){
				globalRowAddress = SLRinfo[slrID].fromRow;
				blockType++;
			} else {
				globalRowAddress++;
			}
		}
	}
}

inline void XCAP_IncrementFAR_BlockType1_old(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	minorAddress++;
	if((columnAddress == numberOfBRAMCols[globalRowAddress])?(minorAddress == EXTRA_FRAMES_PER_ROW):(minorAddress == FRAMES_PER_BRAM_CONTENT_COLUMN)){
		minorAddress = 0;
		columnAddress++;
		if(columnAddress == (numberOfBRAMCols[globalRowAddress] + 1)){
			columnAddress = 0;

			bool isInTopHalf = (globalRowAddress >= (SLRinfo[slrID].fromRow + SLRinfo[slrID].rowsInBottomHalf));
			if(isInTopHalf){
				bool isAtTheTopTop = (globalRowAddress == (SLRinfo[slrID].toRow));
				if(isAtTheTopTop){
					bool hasNoBottomHalf = (SLRinfo[slrID].rowsInBottomHalf == 0);
					if(hasNoBottomHalf){ // go to next blocktype
						globalRowAddress = SLRinfo[slrID].fromRow;
						blockType++;
					} else { //go to bottom half
						globalRowAddress = SLRinfo[slrID].fromRow + SLRinfo[slrID].rowsInBottomHalf - 1;
					}
				} else {
					globalRowAddress++;
				}
			} else { //is in bottom half
				bool isAtTheBottomBottom = (globalRowAddress == (SLRinfo[slrID].fromRow));
				if(isAtTheBottomBottom){
					globalRowAddress = SLRinfo[slrID].fromRow + SLRinfo[slrID].rowsInBottomHalf;
					blockType++;
				} else {
					globalRowAddress--;
				}
			}
		}
	}
}

inline void XCAP_IncrementFAR_BlockType1(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	if(FAR_ENABLE_TOP_BOTTOM_BIT == 0){// US and newer devices
		XCAP_IncrementFAR_BlockType1_new(slrID, blockType, globalRowAddress, columnAddress, minorAddress);
	} else {// Series 7 and older devices
		XCAP_IncrementFAR_BlockType1_old(slrID, blockType, globalRowAddress, columnAddress, minorAddress);
	}
}

inline void XCAP_IncrementFAR(int slrID, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	if(BLOCKTYPE_LOGIC == blockType){
		XCAP_IncrementFAR_BlockType0(slrID, blockType, globalRowAddress, columnAddress, minorAddress);
	} else if(BLOCKTYPE_BLOCKRAM == blockType){
		XCAP_IncrementFAR_BlockType1(slrID, blockType, globalRowAddress, columnAddress, minorAddress);
	}
}

inline void XCAP_parseFAR(int farValue, int slr, int &blockType, int &globalRowAddress, int &columnAddress, int &minorAddress)
{
	blockType = (farValue >> FAR_BLOCKTYPE_SHIFT) & FAR_BLOCKTYPE_MASK;
	if(FAR_ENABLE_TOP_BOTTOM_BIT == 0){// US and newer devices
		globalRowAddress = SLRinfo[slr].fromRow;
		globalRowAddress += (farValue >> FAR_ROWADDRESS_SHIFT) & FAR_ROWADDRESS_MASK;
	} else {//Series 7 and older devices
		int bottomAndNotTop = (farValue >> FAR_TOP_BOTTOM_BIT_SHIFT) & 0x1;
		globalRowAddress = SLRinfo[slr].fromRow;
		globalRowAddress += SLRinfo[slr].rowsInBottomHalf;
		int rowAddress = (farValue >> FAR_ROWADDRESS_SHIFT) & FAR_ROWADDRESS_MASK;
		if(bottomAndNotTop){ //bottom
			globalRowAddress -= (1 + rowAddress);
		} else { //top
			globalRowAddress += rowAddress;
		}
	}
	columnAddress = (farValue >> FAR_COLUMNADDRESS_SHIFT) & FAR_COLUMNADDRESS_MASK;
	minorAddress = (farValue >> FAR_MINORADDRESS_SHIFT) & FAR_MINORADDRESS_MASK;
}

inline uint32_t XCAP_getFAR(int slr, int blockType, int globalRowAddress, int columnAddress, int minorAddress)
{
	if(FAR_ENABLE_TOP_BOTTOM_BIT == 0){// US and newer devices
		int rowAddress = (globalRowAddress - SLRinfo[slr].fromRow);
		return (((blockType & FAR_BLOCKTYPE_MASK) << FAR_BLOCKTYPE_SHIFT) | ((rowAddress & FAR_ROWADDRESS_MASK) << FAR_ROWADDRESS_SHIFT) | ((columnAddress & FAR_COLUMNADDRESS_MASK) << FAR_COLUMNADDRESS_SHIFT) | ((minorAddress & FAR_MINORADDRESS_MASK) << FAR_MINORADDRESS_SHIFT));
	}
	//else Series 7 and older devices
	int rowAddress = (globalRowAddress - SLRinfo[slr].fromRow);
	int bottomAndNotTop;
	if(rowAddress < SLRinfo[slr].rowsInBottomHalf) {
		rowAddress = SLRinfo[slr].rowsInBottomHalf - 1 - rowAddress;
		bottomAndNotTop = 1; // bottom
	} else {
		rowAddress -= SLRinfo[slr].rowsInBottomHalf;
		bottomAndNotTop = 0; //top
	}
	return (((blockType & FAR_BLOCKTYPE_MASK) << FAR_BLOCKTYPE_SHIFT) | ((bottomAndNotTop & 0x1) << FAR_TOP_BOTTOM_BIT_SHIFT) | ((rowAddress & FAR_ROWADDRESS_MASK) << FAR_ROWADDRESS_SHIFT) | ((columnAddress & FAR_COLUMNADDRESS_MASK) << FAR_COLUMNADDRESS_SHIFT) | ((minorAddress & FAR_MINORADDRESS_MASK) << FAR_MINORADDRESS_SHIFT));
}

/// Generate and return the encoding for a Far FAR instruction.
inline uint32_t XCAP_getFarFarInstruction()
{
	return XCAP_getFAR(0,7,63,0,0);
}
