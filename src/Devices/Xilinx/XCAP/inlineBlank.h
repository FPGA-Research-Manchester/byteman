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
	initializedResourceStringPartName = "";
	memset(&resourceString, 0, MAX_ROWS * MAX_COLS * sizeof(char));
}

inline void blankBuffers()
{
	uint8_t blankByte = selectedOptions.intParam % 256;
	if(regionSelection.empty()) {
		if(selectedOptions.clb)
			memset(&bitstreamCLB[0][0][0], blankByte, (&bitstreamBRAM[0][0][0]-&bitstreamCLB[0][0][0]));
		if(selectedOptions.bram)
			memset(&bitstreamBRAM[0][0][0], blankByte, (bitstreamEnd-&bitstreamBRAM[0][0][0]));
	} else {
		if(selectedOptions.clb)
			for (Rect2D &selRect : regionSelection) {
				for(int r = selRect.position.row ; r < (selRect.position.row + selRect.size.row) ; r++){
					int blankSize = numberOfFramesBeforeCol[r][selRect.position.col + selRect.size.col] - numberOfFramesBeforeCol[r][selRect.position.col];
					blankSize *= WORDS_PER_FRAME * 4;
					memset(&bitstreamCLB[r][selRect.position.col][0], blankByte, blankSize); //memset with size 0 is safe, no need to check anything
				}
			}
		if(selectedOptions.bram)
			for (Rect2D &selRect : regionSelection) {
				for(int r = selRect.position.row ; r < (selRect.position.row + selRect.size.row) ; r++){
					int blankSize = numberOfBRAMsBeforeCol[r][selRect.position.col + selRect.size.col] - numberOfBRAMsBeforeCol[r][selRect.position.col];
					blankSize *= FRAMES_PER_BRAM_CONTENT_COLUMN * WORDS_PER_FRAME * 4;
					memset(&bitstreamCLB[r][numberOfBRAMsBeforeCol[r][selRect.position.col]][0], blankByte, blankSize); //memset with size 0 is safe, no need to check anything
				}
			}
	}
}
