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

inline void clearResourceStringArrays()
{
	initializedResourceStringShortPartName = "";
	memset(&resourceString, 0, MAX_ROWS * MAX_COLS * sizeof(char));
}

inline void blankBuffers()
{
	uint8_t blankByte = selectedOptions.intParam % 256;
	if(regionSelection.empty()) {
		if(selectedOptions.clb && selectedOptions.clk) {
			memset(&bitstreamBegin[0], blankByte, ((uint64_t)&bitstreamBRAM[0][0][0]-(uint64_t)&bitstreamBegin[0]));
		}
		else if(selectedOptions.clb || selectedOptions.clk){
			for(int r = 0 ; r < numberOfRows ; r++){
				for(int c = 0 ; c < numberOfCols[r] ; c++){
					for(int m = 0 ; m < LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] ; m++){
						if(selectedOptions.clb){
							memset(&bitstreamCLB[r][c][m*WORDS_PER_FRAME], blankByte, (WORDS_BEFORE_CLK * 4));
							memset(&bitstreamCLB[r][c][m*WORDS_PER_FRAME+(WORDS_BEFORE_CLK+WORDS_AT_CLK)], blankByte, (WORDS_AFTER_CLK * 4));
						}
						if(selectedOptions.clk){
							for(int w = 0 ; w < WORDS_AT_CLK ; w++){
								bitstreamCLB[r][c][m*WORDS_PER_FRAME + WORDS_BEFORE_CLK + w] = ((blankByte << 24) | (blankByte << 16) | (blankByte << 8) | (blankByte));
							}
						}
					}
				}
			}
		}
		if(selectedOptions.bram)
			memset(&bitstreamBRAM[0][0][0], blankByte, ((uint64_t)&bitstreamEnd[0]-(uint64_t)&bitstreamBRAM[0][0][0]));
	} else {
		for (Rect2D &selRect : regionSelection) {
			int fromRow = selRect.position.row / CLB_PER_CLOCK_REGION;
			int toRow = (selRect.position.row + selRect.size.row) / CLB_PER_CLOCK_REGION;
			int fromCol = selRect.position.col;
			int toCol = selRect.position.col + selRect.size.col;
			for(int r = fromRow ; r < toRow ; r++){
				if(selectedOptions.clb && selectedOptions.clk){
					int blankSize = numberOfFramesBeforeCol[r][toCol] - numberOfFramesBeforeCol[r][fromCol];
					blankSize *= WORDS_PER_FRAME * 4;
					memset(&bitstreamCLB[r][fromCol][0], blankByte, blankSize);
				}
				else if(selectedOptions.clb || selectedOptions.clk){
					for(int c = fromCol ; c < toCol ; c++){
						for(int m = 0 ; m < LUT_numberOfFramesForResourceLetter[(uint8_t)resourceString[r][c]] ; m++){
							if(selectedOptions.clb){
								memset(&bitstreamCLB[r][c][m*WORDS_PER_FRAME], blankByte, (WORDS_BEFORE_CLK * 4));
								memset(&bitstreamCLB[r][c][m*WORDS_PER_FRAME+(WORDS_BEFORE_CLK+WORDS_AT_CLK)], blankByte, (WORDS_AFTER_CLK * 4));
							}
							if(selectedOptions.clk){
								for(int w = 0 ; w < WORDS_AT_CLK ; w++){
									bitstreamCLB[r][c][m*WORDS_PER_FRAME + WORDS_BEFORE_CLK + w] = ((blankByte << 24) | (blankByte << 16) | (blankByte << 8) | (blankByte));
								}
							}
						}
					}
				}
				if(selectedOptions.bram){
					int blankSize = numberOfBRAMsBeforeCol[r][toCol] - numberOfBRAMsBeforeCol[r][fromCol];
					blankSize *= FRAMES_PER_BRAM_CONTENT_COLUMN * WORDS_PER_FRAME * 4;
					memset(&bitstreamCLB[r][numberOfBRAMsBeforeCol[r][fromCol]][0], blankByte, blankSize); //memset with size 0 is safe, no need to check anything
				}
			}
		}
	}
	bitstreamHasValidData = true;
}
