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

#ifndef XUSP_FABRIC_H
	#define XUSP_FABRIC_H

	#define XUSP_MAX_SLRS 4

	#define XUSP_MAX_ROWS 20

	//max 692 cols in UltraScale+
	#define XUSP_MAX_COLS 1024

	//max 12 bram cols in UltraScale+
	#define XUSP_MAX_BRAM_COLS 16
	
	#define XUSP_BLOCKTYPE_MAX 2
	#define XUSP_BLOCKTYPE_LOGIC 0
	#define XUSP_BLOCKTYPE_BLOCKRAM 1
	
	#define XUSP_WORDS_BEFORE_CLK 45
	#define XUSP_WORDS_AT_CLK 3
	#define XUSP_WORDS_AFTER_CLK 45
	#define XUSP_WORDS_PER_FRAME (XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK+XUSP_WORDS_AFTER_CLK)
	#define XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN 256
	#define XUSP_CLB_PER_CLOCK_REGION 60
	#define XUSP_BRAM_PER_CLOCK_REGION 12
	
	#define XUSP_EXTRA_FRAMES_PER_ROW 2
	
	#define XUSP_USESTOPBOTTOM 0
	#define XUSP_TOPBOTTOM_SHIFT 0
	
	#define XUSP_BLOCKTYPE_SHIFT 24
	#define XUSP_BLOCKTYPE_MASK 0x7
	#define XUSP_ROWADDRESS_SHIFT 18
	#define XUSP_ROWADDRESS_MASK 0x3F
	#define XUSP_COLUMNADDRESS_SHIFT 8
	#define XUSP_COLUMNADDRESS_MASK 0x3FF
	#define XUSP_MINORADDRESS_SHIFT 0
	#define XUSP_MINORADDRESS_MASK 0xFF
	
	#define archXUSPtemplate FABRIC_TEMPLATE(XUSP_MAX_SLRS, XUSP_MAX_ROWS, XUSP_MAX_COLS, XUSP_MAX_BRAM_COLS, XUSP_BLOCKTYPE_MAX, XUSP_BLOCKTYPE_LOGIC, XUSP_BLOCKTYPE_BLOCKRAM,XUSP_WORDS_BEFORE_CLK, XUSP_WORDS_AT_CLK, XUSP_WORDS_AFTER_CLK, XUSP_WORDS_PER_FRAME,XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN, XUSP_CLB_PER_CLOCK_REGION, XUSP_BRAM_PER_CLOCK_REGION, XUSP_EXTRA_FRAMES_PER_ROW, XUSP_USESTOPBOTTOM, XUSP_TOPBOTTOM_SHIFT, XUSP_BLOCKTYPE_SHIFT, XUSP_BLOCKTYPE_MASK, XUSP_ROWADDRESS_SHIFT, XUSP_ROWADDRESS_MASK, XUSP_COLUMNADDRESS_SHIFT, XUSP_COLUMNADDRESS_MASK, XUSP_MINORADDRESS_SHIFT, XUSP_MINORADDRESS_MASK)

#endif//XUSP_FABRIC_H