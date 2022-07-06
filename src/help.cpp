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
	cout << "  ARCH = {Xilinx Series 7, Xilinx UltraScale, Xilinx UltraScale+}" << endl;
	cout << endl;
	cout << "Commands:        : [Description]                                    " << endl;
	cout << "  -a (assembly)  : (de)compile bitstream assembly files. See \"-help assembly\"" << endl;
	cout << "  -b (blank)     : Erases bitstream buffers. See \"-help blank\"" << endl;
	cout << "  -c (change)    : Modify primitives (i.e., LUT contents). See \"-help change\"" << endl;
	cout << "  -d (device)    : Manually select target device. See \"-help device\"" << endl;
	cout << "  -e (exit)      : Exit byteman. Useful during stdin. See \"-help exit\"" << endl;
	cout << "  -h (help)      : Get help. See \"-help help\"" << endl;
	cout << "  -i (input)     : Input bitstream. See \"-help input\"" << endl;
	cout << "  -m (merge)     : Merge bitstreams. See \"-help merge\"" << endl;
	cout << "  -o (output)    : Output bitstream. See \"-help output\"" << endl;
	cout << "  -r (region)    : Modify region selection. See \"-help region\"" << endl;
	cout << "  -s (stdin)     : Continue using standard input. See \"-help stdin\"" << endl;
	cout << "  -v (verbose)   : Enable/disable spam. See \"-help verbose\"" << endl;
	cout << "  -w (warn)      : Enable/disable warnings. See \"-help warn\"" << endl;
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
		byteman::helpAssembly();
		byteman::helpBlank();
		byteman::helpChange();
		byteman::helpDevice();
		byteman::helpExit();
		byteman::helpHelp();
		byteman::helpInput();
		byteman::helpMerge();
		byteman::helpOutput();
		byteman::helpRegion();
		byteman::helpStdin();
		byteman::helpVerbose();
		byteman::helpWarn();
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
void byteman::helpVerbose()
{
	cout << "byteman: Verbose" << endl;
	cout << "  Enables/disables logging messages. They are disabled by default." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -verbose [verboseValue]" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [verboseValue]: Accepted values are 0 to disable and 1 to enable. Default is 1." << endl;
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
void byteman::helpWarn()
{
	cout << "byteman: Warn" << endl;
	cout << "  Enables/disables warning messages. They are enabled by default." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -warn [warnValue]" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [warnValue]: Accepted values are 0 to disable and 1 to enable. Default is 1." << endl;
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
void byteman::helpRegion()
{
	cout << "byteman: Region" << endl;
	cout << "  Modifies the list of selected regions." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -region [clear,add <srcY> <srcX> <sizeY> <sizeX>]" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [clear]: Clears the selected regions list." << endl;
	cout << "    [add]: Adds a new rectangle in the selected regions list. The rectangle is described by coordinates <srcY>:<srcX> and rectangle size of <sizeY>:<sizeX>." << endl;
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
void byteman::helpBlank()
{
	cout << "byteman: Blank" << endl;
	cout << "  Erases/sets the bitstream in the buffers. Uses the list of selected regions (see \"-help region\"). If the list is empty, works on the whole chip." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -blank [first] [second] [logic] [BlockRAM] [blankValue]" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [first]: Blanks in the first of the two buffers. Enabled by default if no buffer is explicitly specified." << endl;
	cout << "    [second]: Blanks in the second of the two buffers." << endl;
	cout << "    [logic]: Will blank logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [BlockRAM]: Will blank BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [blankValue]: Byte value (0-0xFF) to be written in the selected bitstream location. Default is 0." << endl;
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
void byteman::helpChange()
{
	cout << "byteman: Change" << endl;
	cout << "  Not public at this point." << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpDevice()
{
	cout << "byteman: Device" << endl;
	cout << "  Sets the target device manually." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -device [first] [second] <deviceName>" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [first]: Sets the device for the first of the two buffers. Enabled by default if no buffer is explicitly specified." << endl;
	cout << "    [second]: Sets the device for the second of the two buffers." << endl;
	cout << "    [<deviceName>]: Name of the target FPGA chip or board." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    " EXECUTABLE " XUSP -device AlveoU200" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -d xc7z045" << endl;
	cout << "    " EXECUTABLE " Xilinx US -d XCVU440" << endl;
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
void byteman::helpHelp()
{
	cout << "byteman: Help" << endl;
	cout << "  Prints these messages... Help messages can change if architecture and/or specific device is selected to incorporate more information!" << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -help [command]" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [command]: Prints extra information about the usage of a command." << endl;
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
void byteman::helpInput()
{
	cout << "byteman: Input" << endl;
	cout << "  Reads a bitstream from a file." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -input [first] [second] bitstreamFile.(bit|bin)" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [first]: Reads the bitstream file into the first of the two buffers. Enabled by default if no buffer is explicitly specified." << endl;
	cout << "    [second]: Reads the bitstream file into the second of the two buffers." << endl;
	cout << endl;
	cout << "  Examples:" << endl;
	cout << "    " EXECUTABLE " XUSP -i main zcu102bitstream.bit" << endl;
	cout << "    " EXECUTABLE " Xilinx Series 7 -input zedboardBitstream.bin" << endl;
	cout << "    " EXECUTABLE " Xilinx US -i xcvu440Bitstream.bit" << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpMerge()
{
	cout << "byteman: Merge" << endl;
	cout << "  Merges portion of the bitstream in the second buffer into the first buffer." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -merge [logic] [BlockRAM] [clock] [set,xor,or,and] [full,<srcY> <srcX> <sizeY> <sizeX> <dstY> <dstX>]" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [logic]: Will merge the logic plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [clock]: Will merge the global clock data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [BlockRAM]: Will merge the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [set,xor,or,and]: Performed merge operation. XOR, OR, AND are bitwise operations. Default is \"set\"." << endl;
	cout << "    [full]: Merges the full device bitstreams." << endl;
	cout << "    [<srcY> <srcX> <sizeY> <sizeX> <dstY> <dstX>]: Alternatively, a rectangle could be selected for merging. A source rectangle" << endl; 
	cout << "                                                 : (<srcY> <srcX> <sizeY> <sizeX>) from the second bitstream buffer is merged" << endl;
	cout << "                                                 : into destination rectangle (<dstY> <dstX> <sizeY> <sizeX>) in first bitstream buffer." << endl;
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
void byteman::helpOutput()
{
	cout << "byteman: Output" << endl;
	cout << "  Creates a bitstream file output from the first bitstream buffer in byteman." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -output [logic] [BlockRAM] [blank] [full,<startY> <startX> <sizeY> <sizeX>] bitstreamFile.(bit|bin)" << endl;
	cout << endl;
	cout << "  Options:" << endl;
	cout << "    [logic]: Will output the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [BlockRAM]: Will merge the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified." << endl;
	cout << "    [blank]: Blank the selected regions to zeroes before writing the actual bitstream to them." << endl;
	cout << "    [full]: Outputs the bitstream for all frames in the FPGA." << endl;
	cout << "    [<startY> <startX> <sizeY> <sizeX>]: Alternatively, a rectangle (<startY> <startX> <sizeY> <sizeX>) could be selected for output." << endl;
	cout << "                                       : In this mode, also the list of selected regions will be output (see \"byteman -help region\")." << endl;
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
void byteman::helpAssembly()
{
	cout << "byteman: Assembly" << endl;
	cout << "  Assembles and disassembles between bitstream files (.bit|.bin) and bitstream assembly (.bitasm) format." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH -assembly bitstreamFile.(bit|bin) bitstreamAssemblyFile.bitasm" << endl;
	cout << "    " EXECUTABLE " ARCH -assembly bitstreamAssemblyFile.bitasm bitstreamFile.(bit|bin)" << endl;
	cout << endl;
	cout << "  The bitstream assembly files can be edited and converted back to valid bitstreams." << endl;
	cout << "  In bitstream assembly file format:" << endl;
	cout << "    Every line that doesn't start with '#' is editable." << endl;
	cout << "    Bitstream commands are written one per line and have the format \"<COMMAND NAME>;\"" << endl;
	cout << "    Bitstream register writesare one per line and have the format \"@<REGNAME> = <NEW REG VALUE>\"" << endl;
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
void byteman::helpExit()
{
	cout << "byteman: Exit" << endl;
	cout << "  Finishes execution of byteman. Used with STDin mode (see \"-help stdin\") to identify the end of command sequence." << endl;
	cout << endl;
	cout << endl;
}
void byteman::helpStdin()
{
	cout << "byteman: STDin" << endl;
	cout << "  Continues reading commands from standard input rather than command line arguments. This way commands can be entered by hand or streamed in by another program or file." << endl;
	cout << endl;
	cout << "  Usage:" << endl;
	cout << "    " EXECUTABLE " ARCH [commands]* -stdin" << endl;
	cout << endl;
	cout << "  The input supports the following format rules:" << endl;
	cout << "    All commands are the same as their command line argument counterparts." << endl;
	cout << "    One command (with its arguments) per line." << endl;
	cout << "    '-' at the beginning of a command can be skipped." << endl;
	cout << "    '#' starts a comments until a new line is seen." << endl;
	cout << "    The command \"Exit\" will end reading the input stream and close byteman." << endl;
	cout << endl;
	cout << endl;
}