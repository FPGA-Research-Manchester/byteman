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

#ifndef FILEIO_H
#define FILEIO_H

#include<iostream>
#include<cstdint> //uint
#include<fstream>
#include<string>

#include "Endianness.h"

namespace FileIO {
	inline uint64_t readNative64(std::ifstream& fin)
	{
		uint64_t readValue;
		fin.read(reinterpret_cast<char*>(&readValue), 8);
		return readValue;
	}
	inline uint32_t readNative32(std::ifstream& fin)
	{
		uint32_t readValue;
		fin.read(reinterpret_cast<char*>(&readValue), 4);
		return readValue;
	}
	inline uint16_t readNative16(std::ifstream& fin)
	{
		uint16_t readValue;
		fin.read(reinterpret_cast<char*>(&readValue), 2);
		return readValue;
	}
	inline uint8_t readNative8(std::ifstream& fin)
	{
		uint8_t readValue;
		fin.read(reinterpret_cast<char*>(&readValue), 1);
		return readValue;
	}
	inline uint64_t readBigEndian64(std::ifstream& fin)
	{
		return Endian::NativeToBigEndian64(FileIO::readNative64(fin));
	}
	inline uint32_t readBigEndian32(std::ifstream& fin)
	{
		return Endian::NativeToBigEndian32(FileIO::readNative32(fin));
	}
	inline uint16_t readBigEndian16(std::ifstream& fin)
	{
		return Endian::NativeToBigEndian16(FileIO::readNative16(fin));
	}
	inline uint8_t readBigEndian8(std::ifstream& fin)
	{
		return readNative8(fin);
	}

	inline uint64_t readLittleEndian64(std::ifstream& fin)
	{
		return Endian::NativeToLittleEndian64(FileIO::readNative64(fin));
	}
	inline uint32_t readLittleEndian32(std::ifstream& fin)
	{
		return Endian::NativeToLittleEndian32(FileIO::readNative32(fin));
	}
	inline uint16_t readLittleEndian16(std::ifstream& fin)
	{
		return Endian::NativeToLittleEndian16(FileIO::readNative16(fin));
	}
	inline uint8_t readLittleEndian8(std::ifstream& fin)
	{
		return readNative8(fin);
	}

	inline uint64_t readBitSwappedBigEndian64(std::ifstream& fin)
	{
		return Endian::BitSwap64(FileIO::readBigEndian64(fin));
	}
	inline uint32_t readBitSwappedBigEndian32(std::ifstream& fin)
	{
		return Endian::BitSwap32(FileIO::readBigEndian32(fin));
	}
	inline uint16_t readBitSwappedBigEndian16(std::ifstream& fin)
	{
		return Endian::BitSwap16(FileIO::readBigEndian16(fin));
	}
	inline uint8_t readBitSwappedBigEndian8(std::ifstream& fin)
	{
		return Endian::BitSwap8(FileIO::readBigEndian8(fin));
	}

