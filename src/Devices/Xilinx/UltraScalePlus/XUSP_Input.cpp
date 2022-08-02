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
#include<string>
#include<stdexcept>
#include<fstream>
#include<cstring>

#include "XilinxUltraScalePlus.h"
#include "../../../Common/FileIO.h"
#include "../../../Common/str.h"

using namespace std;

void XilinxUltraScalePlus::readBitstream(string filename)
{
	ifstream fin (filename, ifstream::binary);
	if(!fin.good())
		throw runtime_error(string("Could not open file: \"").append(filename).append("\" .\n"));
	log("Reading Xilinx UltraScale+ bitstream from file \"" + filename + "\"");
	
	if(str::iff::stringEndsWith(filename, ".bit"))
		readBitstreamBIT(fin);
	else if(str::iff::stringEndsWith(filename, ".bin"))
		readBitstreamBIN(fin);
	else
		throw runtime_error(string("Unknown Xilinx UltraScale+ file format tried to be read.\n"));
	fin.close();
	log("Xilinx UltraScale+ bitstream file \"" + filename + "\" read successfully.");
}
