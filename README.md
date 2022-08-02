# ![byteman icon](./Resources/icon32.png) byteman
## *Open-source high-performance bitstream relocation and manipulation tool.*
[![byteman project](https://img.shields.io/badge/byteman-v1.3-blue)](https://github.com/FPGA-Research-Manchester/byteman)
[![byteman build](https://img.shields.io/badge/Build%20216-%E2%9C%94-green)](https://github.com/FPGA-Research-Manchester/byteman/commits/main)
[![Apache license](https://img.shields.io/github/license/FPGA-Research-Manchester/byteman)](https://opensource.org/licenses/Apache-2.0)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-14-green.svg)](https://en.wikipedia.org/wiki/C%2B%2B14)
[![Reports from automated testing](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/test.yml/badge.svg)](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/test.yml)

## ![byteman icon](./Resources/icon16.png) Download executable: [![Download windows x86 binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseWindows.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Windows-x86/byteman.exe) [![Download linux x86 binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseLinux.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Linux-x86/byteman) [![Download linux ARM binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseLinuxARM.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Linux-ARM/byteman) [![Download linux RISC-V binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseLinuxRISCV.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Linux-RISC-V/byteman)

## ![byteman icon](./Resources/icon16.png) Check documentation: [![Browse generated docs](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/generateDocs.yml/badge.svg)](https://FPGA-Research-Manchester.github.io/byteman/)


## $> byteman.exe -help
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
Usage:
  byteman.exe ARCH [-command...]* [-stdin]

  ARCH = {"Xilinx Series 7"/"XS7", "Xilinx UltraScale"/"XUS", "Xilinx UltraScale+ "/"XUSP"}

Commands:        : [Description]                                    
  -a (Assembly)  : (de)assemble bitstreams.       see "-help assembly"
  -b (Blank)     : erases bitstream buffers.      see "-help blank"
  -c (Change)    : modify bitstream.              see "-help change"
  -d (Device)    : manually select target device. see "-help device"
  -e (Exit)      : exit byteman. used with stdin. see "-help exit"
  -h (Help)      : get help.                      see "-help help"
  -i (Input)     : input bitstream.               see "-help input"
  -m (Merge)     : merge bitstreams.              see "-help merge"
  -o (Output)    : output bitstream.              see "-help output"
  -r (Region)    : modify region selection.       see "-help region"
  -s (Stdin)     : continue using standard input. see "-help stdin"
  -v (Verbose)   : enable/disable spam.           see "-help verbose"
  -w (Warn)      : enable/disable warnings.       see "-help warn"

Examples: 
  byteman.exe Xilinx US+ -w -i main static.bit -i temp filter.bit -merge logic 180:67 120:30 240:37 -o logic 240:37 120:30 relocatedFilter.bit
  byteman.exe XUSP -w -d ZCU 102 -b main, logic, blockram -i temp filter.bit -merge logic 180:67 120:30 180:67 -o logic 180:67 120:30 mergedFilter.bit

Need more help? Try one of these:
  byteman.exe -help <command>
  byteman.exe -help all
  byteman.exe Xilinx Series 7 -help all
  byteman.exe Xilinx UltraScale -help all
  byteman.exe Xilinx UltraScale+ -help all
```


## $> byteman.exe -help assembly
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  assembles and disassembles between bitstream files (.bit|.bin) and bitstream assembly (.bitasm) format.

  Usage:
    byteman.exe -assembly <InputBitstreamFileName> <OutputAssemblyFileName>
    byteman.exe -assembly <InputAssemblyFileName> <OutputBitstreamFileName>
  Args:
    <InputBitstreamFileName>: name of the input bitstream file. supports ".bit" and ".bin" file formats.
    <OutputBitstreamFileName>: name of the output bitstream file. supports ".bit" and ".bin" file formats.
    <InputAssemblyFileName>: name of the input assembly file. supports ".bitasm" file format.
    <OutputAssemblyFileName>: name of the output assembly file. supports ".bitasm" file format.

  the bitstream assembly files can be edited and converted back to valid bitstreams.
  in bitstream assembly file format:
    every line that doesn't start with '#' is editable.
    bitstream commands are written one per line and have the format "<COMMAND NAME>;"
    bitstream register writesare one per line and have the format "@<REGNAME> = <NEW REG VALUE>"

  Examples:
    byteman.exe XUSP -assembly bitstream.bit bitstream.bitasm

    byteman.exe Xilinx Series 7 -a bitstream.bitasm bitstream.bit

    # Generate a bitstream file and then convert it to bit assembly:
    byteman.exe Xilinx US -... -o bitstreamOut.bit -a bitstreamOut.bit bitstreamOut.bitasm
```


## $> byteman.exe -help blank
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  erases/sets the bitstream in the buffers. uses the list of selected regions (see "-help Region"). if the list is empty, works on the whole chip.

  Usage:
    byteman.exe -blank [First] [Second] [Logic] [BlockRAM] [<BlankValue>]

  Args:
    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified.
    [Second]: selects the second of the two bitstream buffers.
    [Logic]: selects the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified.
    [BlockRAM]: selects the blockram contents plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [<BlankValue>]: byte value (0-255) to be written in the selected bitstream location. Default is 0.

  Examples:
    # Blanks the bitstream's logic plane to zeroes:
    byteman.exe XUSP -blank main clb

    # All values in the rectangle describing Y from 100 to 149 and X from 7 to 21 are set to 0xA5A5A5A5:
    byteman.exe Xilinx Series 7 -r add 100:7 50:15 -b 0xA5

    # Blanks the bitstream to zeroes:
    byteman.exe Xilinx US -b
```


## $> byteman.exe -help change
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  modifies bitstream data.

  Usage:
    byteman.exe -change [First] [Second] [BigEndian, LittleEndian, BigEndianWithBitSwap, LittleEndianWithBitSwap, NativeEndian] [word <Y>:<X> <blockType> <frameOffset> <wordOffset> <value>]

  Args:
    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified.
    [Second]: selects the second of the two bitstream buffers.
    [BigEndian]: forces the stored bitstream data is in Big Endian format. any future operations will inherit the endianness (such as -Output)
    [LittleEndian]: forces the stored bitstream data is in Little Endian format. any future operations will inherit the endianness (such as -Output)
    [BigEndianWithBitSwap]: forces the stored bitstream data is in Big Endian with Bit Swap format. any future operations will inherit the endianness (such as -Output)
    [LittleEndianWithBitSwap]: forces the stored bitstream data is in Little Endian with Bit Swap format. any future operations will inherit the endianness (such as -Output)
    [NativeEndian]: forces the stored bitstream data is in the native endianness of the system. any future operations will inherit the endianness (such as -Output)
    [word <Y>:<X> <blockType> <frameOffset> <wordOffset> <value>]: forces bitstream value <value> to a word located at coordinates <Y>:<X> in block <blockType>
                                                                        : with internal offsets for the frame <frameOffset> and word <wordOffset>

  Examples:
    # reads a bitstream, forces big endianness, writes back the bitstream in the new endianness:
    byteman.exe XUS -input bitstream.bit -change bigEndian -output full bitstream.bit
    # reads a bitstream, forces little endianness, writes back the bitstream in the new endianness:
    byteman.exe xilinx series 7 -input boot.bin -change LITTLEendian -output full boot.bin
    # load a partial bitstream located at 180:0. targetting the third blockram left to right, located at 180:37.
    # modify its routing at the 3rd frame (frame offset 2) at the 41-st word (word offset 40) to value 0xA5A5A5A5. (the blockram routing is located in CLB block Type - blockType 0):
    byteman.exe xilinx UltraScale+ -device ZCU102 -input pr.bit -change word 180:37 0 2 40 0xA5A5A5A5 -output ...
    # load a partial bitstream located at 180:0. targetting the second blockram left to right, located at 180:37.
    # modify contents at the 233rd frame (frame offset 232) at the 5-th word (word offset 4) to value 0xFFFFFFFF. (the blockram contents are located in blockType 1):
    byteman.exe xilinx UltraScale+ -device ZCU102 -input pr.bit -change word 180:37 1 232 4 0xFFFFFFFF -output ...
```


## $> byteman.exe -help device
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  Sets the target device manually.

  Usage:
    byteman.exe -device [First] [Second] <DeviceName>

  Args:
    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified.
    [Second]: selects the second of the two bitstream buffers.
    <DeviceName>: name of the target chip, package, or board.

  Examples:
    byteman.exe XUSP -device Alveo U200
    byteman.exe Xilinx Series 7 -d Wizarde
    byteman.exe Xilinx US -d xcvu440

A list of currently supported Xilinx Series 7 devices: 
  Series 7 Spartan: 
    XC7S6, XA7S6, XC7S15, XA7S15, XC7S25, XA7S25, XC7S50, XA7S50, XC7S75, XA7S75, XC7S100, XA7S100
  Series 7 Artix: 
    XC7A12, XA7A12, XC7A15, XA7A15, XC7A25, XA7A25, XC7A35, XA7A35, XC7A50, XQ7A50, XA7A50, 
    XC7A75, XA7A75, XC7A100, XQ7A100, XA7A100, XC7A200, XQ7A200
  Series 7 Zynq: 
    XC7Z007, XC7Z010, XA7Z010, XC7Z012, XC7Z014, XC7Z015, XC7Z020, XQ7Z020, XA7Z020, 
    XC7Z030, XQ7Z030, XA7Z030, XC7Z035, XC7Z045, XQ7Z045, XC7Z100, XQ7Z100
  Series 7 Kintex: 
    XC7K70, XC7K160, XA7K160, XC7K325, XQ7K325, XC7K355, XC7K410, XQ7K410, XC7K420, XC7K480
  Series 7 Virtex: 
    XC7V585, XQ7V585, XC7V2000, XC7VX330, XQ7VX330, XC7VX415, XC7VX485, XQ7VX485, XC7VX550, 
    XC7VX690, XQ7VX690, XC7VX980, XQ7VX980, XC7VX1140
  Series 7 Boards: 
    Wizarde, Kintex-7 Connectivity Kit, KC705, KC724, SP701, AC701, Arty A7-35, Arty A7-100, Arty Z7-10, Arty Z7-20, 
    Arty S7-25, Arty S7-50, Basys 3, Pynq Z1, Nexys A7-50, Nexys A7-100, Zybo Z7-10, Zybo Z7-20, Cmod A7-15, 
    Cmod A7-35, Cmod S7, ZedBoard, Cora Z7-07, Cora Z7-10, Nexys Video, Genesys 2, Eclypse Z7, USB104, NetFPGA-SUME, 
    VC707, VC709, VC7203, VC7215, VC7222, ZC702, ZC706
A list of currently supported Xilinx UltraScale devices: 
  UltraScale Kintex: 
    XCKU025, XCKU035, XCKU040, XQKU040, XCKU060, XQKU060, XQRKU060, XCKU085, XCKU095, XQKU095, XCKU115, XQKU0115
  UltraScale Virtex: 
    XCVU065, XCVU080, XCVU095, XCVU125, XCVU160, XCVU190, XCVU440
A list of currently supported Xilinx UltraScale+ devices: 
  UltraScale+ Artix: 
    XCAU10P, XCAU15P, XCAU20P, XCAU25P
  UltraScale+ Kintex: 
    XCKU3P, XQKU3P, XCKU5P, XQKU5P, XCKU9P, XQKU9P, XCKU11P, XQKU11P, XCKU13P, XQKU13P, XCKU15P, XQKU15P, XCKU19P, XQKU19P
  UltraScale+ Virtex: 
    XCVU3P, XQVU3P, XCVU5P, XQVU5P, XCVU7P, XQVU7P, XCVU9P, XQVU9P, XCVU11P, XQVU11P, XCVU13P, XQVU13P, 
    XCVU19P, XQVU19P, XCVU23P, XQVU23P, XCVU27P, XQVU27P, XCVU29P, XQVU29P, XCVU31P, XQVU31P, XCVU33P, 
    XQVU33P, XCVU35P, XQVU35P, XCVU37P, XQVU37P, XCVU45P, XQVU45P, XCVU47P, XQVU47P, XCVU57P, XQVU57P
  UltraScale+ Zynq RFSoC: 
    XCZU21DR, XQZU21DR, XCZU25DR, XCZU27DR, XCZU28DR, XQZU28DR, XCZU29DR, XQZU29DR, XCZU39DR, XCZU42DR, 
    XCZU43DR, XCZU46DR, XQZU48DR, XQZU49DR, XCZU47DR, XCZU48DR, XCZU49DR
  UltraScale+ Zynq MPSoC: 
    XCZU1, XAZU1, XQZU1, XCZU2, XAZU2, XQZU2, XCZU3, XAZU3, XQZU3, XCZU4, XAZU4, XQZU4, 
    XCZU5, XAZU5, XQZU5, XCZU6, XAZU6, XQZU6, XCZU7, XAZU7, XQZU7, XCZU9, XAZU9, XQZU9, 
    XCZU11, XAZU11, XQZU11, XCZU15, XAZU15, XQZU15, XCZU17, XAZU17, XQZU17, XCZU19, XAZU19, XQZU19
  UltraScale+ Alveo: 
    XCU200, Alveo U200, XCU250, Alveo U250, XCU26, Alveo U26, XCUX35, Alveo U35, XCU50, Alveo U50, 
    XCU55N, Alveo U55N, XCU55C, Alveo U55C, XCU280, Alveo U280, XCU30, Alveo U30, XCU25, Alveo U25
  Misc UltraScale+ Boards: 
    ZCU 102, ZU-GMSL2, ZCU104, ZCU106, AXU15EG, AXU2CG, AXU3EG, AXU4EV, AXU5EV, AXU9EG, ULTRAZED-EV, CHAMP-XD1S, ZU-3EG, 
    ZU-5EV, HTG-Z999-06, HTG-Z999-09, HTG-Z999-15, HTG-930-9-3, HTG-930-9, HTG-930-13-3, HTG-930-13, HTG-937-37P, HTG-937-47P, 
    HTG-Z922-11, HTG-Z922-19-DG, HTG-Z922-19, CZU3EG, CZU4EV, TE0802, TE0803-04-3BE11, TE0807-03-7DE21, TE0808-05-9BE21, 
    TE0808-05-9GI21, TE0808-05-BBE21, TE0812, TEB0911, TEB0912, TYSOM-3-ZU7EV, XPEDITE2600, AMC574, ZCU208, ZCU216, ZCU670, 
    Ultra96, K26C, K26I, KV260, AMC584, HTG-960, VCU1525, VCU118, VCU128, VCU129, HTG-940-9-3, HTG-940-9, HTG-940-13-3, 
    HTG-940-13, HTG-9200-5P, HTG-9200-9P, HTG-9200-13P, HTG-VUP-PCIE-HH-9P, HTG-910-9, HTG-VUP-PCIE-HH-13P, HTG-910-13, 
    HTG-ZRF-FMC-28, HTG-ZRF-FMC-48, HTG-ZRF16-29, HTG-ZRF16-49, HTG-ZRF-HH-28, HTG-ZRF-HH-48, HTG-ZUSP-PCIE-11-3, 
    HTG-ZUSP-PCIE-19-2, HTG-ZRF8-R2-28, HTG-ZRF8-28, HTG-ZRF8-R2-48-I, HTG-ZRF8-48-I, HTG-ZRF8-R2-48, HTG-ZRF8-48
    ZUBoard 1CG, KCU 116, Vermeo T1 MPSoC, Vermeo T1 RFSoC, ZCU 1275, ZCU 1285, ZCU 111
```


## $> byteman.exe -help exit
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  finishes execution of byteman. used with STDin mode (see "-help STDin") to identify the end of command sequence.
```


## $> byteman.exe -help help
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  prints these messages... help messages can change if architecture and/or specific device is selected to incorporate more information!

  Usage:
    byteman.exe -help [Command]

  Arg:
    [Command]: prints extra information about the usage of a specific command.

  Examples:
    byteman.exe Xilinx US+ -device Alveo U200 -help device #will print extra information about the selected device
    byteman.exe Xilinx US+ -input bitfile.bit -help device #will print extra information about the bitstream device
    byteman.exe -help
    byteman.exe XUSP -h
    byteman.exe Xilinx Series 7 -h
    byteman.exe Xilinx US -h
    byteman.exe -help help
    byteman.exe -help assembly
    byteman.exe -help blank
    byteman.exe -help change
    byteman.exe -help end
    byteman.exe -help input
    byteman.exe -help merge
    byteman.exe -help region
    byteman.exe -help output
    byteman.exe -help verbose
    byteman.exe -help warn
    byteman.exe -help device
```


## $> byteman.exe -help input
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  reads a bitstream from a file.

  Usage:
    byteman.exe -input [First] [Second] <InputFileName>

  Args:
    [First]: selects the first of the two bitstream buffers. Enabled by default if no buffer is explicitly specified.
    [Second]: selects the second of the two bitstream buffers.
    <InputFileName>: name of the input file. supports ".bit" and ".bin" file formats.

  Examples:
    byteman.exe XUSP -i main zcu102bitstream.bit
    byteman.exe Xilinx Series 7 -input zedboardBitstream.bin
    byteman.exe Xilinx US -i xcvu440Bitstream.bit
```


## $> byteman.exe -help merge
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  merges portion of the bitstream in the second buffer into the first buffer.

  Usage:
    byteman.exe -merge [Logic] [BlockRAM] [Clock] [SET, XOR, OR, AND] [Full, <SrcY> <SrcX> <SizeY> <SizeX> <DstY> <DstX>]

  Args:
    [Logic]: selects the data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [Clock]: selects the global clock data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [BlockRAM]: selects the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [SET, XOR, OR, AND]: selects merge operation. XOR, OR, AND are bitwise operations. Default is "SET".
    [Full]: selects the full device bitstream.
    [<SrcY> <SrcX> <SizeY> <SizeX> <DstY> <DstX>]: selects a rect (<SrcY> <SrcX> <SizeY> <SizeX>) from the second bitstream buffer to be merged
                                                 : into destination rect (<DstY> <DstX> <SizeY> <SizeX>) in first bitstream buffer.

  Examples:
    # Copies second bitstream into first bitstream's buffer:
    byteman.exe XUSP -input bit1.bit -input second bit2.bin -merge full

    # A rect in second buffer (Y from 50 to 99 and X from 10 to 29) is bitwise XORed in the clock and blockram
    # contents planes into a rect in 1st buffer describing Y from 100 to 149 and X from 30 to 49:
    byteman.exe Xilinx Series 7 -... -merge clock, bram, xor 50:10 50:20 100:30

    # Copies the logic plane from buffer 2 into buffer 1:
    byteman.exe Xilinx US -... -m full, logic
```


## $> byteman.exe -help output
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  creates a bitstream file output from the first bitstream buffer in byteman.

  Usage:
    byteman.exe -output [Logic] [BlockRAM] [Blank] [Full, <StartY> <StartX> <SizeY> <SizeX>] <OutputFileName>

  Args:
    [Logic]: selects the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified.
    [BlockRAM]: selects the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [Blank]: blanks the selected regions to zeroes before writing the actual bitstream to them.
    [Full]: selects all frames in the chip.
    [<StartY> <StartX> <SizeY> <SizeX>]: selects a rect (<StartY> <StartX> <SizeY> <SizeX>) for output.
                                       : also the list of selected regions will be output (see "byteman -help Region").
    <OutputFileName>: name of the output file. supports ".bit" and ".bin" file formats.

  Examples:
    # Output only the blockram contents to the whole FPGA:
    byteman.exe XUSP -input bitstream.bit -... -output bram, full bitfil.bit

    # The rectangle describing Y from 50 to 99 and X from 10 to 29 is zeroed and then the logic plane bitstream data is written:
    byteman.exe Xilinx Series 7 -... -o logic, blank 50:10 50:20 bitstream.bin

    # The rectangles (60:10 120:20 and 180:30 240:40) are zeroed and the logic & blockram content planes are written:
    byteman.exe XUS -... -r add 60:10 120:20 -o blank 180:30 240:40 fo.bit
```


## $> byteman.exe -help region
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  modifies the list of selected regions.

  Usage:
    byteman.exe -region [Clear, Add <SrcY> <SrcX> <SizeY> <SizeX>]

  Args:
    [Clear]: clears the selected regions list.
    [Add]: adds a new rect (<SrcY>:<SrcX> <SizeY>:<SizeX>) in the selected regions list.

  Examples:
    # Clears the list:
    byteman.exe XUSP -region clear

    # Adds a rectangle describing Y from 100 to 149 and X from 7 to 21.:
    byteman.exe Xilinx Series 7 -r add 100:7 50:15

    # Adds a rectangle describing Y from 120 to 179 and X from 7 to 21:
    byteman.exe Xilinx US -r add 120:7 60:15.
```


## $> byteman.exe -help stdin
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  continues reading commands from standard input rather than command line arguments. this way commands can be entered by hand or streamed in by another program or file.

  Usage:
    byteman.exe [commands]* -stdin

  the input supports the following format rules:
    all commands are the same as their command line argument counterparts.
    one command (with its arguments) per line.
    '-' at the beginning of a command can be skipped.
    '#' starts a comments until a new line is seen.
    the command Exit will end reading the input stream and close byteman.
```


## $> byteman.exe -help verbose
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  enables/disables logging messages. they are disabled by default.

  Usage:
    byteman.exe -verbose [<VerboseValue>]

  Arg:
    [<VerboseValue>]: accepted values are 0 to disable and 1 to enable. Default is 1.

  Examples:
    # Enable logging:
    byteman.exe XUSP -v

    # Disable logging, but it was disabled by default anyway:
    byteman.exe Xilinx US -verbose 0

    # Enable logging for (-...) and then disable logging again:
    byteman.exe Xilinx Series 7 -v -... -v=0
```


## $> byteman.exe -help warn
```python
byteman 1.3 (Build #216, compiled on Aug  2 2022 20:22:55)
  enables/disables warning messages. they are enabled by default.

  Usage:
    byteman.exe -warn [<WarnValue>]

  Arg:
    [<WarnValue>]: accepted values are 0 to disable and 1 to enable. Default is 1.

  Examples:
    # Enable warnings, but they were enabled by default anyway:
    byteman.exe XUSP -w

    # Disable warnings:
    byteman.exe Xilinx US -warn 0

    # Disable warnings for (-...) and then enable warnings again:
    byteman.exe Xilinx Series 7 -w=0 -... -w
```

