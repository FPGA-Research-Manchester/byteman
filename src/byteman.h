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
 * - Xilinx UltraScale+ (XilinxUltraScalePlus, which implements a CommonDevice2D and XilinxConfigurationAccessPort)
 * 
 *****************************************************************************/

#ifndef BYTEMAN_H
#define BYTEMAN_H

#include<iostream>
#include<string>

#ifdef XUSP
	#include "Devices/Xilinx/UltraScalePlus/XilinxUltraScalePlus.h"
#endif

using namespace std;

//Any changes to version numbers should be done only in CMakeLists.txt, not here!
#define VERSION "0.6"
#define VERSION_BUILD "124"

class byteman
{
	public:
		byteman(){};
		virtual ~byteman(){};
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

		void init();
		
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
		void helpEnd();
		void helpStdin();
};


#endif //BYTEMAN_H
