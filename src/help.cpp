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

#include<iostream>
#include<sstream>
#include<stdexcept>

#include "byteman.h"
#include "version.h"
#include "Common/str.h"

using namespace std;
#ifdef _WIN32
	#define EXECUTABLE "byteman.exe"
#else
	#define EXECUTABLE "byteman"
#endif
void byteman::help(string param, int exitCode)
{
	if(param.empty())
		byteman::help();
	else
		byteman::help(param);
	exit(exitCode);
}
void byteman::help()
{
	cout << "byteman " << VERSION << " (Build " << VERSION_BUILD << ") usage:" << endl;
	cout << "byteman ARCH [-command...]* [-stdin]" << endl;
	cout << "\tCommands:    [Default] : [Description]                                    " << endl;
	cout << "\t-a (assembly)          : (de)compile bitstream assembly files. See \"byteman -h assembly\"" << endl;
	cout << "\t-b (blank)             : Erases bitstream buffers. See \"byteman -h blank\"" << endl;
	cout << "\t-c (change)            : Modify primitives (i.e., LUT contents). See \"byteman -h change\"" << endl;
	cout << "\t-d (device)    NULL    : Manually select target device. See \"byteman -h device\"" << endl;
	cout << "\t-e (end)               : Exit byteman. Useful during stdin. See \"byteman -h end\"" << endl;
	cout << "\t-i (input)             : Input bitstream. See \"byteman -h input\"" << endl;
	cout << "\t-m (merge)             : Merge bitstreams. See \"byteman -h merge\"" << endl;
	cout << "\t-r (region)            : Modify region selection. See \"byteman -h region\"" << endl;
	cout << "\t-o (output)            : Output bitstream. See \"byteman -h output\"" << endl;
	cout << "\t-h (help)              : Get help. See \"byteman -h help\"" << endl;
	cout << "\t-v (verbose)   0       : Enable/disable spam. See \"byteman -h verbose\"" << endl;
	cout << "\t-w (warn)      1       : Enable/disable warnings. See \"byteman -h warn\"" << endl;
	cout << "\t-stdin                 : Continue using standard input. See \"byteman -h stdin\"" << endl;

	cout << "Examples: " << endl;
	cout << "\t" EXECUTABLE " Xilinx US+ -w -i main static.bit -i temp filter.bit -merge logic 180:67 120:30 240:37 -o logic 240:37 120:30 relocatedFilter.bit" << endl;
	cout << "\t" EXECUTABLE " Xilinx US+ -w -d zcu102 -blank main logic blockram -i temp filter.bit -merge logic 180:67 120:30 180:67 -o logic 180:67 120:30 mergedFilter.bit" << endl;
}
void byteman::help(string param)
{
		 if(str::iff::stringIs(param, "v", "verbose"))
		byteman::helpVerbose();
	else if(str::iff::stringIs(param, "w", "warn"))
		byteman::helpWarn();
	else if(str::iff::stringIs(param, "r", "region"))
		byteman::helpRegion();
	else if(str::iff::stringIs(param, "b", "blank"))
		byteman::helpBlank();
	else if(str::iff::stringIs(param, "c", "change"))
		byteman::helpChange();
	else if(str::iff::stringIs(param, "d", "device"))
		byteman::helpDevice();
	else if(str::iff::stringIs(param, "h", "help"))
		byteman::helpHelp();
	else if(str::iff::stringIs(param, "i", "input"))
		byteman::helpInput();
	else if(str::iff::stringIs(param, "m", "merge"))
		byteman::helpMerge();
	else if(str::iff::stringIs(param, "o", "output"))
		byteman::helpOutput();
	else if(str::iff::stringIs(param, "a", "assembly"))
		byteman::helpAssembly();
	else if(str::iff::stringIs(param, "e", "end"))
		byteman::helpEnd();
	else if(str::iff::stringIs(param, "s", "stdin"))
		byteman::helpStdin();
}
void byteman::helpVerbose()
{
	cout<<"TODO usage_verbose" << endl;
}
void byteman::helpWarn()
{
	cout<<"TODO usage_warn" << endl;
}
void byteman::helpRegion()
{
	cout<<"TODO usage_region" << endl;
	cout <<"Region params: main/first, second/temp, add,clear" << endl;
}
void byteman::helpBlank()
{
	cout<<"TODO usage_blank" << endl;
}
void byteman::helpChange()
{
	cout<<"TODO usage_change" << endl;
}
void byteman::helpDevice()
{
	cout<<"TODO helpDevice" << endl;
	
	cout << "Available devices:" <<endl;
	#ifdef XS7
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_Series7 == selectedArchitecture)
			XilinxSeries7::deviceHelp();
	#endif
	#ifdef XUS
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_UltraScale == selectedArchitecture)
			XilinxUltraScale::deviceHelp();
	#endif
	#ifdef XUSP
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_UltraScalePlus == selectedArchitecture)
			XilinxUltraScalePlus::deviceHelp();
	#endif
}
void byteman::helpHelp()
{
	cout<<"TODO helpHelp" << endl;
	cout<<"headache" << endl;
}
void byteman::helpInput()
{
	cout<<"TODO usage_input" << endl;
	cout <<"Input params: main/first, second/temp" << endl;
}
void byteman::helpMerge()
{
	cout<<"TODO usage_merge" << endl;
	cout <<"Merge params: clk/clock, clb/logic, bram, uram, set, xor, or, and" << endl;
}
void byteman::helpOutput()
{
	cout<<"TODO usage_output" << endl;
	cout <<"Output params: main/first, second/temp, clb, bram, blank, full, partial" << endl;
}
void byteman::helpAssembly()
{
	cout<<"TODO usage_assembly" << endl;
	#ifdef XS7
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_Series7 == selectedArchitecture)
			XilinxSeries7::assemblerHelp();
	#endif
	#ifdef XUS
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_UltraScale == selectedArchitecture)
			XilinxUltraScale::assemblerHelp();
	#endif
	#ifdef XUSP
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_UltraScalePlus == selectedArchitecture)
			XilinxUltraScalePlus::assemblerHelp();
	#endif
}
void byteman::helpEnd()
{
	cout<<"TODO usage_end" << endl;
}
void byteman::helpStdin()
{
	cout << "\t-stdin					: Continue using the same commands on standard input." << endl;
	cout << "\t							Standard input ends with a command \"end\"." << endl;
	cout << "\t							Stdin commands don’t need \"-\" in front and can be on multiple lines." << endl;
	cout << "\t							\"#\" starts a comment until a newline is seen." << endl;
}