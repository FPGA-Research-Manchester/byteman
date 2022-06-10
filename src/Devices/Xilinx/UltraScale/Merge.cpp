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

#include<algorithm> //replace
#include<string>
#include<sstream>

#include "../../../Common/str.h"

#include "Fabric.h"
//#include "ConfigurationAccessPort.h"
#include "UltraScale.h"


void XilinxUltraScale::merge(XilinxUltraScale* srcBitstream, string params, Rect2D src, Coord2D dst){
	//This function uses a pointer to the second XUSP instance, but this pointer should get optimized as it is statically provided in the main function
	parseParams(params);
	ensureRegionCompatibility(src, dst);
	
	if(src.position.row%XUS_CLB_PER_CLOCK_REGION != 0)
		throw runtime_error(string("Currently only full clock region height relocations are supported (use row numbers multiple of ").append(to_string(XUS_CLB_PER_CLOCK_REGION)).append(")."));
	if(src.size.row%XUS_CLB_PER_CLOCK_REGION != 0)
		throw runtime_error(string("Currently only full clock region height relocations are supported (use row numbers multiple of ").append(to_string(XUS_CLB_PER_CLOCK_REGION)).append(")."));
	if(dst.row%XUS_CLB_PER_CLOCK_REGION != 0)
		throw runtime_error(string("Currently only full clock region height relocations are supported (use row numbers multiple of ").append(to_string(XUS_CLB_PER_CLOCK_REGION)).append(")."));
	
	Endianness endianDifference = Endian::diff(loadedBitstreamEndianness, srcBitstream->loadedBitstreamEndianness);
	
	if((Endianness::NATIVE == endianDifference) && MergeOP::SET == selectedOptions.op)
		fastMerge(srcBitstream, src, dst);
	else
		flexiMerge(srcBitstream, endianDifference, src, dst);
}

