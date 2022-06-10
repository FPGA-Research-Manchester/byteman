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
#include "UltraScale.h"

#include<iostream>

void XilinxUltraScale::setFabricChar(string tileName, char character, int frameNumber, bool isUnused){
	
	LUT_numberOfFramesForResourceLetter[(uint8_t)character] = frameNumber;
	LUT_isFrameUnusedForResourceLetter[(uint8_t)character]  = isUnused;
	LUT_typeOfFrameForResourceLetter[(uint8_t)character]    = tileName;
}

void XilinxUltraScale::initFabric(){
	//The Xilinx US fabric in byteman is encoded in the following letters:
	setFabricChar("INT",     'S', 58);
	
	setFabricChar("CLEL_L",  'K', 12);
	setFabricChar("CLEL_R",  'P', 12);
	
	setFabricChar("CLE_M",   'X', 12);
	setFabricChar("CLE_M_R", 'W', 12);
	
	setFabricChar("BRAM",    'B', 4); //+128 in blocktype 1
	setFabricChar("DSP6",    'D', 6);
	
	setFabricChar("DSP4",    'E', 4);
	
	setFabricChar("GTY_QUAD_LEFT_FT",     'a', 12);
	setFabricChar("LAGUNA_TILE",          'b', 12);
	setFabricChar("INT_INTERFACE_PCIE_R", 'c', 6);
	setFabricChar("ILMAC_ILMAC_FT",       'd', 2);
	setFabricChar("HPIO_L",               'e', 10);
	setFabricChar("XIPHY_L",              'f', 16);
	setFabricChar("HRIO_L",               'g', 10);
	setFabricChar("PCIE",                 'h', 2);
	setFabricChar("GTH_R",                'i', 12);
	setFabricChar("CMAC_CMAC_FT",         'j', 2);
	setFabricChar("CFG_CFG",              'k', 2);
	setFabricChar("CFGIO_IOB",            'l', 2);
	setFabricChar("GTH_QUAD_LEFT_FT",     'm', 16);
	
	//Also in the fabric exist frames that are mapped in bitstreams, but do not physically exist on the device. We encode these as follows:
	setFabricChar("EMPTY10",  '0', 10,  true);
}
