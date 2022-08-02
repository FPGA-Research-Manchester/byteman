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

#include<iostream>
#include<algorithm> //transform
#include<string>
#include<stdexcept>

#include "XilinxUltraScale.h"
#include "XUS_Devices.h"
#include "../../../Common/str.h"

using namespace std;

int XilinxUltraScale::getDeviceByIDCODEorThrow(int IDCODE)
{
	int deviceID = getDeviceByIDCODE(IDCODE);
	if(deviceID == XUS_DEVICE_NULL)
		throw runtime_error(string("Unknown device with IDCODE = ").append(to_string(IDCODE)).append(" . Maybe the device is not Xilinx UltraScale device?"));
	return deviceID;
}

int XilinxUltraScale::getDeviceByNameOrThrow(string name)
{
	int deviceID = getDeviceByName(name);
	if(deviceID == XUS_DEVICE_NULL)
		throw runtime_error(string("Unknown device: ").append(name).append(". Maybe the device is not Xilinx UltraScale device?"));
	return deviceID;
}

void XilinxUltraScale::setDeviceByPartNameOrThrow(){
	setDeviceByNameOrThrow(partName);
}

void XilinxUltraScale::setDeviceByNameOrThrow(string name){
	int deviceID = getDeviceByNameOrThrow(name);
	setDevice(deviceID, name);
}

void XilinxUltraScale::setDeviceByIDCODEOrThrow(int idcode){
	int deviceID = getDeviceByIDCODEorThrow(idcode);
	setDevice(deviceID);
}

int XilinxUltraScale::getDeviceByIDCODE(int IDCODE)
{
	//US Kintex
	#ifdef XUSKINTEX
	if(IDCODE == XCKU025_IDCODE)
		return XUS_DEVICE_XCKU025;
	if(IDCODE == XCKU035_IDCODE)
		return XUS_DEVICE_XCKU035;
	if(IDCODE == XCKU040_IDCODE)
		return XUS_DEVICE_XCKU040;
	if(IDCODE == XCKU060_IDCODE)
		return XUS_DEVICE_XCKU060;
	if(IDCODE == XCKU085_IDCODE)
		return XUS_DEVICE_XCKU085;
	if(IDCODE == XCKU095_IDCODE)
		return XUS_DEVICE_XCKU095;
	if(IDCODE == XCKU115_IDCODE)
		return XUS_DEVICE_XCKU115;
	#endif
	//US Virtex
	#ifdef XUSVIRTEX
	if(IDCODE == XCVU065_IDCODE)
		return XUS_DEVICE_XCVU065;
	if(IDCODE == XCVU080_IDCODE)
		return XUS_DEVICE_XCVU080;
	if(IDCODE == XCVU095_IDCODE)
		return XUS_DEVICE_XCVU095;
	if(IDCODE == XCVU125_IDCODE)
		return XUS_DEVICE_XCVU125;
	if(IDCODE == XCVU160_IDCODE)
		return XUS_DEVICE_XCVU160;
	if(IDCODE == XCVU190_IDCODE)
		return XUS_DEVICE_XCVU190;
	if(IDCODE == XCVU440_IDCODE)
		return XUS_DEVICE_XCVU440;
	#endif
	
	return XUS_DEVICE_NULL;
}
int XilinxUltraScale::getDeviceByName(string name)
{
	name = str::removeSpaces(name);
	name = str::stringToLower(name);
	
	//US Kintex
	#ifdef XUSKINTEX
		if(str::iff::stringContains(name, XCKU025_NAME))
			return XUS_DEVICE_XCKU025;
		if(str::iff::stringContains(name, XCKU035_NAME))
			return XUS_DEVICE_XCKU035;
		if(str::iff::stringContains(name, XCKU040_NAME, "xqku040"))
			return XUS_DEVICE_XCKU040;
		if(str::iff::stringContains(name, XCKU060_NAME, "xqku060", "xqrku060"))
			return XUS_DEVICE_XCKU060;
		if(str::iff::stringContains(name, XCKU085_NAME))
			return XUS_DEVICE_XCKU085;
		if(str::iff::stringContains(name, XCKU095_NAME, "xqku095"))
			return XUS_DEVICE_XCKU095;
		if(str::iff::stringContains(name, XCKU115_NAME, "xqku115"))
			return XUS_DEVICE_XCKU115;
	#endif
	//US Virtex
	#ifdef XUSVIRTEX
		if(str::iff::stringContains(name, XCVU065_NAME))
			return XUS_DEVICE_XCVU065;
		if(str::iff::stringContains(name, XCVU080_NAME))
			return XUS_DEVICE_XCVU080;
		if(str::iff::stringContains(name, XCVU095_NAME))
			return XUS_DEVICE_XCVU095;
		if(str::iff::stringContains(name, XCVU125_NAME))
			return XUS_DEVICE_XCVU125;
		if(str::iff::stringContains(name, XCVU160_NAME))
			return XUS_DEVICE_XCVU160;
		if(str::iff::stringContains(name, XCVU190_NAME))
			return XUS_DEVICE_XCVU190;
		if(str::iff::stringContains(name, XCVU440_NAME))
			return XUS_DEVICE_XCVU440;
	#endif
	
	return XUS_DEVICE_NULL;
}
/************************************************************************** /**
 * setPartName if available. Otherwise will put a default name.
 *****************************************************************************/
