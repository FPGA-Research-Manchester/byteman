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

#define swapbytes16(x) (uint16_t)(((uint16_t)(x) & 0xffU) << 8 | ((uint16_t)(x) & 0xff00U) >> 8)
#define swapbytes32(x) (uint32_t)(((uint32_t)(x) & 0xff) << 24 | ((uint32_t)(x) & 0xff00) << 8 | ((uint32_t)(x) & 0xff0000) >> 8 | ((uint32_t)(x) & 0xff000000) >> 24)
#define swapbytes64(x) (uint64_t)((((uint64_t)(x) & 0xff) << 56) | ((uint64_t)(x) & 0xff00ULL) << 40 |((uint64_t)(x) & 0xff0000ULL) << 24 |((uint64_t)(x) & 0xff000000ULL) << 8 |((uint64_t)(x) & 0xff00000000ULL) >> 8 |((uint64_t)(x) & 0xff0000000000ULL) >> 24 | ((uint64_t)(x) & 0xff000000000000ULL) >> 40 | ((uint64_t)(x) & 0xff00000000000000ULL) >> 56)

#ifndef htobe8
#define htobe8(x) ((uint8_t)(x))
#endif
#ifndef htobe16
#define htobe16(x) (IS_LITTLE_ENDIAN())?swapbytes16(x):((uint16_t)(x))
#endif
#ifndef htobe32
#define htobe32(x) (IS_LITTLE_ENDIAN())?swapbytes32(x):((uint32_t)(x))
#endif
#ifndef htobe64
#define htobe64(x) (IS_LITTLE_ENDIAN())?swapbytes64(x):((uint64_t)(x))
#endif
#ifndef htole8
#define htole8(x) ((uint8_t)(x))
#endif
#ifndef htole16
#define htole16(x) (IS_LITTLE_ENDIAN())?((uint16_t)(x)):swapbytes16(x)
#endif
#ifndef htole32
#define htole32(x) (IS_LITTLE_ENDIAN())?((uint32_t)(x)):swapbytes32(x)
#endif
#ifndef htole64
#define htole64(x) (IS_LITTLE_ENDIAN())?((uint64_t)(x)):swapbytes64(x)
#endif
