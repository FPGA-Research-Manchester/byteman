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

inline void updateDateAndTime()
{
	time_t timestamp = time(0);
	struct tm  tstruct;
	char	   buf[80];
	tstruct = *localtime(&timestamp);
	strftime(buf, sizeof(buf), "%Y/%m/%d", &tstruct);
	fileDate = std::string(buf);
	strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
	fileTime = std::string(buf);
}

inline void outputBITheader16bString(std::ofstream& fout, Endianness e, std::string s){
	FileIO::write16(fout, static_cast<uint16_t>(s.size() + 1), e);
	FileIO::writeString(fout, s, e);
}

inline void outputBITheader(std::ofstream& fout, Endianness e)
{
	log("Generating bitstream header in " + Endian::to_string(e) + ".");
	outputBITheader16bString(fout, e, std::string("\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0"));
	FileIO::write16(fout, 1, e);//16-bit BE value = 1
	FileIO::write8(fout, 'a', e);
	outputBITheader16bString(fout, e, designName);
	FileIO::write8(fout, 'b', e);//8-bit BE value = 'b'
	outputBITheader16bString(fout, e, partName);
	FileIO::write8(fout, 'c', e);//8-bit BE value = 'c'
	outputBITheader16bString(fout, e, fileDate);
	FileIO::write8(fout, 'd', e);//8-bit BE value = 'd'
	outputBITheader16bString(fout, e, fileTime);
	FileIO::write8(fout, 'e', e);//8-bit BE value = 'e'
	headerLocationOfRemainingFileLength = fout.tellp();
	FileIO::write32(fout, 0, e);//32-bit BE value = 0 representing remaining file size. needs to be fixed later
}

inline void outputBITheaderLengthField(std::ofstream& fout, Endianness e)
{
	std::streamoff finalFileSize = fout.tellp();
	std::streamoff reportDiff = finalFileSize - (headerLocationOfRemainingFileLength + 4);
	fout.seekp(headerLocationOfRemainingFileLength);
	FileIO::write32(fout, static_cast<uint32_t>(reportDiff), e);
}

inline void outputCAPheaderConstant(std::ofstream& fout, Endianness e)
{
	for(int i = 0 ; i < 16 ; i++) // 64 bytes of 0xFF
		FileIO::write32(fout, 0xFFFFFFFF, e);//32-bit BE value = 0xFFFFFFFF
	FileIO::write32(fout, 0x000000BB, e);
	FileIO::write32(fout, 0x11220044, e);
	FileIO::write32(fout, 0xFFFFFFFF, e);
	FileIO::write32(fout, 0xFFFFFFFF, e);
	FileIO::write32(fout, XCAP_getSyncInstruction(), e);
}

inline void writeBitstreamMainEmptySLR(std::ofstream& fout, int slr)
{
	outputCAPheaderConstant(fout, loadedBitstreamEndianness);
	outputBitstreamEmptySLRHeaderSequence(fout, slr, false, loadedBitstreamEndianness);
}

inline void writeBitstreamMainSingleRegion(std::ofstream& fout, int slr, Rect2D writeRect)
{
	void *blankFrame = calloc(WORDS_PER_FRAME, 4);
	
	if(rect::empty(writeRect)) // If output region is empty, just return
		return;
	int srcR  = writeRect.position.row / CLB_PER_CLOCK_REGION;
	int sizeR = writeRect.size.row / CLB_PER_CLOCK_REGION;
	for(int r = 0 ; r < sizeR ; r++){
		int fromCol = writeRect.position.col;
		int toCol = writeRect.position.col + writeRect.size.col;
		if(selectedOptions.skipUnused){
			for( ; (fromCol < numberOfCols[r] && LUT_isFrameUnusedForResourceLetter[(uint8_t)resourceString[srcR + r][fromCol]]) ; fromCol++);
			for( ; (toCol > 0 && LUT_isFrameUnusedForResourceLetter[(uint8_t)resourceString[srcR + r][toCol]]) ; toCol--);
			if(fromCol >= toCol)
				continue;
		}
		if(selectedOptions.clb){
			int framesToWrite = numberOfFramesBeforeCol[r][toCol]-numberOfFramesBeforeCol[r][fromCol];
			uint32_t farValue = XCAP_getFAR(slr, BLOCKTYPE_LOGIC, srcR + r, fromCol, 0);
			if(selectedOptions.blank){
				XCAP_writeRegister(fout, XCAP::Register::FAR, farValue, loadedBitstreamEndianness);
				XCAP_writeCommand(fout, XCAP::Command::WCFG, loadedBitstreamEndianness);
				XCAP_writeNOP(fout, 1, 0, loadedBitstreamEndianness);
				XCAP_writeFDRI(fout, ((framesToWrite + 1)*WORDS_PER_FRAME), loadedBitstreamEndianness);
				for(int i = 0 ; i <= framesToWrite ; i++)
					fout.write((char*)blankFrame, WORDS_PER_FRAME * 4);
			}//selectedOptions.blank
			XCAP_writeRegister(fout, XCAP::Register::FAR, farValue, loadedBitstreamEndianness);
			XCAP_writeCommand(fout, XCAP::Command::WCFG, loadedBitstreamEndianness);
			XCAP_writeNOP(fout, 1, 0, loadedBitstreamEndianness);
			XCAP_writeFDRI(fout, ((framesToWrite + 1)*WORDS_PER_FRAME), loadedBitstreamEndianness);
			
			fout.write((char*)&bitstreamCLB[srcR + r][fromCol][0], framesToWrite * WORDS_PER_FRAME * 4);
			fout.write((char*)blankFrame, WORDS_PER_FRAME * 4);
		}//selectedOptions.clb
		if(selectedOptions.bram){
			int framesToWrite = FRAMES_PER_BRAM_CONTENT_COLUMN * (numberOfBRAMsBeforeCol[r][toCol]-numberOfBRAMsBeforeCol[r][fromCol]);
			if(framesToWrite > 0) {
				uint32_t farValue = XCAP_getFAR(slr, BLOCKTYPE_BLOCKRAM, srcR + r, numberOfBRAMsBeforeCol[r][fromCol], 0);
				XCAP_writeRegister(fout, XCAP::Register::FAR, farValue, loadedBitstreamEndianness);
				XCAP_writeCommand(fout, XCAP::Command::WCFG, loadedBitstreamEndianness);
				XCAP_writeNOP(fout, 1, 0, loadedBitstreamEndianness);
				XCAP_writeFDRI(fout, ((framesToWrite + 1)*WORDS_PER_FRAME), loadedBitstreamEndianness);
				
				fout.write((char*)&bitstreamBRAM[srcR + r][numberOfBRAMsBeforeCol[r][fromCol]][0], framesToWrite * WORDS_PER_FRAME * 4);
				fout.write((char*)blankFrame, WORDS_PER_FRAME * 4);
			}
		}//selectedOptions.bram
	}
}

