/******************************************************************************
 * Copyright 2022 Kristiyan Manev, Joseph Powell (University of Manchester)
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
#ifndef XILINXULTRASCALEPLUSFABRIC_H
#define XILINXULTRASCALEPLUSFABRIC_H

#define XUSP_MAX_SLRS 4
#define XUSP_MAX_ROWS 20
//692 is currently identified widest row
#define XUSP_MAX_COLS 1024
//about 12 is currently identified widest row in terms of BRAMs
#define XUSP_MAX_BRAM_COLS 32

#define XUSP_WORDS_BEFORE_CLK 45
#define XUSP_WORDS_AT_CLK 3
#define XUSP_WORDS_AFTER_CLK 45
#define XUSP_WORDS_PER_FRAME (XUSP_WORDS_BEFORE_CLK+XUSP_WORDS_AT_CLK+XUSP_WORDS_AFTER_CLK)
#define XUSP_FRAMES_PER_BRAM_CONTENT_COLUMN 256
#define XUSP_CLB_PER_CLOCK_REGION 60
#define XUSP_EXTRA_FRAMES_PER_ROW 2

/**
	The Xilinx US+ fabric in byteman is encoded in the following letters:
 - "S": "INT" resource represented by 76 frames
 - "M": "CLEM" resource represented by  16 frames
 - "X": "CLE_M" resource represented by 16 frames
 - "Y": "CLBLM_L" resource represented by 16 frames
 - "N": "CLEM_R" resource represented by 16 frames
 - "W": "CLE_M_R" resource represented by 16 frames
 - "V": "CLBLM_R" resource represented by 16 frames
				 
 - "L": "CLBLL_L" resource represented by 16 frames
 - "K": "CLEL_L" resource represented by 16 frames
 - "R": "CLBLL_R" resource represented by 16 frames
 - "P": "CLEL_R" resource represented by 16 frames
				 
 - "B": "BRAM" resource represented by 6 frames
 - "A": "BRAM_L" resource represented by 6 frames
 - "C": "BRAM_R" resource represented by 6 frames
 - "D": "DSP" resource represented by 8 frames
 - "E": "DSP_R" resource represented by 8 frames
 - "F": "DSP_L" resource represented by 8 frames

 - "a": "INT_INTF_R_PCIE4" resource represented by 4 frames
 - "b": "HDIO_BOT_RIGHT" resource represented by 10 frames
 - "c": "INT_INTF_L_PCIE4" resource represented by 4 frames
 - "d": "INT_INTF_RIGHT_TERM_IO" resource represented by 4 frames
 - "e": "CMT_RIGHT" resource represented by 12 frames
 - "G": "CMT_LEFT_H" resource represented by 12 frames
 - "H": "CMT_L" resource represented by 12 frames
 - "f": "XIPHY_BYTE_RIGHT" resource represented by 4 frames
 - "I": "XIPHY_BYTE_L" resource represented by 4 frames
 - "g": "HPIO_RIGHT" resource represented by 8 frames
 - "J": "HPIO_L" resource represented by 8 frames
 - "h": "GTH_QUAD_LEFT" resource represented by 8 frames
 - "O": "GTM_DUAL_LEFT_FT" resource represented by 8 frames
 - "i": "INT_INTF_L_TERM_GT" resource represented by 4 frames
 - "j": "INT_INTF_R_TERM_GT" resource represented by 4 frames
 - "k": "GTH_QUAD_RIGHT" resource represented by 8 frames
 - "Q": "GTM_DUAL_RIGHT_FT" resource represented by 8 frames
 - "l": "CFGIO_IOB20" resource represented by 10 frames
 - "m": "CFG_CONFIG" resource represented by 10 frames
 - "n": "INT_INTF_L_CMT" resource represented by 4 frames
 - "o": "PCIE4_PCIE4_FT" resource represented by 10 frames
 - "T": "PCIE4C_PCIE4C_FT" resource represented by 10 frames
 - "p": "INT_INTF_LEFT_TERM_PSS" resource represented by 16 frames
 - "q": "URAM_URAM_FT" resource represented by 9 frames
 - "r": "INT_INTF_LEFT_TERM_IO_FT" resource represented by 4 frames
 - "s": "GTY_L" resource represented by 8 frames
 - "t": "CMAC" resource represented by 10 frames
 - "u": "ILKN_ILKN_FT" resource represented by 10 frames
 - "v": "FE_FE_FT" resource represented by 2 frames
 - "w": "HSADC_HSADC_RIGHT_FT" resource represented by 8 frames
 - "U": "HSDAC_HSDAC_RIGHT_FT" resource represented by 8 frames
 - "Z": "RFADC_RFADC_RIGHT_FT" resource represented by 8 frames
 - "-": "RFDAC_RFDAC_RIGHT_FT" resource represented by 8 frames
 - "x": "LAG_LAG" resource represented by 16 frames
 - "y": "GTY_R" resource represented by 8 frames
 - "z": "BLI_BLI_FT" resource represented by 8 frames

Also in the fabric exist frames that are mapped in bitstreams, but do not physically exist on the device. We encode these as follows:
 - "0": "EMPTY4" (unused 4 frames)
 - "1": "EMPTY6" (unused 6 frames, and 256 in block type 1)
 - "2": "EMPTY8" (unused 8 frames)
 - "3": "EMPTY9" (unused 9 frames)
 - "4": "EMPTY10" (unused 10 frames)
 - "5": "EMPTY12" (unused 12 frames)
 - "6": "EMPTY16" (unused 16 frames)
 - "7": "EMPTY76" (unused 76 frames)
 - "8": "EMPTY2" (unused 2 frames)
*/
#define XUSP_LOOKUP_FRAME_SIZES \
{0, 0, 0 , 0, 0, 0 , 0, 0 , 0 , 0, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0, 0, 0, 0, 0, \
 0, 0, 0 , 0, 0, 0 , 0, 0 , 0 , 0, 0, 0 , 0 , 8 , 0 , 0 , 4 , 6, 8 , 9 , 10, 12, 16, 76, 2 , 0 , 0, 0, 0, 0, 0, 0, \
 0, 6, 6 , 6, 8, 8 , 8, 12, 12, 4, 8, 16, 16, 16, 16, 8 , 16, 8, 16, 76, 10, 8 , 16, 16, 16, 16, 8, 0, 0, 0, 0, 0, \
 0, 4, 10, 4, 4, 12, 4, 8 , 8 , 4, 4, 8 , 10, 10, 4 , 10, 16, 9, 4 , 8 , 10, 10, 2 , 8 , 16, 8 , 8, 0, 0, 0, 0, 0, \
 0, 0, 0 , 0, 0, 0 , 0, 0 , 0 , 0, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0, 0, 0, 0, 0, \
 0, 0, 0 , 0, 0, 0 , 0, 0 , 0 , 0, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0, 0, 0, 0, 0, \
 0, 0, 0 , 0, 0, 0 , 0, 0 , 0 , 0, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0, 0, 0, 0, 0, \
 0, 0, 0 , 0, 0, 0 , 0, 0 , 0 , 0, 0, 0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0, 0, 0, 0, 0, 0};

