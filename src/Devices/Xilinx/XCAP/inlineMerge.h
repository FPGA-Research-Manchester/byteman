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

inline void ensureRowCompatibility(Coord2D src, int offsetRow, int sizeCol, Coord2D dst){
	int srcRA = (src.row + offsetRow) / CLB_PER_CLOCK_REGION;
	int dstRA = (dst.row + offsetRow) / CLB_PER_CLOCK_REGION;
	for(int c = 0 ; c < sizeCol ; c++){
		if(resourceString[srcRA][src.col + c] != resourceString[dstRA][dst.col + c]){//if any two letters are different
			if(LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[srcRA][src.col + c]] != LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[dstRA][dst.col + c]]){
				//also the number of frames is different?!
				throw std::runtime_error(std::string("Please check your merge coordinates. Tried to merge incompatible resources (from \"").append(LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[srcRA][src.col + c]]).append("\" to \"").append(LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[dstRA][dst.col + c]]).append("\")."));
			} else {
				//okay, different col, but same width. Throw warning?
				warn("Relocating from frame type " + std::string(LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[srcRA][src.col + c]]) + " to frame type " + std::string(LUT_typeOfFrameForResourceLetter[(uint8_t)resourceString[dstRA][dst.col + c]]) + ".");
			}
		}
	}
}

inline void ensureRegionCompatibility(Rect2D src, Coord2D dst){
	for(int r = 0 ; r < src.size.row ; ){ //this loop will iterate south border and all (srcRow:srcRow + sizeRow) at CLBs 0 and 59
		ensureRowCompatibility(src.position, r, src.size.col, dst);
		if( ((src.position.row + r) % CLB_PER_CLOCK_REGION) == (CLB_PER_CLOCK_REGION-1) ) { // isLastCLBinRow
			r++;
		} else if( ((src.position.row + r) % CLB_PER_CLOCK_REGION) == 0 ) { // isFirstCLBinRow
			r += (CLB_PER_CLOCK_REGION - 2);
		} else { // get to the next {isLastCLBinRow}
			r += (CLB_PER_CLOCK_REGION - 1 - ((src.position.row + r) % CLB_PER_CLOCK_REGION));
		}
	}
	ensureRowCompatibility(src.position, src.size.row-1, src.size.col, dst); //iterate north border as well
}

/**************************************************************************//**
 * If endianesses of the two bitstreams match and you want to do a relocate
 * operation, fastMerge is the way to go.
 *****************************************************************************/
inline void fastMerge(XilinxConfigurationAccessPort* srcBitstream, Rect2D src, Coord2D dst){
	log("fastMerge function called");
	int srcRA = src.position.row  / CLB_PER_CLOCK_REGION;
	int sizeR = src.size.row / CLB_PER_CLOCK_REGION;
	int dstRA = dst.row  / CLB_PER_CLOCK_REGION;

	for(int r = 0 ; r < sizeR ; r++){
		for(int c = 0 ; c < src.size.col ; c++){//relocate dst[dstRA + r][dst.col + c] <= src[srcRA + r][src.position.col + c]
			for(int m = 0 ; m < LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[dstRA + r][dst.col + c]] ; m++){
				if(selectedOptions.clb && selectedOptions.clk){
					memcpy(&bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME] , &srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME], WORDS_PER_FRAME*4);
				} else {
					if(selectedOptions.clb){
						memcpy(&bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME] ,							&srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME],									WORDS_BEFORE_CLK*4);
						memcpy(&bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME + WORDS_BEFORE_CLK + WORDS_AT_CLK] , &srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME + WORDS_BEFORE_CLK + WORDS_AT_CLK], WORDS_AFTER_CLK*4);
					}
					if(selectedOptions.clk){
						for(int w = WORDS_BEFORE_CLK ; w < (WORDS_BEFORE_CLK + WORDS_AT_CLK) ; w++)
							bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME + w] = srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME + w];
					}
				}
			}
		}
	}
	if(selectedOptions.bram){
		for(int r = 0 ; r < sizeR ; r++){
			int bramCols = numberOfBRAMsBeforeCol[r][src.position.col + src.size.col] - numberOfBRAMsBeforeCol[r][src.position.col];
			int srcCol = numberOfBRAMsBeforeCol[r][src.position.col];
			int dstCol = numberOfBRAMsBeforeCol[r][dst.col];
			memcpy(&bitstreamBRAM[dstRA + r][dstCol][0] , &srcBitstream->bitstreamBRAM[srcRA + r][srcCol][0], bramCols * FRAMES_PER_BRAM_CONTENT_COLUMN * WORDS_PER_FRAME * 4);
		}
	}
	bitstreamHasValidData = true;
}

