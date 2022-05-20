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
using namespace std;
#if __has_include(<sys/param.h>)
#include <sys/param.h>
#endif
#if defined(__BYTE_ORDER) && __BYTE_ORDER == __BIG_ENDIAN || defined(BYTE_ORDER) && BYTE_ORDER == BIG_ENDIAN || defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIBSEB) || defined(__MIBSEB) || defined(__MIBSEB__)
	#define IS_LITTLE_ENDIAN() 0
#elif defined(__BYTE_ORDER) && __BYTE_ORDER == __LITTLE_ENDIAN || defined(BYTE_ORDER) && BYTE_ORDER == LITTLE_ENDIAN || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__)
	#define IS_LITTLE_ENDIAN() 1
#else
//static __attribute__((always_inline)) int IS_LITTLE_ENDIAN(void) GCC
//static [[msvc::forceinline]] int IS_LITTLE_ENDIAN(void) microsoft C++20
static int IS_LITTLE_ENDIAN(void)
{
	union {
		unsigned int v;
		unsigned char b;
	} u;
	u.v = 1;
	return u.b;
}
#endif
	
enum class Endianess {
	NATIVE, 			///< Whatever native is, it will always be the fastest endianess to process
	BE,					///< Detect/Select Big endianess
	LE,					///< Detect/Select Little endianess
	BE_BS,				///< Detect/Select Big endianess with bit swaps inside each byte
	LE_BS				///< Detect/Select Little endianess with bit swaps inside each byte
};

namespace Endian {
	inline string to_string(Endianess e){
		if(Endianess::BE == e)
			return "Big Endian";
		if(Endianess::LE == e)
			return "Little Endian";
		if(Endianess::BE_BS == e)
			return "Big Endian with BitSwap";
		if(Endianess::LE_BS == e)
			return "Little Endian with BitSwap";
		return "Native";
	}
	inline Endianess toggleEndianess(Endianess e){
		if(Endianess::BE == e)
			return Endianess::LE;
		if(Endianess::LE == e)
			return Endianess::BE;
		if(Endianess::BE_BS == e)
			return Endianess::LE_BS;
		if(Endianess::LE_BS == e)
			return Endianess::BE_BS;
		//Native then
		if(IS_LITTLE_ENDIAN())
			return Endianess::BE;
		return Endianess::LE;
	}
	inline Endianess toggleEndianBitSwap(Endianess e){
		if(Endianess::BE == e)
			return Endianess::BE_BS;
		if(Endianess::LE == e)
			return Endianess::LE_BS;
		if(Endianess::BE_BS == e)
			return Endianess::BE;
		if(Endianess::LE_BS == e)
			return Endianess::LE;
		//Native then
		if(IS_LITTLE_ENDIAN())
			return Endianess::LE_BS;
		return Endianess::BE_BS;
	}
	inline Endianess fixEndianIfNative(Endianess e){
		if(IS_LITTLE_ENDIAN() && Endianess::LE == e)
			return Endianess::NATIVE;
		if((!(IS_LITTLE_ENDIAN())) && Endianess::BE == e)
			return Endianess::NATIVE;
		return e;
	}
/**************************************************************************//**
 * Returns what endianess operation is needed to convert between two endianesses.
 * The returned endianess represents the operation needed to be done between
 * host system's endianess and returned one, to achieve the same conversion as
 * between the two given endianesses.
 * For example, will return Endianess::NATIVE if no conversion is needed.
 *****************************************************************************/
	inline Endianess diff(Endianess e1, Endianess e2){
		Endianess accumulatedEndianessDiff = Endianess::NATIVE;
		
		if(Endianess::BE_BS == e1 || Endianess::LE_BS == e1)
			accumulatedEndianessDiff = toggleEndianBitSwap(accumulatedEndianessDiff);
		if(Endianess::BE_BS == e2 || Endianess::LE_BS == e2)
			accumulatedEndianessDiff = toggleEndianBitSwap(accumulatedEndianessDiff);
		
		if(IS_LITTLE_ENDIAN()){ // If native system is little endian
			if(Endianess::BE == e1 || Endianess::BE_BS == e1)
				accumulatedEndianessDiff = toggleEndianess(accumulatedEndianessDiff);
			if(Endianess::BE == e2 || Endianess::BE_BS == e2)
				accumulatedEndianessDiff = toggleEndianess(accumulatedEndianessDiff);
		} else { // If native system is big endian
			if(Endianess::LE == e1 || Endianess::LE_BS == e1)
				accumulatedEndianessDiff = toggleEndianess(accumulatedEndianessDiff);
			if(Endianess::LE == e2 || Endianess::LE_BS == e2)
				accumulatedEndianessDiff = toggleEndianess(accumulatedEndianessDiff);
		}
		
		return fixEndianIfNative(accumulatedEndianessDiff);
	}
	
	inline uint16_t swapbytes16(uint16_t x){
		return (uint16_t)(((uint16_t)(x) & 0xffU) << 8 | ((uint16_t)(x) & 0xff00U) >> 8);
	}
	inline uint32_t swapbytes32(uint32_t x){
		return (uint32_t)(((uint32_t)(x) & 0xff) << 24 | ((uint32_t)(x) & 0xff00) << 8 | ((uint32_t)(x) & 0xff0000) >> 8 | ((uint32_t)(x) & 0xff000000) >> 24);
	}
	inline uint64_t swapbytes64(uint64_t x){
		return (uint64_t)((((uint64_t)(x) & 0xff) << 56) | ((uint64_t)(x) & 0xff00ULL) << 40 |((uint64_t)(x) & 0xff0000ULL) << 24 |((uint64_t)(x) & 0xff000000ULL) << 8 |((uint64_t)(x) & 0xff00000000ULL) >> 8 |((uint64_t)(x) & 0xff0000000000ULL) >> 24 | ((uint64_t)(x) & 0xff000000000000ULL) >> 40 | ((uint64_t)(x) & 0xff00000000000000ULL) >> 56);
	}