inline void writeBitstreamMainSingleSLR(std::ofstream& fout, int slr, Rect2D cmdRect)
{
	Rect2D slrCoordsRect = {{0, 0}, {0, 0}};
	slrCoordsRect.position.row = SLRinfo[slr].fromRow * CLB_PER_CLOCK_REGION;
	slrCoordsRect.size.row = (SLRinfo[slr].toRow - SLRinfo[slr].fromRow + 1) * CLB_PER_CLOCK_REGION;
	slrCoordsRect.size.col = maxNumberOfCols;
	
	outputCAPheaderConstant(fout, loadedBitstreamEndianness);
	outputBitstreamSLRHeaderBitstreamSequence(fout, slr, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
	
	if(selectedOptions.partialNotFull){
		for (Rect2D selRect : regionSelection) {
			writeBitstreamMainSingleRegion(fout, slr, rect::getOverlap(selRect, slrCoordsRect));
		}
		writeBitstreamMainSingleRegion(fout, slr, rect::getOverlap(cmdRect, slrCoordsRect));
	} else {// full
		writeBitstreamMainSingleRegion(fout, slr, slrCoordsRect);
	}
	
	outputBitstreamSLRFooterBitstreamSequence(fout, slr, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
	outputBitstreamSLRHeaderAfterBitstreamSequence(fout, slr, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
}

inline void writeBitstreamMain(std::ofstream& fout, Rect2D cmdRect)
{
	bool overlap[MAX_SLRS];
	for(int slr = 0 ; slr < numberOfSLRs ; slr++){
		//Check if there is overlap between selected regions and the current SLR.
		Rect2D slrCoordsRect = {{0, 0}, {0, 0}};
		slrCoordsRect.position.row = SLRinfo[slr].fromRow * CLB_PER_CLOCK_REGION;
		slrCoordsRect.size.row = (SLRinfo[slr].toRow - SLRinfo[slr].fromRow + 1) * CLB_PER_CLOCK_REGION;
		slrCoordsRect.size.col = maxNumberOfCols;
		overlap[slr] = false;
		if(selectedOptions.partialNotFull){
			for (Rect2D selRect : regionSelection) {
				if(!rect::empty(rect::getOverlap(selRect, slrCoordsRect))){
					overlap[slr] = true;
					break;
				}
			}
			if(!rect::empty(rect::getOverlap(cmdRect, slrCoordsRect)))
				overlap[slr] = true;
		} else {// full bitstream output
			overlap[slr] = true;
		}
	}
	for(int slr = 0 ; slr < numberOfSLRs ; slr++){
		if(overlap[slr])
			writeBitstreamMainSingleSLR(fout, slr, cmdRect);
		else
			writeBitstreamMainEmptySLR(fout, slr);
	}
	for(int slr = numberOfSLRs - 2 ; slr >= 0 ; slr--){
		if(overlap[slr])
			outputBitstreamSLRWrapUpSequence(fout, slr, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
		else
			outputBitstreamEmptySLRWrapUpSequence(fout, slr, false, loadedBitstreamEndianness);
	}
}

inline void writeBitstreamBIT(std::ofstream& fout, Rect2D cmdRect)
{
	//if needing to ensure all .bit files MUST be bigendian, uncomment next line
	//ensureSelectedEndianness(Endianness::BE);
	if(selectedOptions.partialNotFull)
		designName.append(";PARTIAL=TRUE");
	designName.append(";bytemanVersion=").append(VERSION).append(":").append(VERSION_BUILD);
	
	updateDateAndTime();
	outputBITheader(fout, loadedBitstreamEndianness);//.bit always big endian
	
	outputBitstreamGlobalHeaderSequence(fout, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
	writeBitstreamMain(fout, cmdRect);
	outputBitstreamGlobalFooterSequence(fout, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
	
	outputBITheaderLengthField(fout, loadedBitstreamEndianness);
}

inline void writeBitstreamBIN(std::ofstream& fout, Rect2D cmdRect)
{
	outputBitstreamGlobalHeaderSequence(fout, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
	writeBitstreamMain(fout, cmdRect);
	outputBitstreamGlobalFooterSequence(fout, (!selectedOptions.partialNotFull), loadedBitstreamEndianness);
}
