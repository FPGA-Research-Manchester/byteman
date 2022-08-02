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

#ifndef XS7_FABRIC_H
	#define XS7_FABRIC_H
	
	#define XS7_MAX_SLRS 5

	#define XS7_MAX_ROWS 12

	//max is 286 columns
	#define XS7_MAX_COLS 512

	//max is 17
	#define XS7_MAX_BRAM_COLS 32
	
	
	#define XS7_BLOCKTYPE_MAX 2
	#define XS7_BLOCKTYPE_LOGIC 0
	#define XS7_BLOCKTYPE_BLOCKRAM 1
	

	#define XS7_WORDS_BEFORE_CLK 50
	#define XS7_WORDS_AT_CLK 1
	#define XS7_WORDS_AFTER_CLK 50
	#define XS7_WORDS_PER_FRAME (XS7_WORDS_BEFORE_CLK + XS7_WORDS_AT_CLK + XS7_WORDS_AFTER_CLK)
	#define XS7_FRAMES_PER_BRAM_CONTENT_COLUMN 128
	#define XS7_CLB_PER_CLOCK_REGION 50
	#define XS7_BRAM_PER_CLOCK_REGION 10
	
	#define XS7_EXTRA_FRAMES_PER_ROW 2
	
	#define XS7_USESTOPBOTTOM 1
	#define XS7_TOPBOTTOM_SHIFT 22
	
	#define XS7_BLOCKTYPE_SHIFT 23
	#define XS7_BLOCKTYPE_MASK 0x7
	#define XS7_ROWADDRESS_SHIFT 17
	#define XS7_ROWADDRESS_MASK 0x1F
	#define XS7_COLUMNADDRESS_SHIFT 7
	#define XS7_COLUMNADDRESS_MASK 0x3FF
	#define XS7_MINORADDRESS_SHIFT 0
	#define XS7_MINORADDRESS_MASK 0x7F
	
	#define archXS7template FABRIC_TEMPLATE(XS7_MAX_SLRS, XS7_MAX_ROWS, XS7_MAX_COLS, XS7_MAX_BRAM_COLS, XS7_BLOCKTYPE_MAX, XS7_BLOCKTYPE_LOGIC, XS7_BLOCKTYPE_BLOCKRAM, XS7_WORDS_BEFORE_CLK, XS7_WORDS_AT_CLK, XS7_WORDS_AFTER_CLK, XS7_WORDS_PER_FRAME, XS7_FRAMES_PER_BRAM_CONTENT_COLUMN, XS7_CLB_PER_CLOCK_REGION, XS7_BRAM_PER_CLOCK_REGION, XS7_EXTRA_FRAMES_PER_ROW, XS7_USESTOPBOTTOM, XS7_TOPBOTTOM_SHIFT, XS7_BLOCKTYPE_SHIFT, XS7_BLOCKTYPE_MASK, XS7_ROWADDRESS_SHIFT, XS7_ROWADDRESS_MASK, XS7_COLUMNADDRESS_SHIFT, XS7_COLUMNADDRESS_MASK, XS7_MINORADDRESS_SHIFT, XS7_MINORADDRESS_MASK)

#endif//S7_FABRIC_H