#define XUSP_LOOKUP_FRAME_NAMES \
{"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "RFDAC_RFDAC_RIGHT_FT", "NIL", "NIL", "NULL4", "NULL6", "NULL8", "NULL9", "NULL10", "NULL12", "NULL16", "NULL76", "NULL2", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "BRAM_L", "BRAM", "BRAM_R", "DSP", "DSP_R", "DSP_L", "CMT_LEFT_H", "CMT_L", "XIPHY_BYTE_L", "HPIO_L", "CLEL_L", "CLBLL_L", "CLEM", "CLEM_R", "GTM_DUAL_LEFT_FT", "CLEL_R", "GTM_DUAL_RIGHT_FT", "CLBLL_R", "INT", "PCIE4C_PCIE4C_FT", "HSDAC_HSDAC_RIGHT_FT", "CLBLM_R", "CLE_M_R", "CLE_M", "CLBLM_L", "RFADC_RFADC_RIGHT_FT", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "INT_INTF_R_PCIE4", "HDIO_BOT_RIGHT", "INT_INTF_L_PCIE4", "INT_INTF_RIGHT_TERM_IO", "CMT_RIGHT", "XIPHY_BYTE_RIGHT", "HPIO_RIGHT", "GTH_QUAD_LEFT", "INT_INTF_L_TERM_GT", "INT_INTF_R_TERM_GT", "GTH_QUAD_RIGHT", "CFGIO_IOB20", "CFG_CONFIG", "INT_INTF_L_CMT", "PCIE4_PCIE4_FT", "INT_INTF_LEFT_TERM_PSS", "URAM_URAM_FT", "INT_INTF_LEFT_TERM_IO_FT", "GTY_L", "CMAC", "ILKN_ILKN_FT", "FE_FE_FT", "HSADC_HSADC_RIGHT_FT", "LAG_LAG", "GTY_R", "BLI_BLI_FT", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL"};

#endif // XILINXULTRASCALEPLUSFABRIC_H
