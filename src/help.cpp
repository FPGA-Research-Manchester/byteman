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


#ifdef _WIN32
	#define EXECUTABLE "byteman.exe"
#else
	#define EXECUTABLE "byteman"
#endif
void byteman::help(std::string param, int exitCode)
{
	std::cout << "byteman " << VERSION << " (Build #" << VERSION_BUILD << ", compiled on " << __DATE__ << " " << __TIME__ << ")" << std::endl;
	if(param.empty())
		byteman::help();
	else
		byteman::help(param);
	exit(exitCode);
}
void byteman::help()
{
	std::cout << "Usage:" << std::endl;
	std::cout << "  " EXECUTABLE " ARCH [-command...]* [-stdin]" << std::endl;
	std::cout << std::endl;
	std::cout << "  ARCH = {\"Xilinx Series 7\"/\"XS7\", \"Xilinx UltraScale\"/\"XUS\", \"Xilinx UltraScale+ \"/\"XUSP\"}" << std::endl;
	std::cout << std::endl;
	std::cout << "Commands:        : [Description]                                    " << std::endl;
	std::cout << "  -a (Assembly)  : (de)assemble bitstreams.       see \"-help assembly\"" << std::endl;
	std::cout << "  -b (Blank)     : erases bitstream buffers.      see \"-help blank\"" << std::endl;
	std::cout << "  -c (Change)    : modify bitstream.              see \"-help change\"" << std::endl;
	std::cout << "  -d (Device)    : manually select target device. see \"-help device\"" << std::endl;
	std::cout << "  -e (Exit)      : exit byteman. used with stdin. see \"-help exit\"" << std::endl;
	std::cout << "  -h (Help)      : get help.                      see \"-help help\"" << std::endl;
	std::cout << "  -i (Input)     : input bitstream.               see \"-help input\"" << std::endl;
	std::cout << "  -m (Merge)     : merge bitstreams.              see \"-help merge\"" << std::endl;
	std::cout << "  -o (Output)    : output bitstream.              see \"-help output\"" << std::endl;
	std::cout << "  -r (Region)    : modify region selection.       see \"-help region\"" << std::endl;
	std::cout << "  -s (Stdin)     : continue using standard input. see \"-help stdin\"" << std::endl;
	std::cout << "  -v (Verbose)   : enable/disable spam.           see \"-help verbose\"" << std::endl;
	std::cout << "  -w (Warn)      : enable/disable warnings.       see \"-help warn\"" << std::endl;
	std::cout << std::endl;
	std::cout << "Examples: " << std::endl;
	std::cout << "  " EXECUTABLE " Xilinx US+ -w -i main static.bit -i temp filter.bit -merge logic 180:67 120:30 240:37 -o logic 240:37 120:30 relocatedFilter.bit" << std::endl;
	std::cout << "  " EXECUTABLE " XUSP -w -d ZCU 102 -b main, logic, blockram -i temp filter.bit -merge logic 180:67 120:30 180:67 -o logic 180:67 120:30 mergedFilter.bit" << std::endl;
	std::cout << std::endl;
	std::cout << "Need more help? Try one of these:" << std::endl;
	std::cout << "  " EXECUTABLE " -help <command>" << std::endl;
	std::cout << "  " EXECUTABLE " -help all" << std::endl;
	std::cout << "  " EXECUTABLE " Xilinx Series 7 -help all" << std::endl;
	std::cout << "  " EXECUTABLE " Xilinx UltraScale -help all" << std::endl;
	std::cout << "  " EXECUTABLE " Xilinx UltraScale+ -help all" << std::endl;
}
void byteman::help(std::string param)
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
		std::cout << "Unknown command to help you with: \"" << param << "\"!" << std::endl;
		std::cout << "Try one of \"assembly\", \"blank\", \"change\", \"device\", \"exit\", \"help\", \"input\", \"merge\", \"output\", \"region\", \"stdin\", \"verbose\", \"warn\" maybe?" << std::endl;
		std::cout << std::endl;
	}
}
void byteman::helpVerbose(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Verbose" << std::endl;
	std::cout << "  enables/disables logging messages. they are disabled by default." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -verbose [<VerboseValue>]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Arg:" << std::endl;
	std::cout << "    [<VerboseValue>]: accepted values are 0 to disable and 1 to enable. Default is 1." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # Enable logging:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -v" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Disable logging, but it was disabled by default anyway:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -verbose 0" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Enable logging for (-...) and then disable logging again:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -v -... -v=0" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpWarn(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Warn" << std::endl;
	std::cout << "  enables/disables warning messages. they are enabled by default." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -warn [<WarnValue>]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Arg:" << std::endl;
	std::cout << "    [<WarnValue>]: accepted values are 0 to disable and 1 to enable. Default is 1." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # Enable warnings, but they were enabled by default anyway:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -w" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Disable warnings:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -warn 0" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Disable warnings for (-...) and then enable warnings again:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -w=0 -... -w" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpRegion(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Region" << std::endl;
	std::cout << "  modifies the list of selected regions." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -region [Clear, Add <SrcY> <SrcX> <SizeY> <SizeX>]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [Clear]: clears the selected regions list." << std::endl;
	std::cout << "    [Add]: adds a new rect (<SrcY>:<SrcX> <SizeY>:<SizeX>) in the selected regions list." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # Clears the list:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -region clear" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Adds a rectangle describing Y from 100 to 149 and X from 7 to 21.:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -r add 100:7 50:15" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Adds a rectangle describing Y from 120 to 179 and X from 7 to 21:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -r add 120:7 60:15." << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpBlank(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Blank" << std::endl;
	std::cout << "  erases/sets the bitstream in the buffers. uses the list of selected regions (see \"-help Region\"). if the list is empty, works on the whole chip." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -blank [First] [Second] [Logic] [BlockRAM] [<BlankValue>]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << std::endl;
	std::cout << "    [Second]: selects the second of the two bitstream buffers." << std::endl;
	std::cout << "    [Logic]: selects the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [BlockRAM]: selects the blockram contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [<BlankValue>]: byte value (0-255) to be written in the selected bitstream location. Default is 0." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # Blanks the bitstream's logic plane to zeroes:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -blank main clb" << std::endl;
	std::cout << std::endl;
	std::cout << "    # All values in the rectangle describing Y from 100 to 149 and X from 7 to 21 are set to 0xA5A5A5A5:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -r add 100:7 50:15 -b 0xA5" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Blanks the bitstream to zeroes:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -b" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpChange(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Change" << std::endl;
	std::cout << "  modifies bitstream data." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -change [First] [Second] [BigEndian, LittleEndian, BigEndianWithBitSwap, LittleEndianWithBitSwap, NativeEndian] [word <Y>:<X> <blockType> <frameOffset> <wordOffset> <mask> <value>]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << std::endl;
	std::cout << "    [Second]: selects the second of the two bitstream buffers." << std::endl;
	std::cout << "    [BigEndian]: forces the stored bitstream data is in Big Endian format. any future operations will inherit the endianness (such as -Output)" << std::endl;
	std::cout << "    [LittleEndian]: forces the stored bitstream data is in Little Endian format. any future operations will inherit the endianness (such as -Output)" << std::endl;
	std::cout << "    [BigEndianWithBitSwap]: forces the stored bitstream data is in Big Endian with Bit Swap format. any future operations will inherit the endianness (such as -Output)" << std::endl;
	std::cout << "    [LittleEndianWithBitSwap]: forces the stored bitstream data is in Little Endian with Bit Swap format. any future operations will inherit the endianness (such as -Output)" << std::endl;
	std::cout << "    [NativeEndian]: forces the stored bitstream data is in the native endianness of the system. any future operations will inherit the endianness (such as -Output)" << std::endl;
	std::cout << "    [word <Y>:<X> <blockType> <frameOffset> <wordOffset> <mask> <value>]: forces bitstream value <value> (masked by <mask>) to a word located at coordinates <Y>:<X> in block <blockType>" << std::endl;
	std::cout << "                                                                        : with internal offsets for the frame <frameOffset> and word <wordOffset>" << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # reads a bitstream, forces big endianness, writes back the bitstream in the new endianness:" << std::endl;
	std::cout << "    " EXECUTABLE " XUS -input bitstream.bit -change bigEndian -output full bitstream.bit" << std::endl;
	std::cout << "    # reads a bitstream, forces little endianness, writes back the bitstream in the new endianness:" << std::endl;
	std::cout << "    " EXECUTABLE " xilinx series 7 -input boot.bin -change LITTLEendian -output full boot.bin" << std::endl;
	std::cout << "    # load a partial bitstream located at 180:0. targetting the third blockram left to right, located at 180:37." << std::endl;
	std::cout << "    # modify its routing at the 3rd frame (frame offset 2) at the 41-st word (word offset 40) to value 0xA5A50000 (masked by 0xFFFF0000). (the blockram routing is located in CLB block Type - blockType 0):" << std::endl;
	std::cout << "    " EXECUTABLE " xilinx UltraScale+ -device ZCU102 -input pr.bit -change word 180:37 0 2 40 0xFFFF0000 0xA5A50000 -output ..." << std::endl;
	std::cout << "    # load a partial bitstream located at 180:0. targetting the second blockram left to right, located at 180:37." << std::endl;
	std::cout << "    # modify contents at the 233rd frame (frame offset 232) at the 5-th word (word offset 4) to value 0xFFFFFFFF (masked by 0xFFFFFFFF). (the blockram contents are located in blockType 1):" << std::endl;
	std::cout << "    " EXECUTABLE " xilinx UltraScale+ -device ZCU102 -input pr.bit -change word 180:37 1 232 4 0xFFFFFFFF 0xFFFFFFFF -output ..." << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpDevice(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Device" << std::endl;
	std::cout << "  Sets the target device manually." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -device [First] [Second] <DeviceName>" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << std::endl;
	std::cout << "    [Second]: selects the second of the two bitstream buffers." << std::endl;
	std::cout << "    <DeviceName>: name of the target chip, package, or board." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -device Alveo U200" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -d Wizarde" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -d xcvu440" << std::endl;
	std::cout << std::endl;
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
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpHelp(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Help" << std::endl;
	std::cout << "  prints these messages... help messages can change if architecture and/or specific device is selected to incorporate more information!" << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -help [Command]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Arg:" << std::endl;
	std::cout << "    [Command]: prints extra information about the usage of a specific command." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US+ -device Alveo U200 -help device #will print extra information about the selected device" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US+ -input bitfile.bit -help device #will print extra information about the bitstream device" << std::endl;
	std::cout << "    " EXECUTABLE " -help" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -h" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -h" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -h" << std::endl;
	std::cout << "    " EXECUTABLE " -help help" << std::endl;
	std::cout << "    " EXECUTABLE " -help assembly" << std::endl;
	std::cout << "    " EXECUTABLE " -help blank" << std::endl;
	std::cout << "    " EXECUTABLE " -help change" << std::endl;
	std::cout << "    " EXECUTABLE " -help end" << std::endl;
	std::cout << "    " EXECUTABLE " -help input" << std::endl;
	std::cout << "    " EXECUTABLE " -help merge" << std::endl;
	std::cout << "    " EXECUTABLE " -help region" << std::endl;
	std::cout << "    " EXECUTABLE " -help output" << std::endl;
	std::cout << "    " EXECUTABLE " -help verbose" << std::endl;
	std::cout << "    " EXECUTABLE " -help warn" << std::endl;
	std::cout << "    " EXECUTABLE " -help device" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpInput(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Input" << std::endl;
	std::cout << "  reads a bitstream from a file." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -input [First] [Second] <InputFileName>" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified." << std::endl;
	std::cout << "    [Second]: selects the second of the two bitstream buffers." << std::endl;
	std::cout << "    <InputFileName>: name of the input file. supports \".bit\" and \".bin\" file formats." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -i main zcu102bitstream.bit" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -input zedboardBitstream.bin" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -i xcvu440Bitstream.bit" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpMerge(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Merge" << std::endl;
	std::cout << "  merges portion of the bitstream in the second buffer into the first buffer." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -merge [Logic] [BlockRAM] [Clock] [SET, XOR, OR, AND] [Full, <SrcY> <SrcX> <SizeY> <SizeX> <DstY> <DstX>]" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [Logic]: selects the data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [Clock]: selects the global clock data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [BlockRAM]: selects the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [SET, XOR, OR, AND]: selects merge operation. XOR, OR, AND are bitwise operations. Default is \"SET\"." << std::endl;
	std::cout << "    [Full]: selects the full device bitstream." << std::endl;
	std::cout << "    [<SrcY> <SrcX> <SizeY> <SizeX> <DstY> <DstX>]: selects a rect (<SrcY> <SrcX> <SizeY> <SizeX>) from the second bitstream buffer to be merged" << std::endl;
	std::cout << "                                                 : into destination rect (<DstY> <DstX> <SizeY> <SizeX>) in first bitstream buffer." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # Copies second bitstream into first bitstream's buffer:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -input bit1.bit -input second bit2.bin -merge full" << std::endl;
	std::cout << std::endl;
	std::cout << "    # A rect in second buffer (Y from 50 to 99 and X from 10 to 29) is bitwise XORed in the clock and blockram" << std::endl;
	std::cout << "    # contents planes into a rect in 1st buffer describing Y from 100 to 149 and X from 30 to 49:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -... -merge clock, bram, xor 50:10 50:20 100:30" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Copies the logic plane from buffer 2 into buffer 1:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -... -m full, logic" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpOutput(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Output" << std::endl;
	std::cout << "  creates a bitstream file output from the first bitstream buffer in byteman." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -output [Logic] [BlockRAM] [Blank] [Full, <StartY> <StartX> <SizeY> <SizeX>] <OutputFileName>" << std::endl;
	std::cout << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    [Logic]: selects the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [BlockRAM]: selects the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << std::endl;
	std::cout << "    [Blank]: blanks the selected regions to zeroes before writing the actual bitstream to them." << std::endl;
	std::cout << "    [Full]: selects all frames in the chip." << std::endl;
	std::cout << "    [<StartY> <StartX> <SizeY> <SizeX>]: selects a rect (<StartY> <StartX> <SizeY> <SizeX>) for output." << std::endl;
	std::cout << "                                       : also the list of selected regions will be output (see \"byteman -help Region\")." << std::endl;
	std::cout << "    <OutputFileName>: name of the output file. supports \".bit\" and \".bin\" file formats." << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    # Output only the blockram contents to the whole FPGA:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -input bitstream.bit -... -output bram, full bitfil.bit" << std::endl;
	std::cout << std::endl;
	std::cout << "    # The rectangle describing Y from 50 to 99 and X from 10 to 29 is zeroed and then the logic plane bitstream data is written:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -... -o logic, blank 50:10 50:20 bitstream.bin" << std::endl;
	std::cout << std::endl;
	std::cout << "    # The rectangles (60:10 120:20 and 180:30 240:40) are zeroed and the logic & blockram content planes are written:" << std::endl;
	std::cout << "    " EXECUTABLE " XUS -... -r add 60:10 120:20 -o blank 180:30 240:40 fo.bit" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpAssembly(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Assembly" << std::endl;
	std::cout << "  assembles and disassembles between bitstream files (.bit|.bin) and bitstream assembly (.bitasm) format." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " -assembly <InputBitstreamFileName> <OutputAssemblyFileName>" << std::endl;
	std::cout << "    " EXECUTABLE " -assembly <InputAssemblyFileName> <OutputBitstreamFileName>" << std::endl;
	std::cout << "  Args:" << std::endl;
	std::cout << "    <InputBitstreamFileName>: name of the input bitstream file. supports \".bit\" and \".bin\" file formats." << std::endl;
	std::cout << "    <OutputBitstreamFileName>: name of the output bitstream file. supports \".bit\" and \".bin\" file formats." << std::endl;
	std::cout << "    <InputAssemblyFileName>: name of the input assembly file. supports \".bitasm\" file format." << std::endl;
	std::cout << "    <OutputAssemblyFileName>: name of the output assembly file. supports \".bitasm\" file format." << std::endl;
	std::cout << std::endl;
	std::cout << "  the bitstream assembly files can be edited and converted back to valid bitstreams." << std::endl;
	std::cout << "  in bitstream assembly file format:" << std::endl;
	std::cout << "    every line that doesn't start with '#' is editable." << std::endl;
	std::cout << "    bitstream commands are written one per line and have the format \"<COMMAND NAME>;\"" << std::endl;
	std::cout << "    bitstream register writesare one per line and have the format \"@<REGNAME> = <NEW REG VALUE>\"" << std::endl;
	std::cout << std::endl;
	std::cout << "  Examples:" << std::endl;
	std::cout << "    " EXECUTABLE " XUSP -assembly bitstream.bit bitstream.bitasm" << std::endl;
	std::cout << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx Series 7 -a bitstream.bitasm bitstream.bit" << std::endl;
	std::cout << std::endl;
	std::cout << "    # Generate a bitstream file and then convert it to bit assembly:" << std::endl;
	std::cout << "    " EXECUTABLE " Xilinx US -... -o bitstreamOut.bit -a bitstreamOut.bit bitstreamOut.bitasm" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpExit(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: Exit" << std::endl;
	std::cout << "  finishes execution of byteman. used with STDin mode (see \"-help STDin\") to identify the end of command sequence." << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}
void byteman::helpStdin(bool selectedAll)
{
	if(selectedAll)
		std::cout << "byteman: STDin" << std::endl;
	std::cout << "  continues reading commands from standard input rather than command line arguments. this way commands can be entered by hand or streamed in by another program or file." << std::endl;
	std::cout << std::endl;
	std::cout << "  Usage:" << std::endl;
	std::cout << "    " EXECUTABLE " [commands]* -stdin" << std::endl;
	std::cout << std::endl;
	std::cout << "  the input supports the following format rules:" << std::endl;
	std::cout << "    all commands are the same as their command line argument counterparts." << std::endl;
	std::cout << "    one command (with its arguments) per line." << std::endl;
	std::cout << "    '-' at the beginning of a command can be skipped." << std::endl;
	std::cout << "    '#' starts a comments until a new line is seen." << std::endl;
	std::cout << "    the command Exit will end reading the input stream and close byteman." << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}