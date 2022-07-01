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

#if !defined(NDEBUG)
inline void ensureFramesAre(bool usableFramesOnly, bool testValueEqual, uint32_t testValue)
{
	for(int r = 0 ; r < numberOfRows ; r++){
		for(int c = 0 ; c < numberOfCols[r] ; c++){
			uint8_t frameLetter = (uint8_t)resourceString[r][c];
			if(frameLetter <= 1) //don't include the extra frames on each row in the test
				continue;
			if(!usableFramesOnly || (!LUT_isFrameUnusedForResourceLetter[frameLetter])){
				for(int m = 0 ; m < LUT_numberOfFramesForResourceLetter[frameLetter] ; m++){
					for(int f = 0 ; f < WORDS_PER_FRAME ; f++){
						if(testValueEqual) {
							throwingAssertPrintVar_5(bitstreamCLB[r][c][m * WORDS_PER_FRAME + f], ==, testValue, r, c, m, WORDS_PER_FRAME, f);
						} else {
							throwingAssertPrintVar_5(bitstreamCLB[r][c][m * WORDS_PER_FRAME + f], !=, testValue, r, c, m, WORDS_PER_FRAME, f);
						}
					}
				}
			}
		}
		for(int c = 0 ; c < numberOfBRAMCols[r] ; c++){
			for(int m = 0 ; m < FRAMES_PER_BRAM_CONTENT_COLUMN ; m++){
				for(int f = 0 ; f < WORDS_PER_FRAME ; f++){
					if(testValueEqual){
						throwingAssertPrintVar_5(bitstreamBRAM[r][c][m * WORDS_PER_FRAME + f], ==, testValue, r, c, m, WORDS_PER_FRAME, f);
					} else {
						throwingAssertPrintVar_5(bitstreamBRAM[r][c][m * WORDS_PER_FRAME + f], !=, testValue, r, c, m, WORDS_PER_FRAME, f);
					}
				}
			}
		}
	}
}
#endif

