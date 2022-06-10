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

#ifdef FABRIC_H
	#error "You should not include fabric definition twice in an object."
#endif
#define FABRIC_H

#define FABRIC_WORDS_BEFORE_CLK 40
#define FABRIC_WORDS_AT_CLK 1
#define FABRIC_WORDS_AFTER_CLK 40
#define FABRIC_WORDS_PER_FRAME (FABRIC_WORDS_BEFORE_CLK+FABRIC_WORDS_AT_CLK+FABRIC_WORDS_AFTER_CLK)
#define FABRIC_FRAMES_PER_BRAM_CONTENT_COLUMN 128
#define FABRIC_CLB_PER_CLOCK_REGION 40
#define FABRIC_BRAM_PER_CLOCK_REGION 10
#define FABRIC_EXTRA_FRAMES_PER_ROW 2

/**
	The Xilinx US fabric in byteman is encoded in the following letters:
	
	
    "UltraScale": {
      //"L": 12,
      //"R": 12,
      //"M": 12,
      //"Y": 12,
      //"N": 12,
      //"V": 12,
      //"A": 4,
      //"C": 4,
      //"F": 6,
	  
	  
	  
 - "S": "INT" resource represented by 58 frames
 - "K": "CLEL_L" resource represented by 12 frames
 - "P": "CLEL_R" resource represented by 12 frames
 - "X": "CLE_M" resource represented by 12 frames
 - "W": "CLE_M_R" resource represented by 12 frames
 - "B": "BRAM" resource represented by 4 frames
 - "D": "DSP" resource represented by 6 frames
 - "E": "DSP_R" resource represented by 6 frames
 - "F": "DSP_L" resource represented by 4 frames

 - "a": "GTY_QUAD_LEFT_FT" resource represented by 12 frames
 - "b": "LAGUNA_TILE" resource represented by 12 frames
 - "c": "INT_INTERFACE_PCIE_R" resource represented by 6 frames
 - "d": "ILMAC_ILMAC_FT" resource represented by 2 frames
 - "e": "HPIO_L" resource represented by 10 frames
 - "f": "XIPHY_L" resource represented by 16 frames
 - "g": "HRIO_L" resource represented by 10 frames
 - "h": "PCIE" resource represented by 2 frames
 - "i": "GTH_R" resource represented by 12 frames
 - "j": "CMAC_CMAC_FT" resource represented by 2 frames
 - "k": "CFG_CFG" resource represented by 2 frames
 - "l": "CFGIO_IOB" resource represented by 2 frames
 - "m": "GTH_QUAD_LEFT_FT" resource represented by 16 frames

Also in the fabric exist frames that are mapped in bitstreams, but do not physically exist on the device. We encode these as follows:
 - "0": "EMPTY4" (unused 4 frames)
 - "1" - "9": Reserved for more unused frames
*/

/*

#define XUS_LOOKUP_FRAME_SIZES \
{0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, \
 0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0, 0,  0,  8,  0,  0,  4,  6, 8,  9,  10, 12, 16, 76, 2,  0,  0, 0, 0, 0, 0, 0, \
 0, 6, 6,  6, 8, 8,  8, 12, 12, 4, 8, 16, 16, 16, 16, 8,  16, 8, 16, 76, 10, 8,  16, 16, 16, 16, 8, 0, 0, 0, 0, 0, \
 0, 4, 10, 4, 4, 12, 4, 8,  8,  4, 4, 8,  10, 10, 4,  10, 16, 9, 4,  8,  10, 10, 2,  8,  16, 8,  8, 0, 0, 0, 0, 0, \
 0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, \
 0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, \
 0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0, \
 0, 0, 0,  0, 0, 0,  0, 0,  0,  0, 0, 0,  0,  0,  0,  0,  0,  0, 0,  0,  0,  0,  0,  0,  0,  0,  0, 0, 0, 0, 0, 0};


#define XUS_LOOKUP_FRAME_NAMES \
{"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "RFDAC_RFDAC_RIGHT_FT", "NIL", "NIL", "NULL4", "NULL6", "NULL8", "NULL9", "NULL10", "NULL12", "NULL16", "NULL76", "NULL2", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "BRAM_L", "BRAM", "BRAM_R", "DSP", "DSP_R", "DSP_L", "CMT_LEFT_H", "CMT_L", "XIPHY_BYTE_L", "HPIO_L", "CLEL_L", "CLBLL_L", "CLEM", "CLEM_R", "GTM_DUAL_LEFT_FT", "CLEL_R", "GTM_DUAL_RIGHT_FT", "CLBLL_R", "INT", "PCIE4C_PCIE4C_FT", "HSDAC_HSDAC_RIGHT_FT", "CLBLM_R", "CLE_M_R", "CLE_M", "CLBLM_L", "RFADC_RFADC_RIGHT_FT", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "INT_INTF_R_PCIE4", "HDIO_BOT_RIGHT", "INT_INTF_L_PCIE4", "INT_INTF_RIGHT_TERM_IO", "CMT_RIGHT", "XIPHY_BYTE_RIGHT", "HPIO_RIGHT", "GTH_QUAD_LEFT", "INT_INTF_L_TERM_GT", "INT_INTF_R_TERM_GT", "GTH_QUAD_RIGHT", "CFGIO_IOB20", "CFG_CONFIG", "INT_INTF_L_CMT", "PCIE4_PCIE4_FT", "INT_INTF_LEFT_TERM_PSS", "URAM_URAM_FT", "INT_INTF_LEFT_TERM_IO_FT", "GTY_L", "CMAC", "ILKN_ILKN_FT", "FE_FE_FT", "HSADC_HSADC_RIGHT_FT", "LAG_LAG", "GTY_R", "BLI_BLI_FT", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", \
"NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL", "NIL"};


#define XUS_LOOKUP_FRAME_UNUSED \
{false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, true,  true,  true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, \
 false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false};

*/