void XilinxUltraScale::setDevice(int deviceID, string customPartName)
{
	clearResourceStringArrays();
	bool customNameSet = false;
	#define setCustomPartName(x) {customNameSet = true; partName = x;}
	switch(deviceID){
		//US Kintex
		#ifdef XUSKINTEX
		case XUS_DEVICE_XCKU025:
			XCKU025();
			break;
		case XUS_DEVICE_XCKU035:
			XCKU035();
			break;
		case XUS_DEVICE_XCKU040:
			XCKU040();
			break;
		case XUS_DEVICE_XCKU060:
			XCKU060();
			break;
		case XUS_DEVICE_XCKU085:
			XCKU085();
			break;
		case XUS_DEVICE_XCKU095:
			XCKU095();
			break;
		case XUS_DEVICE_XCKU115:
			XCKU115();
			break;
		#endif
		//US Virtex
		#ifdef XUSVIRTEX
		case XUS_DEVICE_XCVU065:
			XCVU065();
			break;
		case XUS_DEVICE_XCVU080:
			XCVU080();
			break;
		case XUS_DEVICE_XCVU095:
			XCVU095();
			break;
		case XUS_DEVICE_XCVU125:
			XCVU125();
			break;
		case XUS_DEVICE_XCVU160:
			XCVU160();
			break;
		case XUS_DEVICE_XCVU190:
			XCVU190();
			break;
		case XUS_DEVICE_XCVU440:
			XCVU440();
			break;
		#endif
		default:throw runtime_error("Unknown device ID.");
	}
	#undef setCustomPartName
	if(!customNameSet) // if we didn't set already a good full name
		if(customPartName != "")// if not empty string
			partName = customPartName;
	
	
	log("Initialized device resource string for \"" + initializedResourceStringShortPartName + "\"");
}

void XilinxUltraScale::deviceHelp()
{
	if(initializedResourceStringShortPartName == "") {//if no device is initialized
		cout << "A list of currently supported Xilinx UltraScale devices: " << endl;
		#ifdef XUSKINTEX
		cout << "  UltraScale Kintex: " << endl;
		cout << "    XCKU025, XCKU035, XCKU040, XQKU040, XCKU060, XQKU060, XQRKU060, XCKU085, XCKU095, XQKU095, XCKU115, XQKU0115" << endl;
		#endif
		#ifdef XUSVIRTEX
		cout << "  UltraScale Virtex: " << endl;
		cout << "    XCVU065, XCVU080, XCVU095, XCVU125, XCVU160, XCVU190, XCVU440" << endl;
		#endif
	} else {
		printResourceStringInfo();
	}
}