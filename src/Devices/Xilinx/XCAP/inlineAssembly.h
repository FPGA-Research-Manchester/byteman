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

inline void assemblyOutputData(ifstream& fin, ofstream& fout, streamoff sizeInBytes)
{
	streamoff numOfWords = sizeInBytes/(streamoff)4;
	for(streamoff i = 0 ; i < numOfWords ; i++){
		uint32_t aWord = FileIO::read32(fin, loadedBitstreamEndianness);
		fout << ".word " <<"0x" << uppercase << hex << setw(8) << setfill('0') << aWord << endl;
	}
	streamoff numOfBytes = sizeInBytes % (streamoff)4;
	for(streamoff i = 0 ; i < numOfBytes ; i++){
		uint8_t aByte = FileIO::read32(fin, loadedBitstreamEndianness);
		fout << ".byte " <<"0x" << uppercase << hex << setw(2) << setfill('0') << aByte << endl;
	}
}
