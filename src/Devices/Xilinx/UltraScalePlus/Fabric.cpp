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

#include "Fabric.h"
//#include "ConfigurationAccessPort.h"
#include "UltraScalePlus.h"

#include<iostream>

void XilinxUltraScalePlus::setFabricChar(string tileName, char character, int frameNumber, bool isUnused){
	
	LUT_numberOfFramesForResourceLetter[(uint8_t)character] = frameNumber;
	LUT_isFrameUnusedForResourceLetter[(uint8_t)character]  = isUnused;
	LUT_typeOfFrameForResourceLetter[(uint8_t)character]    = tileName;
}
void XilinxUltraScalePlus::initFabric(){
	//The Xilinx US+ fabric in byteman is encoded in the following letters:
	setFabricChar("INT",    'S', 76);
	setFabricChar("CLEM",   'M', 16);
	setFabricChar("CLEM_R", 'N', 16);
	setFabricChar("CLEL_L", 'K', 16);
	setFabricChar("CLEL_R", 'P', 16);
	setFabricChar("BRAM",   'B', 6); //+256 in blocktype 1
	setFabricChar("DSP",    'D', 8);
	
	setFabricChar("INT_INTF_R_PCIE4",         'a', 4);
	setFabricChar("HDIO_BOT_RIGHT",           'b', 10);
	setFabricChar("INT_INTF_L_PCIE4",         'c', 4);
	setFabricChar("INT_INTF_RIGHT_TERM_IO",   'd', 4);
	setFabricChar("CMT_RIGHT",                'e', 12);
	setFabricChar("CMT_LEFT_H",               'G', 12);
	setFabricChar("CMT_L",                    'H', 12);
	setFabricChar("XIPHY_BYTE_RIGHT",         'f', 4);
	setFabricChar("XIPHY_BYTE_L",             'I', 4);
	setFabricChar("HPIO_RIGHT",               'g', 8);
	setFabricChar("HPIO_L",                   'J', 8);
	setFabricChar("GTH_QUAD_LEFT",            'h', 8);
	setFabricChar("GTM_DUAL_LEFT_FT",         'O', 8);
	setFabricChar("INT_INTF_L_TERM_GT",       'i', 4);
	setFabricChar("INT_INTF_R_TERM_GT",       'j', 4);
	setFabricChar("GTH_QUAD_RIGHT",           'k', 8);
	setFabricChar("GTM_DUAL_RIGHT_FT",        'Q', 8);
	setFabricChar("CFGIO_IOB20",              'l', 10);
	setFabricChar("CFG_CONFIG",               'm', 10);
	setFabricChar("INT_INTF_L_CMT",           'n', 4);
	setFabricChar("PCIE4_PCIE4_FT",           'o', 10);
	setFabricChar("PCIE4C_PCIE4C_FT",         'T', 10);
	setFabricChar("INT_INTF_LEFT_TERM_PSS",   'p', 16);
	setFabricChar("URAM_URAM_FT",             'q', 9);
	setFabricChar("INT_INTF_LEFT_TERM_IO_FT", 'r', 4);
	setFabricChar("GTY_L",                    's', 8);
	setFabricChar("CMAC",                     't', 10);
	setFabricChar("ILKN_ILKN_FT",             'u', 10);
	setFabricChar("FE_FE_FT",                 'v', 2);
	setFabricChar("HSADC_HSADC_RIGHT_FT",     'w', 8);
	setFabricChar("HSDAC_HSDAC_RIGHT_FT",     'U', 8);
	setFabricChar("RFADC_RFADC_RIGHT_FT",     'Z', 8);
	setFabricChar("RFDAC_RFDAC_RIGHT_FT",     '-', 8);
	setFabricChar("LAG_LAG",                  'x', 16);
	setFabricChar("GTY_R",                    'y', 8);
	setFabricChar("BLI_BLI_FT",               'z', 8);
	
	//Also in the fabric exist frames that are mapped in bitstreams, but do not physically exist on the device. We encode these as follows:
	setFabricChar("EMPTY4",  '0', 4,  true);
	setFabricChar("EMPTY6",  '1', 6,  true); //and 256 empty frames in block type 1
	setFabricChar("EMPTY8",  '2', 8,  true);
	setFabricChar("EMPTY9",  '3', 9,  true);
	setFabricChar("EMPTY10", '4', 10, true);
	setFabricChar("EMPTY12", '5', 12, true);
	setFabricChar("EMPTY16", '6', 16, true);
	setFabricChar("EMPTY76", '7', 76, true);
	setFabricChar("EMPTY2",  '8', 2,  true);
}