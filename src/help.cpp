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
#include "../VERSION"
#include "Common/str.h"

using namespace std;
#ifdef _WIN32
	#define EXECUTABLE "byteman.exe"
#else
	#define EXECUTABLE "byteman"
#endif
void byteman::help(string param, int exitCode)
{
	cout << "byteman " << VERSION << " (Build #" << VERSION_BUILD << ", compiled on " << __DATE__ << " " << __TIME__ << ")" << endl;
	if(param.empty())
		byteman::help();
	else
		byteman::help(param);
	exit(exitCode);
}
void byteman::help()
{
	cout << "Usage:" << endl;
	cout << "  " EXECUTABLE " ARCH [-command...]* [-stdin]" << endl;
	cout << endl;
	cout << "  ARCH = {\"Xilinx Series 7\", \"Xilinx UltraScale\", \"Xilinx UltraScale+\"}" << endl;
	cout << endl;
	cout << "Commands:        : [Description]                                    " << endl;
	cout << "  -a (Assembly)  : (de)assemble bitstreams.       see \"-help assembly\"" << endl;
	cout << "  -b (Blank)     : erases bitstream buffers.      see \"-help blank\"" << endl;
	cout << "  -c (Change)    : modify primitives.             see \"-help change\"" << endl;
	cout << "  -d (Device)    : manually select target device. see \"-help device\"" << endl;
	cout << "  -e (Exit)      : exit byteman. used with stdin. see \"-help exit\"" << endl;
	cout << "  -h (Help)      : get help.                      see \"-help help\"" << endl;
	cout << "  -i (Input)     : input bitstream.               see \"-help input\"" << endl;
	cout << "  -m (Merge)     : merge bitstreams.              see \"-help merge\"" << endl;
	cout << "  -o (Output)    : output bitstream.              see \"-help output\"" << endl;
	cout << "  -r (Region)    : modify region selection.       see \"-help region\"" << endl;
	cout << "  -s (Stdin)     : continue using standard input. see \"-help stdin\"" << endl;
	cout << "  -v (Verbose)   : enable/disable spam.           see \"-help verbose\"" << endl;
	cout << "  -w (Warn)      : enable/disable warnings.       see \"-help warn\"" << endl;
	cout << endl;
	cout << "Examples: " << endl;
	cout << "  " EXECUTABLE " Xilinx US+ -w -i main static.bit -i temp filter.bit -merge logic 180:67 120:30 240:37 -o logic 240:37 120:30 relocatedFilter.bit" << endl;
	cout << "  " EXECUTABLE " XUSP -w -d zcu102 -blank main logic blockram -i temp filter.bit -merge logic 180:67 120:30 180:67 -o logic 180:67 120:30 mergedFilter.bit" << endl;
	cout << endl;
	cout << "Need more help? Try one of these:" << endl;
	cout << "  " EXECUTABLE " -help <command>" << endl;
	cout << "  " EXECUTABLE " -help all" << endl;
	cout << "  " EXECUTABLE " Xilinx Series 7 -help all" << endl;
	cout << "  " EXECUTABLE " Xilinx UltraScale -help all" << endl;
	cout << "  " EXECUTABLE " Xilinx UltraScale+ -help all" << endl;
}
void byteman::help(string param)
{
	if(str::iff::stringIs(param, "all")){
		help();
		byteman::helpAssembly(true);
		byteman::helpBlank(true);
		byteman::helpChange(true);
		byteman::helpDevice(true);
		byteman::helpExit(true);
		byteman::helpHelp(true);
		byteman::helpInput(true);
		byteman::helpMerge(true);
		byteman::helpOutput(true);
		byteman::helpRegion(true);
		byteman::helpStdin(true);
		byteman::helpVerbose(true);
		byteman::helpWarn(true);
	} else if(str::iff::stringIs(param, "v", "verbose"))
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
	else if(str::iff::stringIs(param, "e", "exit"))
		byteman::helpExit();
	else if(str::iff::stringIs(param, "s", "stdin"))
		byteman::helpStdin();
	else {
		cout << "Unknown command to help you with: \"" << param << "\"!" << endl;
		cout << "Try one of \"assembly\", \"blank\", \"change\", \"device\", \"exit\", \"help\", \"input\", \"merge\", \"output\", \"region\", \"stdin\", \"verbose\", \"warn\" maybe?" << endl;
		cout << endl;
	}
}
void byteman::helpVerbose(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Verbose" << endl;
	cout << "  enables/disables logging messages. they are disabled by default." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -verbose [VerboseValue]" << endl;
	cout << endl;
	cout << "  Arg:" << endl;
	cout << "    [VerboseValue]: accepted values are 0 to disable and 1 to enable. Default is 1." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    # Enable logging:" << endl;
	cout << "    " EXECUTABLE " XUSP -v" << endl;
	cout << endl;
	cout << "    # Disable logging, but it was disabled by default anyway:" << endl;
	cout << "    " EXECUTABLE " Xilinx US -verbose 0" << endl;
	cout << endl;
	cout << "    # Enable logging for (-...) and then disable logging again:" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -v -... -v=0" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpWarn(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Warn" << endl;
	cout << "  enables/disables warning messages. they are enabled by default." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -warn [WarnValue]" << endl;
	cout << endl;
	cout << "  Arg:" << endl;
	cout << "    [WarnValue]: accepted values are 0 to disable and 1 to enable. Default is 1." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    # Enable warnings, but they were enabled by default anyway:" << endl;
	cout << "    " EXECUTABLE " XUSP -w" << endl;
	cout << endl;
	cout << "    # Disable warnings:" << endl;
	cout << "    " EXECUTABLE " Xilinx US -warn 0" << endl;
	cout << endl;
	cout << "    # Disable warnings for (-...) and then enable warnings again:" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -w=0 -... -w" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpRegion(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Region" << endl;
	cout << "  modifies the list of selected regions." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -region [Clear,Add <SrcY> <SrcX> <SizeY> <SizeX>]" << endl;
	cout << endl;
	cout << "  Args:" << endl;
	cout << "    [Clear]: clears the selected regions list." << endl;
	cout << "    [Add]: adds a new rect (<SrcY>:<SrcX> <SizeY>:<SizeX>) in the selected regions list." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    # Clears the list:" << endl;
	cout << "    " EXECUTABLE " XUSP -region clear" << endl;
	cout << endl;
	cout << "    # Adds a rectangle describing Y from 100 to 149 and X from 7 to 21.:" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -r add 100:7 50:15" << endl;
	cout << endl;
	cout << "    # Adds a rectangle describing Y from 120 to 179 and X from 7 to 21:" << endl;
	cout << "    " EXECUTABLE " Xilinx US -r add 120:7 60:15." << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpBlank(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Blank" << endl;
	cout << "  erases/sets the bitstream in the buffers. uses the list of selected regions (see \"-help Region\"). if the list is empty, works on the whole chip." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -blank [First] [Second] [Logic] [BlockRAM] [<BlankValue>]" << endl;
	cout << endl;
	cout << "  Args:" << endl;
	cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << endl;
	cout << "    [Second]: selects the second of the two bitstream buffers." << endl;
	cout << "    [Logic]: selects the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [BlockRAM]: selects the blockram contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [<BlankValue>]: byte value (0-255) to be written in the selected bitstream location. Default is 0." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    # Blanks the bitstream's logic plane to zeroes:" << endl;
	cout << "    " EXECUTABLE " XUSP -blank main clb" << endl;
	cout << endl;
	cout << "    # All values in the rectangle describing Y from 100 to 149 and X from 7 to 21 are set to 0xA5A5A5A5:" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -r add 100:7 50:15 -b 0xA5" << endl;
	cout << endl;
	cout << "    # Blanks the bitstream to zeroes:" << endl;
	cout << "    " EXECUTABLE " Xilinx US -b" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpChange(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Change" << endl;
	cout << "  Not public at this point." << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpDevice(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Device" << endl;
	cout << "  Sets the target device manually." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -device [First] [Second] <DeviceName>" << endl;
	cout << endl;
	cout << "  Args:" << endl;
	cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << endl;
	cout << "    [Second]: selects the second of the two bitstream buffers." << endl;
	cout << "    <DeviceName>: name of the target chip, package, or board." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    " EXECUTABLE " XUSP -device AlveoU200" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -d xc7z045" << endl;
	cout << "    " EXECUTABLE " Xilinx US -d xcvu440" << endl;
	cout << endl;
	#ifdef XS7
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_Series7 == selectedArchitecture)
			mainXS7.deviceHelp();
	#endif
	#ifdef XUS
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_UltraScale == selectedArchitecture)
			mainXUS.deviceHelp();
	#endif
	#ifdef XUSP
		if(Architecture::Unknown == selectedArchitecture || Architecture::Xilinx_UltraScalePlus == selectedArchitecture)
			mainXUSP.deviceHelp();
	#endif
	cout << endl;
	cout << endl;
}
void byteman::helpHelp(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Help" << endl;
	cout << "  prints these messages... help messages can change if architecture and/or specific device is selected to incorporate more information!" << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -help [Command]" << endl;
	cout << endl;
	cout << "  Arg:" << endl;
	cout << "    [Command]: prints extra information about the usage of a specific command." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    " EXECUTABLE " Xilinx US+ -device AlveoU200  -help device #will print extra information about the selected device" << endl;
	cout << "    " EXECUTABLE " Xilinx US+ -input bitfile.bit -help device #will print extra information about the bitstream device" << endl;
	cout << "    " EXECUTABLE " -help" << endl;
	cout << "    " EXECUTABLE " XUSP -h" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -h" << endl;
	cout << "    " EXECUTABLE " Xilinx US -h" << endl;
	cout << "    " EXECUTABLE " -help help" << endl;
	cout << "    " EXECUTABLE " -help assembly" << endl;
	cout << "    " EXECUTABLE " -help blank" << endl;
	cout << "    " EXECUTABLE " -help change" << endl;
	cout << "    " EXECUTABLE " -help end" << endl;
	cout << "    " EXECUTABLE " -help input" << endl;
	cout << "    " EXECUTABLE " -help merge" << endl;
	cout << "    " EXECUTABLE " -help region" << endl;
	cout << "    " EXECUTABLE " -help output" << endl;
	cout << "    " EXECUTABLE " -help verbose" << endl;
	cout << "    " EXECUTABLE " -help warn" << endl;
	cout << "    " EXECUTABLE " -help device" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpInput(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Input" << endl;
	cout << "  reads a bitstream from a file." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -input [First] [Second] <InputFileName>" << endl;
	cout << endl;
	cout << "  Args:" << endl;
	cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << endl;
	cout << "    [Second]: selects the second of the two bitstream buffers." << endl;
	cout << "    <InputFileName>: name of the input file. supports \".bit\" and \".bin\" file formats." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    " EXECUTABLE " XUSP -i main zcu102bitstream.bit" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -input zedboardBitstream.bin" << endl;
	cout << "    " EXECUTABLE " Xilinx US -i xcvu440Bitstream.bit" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpMerge(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Merge" << endl;
	cout << "  merges portion of the bitstream in the second buffer into the first buffer." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -merge [Logic] [BlockRAM] [Clock] [SET,XOR,OR,AND] [Full,<SrcY> <SrcX> <SizeY> <SizeX> <DstY> <DstX>]" << endl;
	cout << endl;
	cout << "  Args:" << endl;
	cout << "    [Logic]: selects the data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [Clock]: selects the global clock data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [BlockRAM]: selects the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [SET,XOR,OR,AND]: selects merge operation. XOR, OR, AND are bitwise operations. Default is \"SET\"." << endl;
	cout << "    [Full]: selects the full device bitstream." << endl;
	cout << "    [<SrcY> <SrcX> <SizeY> <SizeX> <DstY> <DstX>]: selects a rect (<SrcY> <SrcX> <SizeY> <SizeX>) from the second bitstream buffer to be merged" << endl; 
	cout << "                                                 : into destination rect (<DstY> <DstX> <SizeY> <SizeX>) in first bitstream buffer." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    # Copies second bitstream into first bitstream's buffer:" << endl;
	cout << "    " EXECUTABLE " XUSP -input bit1.bit -input second bit2.bin -merge full" << endl;
	cout << endl;
	cout << "    # A rect in second buffer (Y from 50 to 99 and X from 10 to 29) is bitwise XORed in the clock and blockram" << endl;
	cout << "    # contents planes into a rect in 1st buffer describing Y from 100 to 149 and X from 30 to 49:" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -... -merge clock,bram,xor 50:10 50:20 100:30" << endl;
	cout << endl;
	cout << "    # Copies the logic plane from buffer 2 into buffer 1:" << endl;
	cout << "    " EXECUTABLE " Xilinx US -... -m full,logic" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpOutput(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Output" << endl;
	cout << "  creates a bitstream file output from the first bitstream buffer in byteman." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -output [Logic] [BlockRAM] [Blank] [Full,<StartY> <StartX> <SizeY> <SizeX>] <OutputFileName>" << endl;
	cout << endl;
	cout << "  Args:" << endl;
	cout << "    [Logic]: selects the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [BlockRAM]: selects the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [Blank]: blanks the selected regions to zeroes before writing the actual bitstream to them." << endl;
	cout << "    [Full]: selects all frames in the FPGA." << endl;
	cout << "    [<StartY> <StartX> <SizeY> <SizeX>]: selects a rect (<StartY> <StartX> <SizeY> <SizeX>) for output." << endl;
	cout << "                                       : also the list of selected regions will be output (see \"byteman -help Region\")." << endl;
	cout << "    <OutputFileName>: name of the output file. supports \".bit\" and \".bin\" file formats." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    # Output only the blockram contents to the whole FPGA:" << endl;
	cout << "    " EXECUTABLE " XUSP -input bitstream.bit -... -output bram,full bitfil.bit" << endl;
	cout << endl;
	cout << "    # The rectangle describing Y from 50 to 99 and X from 10 to 29 is zeroed and then the logic plane bitstream data is written:" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -... -o logic,blank 50:10 50:20 bitstream.bin" << endl;
	cout << endl;
	cout << "    # The rectangles (60:10 120:20 and 180:30 240:40) are zeroed and the logic & blockram content planes are written:" << endl;
	cout << "    " EXECUTABLE " XUS -... -r add 60:10 120:20 -o blank 180:30 240:40 fo.bit"<< endl;
	cout << endl;
	cout << endl;
}
void byteman::helpAssembly(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Assembly" << endl;
	cout << "  assembles and disassembles between bitstream files (.bit|.bin) and bitstream assembly (.bitasm) format." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " -assembly <InputBitstreamFileName> <OutputAssemblyFileName>" << endl;
	cout << "    " EXECUTABLE " -assembly <InputAssemblyFileName> <OutputBitstreamFileName>" << endl;
	cout << "  Args:" << endl;
	cout << "    <InputBitstreamFileName>: name of the input bitstream file. supports \".bit\" and \".bin\" file formats." << endl;
	cout << "    <OutputBitstreamFileName>: name of the output bitstream file. supports \".bit\" and \".bin\" file formats." << endl;
	cout << "    <InputAssemblyFileName>: name of the input assembly file. supports \".bitasm\" file format." << endl;
	cout << "    <OutputAssemblyFileName>: name of the output assembly file. supports \".bitasm\" file format." << endl;
	cout << endl;
	cout << "  the bitstream assembly files can be edited and converted back to valid bitstreams." << endl;
	cout << "  in bitstream assembly file format:" << endl;
	cout << "    every line that doesn't start with '#' is editable." << endl;
	cout << "    bitstream commands are written one per line and have the format \"<COMMAND NAME>;\"" << endl;
	cout << "    bitstream register writesare one per line and have the format \"@<REGNAME> = <NEW REG VALUE>\"" << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    " EXECUTABLE " XUSP -assembly bitstream.bit bitstream.bitasm" << endl;
	cout << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -a bitstream.bitasm bitstream.bit" << endl;
	cout << endl;
	cout << "    # Generate a bitstream file and then convert it to bit assembly:" << endl;
	cout << "    " EXECUTABLE " Xilinx US -... -o bitstreamOut.bit -a bitstreamOut.bit bitstreamOut.bitasm" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpExit(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: Exit" << endl;
	cout << "  finishes execution of byteman. used with STDin mode (see \"-help STDin\") to identify the end of command sequence." << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpStdin(bool selectedAll)
{
	if(selectedAll)
		cout << "byteman: STDin" << endl;
	cout << "  continues reading commands from standard input rather than command line arguments. this way commands can be entered by hand or streamed in by another program or file." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " [commands]* -stdin" << endl;
	cout << endl;
	cout << "  the input supports the following format rules:" << endl;
	cout << "    all commands are the same as their command line argument counterparts." << endl;
	cout << "    one command (with its arguments) per line." << endl;
	cout << "    '-' at the beginning of a command can be skipped." << endl;
	cout << "    '#' starts a comments until a new line is seen." << endl;
	cout << "    the command Exit will end reading the input stream and close byteman." << endl;
	cout << endl;
	cout << endl;
}