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

#ifndef XILINXCONFIGURATIONACCESSPORT_H
#define XILINXCONFIGURATIONACCESSPORT_H

#include<iostream>
#include<cstdint> //uint
#include<fstream>
#include<string>
#include "../CommonDevice.h"
#include "../../Common/Endianess.h"
using namespace std;

class XilinxConfigurationAccessPort: virtual public CommonDevice
{
	public:
        XilinxConfigurationAccessPort();
        virtual ~XilinxConfigurationAccessPort();
		//
		void updateDateAndTime();
		
		//Input
		void parseBITheader(ifstream&, Endianess);
		Endianess parseBitstreamEndianess(ifstream&);
		uint32_t parseBitstreamIDCODE(ifstream&, Endianess);
		void findBitstreamSync(ifstream&, Endianess);
		
		//Output
		streamoff outputBITheader(ofstream&, Endianess=Endianess::NATIVE);
		void outputBITheaderLengthField(ofstream&, streamoff, Endianess=Endianess::NATIVE);
		
		void outputCAPheaderConstant(ofstream&, Endianess=Endianess::NATIVE);
		
		//bitstream meta
        string designName;
		string partName;
		string fileDate;
		string fileTime;
		
		//Family-specific
		virtual uint32_t XCAP_SyncInstruction() = 0;
		virtual uint32_t XCAP_IDCODEInstruction() = 0;
};

#endif //XILINXCONFIGURATIONACCESSPORT_H