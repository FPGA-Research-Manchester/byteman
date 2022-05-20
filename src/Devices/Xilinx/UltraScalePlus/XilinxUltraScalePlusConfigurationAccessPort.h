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
 
#ifndef XILINXULTRASCALEPLUSCONFIGURATIONACCESSPORT_H
#define XILINXULTRASCALEPLUSCONFIGURATIONACCESSPORT_H
#include<cstdint> //uint
#include "XilinxUltraScalePlus.h"
namespace CAP {
	enum class BlockType{
		LOGIC = 0,
		BLOCKRAM = 1
	};
	
	enum class Register{
		UNDEFINED = -1,
		CRC = 0,
		FAR = 1,
		FDRI = 2,
		FDRO = 3,
		CMD = 4,
		CTRL0 = 5,
		MASK = 6,
		STAT = 7,
		LOUT = 8,
		COR0 = 9,
		MFWR = 10,
		CBC = 11,
		IDCODE = 12,
		AXSS = 13,
		COR1 = 14,
		WBSTAR = 16,
		TIMER = 17,
		MAGIC0 = 19,
		BOOTSTS = 22,
		CTRL1 = 24,
		MAGIC1 = 30,
		BSPI = 31
	};
	
	enum class Command{
		UNDEFINED = -1,
		NULLCMD = 0,
		WCFG = 1,
		MFW = 2,
		DGHIGH = 3,
		RCFG = 4,
		START = 5,
		URAM = 6,
		RCRC = 7,
		AGHIGH = 8,
		SWITCH = 9,
		GRESTORE = 10,
		SHUTDOWN = 11,
		DESYNC = 13,
		IPROG = 15,
		CRCC = 16,
		LTIMER = 17,
		BSPI_READ = 18,
		FALL_EDGE = 19
	};
	
	enum class Operation{
		NOP = 0,
		READ = 1,
		WRITE = 2,
		RESERVED = 3
	};
	
	
	const uint32_t SYNC = 0xAA995566;
}

#endif //XILINXULTRASCALEPLUSCONFIGURATIONACCESSPORT_H
