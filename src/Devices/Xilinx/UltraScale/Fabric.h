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

#ifndef XUS_FABRIC_H
	#define XUS_FABRIC_H
	
	#define XUS_BLOCKTYPE_LOGIC 0
	#define XUS_BLOCKTYPE_BLOCKRAM 1
	
	#define XUS_WORDS_BEFORE_CLK 60
	#define XUS_WORDS_AT_CLK 3
	#define XUS_WORDS_AFTER_CLK 60
	#define XUS_WORDS_PER_FRAME (XUS_WORDS_BEFORE_CLK+XUS_WORDS_AT_CLK+XUS_WORDS_AFTER_CLK)
	#define XUS_FRAMES_PER_BRAM_CONTENT_COLUMN 128
	#define XUS_CLB_PER_CLOCK_REGION 60
	#define XUS_BRAM_PER_CLOCK_REGION 12
	#define XUS_EXTRA_FRAMES_PER_ROW 2
	
	#define XUS_BT_S 23
	#define XUS_BT_M 0x7
	#define XUS_RA_S 17
	#define XUS_RA_M 0x3F
	#define XUS_CA_S 7
	#define XUS_CA_M 0x3FF
	#define XUS_MA_S 0
	#define XUS_MA_M 0x7F
	
	#define archXUStemplate FABRIC_TEMPLATE(XUS_BLOCKTYPE_LOGIC, XUS_BLOCKTYPE_BLOCKRAM,XUS_WORDS_BEFORE_CLK, XUS_WORDS_AT_CLK, XUS_WORDS_AFTER_CLK, XUS_WORDS_PER_FRAME,XUS_FRAMES_PER_BRAM_CONTENT_COLUMN, XUS_CLB_PER_CLOCK_REGION, XUS_BRAM_PER_CLOCK_REGION, XUS_EXTRA_FRAMES_PER_ROW, XUS_BT_S, XUS_BT_M, XUS_RA_S, XUS_RA_M, XUS_CA_S, XUS_CA_M, XUS_MA_S, XUS_MA_M)

#endif//XUS_FABRIC_H