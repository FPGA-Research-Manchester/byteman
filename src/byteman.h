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

/**************************************************************************//**
 * @mainpage
 * byteman is a high-throughput bitstream relocation and manipulation tool.
 * 
 * 
 * A good starting point to explore the code is byteman.cpp where the main()
 * function parses the command line arguments.
 *
 * A list of currently implemented FPGA architectures:
 * - Xilinx Series 7 (XilinxSeries7, which implements a CommonDevice2D and XilinxConfigurationAccessPort)
 * - Xilinx UltraScale (XilinxUltraScale, which implements a CommonDevice2D and XilinxConfigurationAccessPort)
 * - Xilinx UltraScale+ (XilinxUltraScalePlus, which implements a CommonDevice2D and XilinxConfigurationAccessPort)
 * 
 *****************************************************************************/

#ifndef BYTEMAN_H
#define BYTEMAN_H

#include<iostream>
#include<string>

#ifdef XS7
	#include "Devices/Xilinx/Series7/XilinxSeries7.h"
#endif

#ifdef XUS
	#include "Devices/Xilinx/UltraScale/XilinxUltraScale.h"
#endif

#ifdef XUSP
	#include "Devices/Xilinx/UltraScalePlus/XilinxUltraScalePlus.h"
#endif

using namespace std;

class byteman
{
	public:
		byteman();
		virtual ~byteman();
		#ifdef XS7
			XilinxSeries7 mainXS7, tempXS7;
		#endif //XS7
		
		#ifdef XUS
			XilinxUltraScale mainXUS, tempXUS;
		#endif //XUS
		
		#ifdef XUSP
			XilinxUltraScalePlus mainXUSP, tempXUSP;
		#endif //XUSP
		
		enum class Architecture{
			Unknown,
			Xilinx_Series6,
			Xilinx_Series7,
			Xilinx_UltraScale,
			Xilinx_UltraScalePlus,
			Xilinx_Versal
		} selectedArchitecture = Architecture::Unknown;
		struct SelectedOptions
		{
			int mainBufferSelected, tempBufferSelected;
			SelectedOptions():mainBufferSelected(0),tempBufferSelected(0){}
		};
		//parse
		SelectedOptions parseParams(string);
		
		void parse(string);
		void setArchitecture(string);
		void parseAssembly(string, SelectedOptions);
		void parseOutput(string, SelectedOptions);
		void parseMerge(string, SelectedOptions);
		void parseInput(string, SelectedOptions);
		void parseDevice(string, SelectedOptions);
		void parseChange(string, SelectedOptions);
		void parseBlank(string, SelectedOptions);
		void parseRegion(string, SelectedOptions);
		void parseWarn(string, SelectedOptions);
		void parseVerbose(string, SelectedOptions);
		
		
		void help(string, int);
		void help(string);
		void help();

		void helpAssembly();
		void helpOutput();
		void helpMerge();
		void helpInput();
		void helpDevice();
		void helpChange();
		void helpBlank();
		void helpRegion();
		void helpWarn();
		void helpVerbose();
		void helpHelp();
		void helpExit();
		void helpStdin();
		
		#if !defined(NDEBUG)
		void parseTest(string, SelectedOptions);
		#endif
};


#endif //BYTEMAN_H