inline void flexiMerge(XilinxConfigurationAccessPort* srcBitstream, Endianness endianConversionNeeded, Rect2D src, Coord2D dst){
	log("flexiMerge function called");
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before modifications
	srcBitstream->ensureInitializedBitstreamArrays();//initialize bitstream arrays before modifications
	int srcRA = src.position.row  / CLB_PER_CLOCK_REGION;
	int sizeR = src.size.row / CLB_PER_CLOCK_REGION;
	int dstRA = dst.row  / CLB_PER_CLOCK_REGION;

	for(int r = 0 ; r < sizeR ; r++){
		for(int c = 0 ; c < src.size.col ; c++){//relocate dst[dstRA + r][dst.col + c] <= src[srcRA + r][src.position.col + c]
			for(int m = 0 ; m < LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[dstRA + r][dst.col + c]] ; m++){
				for(int w = 0 ; w < WORDS_PER_FRAME ; w++){
					if(WORDS_BEFORE_CLK <= w && (WORDS_BEFORE_CLK + WORDS_AT_CLK) > w){
						if(!selectedOptions.clk)
							continue;
					} else {
						if(!selectedOptions.clb)
							continue;
					}
					switch(selectedOptions.op){
						case MergeOP::SET:
							bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME + w]  = Endian::NativeToAnyEndianness32(srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME + w], endianConversionNeeded);
							break;
						case MergeOP::XOR:
							bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME + w] ^= Endian::NativeToAnyEndianness32(srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME + w], endianConversionNeeded);
							break;
						case MergeOP::OR:
							bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME + w] |= Endian::NativeToAnyEndianness32(srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME + w], endianConversionNeeded);
							break;
						case MergeOP::AND:
							bitstreamCLB[dstRA + r][dst.col + c][m*WORDS_PER_FRAME + w] &= Endian::NativeToAnyEndianness32(srcBitstream->bitstreamCLB[srcRA + r][src.position.col + c][m*WORDS_PER_FRAME + w], endianConversionNeeded);
							break;
					}
				}
			}
		}
	}
	if(selectedOptions.bram){
		for(int r = 0 ; r < sizeR ; r++){
			int bramCols = numberOfBRAMsBeforeCol[r][src.position.col + src.size.col] - numberOfBRAMsBeforeCol[r][src.position.col];
			int srcCol = numberOfBRAMsBeforeCol[r][src.position.col];
			int dstCol = numberOfBRAMsBeforeCol[r][dst.col];
			for(int c = 0 ; c < bramCols ; c++){
				for(int w = 0 ; w < WORDS_PER_FRAME * FRAMES_PER_BRAM_CONTENT_COLUMN ; w++){
					switch(selectedOptions.op){
						case MergeOP::SET:
							bitstreamBRAM[dstRA + r][dstCol + c][w]  = Endian::NativeToAnyEndianness32(srcBitstream->bitstreamBRAM[srcRA + r][srcCol + c][w], endianConversionNeeded);
							break;
						case MergeOP::XOR:
							bitstreamBRAM[dstRA + r][dstCol + c][w] ^= Endian::NativeToAnyEndianness32(srcBitstream->bitstreamBRAM[srcRA + r][srcCol + c][w], endianConversionNeeded);
							break;
						case MergeOP::OR:
							bitstreamBRAM[dstRA + r][dstCol + c][w] |= Endian::NativeToAnyEndianness32(srcBitstream->bitstreamBRAM[srcRA + r][srcCol + c][w], endianConversionNeeded);
							break;
						case MergeOP::AND:
							bitstreamBRAM[dstRA + r][dstCol + c][w] &= Endian::NativeToAnyEndianness32(srcBitstream->bitstreamBRAM[srcRA + r][srcCol + c][w], endianConversionNeeded);
							break;
					}
				}
			}
		}
	}//-if(reloBRAM)
	bitstreamHasValidData = true;
}