	inline uint64_t readBitSwappedLittleEndian64(std::ifstream& fin)
	{
		return Endian::BitSwap64(FileIO::readLittleEndian64(fin));
	}
	inline uint32_t readBitSwappedLittleEndian32(std::ifstream& fin)
	{
		return Endian::BitSwap32(FileIO::readLittleEndian32(fin));
	}
	inline uint16_t readBitSwappedLittleEndian16(std::ifstream& fin)
	{
		return Endian::BitSwap16(FileIO::readLittleEndian16(fin));
	}
	inline uint8_t readBitSwappedLittleEndian8(std::ifstream& fin)
	{
		return Endian::BitSwap8(FileIO::readLittleEndian8(fin));
	}
	inline uint64_t read64(std::ifstream& fin, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::BE)
			return FileIO::readBigEndian64(fin);
		else if(e == Endianness::LE)
			return FileIO::readLittleEndian64(fin);
		else if(e == Endianness::BE_BS)
			return FileIO::readBitSwappedBigEndian64(fin);
		else if(e == Endianness::LE_BS)
			return FileIO::readBitSwappedLittleEndian64(fin);
		return FileIO::readNative64(fin);
	}
	inline uint32_t read32(std::ifstream& fin, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::BE)
			return FileIO::readBigEndian32(fin);
		else if(e == Endianness::LE)
			return FileIO::readLittleEndian32(fin);
		else if(e == Endianness::BE_BS)
			return FileIO::readBitSwappedBigEndian32(fin);
		else if(e == Endianness::LE_BS)
			return FileIO::readBitSwappedLittleEndian32(fin);
		return FileIO::readNative32(fin);
	}
	inline uint16_t read16(std::ifstream& fin, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::BE)
			return FileIO::readBigEndian16(fin);
		else if(e == Endianness::LE)
			return FileIO::readLittleEndian16(fin);
		else if(e == Endianness::BE_BS)
			return FileIO::readBitSwappedBigEndian16(fin);
		else if(e == Endianness::LE_BS)
			return FileIO::readBitSwappedLittleEndian16(fin);
		return FileIO::readNative16(fin);
	}
	inline uint8_t read8 (std::ifstream& fin, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::BE)
			return FileIO::readBigEndian8(fin);
		else if(e == Endianness::LE)
			return FileIO::readLittleEndian8(fin);
		else if(e == Endianness::BE_BS)
			return FileIO::readBitSwappedBigEndian8(fin);
		else if(e == Endianness::LE_BS)
			return FileIO::readBitSwappedLittleEndian8(fin);
		return FileIO::readNative8(fin);
	}


	inline char* readStringIntoCharArray(std::ifstream& fin, int stringSize)
	{
		char* readCharArray = new char[stringSize + 1];
		fin.read(readCharArray, stringSize);
		readCharArray[stringSize]='\0';
		return readCharArray;
	}
	inline std::string readString(std::ifstream& fin, int stringSize)
	{
		char* readCharArray = readStringIntoCharArray(fin, stringSize);
		std::string returnString = std::string(readCharArray);
		delete readCharArray;
		return returnString;
	}

	inline char* readBitSwappedStringIntoCharArray(std::ifstream& fin, int stringSize)
	{
		char* readCharArray = FileIO::readStringIntoCharArray(fin, stringSize);
		for(int i = 0 ; i < stringSize ; i++)
			readCharArray[i] = Endian::BitSwap8(readCharArray[i]);
		return readCharArray;
	}
	inline std::string readBitSwappedString(std::ifstream& fin, int stringSize)
	{
		char* readCharArray = readBitSwappedStringIntoCharArray(fin, stringSize);
		std::string returnString = std::string(readCharArray);
		delete readCharArray;
		return returnString;
	}

	inline char* readStringIntoCharArray(std::ifstream& fin, int stringSize, Endianness e)
	{
		if(e == Endianness::BE_BS || e == Endianness::LE_BS)
			return FileIO::readBitSwappedStringIntoCharArray(fin, stringSize);
		else
			return FileIO::readStringIntoCharArray(fin, stringSize);
	}
	inline std::string readString(std::ifstream& fin, int stringSize, Endianness e)
	{
		if(e == Endianness::BE_BS || e == Endianness::LE_BS)
			return FileIO::readBitSwappedString(fin, stringSize);
		else
			return FileIO::readString(fin, stringSize);
	}

	inline void writeNative64(std::ofstream& fout, uint64_t writeValue)
	{
		fout.write(reinterpret_cast<char*>(&writeValue), sizeof(writeValue));
	}
	inline void writeNative32(std::ofstream& fout, uint32_t writeValue)
	{
		fout.write(reinterpret_cast<char*>(&writeValue), sizeof(writeValue));
	}
	inline void writeNative16(std::ofstream& fout, uint16_t writeValue)
	{
		fout.write(reinterpret_cast<char*>(&writeValue), sizeof(writeValue));
	}
	inline void writeNative8 (std::ofstream& fout, uint8_t writeValue)
	{
		fout.write(reinterpret_cast<char*>(&writeValue), sizeof(writeValue));
	}

	inline void writeBigEndian64(std::ofstream& fout, uint64_t writeValue)
	{
		uint64_t writeValueBigEndian = Endian::BigEndianToNative64(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueBigEndian), sizeof(writeValueBigEndian));
	}
	inline void writeBigEndian32(std::ofstream& fout, uint32_t writeValue)
	{
		uint32_t writeValueBigEndian = Endian::BigEndianToNative32(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueBigEndian), sizeof(writeValueBigEndian));
	}
	inline void writeBigEndian16(std::ofstream& fout, uint16_t writeValue)
	{
		uint16_t writeValueBigEndian = Endian::BigEndianToNative16(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueBigEndian), sizeof(writeValueBigEndian));
	}
	inline void writeBigEndian8 (std::ofstream& fout, uint8_t writeValue)
	{
		uint8_t writeValueBigEndian = Endian::BigEndianToNative8(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueBigEndian), sizeof(writeValueBigEndian));
	}

	inline void writeLittleEndian64(std::ofstream& fout, uint64_t writeValue)
	{
		uint64_t writeValueLittleEndian = Endian::LittleEndianToNative64(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueLittleEndian), sizeof(writeValueLittleEndian));
	}
	inline void writeLittleEndian32(std::ofstream& fout, uint32_t writeValue)
	{
		uint32_t writeValueLittleEndian = Endian::LittleEndianToNative32(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueLittleEndian), sizeof(writeValueLittleEndian));
	}
	inline void writeLittleEndian16(std::ofstream& fout, uint16_t writeValue)
	{
		uint16_t writeValueLittleEndian = Endian::LittleEndianToNative16(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueLittleEndian), sizeof(writeValueLittleEndian));
	}
	inline void writeLittleEndian8 (std::ofstream& fout, uint8_t writeValue)
	{
		uint8_t writeValueLittleEndian = Endian::LittleEndianToNative8(writeValue);
		fout.write(reinterpret_cast<char*>(&writeValueLittleEndian), sizeof(writeValueLittleEndian));
	}

	inline void writeBitSwappedBigEndian64(std::ofstream& fout, uint64_t writeValue)
	{
		uint64_t writeValueByteSwapped = Endian::BitSwap64(writeValue);
		uint64_t writeValueByteSwappedBigEndian = Endian::BigEndianToNative64(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedBigEndian), sizeof(writeValueByteSwappedBigEndian));
	}
	inline void writeBitSwappedBigEndian32(std::ofstream& fout, uint32_t writeValue)
	{
		uint32_t writeValueByteSwapped = Endian::BitSwap32(writeValue);
		uint32_t writeValueByteSwappedBigEndian = Endian::BigEndianToNative32(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedBigEndian), sizeof(writeValueByteSwappedBigEndian));
	}
	inline void writeBitSwappedBigEndian16(std::ofstream& fout, uint16_t writeValue)
	{
		uint16_t writeValueByteSwapped = Endian::BitSwap16(writeValue);
		uint16_t writeValueByteSwappedBigEndian = Endian::BigEndianToNative16(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedBigEndian), sizeof(writeValueByteSwappedBigEndian));
	}
	inline void writeBitSwappedBigEndian8 (std::ofstream& fout, uint8_t writeValue)
	{
		uint8_t writeValueByteSwapped = Endian::BitSwap8(writeValue);
		uint8_t writeValueByteSwappedBigEndian = Endian::BigEndianToNative8(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedBigEndian), sizeof(writeValueByteSwappedBigEndian));
	}

	inline void writeBitSwappedLittleEndian64(std::ofstream& fout, uint64_t writeValue)
	{
		uint64_t writeValueByteSwapped = Endian::BitSwap64(writeValue);
		uint64_t writeValueByteSwappedLittleEndian = Endian::LittleEndianToNative64(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedLittleEndian), sizeof(writeValueByteSwappedLittleEndian));
	}
	inline void writeBitSwappedLittleEndian32(std::ofstream& fout, uint32_t writeValue)
	{
		uint32_t writeValueByteSwapped = Endian::BitSwap32(writeValue);
		uint32_t writeValueByteSwappedLittleEndian = Endian::LittleEndianToNative32(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedLittleEndian), sizeof(writeValueByteSwappedLittleEndian));
	}
	inline void writeBitSwappedLittleEndian16(std::ofstream& fout, uint16_t writeValue)
	{
		uint16_t writeValueByteSwapped = Endian::BitSwap16(writeValue);
		uint16_t writeValueByteSwappedLittleEndian = Endian::LittleEndianToNative16(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedLittleEndian), sizeof(writeValueByteSwappedLittleEndian));
	}
	inline void writeBitSwappedLittleEndian8 (std::ofstream& fout, uint8_t writeValue)
	{
		uint8_t writeValueByteSwapped = Endian::BitSwap8(writeValue);
		uint8_t writeValueByteSwappedLittleEndian = Endian::LittleEndianToNative8(writeValueByteSwapped);
		fout.write(reinterpret_cast<char*>(&writeValueByteSwappedLittleEndian), sizeof(writeValueByteSwappedLittleEndian));
	}

	inline void writeCharArray(std::ofstream& fout, char* writeCharArray, size_t outputSize = 0)
	{
		uint8_t nullChar = 0;
		size_t charArraySize = 1;
		for( ; (0 != writeCharArray[charArraySize-1] && charArraySize != outputSize); charArraySize++);
		fout.write(writeCharArray, charArraySize);
		for( ; charArraySize < outputSize ; charArraySize++)
			fout.write(reinterpret_cast<char*>(&nullChar), 1);
	}
	inline void writeString(std::ofstream& fout, std::string writeString, size_t outputSize = 0)
	{
		uint8_t nullChar = 0;
		size_t stringSize = writeString.size() + 1;
		if((0 == outputSize) || (outputSize >= stringSize))
			fout.write(writeString.c_str(), stringSize);
		else
			fout.write(writeString.c_str(), outputSize);
		for( ; stringSize < outputSize ; stringSize++)
			fout.write(reinterpret_cast<char*>(&nullChar), 1);
	}

	inline void writeBitSwappedCharArray(std::ofstream& fout, char* writeCharArray, size_t outputSize = 0)
	{
		uint8_t nextChar = 0;
		size_t charArraySize = 1;
		for( ; (0 != writeCharArray[charArraySize-1] && charArraySize != outputSize); charArraySize++);
		for(size_t i = 0 ; i < charArraySize ; i++){
			nextChar = Endian::BitSwap8(writeCharArray[i]);
			fout.write(reinterpret_cast<char*>(&nextChar), 1);
		}
		nextChar = 0;
		for( ; charArraySize < outputSize ; charArraySize++)
			fout.write(reinterpret_cast<char*>(&nextChar), 1);
	}
	inline void writeBitSwappedString(std::ofstream& fout, std::string writeString, size_t outputSize = 0)
	{
		uint8_t nextChar = 0;
		size_t stringSize = writeString.size() + 1;
		if((0 == outputSize) || (outputSize >= stringSize)) {
			for(size_t i = 0 ; i < stringSize ; i++){
				nextChar = Endian::BitSwap8(writeString.at(i));
				fout.write(reinterpret_cast<char*>(&nextChar), 1);
			}
		} else {
			for(size_t i = 0 ; i < outputSize ; i++){
				nextChar = Endian::BitSwap8(writeString.at(i));
				fout.write(reinterpret_cast<char*>(&nextChar), 1);
			}
		}
		nextChar = 0;
		for( ; stringSize < outputSize ; stringSize++)
			fout.write(reinterpret_cast<char*>(&nextChar), 1);
	}

	inline void writeCharArray(std::ofstream& fout, char* writeCharArray, size_t outputSize, Endianness e)
	{
		if(e == Endianness::BE_BS || e == Endianness::LE_BS)
			return FileIO::writeBitSwappedCharArray(fout, writeCharArray, outputSize);
		else
			return FileIO::writeCharArray(fout, writeCharArray, outputSize);
	}
	inline void writeString(std::ofstream& fout, std::string writeString, size_t outputSize, Endianness e)
	{
		if(e == Endianness::BE_BS || e == Endianness::LE_BS)
			return FileIO::writeBitSwappedString(fout, writeString, outputSize);
		else
			return FileIO::writeString(fout, writeString, outputSize);
	}
	inline void writeCharArray(std::ofstream& fout, char* writeCharArray, Endianness e)
	{
		if(e == Endianness::BE_BS || e == Endianness::LE_BS)
			return FileIO::writeBitSwappedCharArray(fout, writeCharArray, 0);
		else
			return FileIO::writeCharArray(fout, writeCharArray, 0);
	}
	inline void writeString(std::ofstream& fout, std::string writeString, Endianness e)
	{
		if(e == Endianness::BE_BS || e == Endianness::LE_BS)
			return FileIO::writeBitSwappedString(fout, writeString, 0);
		else
			return FileIO::writeString(fout, writeString, 0);
	}

	inline void write64(std::ofstream& fout, uint64_t writeValue, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::NATIVE)
			FileIO::writeNative64(fout, writeValue);
		else if(e == Endianness::BE)
			FileIO::writeBigEndian64(fout, writeValue);
		else if(e == Endianness::LE)
			FileIO::writeLittleEndian64(fout, writeValue);
		else if(e == Endianness::BE_BS)
			FileIO::writeBitSwappedBigEndian64(fout, writeValue);
		else if(e == Endianness::LE_BS)
			FileIO::writeBitSwappedLittleEndian64(fout, writeValue);
	}
	inline void write32(std::ofstream& fout, uint32_t writeValue, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::NATIVE)
			FileIO::writeNative32(fout, writeValue);
		else if(e == Endianness::BE)
			FileIO::writeBigEndian32(fout, writeValue);
		else if(e == Endianness::LE)
			FileIO::writeLittleEndian32(fout, writeValue);
		else if(e == Endianness::BE_BS)
			FileIO::writeBitSwappedBigEndian32(fout, writeValue);
		else if(e == Endianness::LE_BS)
			FileIO::writeBitSwappedLittleEndian32(fout, writeValue);
	}
	inline void write16(std::ofstream& fout, uint16_t writeValue, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::NATIVE)
			FileIO::writeNative16(fout, writeValue);
		else if(e == Endianness::BE)
			FileIO::writeBigEndian16(fout, writeValue);
		else if(e == Endianness::LE)
			FileIO::writeLittleEndian16(fout, writeValue);
		else if(e == Endianness::BE_BS)
			FileIO::writeBitSwappedBigEndian16(fout, writeValue);
		else if(e == Endianness::LE_BS)
			FileIO::writeBitSwappedLittleEndian16(fout, writeValue);
	}
	inline void write8 (std::ofstream& fout, uint8_t  writeValue, Endianness e = Endianness::NATIVE)
	{
		if(e == Endianness::NATIVE)
			FileIO::writeNative8(fout, writeValue);
		else if(e == Endianness::BE)
			FileIO::writeBigEndian8(fout, writeValue);
		else if(e == Endianness::LE)
			FileIO::writeLittleEndian8(fout, writeValue);
		else if(e == Endianness::BE_BS)
			FileIO::writeBitSwappedBigEndian8(fout, writeValue);
		else if(e == Endianness::LE_BS)
			FileIO::writeBitSwappedLittleEndian8(fout, writeValue);
	}
}

#endif // FILEIO_H
