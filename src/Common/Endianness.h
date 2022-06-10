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

#ifndef ENDIANESS_H
#define ENDIANESS_H

#include<cstdint> //uint
#include<string>

#if __has_include(<sys/param.h>)
#include<sys/param.h>
#endif

using namespace std;

#if (defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN) || (defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN) || defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
	#define IS_LITTLE_ENDIAN() 0
#elif defined(_WIN32) || (defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN) || (defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN) || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
	#define IS_LITTLE_ENDIAN() 1
#else
	#error Could not determine machine endianness in the preprocessor!
	inline int IS_LITTLE_ENDIAN()
	{
		union {
			unsigned int v;
			unsigned char b;
		} u;
		u.v = 1;
		return u.b;
	}
#endif

enum class Endianness	///< Endianness in byteman is represented not only by big/little endian, but also by potential bit swapping inside bytes
{
	BE,					///< Big endian ("BE" instead of full-er name, so it does not conflict with linux's reserved endianess words)
	LE,					///< Little endian ("LE" instead of full-er name, so it does not conflict with linux's reserved endianess words)
	BE_BS,				///< Big endian with bit swaps inside each byte
	LE_BS,				///< Little endian with bit swaps inside each byte
	NATIVE = (IS_LITTLE_ENDIAN()?LE:BE)	///< System native will always be the fastest endianess to process
};

namespace Endian {
	inline string to_string(Endianness e)
	{
		if(Endianness::NATIVE == e)
			return "Native";
		if(Endianness::BE == e)
			return "Big Endian";
		if(Endianness::LE == e)
			return "Little Endian";
		if(Endianness::BE_BS == e)
			return "Big Endian with BitSwap";
		if(Endianness::LE_BS == e)
			return "Little Endian with BitSwap";
		return "null";	//should not be reached.
	}
	inline Endianness toggleEndianness(Endianness e, bool toggle = true)
	{
		if(!toggle)
			return e;
		if(Endianness::BE == e)
			return Endianness::LE;
		if(Endianness::LE == e)
			return Endianness::BE;
		if(Endianness::BE_BS == e)
			return Endianness::LE_BS;
		if(Endianness::LE_BS == e)
			return Endianness::BE_BS;
		return Endianness::NATIVE;	//should not be reached.
	}
	inline Endianness toggleEndianBitSwap(Endianness e, bool toggle = true)
	{
		if(!toggle)
			return e;
		if(Endianness::BE == e)
			return Endianness::BE_BS;
		if(Endianness::LE == e)
			return Endianness::LE_BS;
		if(Endianness::BE_BS == e)
			return Endianness::BE;
		if(Endianness::LE_BS == e)
			return Endianness::LE;
		return Endianness::NATIVE;	//should not be reached.
	}
	inline bool isBitSwap(Endianness e)
	{
		return (Endianness::BE_BS == e || Endianness::LE_BS == e);
	}
	inline bool isLE(Endianness e)
	{
		return (Endianness::LE == e || Endianness::LE_BS == e);
	}
	inline bool isBE(Endianness e)
	{
		return (Endianness::BE == e || Endianness::BE_BS == e);
	}
/**************************************************************************//**
 * Returns what endianess operation is needed to convert between two endianesses.
 * The returned endianess represents the operation needed to be done between
 * host system's endianess and returned one, to achieve the same conversion as
 * between the two given endianesses.
 * For example, will return Endianness::NATIVE if no conversion is needed.
 *****************************************************************************/
	inline Endianness diff(Endianness e1, Endianness e2)
	{
		Endianness accumulatedEndiannessDiff = Endianness::NATIVE;

		accumulatedEndiannessDiff = toggleEndianBitSwap(accumulatedEndiannessDiff, isBitSwap(e1));
		accumulatedEndiannessDiff = toggleEndianBitSwap(accumulatedEndiannessDiff, isBitSwap(e2));

		accumulatedEndiannessDiff = toggleEndianness(accumulatedEndiannessDiff, isLE(e1));
		accumulatedEndiannessDiff = toggleEndianness(accumulatedEndiannessDiff, isLE(e2));

		return accumulatedEndiannessDiff;
	}
	
	inline uint16_t swapbytes16(uint16_t x)
	{
		return (uint16_t)(((uint16_t)(x) & 0xFFU) << 8 | ((uint16_t)(x) & 0xFF00U) >> 8);
	}
	inline uint32_t swapbytes32(uint32_t x)
	{
		return (uint32_t)(((uint32_t)(x) & 0xFF) << 24 | ((uint32_t)(x) & 0xFF00) << 8 | ((uint32_t)(x) & 0xFF0000) >> 8 | ((uint32_t)(x) & 0xFF000000) >> 24);
	}
	inline uint64_t swapbytes64(uint64_t x)
	{
		return (uint64_t)((((uint64_t)(x) & 0xFF) << 56) | ((uint64_t)(x) & 0xFF00ULL) << 40 |((uint64_t)(x) & 0xFF0000ULL) << 24 |((uint64_t)(x) & 0xFF000000ULL) << 8 |((uint64_t)(x) & 0xFF00000000ULL) >> 8 |((uint64_t)(x) & 0xFF0000000000ULL) >> 24 | ((uint64_t)(x) & 0xFF000000000000ULL) >> 40 | ((uint64_t)(x) & 0xFF00000000000000ULL) >> 56);
	}


