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
	
	#define XUSP_BT_S 24
	#define XUSP_BT_M 0x7
	#define XUSP_RA_S 18
	#define XUSP_RA_M 0x3F
	#define XUSP_CA_S 8
	#define XUSP_CA_M 0x3FF
	#define XUSP_MA_S 0
	#define XUSP_MA_M 0xFF
	
	#define archXUSPtemplate FABRIC_TEMPLATE(XUSP_BLOCKTYPE_LOGIC, XUSP_BLOCKTYPE_BLOCKRAM,XUSP_WORDS_BEFORE_CLK, XUSP_WORDS_AT_CLK, XUSP_WORDS_AFTER_CLK, XUSP_WORDS_PER_FRAME,XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN, XUSP_CLB_PER_CLOCK_REGION, XUSP_BRAM_PER_CLOCK_REGION, XUSP_EXTRA_FRAMES_PER_ROW, XUSP_BT_S, XUSP_BT_M, XUSP_RA_S, XUSP_RA_M, XUSP_CA_S, XUSP_CA_M, XUSP_MA_S, XUSP_MA_M)

#endif//XUSP_FABRIC_H