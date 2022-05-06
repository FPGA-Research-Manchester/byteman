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
#include <iostream>
#include "byteman.h"
#include <sstream>
#include <stdexcept>
using namespace std;
//TODO prettify all this.
void parseHelp(string args, int shouldExit, int exitCode){
	stringstream ss(args);
	string word;
	ss >> word;
	if(ss.eof())
		usage();
    else
		usageArg(args);
	if(shouldExit)
		exit(exitCode);
}
void usage(){
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
	cout << "\tbyteman.exe Xilinx US+ -w -i main static.bit -i temp filter.bit -merge logic 180 67 120 30 240 37 -o logic 180 67 120 30 relocatedFilter.bit" << endl;
	cout << "\tbyteman.exe Xilinx US+ -w -d zcu102 -blank main logic blockram -i temp filter.bit -merge logic 180 67 120 30 240 37 -o logic 180 67 120 30 relocatedFilter.bit" << endl;
	
	
}
void usageArg(string args){
	stringstream ss(args);
	string word;
	ss >> word;
    while (!ss.eof()) {
        ss >> word;
	#define argIs(x,y) (word == x || word == y)
	if(argIs("v", "verbose"))
		usage_verbose();
	else if(argIs("w", "warn"))
		usage_warn();
	else if(argIs("r", "region"))
		usage_region();
	else if(argIs("b", "blank"))
		usage_blank();
	else if(argIs("c", "change"))
		usage_change();
	else if(argIs("d", "device"))
		usage_device();
	else if(argIs("h", "help"))
		usage_help();
	else if(argIs("i", "input"))
		usage_input();
	else if(argIs("m", "merge"))
		usage_merge();
	else if(argIs("o", "output"))
		usage_output();
	else if(argIs("a", "assembly"))
		usage_assembly();
	else if(argIs("e", "end"))
		usage_end();
	else if(argIs("s", "stdin"))
		usage_stdin();
	#undef argIs
    }
}
void usage_verbose(){
	cout<<"TODO usage_verbose" << endl;
}
void usage_warn(){
	cout<<"TODO usage_warn" << endl;
}
void usage_region(){
	cout<<"TODO usage_region" << endl;
	cout <<"Region params: main/first, second/temp, add,clear" << endl;
}
void usage_blank(){
	cout<<"TODO usage_blank" << endl;
}
void usage_change(){
	cout<<"TODO usage_change" << endl;
}
void usage_device(){
	cout<<"TODO usage_device" << endl;
	
	cout << "Available devices:" <<endl;
	ArchDeviceHelp();
}
void usage_help(){
	cout<<"TODO usage_help" << endl;
	cout<<"headache" << endl;
}
void usage_input(){
	cout<<"TODO usage_input" << endl;
	cout <<"Input params: main/first, second/temp" << endl;
}
void usage_merge(){
	cout<<"TODO usage_merge" << endl;
	cout <<"Merge params: clk/clock, clb/logic, bram, uram, set, xor, or, and" << endl;
}
void usage_output(){
	cout<<"TODO usage_output" << endl;
	cout <<"Output params: main/first, second/temp, clb, bram, blank, full, partial" << endl;
}
void usage_assembly(){
	cout<<"TODO usage_assembly" << endl;
	ArchAssemblyHelp();
}
void usage_end(){
	cout<<"TODO usage_end" << endl;
}
void usage_stdin(){
	cout << "\t-stdin                    : Continue using the same commands on standard input." << endl;
	cout << "\t                            Standard input ends with a command \"end\"." << endl;
	cout << "\t                            Stdin commands don’t need \"-\" in front and can be on multiple lines." << endl;
	cout << "\t                            \"#\" starts a comment until a newline is seen." << endl;
}