	inline uint8_t  NativeToBigEndian8 (uint8_t  x)
	{
		return ((uint8_t)(x));
	}
	inline uint16_t NativeToBigEndian16(uint16_t x)
	{
		return (Endianness::NATIVE == Endianness::BE)?((uint16_t)(x)):swapbytes16(x);
	}
	inline uint32_t NativeToBigEndian32(uint32_t x)
	{
		return (Endianness::NATIVE == Endianness::BE)?((uint32_t)(x)):swapbytes32(x);
	}
	inline uint64_t NativeToBigEndian64(uint64_t x)
	{
		return (Endianness::NATIVE == Endianness::BE)?((uint64_t)(x)):swapbytes64(x);
	}
	
	inline uint8_t  NativeToLittleEndian8 (uint8_t  x)
	{
		return ((uint8_t)(x));
	}
	inline uint16_t NativeToLittleEndian16(uint16_t x)
	{
		return (Endianness::NATIVE == Endianness::LE)?((uint16_t)(x)):swapbytes16(x);
	}
	inline uint32_t NativeToLittleEndian32(uint32_t x)
	{
		return (Endianness::NATIVE == Endianness::LE)?((uint32_t)(x)):swapbytes32(x);
	}
	inline uint64_t NativeToLittleEndian64(uint64_t x)
	{
		return (Endianness::NATIVE == Endianness::LE)?((uint64_t)(x)):swapbytes64(x);
	}
	
	inline uint8_t  BigEndianToNative8(uint8_t  x)
	{
		return ((uint8_t)(x));
	}
	inline uint16_t BigEndianToNative16(uint16_t x)
	{
		return NativeToBigEndian16(x);
	} 
	inline uint32_t BigEndianToNative32(uint32_t x)
	{
		return NativeToBigEndian32(x);
	} 
	inline uint64_t BigEndianToNative64(uint64_t x)
	{
		return NativeToBigEndian64(x);
	}
	inline uint8_t  LittleEndianToNative8(uint8_t  x)
	{
		return ((uint8_t)(x));
	}
	inline uint16_t LittleEndianToNative16(uint16_t x)
	{
		return NativeToLittleEndian16(x);
	} 
	inline uint32_t LittleEndianToNative32(uint32_t x)
	{
		return NativeToLittleEndian32(x);
	} 
	inline uint64_t LittleEndianToNative64(uint64_t x)
	{
		return NativeToLittleEndian64(x);
	} 

	//BitSwaps: swap bit order within the bytes
	inline uint8_t  BitSwap8(uint8_t  x)
	{
		return (((x & 0x80)>>7) | ((x & 0x40)>>5) | ((x & 0x20)>>3) | ((x & 0x10)>>1) | ((x & 0x08)<<1) | ((x & 0x04)<<3) | ((x & 0x02)<<5) | ((x & 0x01)<<7));
	}
	inline uint16_t BitSwap16(uint16_t x)
	{
		return (((x & 0x8080)>>7) | ((x & 0x4040)>>5) | ((x & 0x2020)>>3) | ((x & 0x1010)>>1) | ((x & 0x0808)<<1) | ((x & 0x0404)<<3) | ((x & 0x0202)<<5) | ((x & 0x0101)<<7));
	} 
	inline uint32_t BitSwap32(uint32_t x)
	{
		return (((x & 0x80808080)>>7) | ((x & 0x40404040)>>5) | ((x & 0x20202020)>>3) | ((x & 0x10101010)>>1) | ((x & 0x08080808)<<1) | ((x & 0x04040404)<<3) | ((x & 0x02020202)<<5) | ((x & 0x01010101)<<7));
	} 
	inline uint64_t BitSwap64(uint64_t x)
	{
		return (((x & 0x8080808080808080)>>7) | ((x & 0x4040404040404040)>>5) | ((x & 0x2020202020202020)>>3) | ((x & 0x1010101010101010)>>1) | ((x & 0x0808080808080808)<<1) | ((x & 0x0404040404040404)<<3) | ((x & 0x0202020202020202)<<5) | ((x & 0x0101010101010101)<<7));
	}

	inline uint8_t  NativeToAnyEndianness8(uint8_t  x, Endianness e)
	{
		return ((uint8_t)(x));
	}
	inline uint16_t NativeToAnyEndianness16(uint16_t x, Endianness e)
	{
		if(Endianness::BE == e)
			return NativeToBigEndian16(x);
		if(Endianness::LE == e)
			return NativeToLittleEndian16(x);
		if(Endianness::BE_BS == e)
			return BitSwap16(NativeToBigEndian16(x));
		if(Endianness::LE_BS == e)
			return BitSwap16(NativeToLittleEndian16(x));
		//Native then
		return ((uint16_t)(x));
	} 
	inline uint32_t NativeToAnyEndianness32(uint32_t x, Endianness e)
	{
		if(Endianness::BE == e)
			return NativeToBigEndian32(x);
		if(Endianness::LE == e)
			return NativeToLittleEndian32(x);
		if(Endianness::BE_BS == e)
			return BitSwap32(NativeToBigEndian32(x));
		if(Endianness::LE_BS == e)
			return BitSwap32(NativeToLittleEndian32(x));
		//Native then
		return ((uint32_t)(x));
	} 
	inline uint64_t NativeToAnyEndianness64(uint64_t x, Endianness e)
	{
		if(Endianness::BE == e)
			return NativeToBigEndian64(x);
		if(Endianness::LE == e)
			return NativeToLittleEndian64(x);
		if(Endianness::BE_BS == e)
			return BitSwap64(NativeToBigEndian64(x));
		if(Endianness::LE_BS == e)
			return BitSwap64(NativeToLittleEndian64(x));
		//Native then
		return ((uint64_t)(x));
	} 
}
#endif //ENDIANESS_H
