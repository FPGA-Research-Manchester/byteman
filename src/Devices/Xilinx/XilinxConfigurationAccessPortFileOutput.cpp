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

#include "XilinxConfigurationAccessPort.h"
#include "../../Common/FileIO.h"

streamoff XilinxConfigurationAccessPort::outputBITheader(ofstream& fout, Endianess e)
{
	cout << "Output endianess is " << Endian::to_string(e) << endl;
	FileIO::write16(fout, 9, e);//16-bit BE value = 9
	FileIO::writeString(fout, string("\x0F\xF0\x0F\xF0\x0F\xF0\x0F\xF0"), e);
	FileIO::write16(fout, 1, e);//16-bit BE value = 1
	FileIO::write8(fout, 'a', e);
	FileIO::write16(fout, static_cast<uint16_t>(designName.size()+1), e);
	FileIO::writeString(fout, designName, e);
	FileIO::write8(fout, 'b', e);//8-bit BE value = 'b'
	FileIO::write16(fout, static_cast<uint16_t>(partName.size()+1), e);
	FileIO::writeString(fout, partName, e);
	FileIO::write8(fout, 'c', e);//8-bit BE value = 'c'
	FileIO::write16(fout, static_cast<uint16_t>(fileDate.size()+1), e);
	FileIO::writeString(fout, fileDate, e);
	FileIO::write8(fout, 'd', e);//8-bit BE value = 'd'
	FileIO::write16(fout, static_cast<uint16_t>(fileTime.size()+1), e);
	FileIO::writeString(fout, fileTime, e);
	FileIO::write8(fout, 'e', e);//8-bit BE value = 'e'
	streamoff headerLocationOfRemainingFileLength = fout.tellp();
	FileIO::write32(fout, 0, e);//32-bit BE value = 0 representing remaining file size. needs to be fixed later
	
	XilinxConfigurationAccessPort::outputCAPheaderConstant(fout, e);
	
	return headerLocationOfRemainingFileLength;
}
void XilinxConfigurationAccessPort::outputBITheaderLengthField(ofstream& fout, streamoff headerLocationOfRemainingFileLength, Endianess e)
{
	streamoff finalFileSize = fout.tellp();
	streamoff reportDiff = finalFileSize - (headerLocationOfRemainingFileLength + 4);
	fout.seekp(headerLocationOfRemainingFileLength);
	FileIO::write32(fout, static_cast<uint32_t>(reportDiff), e);
}

void XilinxConfigurationAccessPort::outputCAPheaderConstant(ofstream& fout, Endianess e)
{
	for(int i = 0 ; i < 16 ; i++) // 64 bytes of 0xFF
		FileIO::write32(fout, 0xFFFFFFFF, e);//32-bit BE value = 0xFFFFFFFF
	FileIO::write32(fout, 0x000000BB, e);
	FileIO::write32(fout, 0x11220044, e);
	FileIO::write32(fout, 0xFFFFFFFF, e);
	FileIO::write32(fout, 0xFFFFFFFF, e);
	FileIO::write32(fout, XCAP_SyncInstruction(), e);
}