	inline uint8_t  NativeToBigEndian8 (uint8_t  x){
		return ((uint8_t)(x));
	}
	inline uint16_t NativeToBigEndian16(uint16_t x){
		return (IS_LITTLE_ENDIAN())?swapbytes16(x):((uint16_t)(x));
	}
	inline uint32_t NativeToBigEndian32(uint32_t x){
		return (IS_LITTLE_ENDIAN())?swapbytes32(x):((uint32_t)(x));
	}
	inline uint64_t NativeToBigEndian64(uint64_t x){
		return (IS_LITTLE_ENDIAN())?swapbytes64(x):((uint64_t)(x));
	}
	
	inline uint8_t  NativeToLittleEndian8 (uint8_t  x){
		return ((uint8_t)(x));
	}
	inline uint16_t NativeToLittleEndian16(uint16_t x){
		return (IS_LITTLE_ENDIAN())?((uint16_t)(x)):swapbytes16(x);
	}
	inline uint32_t NativeToLittleEndian32(uint32_t x){
		return (IS_LITTLE_ENDIAN())?((uint32_t)(x)):swapbytes32(x);
	}
	inline uint64_t NativeToLittleEndian64(uint64_t x){
		return (IS_LITTLE_ENDIAN())?((uint64_t)(x)):swapbytes64(x);
	}
	
	inline uint8_t  BigEndianToNative8(uint8_t  x){
		return ((uint8_t)(x));
	}
	inline uint16_t BigEndianToNative16(uint16_t x){
		return NativeToBigEndian16(x);
	} 
	inline uint32_t BigEndianToNative32(uint32_t x){
		return NativeToBigEndian32(x);
	} 
	inline uint64_t BigEndianToNative64(uint64_t x){
		return NativeToBigEndian64(x);
	}
	inline uint8_t  LittleEndianToNative8(uint8_t  x){
		return ((uint8_t)(x));
	}
	inline uint16_t LittleEndianToNative16(uint16_t x){
		return NativeToLittleEndian16(x);
	} 
	inline uint32_t LittleEndianToNative32(uint32_t x){
		return NativeToLittleEndian32(x);
	} 
	inline uint64_t LittleEndianToNative64(uint64_t x){
		return NativeToLittleEndian64(x);
	} 

	//BitSwaps: swap bit order within the bytes
	inline uint8_t  BitSwap8(uint8_t  x){
		return (((x & 0x80)>>7) | ((x & 0x40)>>5) | ((x & 0x20)>>3) | ((x & 0x10)>>1) | ((x & 0x08)<<1) | ((x & 0x04)<<3) | ((x & 0x02)<<5) | ((x & 0x01)<<7));
	}
	inline uint16_t BitSwap16(uint16_t x){
		return (((x & 0x8080)>>7) | ((x & 0x4040)>>5) | ((x & 0x2020)>>3) | ((x & 0x1010)>>1) | ((x & 0x0808)<<1) | ((x & 0x0404)<<3) | ((x & 0x0202)<<5) | ((x & 0x0101)<<7));
	} 
	inline uint32_t BitSwap32(uint32_t x){
		return (((x & 0x80808080)>>7) | ((x & 0x40404040)>>5) | ((x & 0x20202020)>>3) | ((x & 0x10101010)>>1) | ((x & 0x08080808)<<1) | ((x & 0x04040404)<<3) | ((x & 0x02020202)<<5) | ((x & 0x01010101)<<7));
	} 
	inline uint64_t BitSwap64(uint64_t x){
		return (((x & 0x8080808080808080)>>7) | ((x & 0x4040404040404040)>>5) | ((x & 0x2020202020202020)>>3) | ((x & 0x1010101010101010)>>1) | ((x & 0x0808080808080808)<<1) | ((x & 0x0404040404040404)<<3) | ((x & 0x0202020202020202)<<5) | ((x & 0x0101010101010101)<<7));
	}

	inline uint8_t  NativeToAnyEndianess8(uint8_t  x, Endianess e){
		return ((uint8_t)(x));
	}
	inline uint16_t NativeToAnyEndianess16(uint16_t x, Endianess e){
		if(Endianess::BE == e)
			return NativeToBigEndian16(x);
		if(Endianess::LE == e)
			return NativeToLittleEndian16(x);
		if(Endianess::BE_BS == e)
			return BitSwap16(NativeToBigEndian16(x));
		if(Endianess::LE_BS == e)
			return BitSwap16(NativeToLittleEndian16(x));
		//Native then
		return ((uint16_t)(x));
	} 
	inline uint32_t NativeToAnyEndianess32(uint32_t x, Endianess e){
		if(Endianess::BE == e)
			return NativeToBigEndian32(x);
		if(Endianess::LE == e)
			return NativeToLittleEndian32(x);
		if(Endianess::BE_BS == e)
			return BitSwap32(NativeToBigEndian32(x));
		if(Endianess::LE_BS == e)
			return BitSwap32(NativeToLittleEndian32(x));
		//Native then
		return ((uint32_t)(x));
	} 
	inline uint64_t NativeToAnyEndianess64(uint64_t x, Endianess e){
		if(Endianess::BE == e)
			return NativeToBigEndian64(x);
		if(Endianess::LE == e)
			return NativeToLittleEndian64(x);
		if(Endianess::BE_BS == e)
			return BitSwap64(NativeToBigEndian64(x));
		if(Endianess::LE_BS == e)
			return BitSwap64(NativeToLittleEndian64(x));
		//Native then
		return ((uint64_t)(x));
	} 
}
#endif //ENDIANESS_H
