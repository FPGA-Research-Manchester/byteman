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

inline void ensureSelectedEndianness(Endianness newEndianness){
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before modifications
	Endianness endiannessConvertion = Endian::diff(loadedBitstreamEndianness, newEndianness);
	if(endiannessConvertion == Endianness::NATIVE)// if there is no actual convertion needed
		return;
	int bitstreamSize = bitstreamEnd - bitstreamBegin;
	
	if(endiannessConvertion == Endianness::BE)
		for( int i = 0 ; i < bitstreamSize ; i++)
			bitstreamBegin[i] = Endian::NativeToBigEndian32(bitstreamBegin[i]);
		
	if(endiannessConvertion == Endianness::LE)
		for( int i = 0 ; i < bitstreamSize ; i++)
			bitstreamBegin[i] = Endian::NativeToLittleEndian32(bitstreamBegin[i]);
		
	if(endiannessConvertion == Endianness::BE_BS)
		for( int i = 0 ; i < bitstreamSize ; i++)
			bitstreamBegin[i] = Endian::BitSwap32(Endian::NativeToBigEndian32(bitstreamBegin[i]));
		
	if(endiannessConvertion == Endianness::LE_BS)
		for( int i = 0 ; i < bitstreamSize ; i++)
			bitstreamBegin[i] = Endian::BitSwap32(Endian::NativeToLittleEndian32(bitstreamBegin[i]));
		
	loadedBitstreamEndianness = newEndianness;
}

inline void setBitstreamWord(std::string params){
	ensureInitializedBitstreamArrays();//initialize bitstream arrays before modifications
	uint32_t r, y, c, b, m, w, mask, newVal;
	str::parse::multipleUints(params, y, c, b, m, w, mask, newVal);
	mask = Endian::NativeToAnyEndianness32(mask, loadedBitstreamEndianness);
	newVal = Endian::NativeToAnyEndianness32(newVal, loadedBitstreamEndianness);
	r = y / CLB_PER_CLOCK_REGION;
	if(b == BLOCKTYPE_LOGIC){
		bitstreamCLB[r][c][m*WORDS_PER_FRAME + w] = (mask & newVal) | ((mask^0xFFFFFFFF) & bitstreamCLB[r][c][m*WORDS_PER_FRAME + w]);
	} else if(b == BLOCKTYPE_BLOCKRAM){
		bitstreamBRAM[r][numberOfBRAMsBeforeCol[r][c]][m*WORDS_PER_FRAME + w] = (mask & newVal) | ((mask^0xFFFFFFFF) & bitstreamBRAM[r][numberOfBRAMsBeforeCol[r][c]][m*WORDS_PER_FRAME + w]);
	}
	bitstreamHasValidData = true;
}

