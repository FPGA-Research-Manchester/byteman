/******************************************************************************
 * Copyright 2022 Kristiyan Manev, Joseph Powell (University of Manchester)
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

#include "../XilinxConfigurationAccessPort.h"

#ifndef XS6_FABRIC_H
	#define XS6_FABRIC_H

	//
	#define XS6_MAX_SLRS 0

	//
	#define XS6_MAX_ROWS 0

	//
	#define XS6_MAX_COLS 0

	//
	#define XS6_MAX_BRAM_COLS 0
	
	#define XS6_WORDS_BEFORE_CLK 40
	#define XS6_WORDS_AT_CLK 1
	#define XS6_WORDS_AFTER_CLK 40
	#define XS6_WORDS_PER_FRAME (XS6_WORDS_BEFORE_CLK + XS6_WORDS_AT_CLK + XS6_WORDS_AFTER_CLK)
	#define XS6_FRAMES_PER_BRAM_CONTENT_COLUMN 128
	#define XS6_CLB_PER_CLOCK_REGION 40
	#define XS6_BRAM_PER_CLOCK_REGION 10
	#define XS6_EXTRA_FRAMES_PER_ROW 2
	
	#define XS6_TOPBOTTOM_SHIFT 20
	
	#define XS6_BLOCKTYPE_SHIFT 21
	#define XS6_BLOCKTYPE_MASK 0x7
	#define XS6_ROWADDRESS_SHIFT 15
	#define XS6_ROWADDRESS_MASK 0x1F
	#define XS6_COLUMNADDRESS_SHIFT 7
	#define XS6_COLUMNADDRESS_MASK 0xFF
	#define XS6_MINORADDRESS_SHIFT 0
	#define XS6_MINORADDRESS_MASK 0x7F
	
	#define archXS6template FABRIC_TEMPLATE(XS6_MAX_SLRS, XS6_MAX_ROWS, XS6_MAX_COLS, XS6_MAX_BRAM_COLS, XS6_BLOCKTYPE_LOGIC, XS6_BLOCKTYPE_BLOCKRAM, XS6_WORDS_BEFORE_CLK, XS6_WORDS_AT_CLK, XS6_WORDS_AFTER_CLK, XS6_WORDS_PER_FRAME, XS6_FRAMES_PER_BRAM_CONTENT_COLUMN, XS6_CLB_PER_CLOCK_REGION, XS6_BRAM_PER_CLOCK_REGION, XS6_EXTRA_FRAMES_PER_ROW, XS6_TOPBOTTOM_SHIFT, XS6_BLOCKTYPE_SHIFT, XS6_BLOCKTYPE_MASK, XS6_ROWADDRESS_SHIFT, XS6_ROWADDRESS_MASK, XS6_COLUMNADDRESS_SHIFT, XS6_COLUMNADDRESS_MASK, XS6_MINORADDRESS_SHIFT, XS6_MINORADDRESS_MASK)

#endif//XS6_FABRIC_H
