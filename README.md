# ![byteman icon](./Resources/icon32.png) byteman
## *Open-source high-performance bitstream relocation and manipulation tool.*
[![byteman project](https://img.shields.io/badge/byteman-v1.2-blue)](https://github.com/FPGA-Research-Manchester/byteman)
[![byteman build](https://img.shields.io/badge/Build%20197-%E2%9C%94-green)](https://github.com/FPGA-Research-Manchester/byteman/commits/main)
[![Apache license](https://img.shields.io/github/license/FPGA-Research-Manchester/byteman)](https://opensource.org/licenses/Apache-2.0)
[![C++ Standard](https://img.shields.io/badge/C%2B%2B-14-green.svg)](https://en.wikipedia.org/wiki/C%2B%2B14)
[![Reports from automated testing](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/test.yml/badge.svg)](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/test.yml)

## ![byteman icon](./Resources/icon16.png) Download executable: [![Download windows x86 binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseWindows.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Windows-x86/byteman.exe) [![Download linux x86 binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseLinux.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Linux-x86/byteman) [![Download linux ARM binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseLinuxARM.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Linux-ARM/byteman) [![Download linux RISC-V binary](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/releaseLinuxRISCV.yml/badge.svg)](https://raw.githubusercontent.com/FPGA-Research-Manchester/byteman/main/Binaries/Linux-RISC-V/byteman)

## ![byteman icon](./Resources/icon16.png) Check documentation: [![Browse generated docs](https://github.com/FPGA-Research-Manchester/byteman/actions/workflows/generateDocs.yml/badge.svg)](https://FPGA-Research-Manchester.github.io/byteman/)


## $> byteman.exe -help
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
Usage:
  byteman.exe ARCH [-command...]* [-stdin]

  ARCH = {Xilinx Series 7, Xilinx UltraScale, Xilinx UltraScale+}

Commands:        : [Description]                                    
  -a (assembly)  : (de)compile bitstream assembly files. See "-help assembly"
  -b (blank)     : Erases bitstream buffers. See "-help blank"
  -c (change)    : Modify primitives (i.e., LUT contents). See "-help change"
  -d (device)    : Manually select target device. See "-help device"
  -e (exit)      : Exit byteman. Useful during stdin. See "-help exit"
  -h (help)      : Get help. See "-help help"
  -i (input)     : Input bitstream. See "-help input"
  -m (merge)     : Merge bitstreams. See "-help merge"
  -o (output)    : Output bitstream. See "-help output"
  -r (region)    : Modify region selection. See "-help region"
  -s (stdin)     : Continue using standard input. See "-help stdin"
  -v (verbose)   : Enable/disable spam. See "-help verbose"
  -w (warn)      : Enable/disable warnings. See "-help warn"

Examples: 
  byteman.exe Xilinx US+ -w -i main static.bit -i temp filter.bit -merge logic 180:67 120:30 240:37 -o logic 240:37 120:30 relocatedFilter.bit
  byteman.exe XUSP -w -d zcu102 -blank main logic blockram -i temp filter.bit -merge logic 180:67 120:30 180:67 -o logic 180:67 120:30 mergedFilter.bit

Need more help? Try one of these:
  byteman.exe -help <command>
  byteman.exe -help all
  byteman.exe Xilinx Series 7 -help all
  byteman.exe Xilinx UltraScale -help all
  byteman.exe Xilinx UltraScale+ -help all
```


## $> byteman.exe -help assembly
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Assembly
  Assembles and disassembles between bitstream files (.bit|.bin) and bitstream assembly (.bitasm) format.

  Usage:
    byteman.exe ARCH -assembly bitstreamFile.(bit|bin) bitstreamAssemblyFile.bitasm
    byteman.exe ARCH -assembly bitstreamAssemblyFile.bitasm bitstreamFile.(bit|bin)

  The bitstream assembly files can be edited and converted back to valid bitstreams.
  In bitstream assembly file format:
    Every line that doesn't start with '#' is editable.
    Bitstream commands are written one per line and have the format "<COMMAND NAME>;"
    Bitstream register writesare one per line and have the format "@<REGNAME> = <NEW REG VALUE>"

  Examples:
    byteman.exe XUSP -assembly bitstream.bit bitstream.bitasm

    byteman.exe Xilinx Series 7 -a bitstream.bitasm bitstream.bit

    # Generate a bitstream file and then convert it to bit assembly:
    byteman.exe Xilinx US -... -o bitstreamOut.bit -a bitstreamOut.bit bitstreamOut.bitasm
```


## $> byteman.exe -help blank
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Blank
  Erases/sets the bitstream in the buffers. Uses the list of selected regions (see "-help region"). If the list is empty, works on the whole chip.

  Usage:
    byteman.exe ARCH -blank [first] [second] [logic] [BlockRAM] [blankValue]

  Options:
    [first]: Blanks in the first of the two buffers. Enabled by default if no buffer is explicitly specified.
    [second]: Blanks in the second of the two buffers.
    [logic]: Will blank logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified.
    [BlockRAM]: Will blank BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [blankValue]: Byte value (0-0xFF) to be written in the selected bitstream location. Default is 0.

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
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Change
  Not public at this point.
```


## $> byteman.exe -help device
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Device
  Sets the target device manually.

  Usage:
    byteman.exe ARCH -device [first] [second] <deviceName>

  Options:
    [first]: Sets the device for the first of the two buffers. Enabled by default if no buffer is explicitly specified.
    [second]: Sets the device for the second of the two buffers.
    [<deviceName>]: Name of the target FPGA chip or board.

  Examples:
    byteman.exe XUSP -device AlveoU200
    byteman.exe Xilinx Series 7 -d xc7z045
    byteman.exe Xilinx US -d XCVU440

A list of currently recognized Series 7 devices: 
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
A list of currently recognized US devices: 
  UltraScale Kintex: 
    XCKU025, XCKU035, XCKU040, XQKU040, XCKU060, XQKU060, XQRKU060, XCKU085, XCKU095, XQKU095, XCKU115, XQKU0115
  UltraScale Virtex: 
    XCVU065, XCVU080, XCVU095, XCVU125, XCVU160, XCVU190, XCVU440
A list of currently recognized US+ devices: 
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
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Exit
  Finishes execution of byteman. Used with STDin mode (see "-help stdin") to identify the end of command sequence.
```


## $> byteman.exe -help help
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Help
  Prints these messages... Help messages can change if architecture and/or specific device is selected to incorporate more information!

  Usage:
    byteman.exe ARCH -help [command]

  Options:
    [command]: Prints extra information about the usage of a command.

  Examples:
    byteman.exe Xilinx US+ -device AlveoU200  -help device #will print extra information about the selected device
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
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Input
  Reads a bitstream from a file.

  Usage:
    byteman.exe ARCH -input [first] [second] bitstreamFile.(bit|bin)

  Options:
    [first]: Reads the bitstream file into the first of the two buffers. Enabled by default if no buffer is explicitly specified.
    [second]: Reads the bitstream file into the second of the two buffers.

  Examples:
    byteman.exe XUSP -i main zcu102bitstream.bit
    byteman.exe Xilinx Series 7 -input zedboardBitstream.bin
    byteman.exe Xilinx US -i xcvu440Bitstream.bit
```


## $> byteman.exe -help merge
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Merge
  Merges portion of the bitstream in the second buffer into the first buffer.

  Usage:
    byteman.exe ARCH -merge [logic] [BlockRAM] [clock] [set,xor,or,and] [full,<srcY> <srcX> <sizeY> <sizeX> <dstY> <dstX>]

  Options:
    [logic]: Will merge the logic plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [clock]: Will merge the global clock data from the logic plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [BlockRAM]: Will merge the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [set,xor,or,and]: Performed merge operation. XOR, OR, AND are bitwise operations. Default is "set".
    [full]: Merges the full device bitstreams.
    [<srcY> <srcX> <sizeY> <sizeX> <dstY> <dstX>]: Alternatively, a rectangle could be selected for merging. A source rectangle
                                                 : (<srcY> <srcX> <sizeY> <sizeX>) from the second bitstream buffer is merged
                                                 : into destination rectangle (<dstY> <dstX> <sizeY> <sizeX>) in first bitstream buffer.

  Examples:
    # Copies second bitstream into first bitstream's buffer:
    byteman.exe XUSP -input bit1.bit -input second bit2.bin -merge full

    # A rect in second buffer (Y from 50 to 99 and X from 10 to 29) is bitwise XORed in the clock and blockram
    # contents planes into a rect in 1st buffer describing Y from 100 to 149 and X from 30 to 49:
    byteman.exe Xilinx Series 7 -... -merge clock,bram,xor 50:10 50:20 100:30

    # Copies the logic plane from buffer 2 into buffer 1:
    byteman.exe Xilinx US -... -m full,logic
```


## $> byteman.exe -help output
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Output
  Creates a bitstream file output from the first bitstream buffer in byteman.

  Usage:
    byteman.exe ARCH -output [logic] [BlockRAM] [blank] [full,<startY> <startX> <sizeY> <sizeX>] bitstreamFile.(bit|bin)

  Options:
    [logic]: Will output the logic plane (incl global clock) of the bitstream. Enabled by default if no plane is explicitly specified.
    [BlockRAM]: Will merge the BlockRAM contents plane of the bitstream. Enabled by default if no plane is explicitly specified.
    [blank]: Blank the selected regions to zeroes before writing the actual bitstream to them.
    [full]: Outputs the bitstream for all frames in the FPGA.
    [<startY> <startX> <sizeY> <sizeX>]: Alternatively, a rectangle (<startY> <startX> <sizeY> <sizeX>) could be selected for output.
                                       : In this mode, also the list of selected regions will be output (see "byteman -help region").

  Examples:
    # Output only the blockram contents to the whole FPGA:
    byteman.exe XUSP -input bitstream.bit -... -output bram,full bitfil.bit

    # The rectangle describing Y from 50 to 99 and X from 10 to 29 is zeroed and then the logic plane bitstream data is written:
    byteman.exe Xilinx Series 7 -... -o logic,blank 50:10 50:20 bitstream.bin

    # The rectangles (60:10 120:20 and 180:30 240:40) are zeroed and the logic & blockram content planes are written:
    byteman.exe XUS -... -r add 60:10 120:20 -o blank 180:30 240:40 fo.bit
```


## $> byteman.exe -help region
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Region
  Modifies the list of selected regions.

  Usage:
    byteman.exe ARCH -region [clear,add <srcY> <srcX> <sizeY> <sizeX>]

  Options:
    [clear]: Clears the selected regions list.
    [add]: Adds a new rectangle in the selected regions list. The rectangle is described by coordinates <srcY>:<srcX> and rectangle size of <sizeY>:<sizeX>.

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
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: STDin
  Continues reading commands from standard input rather than command line arguments. This way commands can be entered by hand or streamed in by another program or file.

  Usage:
    byteman.exe ARCH [commands]* -stdin

  The input supports the following format rules:
    All commands are the same as their command line argument counterparts.
    One command (with its arguments) per line.
    '-' at the beginning of a command can be skipped.
    '#' starts a comments until a new line is seen.
    The command "Exit" will end reading the input stream and close byteman.
```


## $> byteman.exe -help verbose
```python
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Verbose
  Enables/disables logging messages. They are disabled by default.

  Usage:
    byteman.exe ARCH -verbose [verboseValue]

  Options:
    [verboseValue]: Accepted values are 0 to disable and 1 to enable. Default is 1.

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
byteman 1.2 (Build #197, compiled on Jul  6 2022 17:43:43)
byteman: Warn
  Enables/disables warning messages. They are enabled by default.

  Usage:
    byteman.exe ARCH -warn [warnValue]

  Options:
    [warnValue]: Accepted values are 0 to disable and 1 to enable. Default is 1.

  Examples:
    # Enable warnings, but they were enabled by default anyway:
    byteman.exe XUSP -w

    # Disable warnings:
    byteman.exe Xilinx US -warn 0

    # Disable warnings for (-...) and then enable warnings again:
    byteman.exe Xilinx Series 7 -w=0 -... -w
```

