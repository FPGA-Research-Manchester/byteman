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

#include<string>
#include<stdexcept>
#include<fstream>

#include "XilinxSeries7.h"
#include "../../../Common/FileIO.h"
#include "../../../Common/str.h"

using namespace std;

void XilinxSeries7::readBitstream(string filename)
{
	ifstream fin (filename, ifstream::binary);
	if(!fin.good())
		throw runtime_error(string("Could not open file: \"").append(filename).append("\" .\n"));
	log("Reading Xilinx Series 7 bitstream from file \"" + filename + "\"");
	
	if(str::iff::stringEndsWith(filename, ".bit"))
		readBitstreamBIT(fin);
	else if(str::iff::stringEndsWith(filename, ".bin"))
		readBitstreamBIN(fin);
	else if(str::iff::stringEndsWith(filename, ".rbd"))
		readBitstreamRBD(fin);
	else
		throw runtime_error(string("Unknown Xilinx Series 7 file format tried to be read.\n"));
	fin.close();
	log("Xilinx Series 7 bitstream file \"" + filename + "\" read successfully.");
}
