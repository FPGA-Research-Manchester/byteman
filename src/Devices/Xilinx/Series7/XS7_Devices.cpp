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

#include "XilinxSeries7.h"
#include "XS7_Devices.h"
#include "../../../Common/str.h"

using namespace std;

int XilinxSeries7::getDeviceByIDCODEorThrow(int IDCODE)
{
	int deviceID = getDeviceByIDCODE(IDCODE);
	if(deviceID == XS7_DEVICE_NULL)
		throw runtime_error(string("Unknown device with IDCODE = ").append(to_string(IDCODE)).append(" ."));
	return deviceID;
}

int XilinxSeries7::getDeviceByNameOrThrow(string name)
{
	int deviceID = getDeviceByName(name);
	if(deviceID == XS7_DEVICE_NULL)
		throw runtime_error(string("Unknown device: ").append(name).append("."));
	return deviceID;
}

void XilinxSeries7::setDeviceByPartNameOrThrow(){
	setDeviceByNameOrThrow(partName);
}

void XilinxSeries7::setDeviceByNameOrThrow(string name){
	int deviceID = getDeviceByNameOrThrow(name);
	setDevice(deviceID, name);
}

void XilinxSeries7::setDeviceByIDCODEOrThrow(int idcode){
	int deviceID = getDeviceByIDCODEorThrow(idcode);
	setDevice(deviceID);
}

int XilinxSeries7::getDeviceByIDCODE(int IDCODE)
{
	//Series 7 Spartan
	#ifdef XS7SPARTAN
	if(IDCODE == XC7S6_IDCODE)
		return XS7_DEVICE_XC7S6;
	if(IDCODE == XC7S15_IDCODE)
		return XS7_DEVICE_XC7S15;
	if(IDCODE == XC7S25_IDCODE)
		return XS7_DEVICE_XC7S25;
	if(IDCODE == XC7S50_IDCODE)
		return XS7_DEVICE_XC7S50;
	if(IDCODE == XC7S75_IDCODE)
		return XS7_DEVICE_XC7S75;
	if(IDCODE == XC7S100_IDCODE)
		return XS7_DEVICE_XC7S100;
	#endif
	//Series 7 Artix
	#ifdef XS7ARTIX
	if(IDCODE == XC7A12_IDCODE)
		return XS7_DEVICE_XC7A12;
	if(IDCODE == XC7A15_IDCODE)
		return XS7_DEVICE_XC7A15;
	if(IDCODE == XC7A25_IDCODE)
		return XS7_DEVICE_XC7A25;
	if(IDCODE == XC7A35_IDCODE)
		return XS7_DEVICE_XC7A35;
	if(IDCODE == XC7A50_IDCODE)
		return XS7_DEVICE_XC7A50;
	if(IDCODE == XC7A75_IDCODE)
		return XS7_DEVICE_XC7A75;
	if(IDCODE == XC7A100_IDCODE)
		return XS7_DEVICE_XC7A100;
	if(IDCODE == XC7A200_IDCODE)
		return XS7_DEVICE_XC7A200;
	#endif
	//Series 7 Zynq
	#ifdef XS7ZYNQ
	if(IDCODE == XC7Z007_IDCODE)
		return XS7_DEVICE_XC7Z007;
	if(IDCODE == XC7Z010_IDCODE)
		return XS7_DEVICE_XC7Z010;
	if(IDCODE == XC7Z012_IDCODE)
		return XS7_DEVICE_XC7Z012;
	if(IDCODE == XC7Z014_IDCODE)
		return XS7_DEVICE_XC7Z014;
	if(IDCODE == XC7Z015_IDCODE)
		return XS7_DEVICE_XC7Z015;
	if(IDCODE == XC7Z020_IDCODE)
		return XS7_DEVICE_XC7Z020;
	if(IDCODE == XC7Z030_IDCODE)
		return XS7_DEVICE_XC7Z030;
	if(IDCODE == XC7Z035_IDCODE)
		return XS7_DEVICE_XC7Z035;
	if(IDCODE == XC7Z045_IDCODE)
		return XS7_DEVICE_XC7Z045;
	if(IDCODE == XC7Z100_IDCODE)
		return XS7_DEVICE_XC7Z100;
	#endif
	//Series 7 Kintex
	#ifdef XS7KINTEX
	if(IDCODE == XC7K70_IDCODE)
		return XS7_DEVICE_XC7K70;
	if(IDCODE == XC7K160_IDCODE)
		return XS7_DEVICE_XC7K160;
	if(IDCODE == XC7K325_IDCODE)
		return XS7_DEVICE_XC7K325;
	if(IDCODE == XC7K355_IDCODE)
		return XS7_DEVICE_XC7K355;
	if(IDCODE == XC7K410_IDCODE)
		return XS7_DEVICE_XC7K410;
	if(IDCODE == XC7K420_IDCODE)
		return XS7_DEVICE_XC7K420;
	if(IDCODE == XC7K480_IDCODE)
		return XS7_DEVICE_XC7K480;
	#endif
	//Series 7 Virtex
	#ifdef XS7VIRTEX
	if(IDCODE == XC7V585_IDCODE)
		return XS7_DEVICE_XC7V585;
	if(IDCODE == XC7V2000_IDCODE)
		return XS7_DEVICE_XC7V2000;
	if(IDCODE == XC7VH580_IDCODE)
		return XS7_DEVICE_XC7VH580;
	if(IDCODE == XC7VH870_IDCODE)
		return XS7_DEVICE_XC7VH870;
	if(IDCODE == XC7VX330_IDCODE)
		return XS7_DEVICE_XC7VX330;
	if(IDCODE == XC7VX415_IDCODE)
		return XS7_DEVICE_XC7VX415;
	if(IDCODE == XC7VX485_IDCODE)
		return XS7_DEVICE_XC7VX485;
	if(IDCODE == XC7VX550_IDCODE)
		return XS7_DEVICE_XC7VX550;
	if(IDCODE == XC7VX690_IDCODE)
		return XS7_DEVICE_XC7VX690;
	if(IDCODE == XC7VX980_IDCODE)
		return XS7_DEVICE_XC7VX980;
	if(IDCODE == XC7VX1140_IDCODE)
		return XS7_DEVICE_XC7VX1140;
	#endif

	return XS7_DEVICE_NULL;
}
int XilinxSeries7::getDeviceByName(string name)
{
	name = str::removeSpaces(name);
	name = str::stringToLower(name);
	
	//Series 7 Spartan
	#ifdef XS7SPARTAN
		if(str::iff::stringContains(name, XC7S6_NAME))
			return XS7_DEVICE_XC7S6;
		if(str::iff::stringContains(name, XC7S15_NAME))
			return XS7_DEVICE_XC7S15;
		if(str::iff::stringContains(name, XC7S25_NAME))
			return XS7_DEVICE_XC7S25;
		if(str::iff::stringContains(name, XC7S50_NAME))
			return XS7_DEVICE_XC7S50;
		if(str::iff::stringContains(name, XC7S75_NAME))
			return XS7_DEVICE_XC7S75;
		if(str::iff::stringContains(name, XC7S100_NAME))
			return XS7_DEVICE_XC7S100;
	#endif
	//Series 7 Artix
	#ifdef XS7ARTIX
		if(str::iff::stringContains(name, XC7A12_NAME))
			return XS7_DEVICE_XC7A12;
		if(str::iff::stringContains(name, XC7A15_NAME))
			return XS7_DEVICE_XC7A15;
		if(str::iff::stringContains(name, XC7A25_NAME))
			return XS7_DEVICE_XC7A25;
		if(str::iff::stringContains(name, XC7A35_NAME))
			return XS7_DEVICE_XC7A35;
		if(str::iff::stringContains(name, XC7A50_NAME))
			return XS7_DEVICE_XC7A50;
		if(str::iff::stringContains(name, XC7A75_NAME))
			return XS7_DEVICE_XC7A75;
		if(str::iff::stringContains(name, XC7A100_NAME))
			return XS7_DEVICE_XC7A100;
		if(str::iff::stringContains(name, XC7A200_NAME))
			return XS7_DEVICE_XC7A200;
	#endif
	//Series 7 Zynq
	#ifdef XS7ZYNQ
		if(str::iff::stringContains(name, XC7Z007_NAME))
			return XS7_DEVICE_XC7Z007;
		if(str::iff::stringContains(name, XC7Z010_NAME))
			return XS7_DEVICE_XC7Z010;
		if(str::iff::stringContains(name, XC7Z012_NAME))
			return XS7_DEVICE_XC7Z012;
		if(str::iff::stringContains(name, XC7Z014_NAME))
			return XS7_DEVICE_XC7Z014;
		if(str::iff::stringContains(name, XC7Z015_NAME))
			return XS7_DEVICE_XC7Z015;
		if(str::iff::stringContains(name, XC7Z020_NAME))
			return XS7_DEVICE_XC7Z020;
		if(str::iff::stringContains(name, XC7Z030_NAME))
			return XS7_DEVICE_XC7Z030;
		if(str::iff::stringContains(name, XC7Z035_NAME))
			return XS7_DEVICE_XC7Z035;
		if(str::iff::stringContains(name, XC7Z045_NAME))
			return XS7_DEVICE_XC7Z045;
		if(str::iff::stringContains(name, XC7Z100_NAME))
			return XS7_DEVICE_XC7Z100;
	#endif
	//Series 7 Kintex
	#ifdef XS7KINTEX
		if(str::iff::stringContains(name, XC7K70_NAME))
			return XS7_DEVICE_XC7K70;
		if(str::iff::stringContains(name, XC7K160_NAME))
			return XS7_DEVICE_XC7K160;
		if(str::iff::stringContains(name, XC7K325_NAME))
			return XS7_DEVICE_XC7K325;
		if(str::iff::stringContains(name, XC7K355_NAME))
			return XS7_DEVICE_XC7K355;
		if(str::iff::stringContains(name, XC7K410_NAME))
			return XS7_DEVICE_XC7K410;
		if(str::iff::stringContains(name, XC7K420_NAME))
			return XS7_DEVICE_XC7K420;
		if(str::iff::stringContains(name, XC7K480_NAME))
			return XS7_DEVICE_XC7K480;
	#endif
	//Series 7 Virtex
	#ifdef XS7VIRTEX
		if(str::iff::stringContains(name, XC7V585_NAME))
			return XS7_DEVICE_XC7V585;
		if(str::iff::stringContains(name, XC7V2000_NAME))
			return XS7_DEVICE_XC7V2000;
		if(str::iff::stringContains(name, XC7VH580_NAME))
			return XS7_DEVICE_XC7VH580;
		if(str::iff::stringContains(name, XC7VH870_NAME))
			return XS7_DEVICE_XC7VH870;
		if(str::iff::stringContains(name, XC7VX330_NAME))
			return XS7_DEVICE_XC7VX330;
		if(str::iff::stringContains(name, XC7VX415_NAME))
			return XS7_DEVICE_XC7VX415;
		if(str::iff::stringContains(name, XC7VX485_NAME))
			return XS7_DEVICE_XC7VX485;
		if(str::iff::stringContains(name, XC7VX550_NAME))
			return XS7_DEVICE_XC7VX550;
		if(str::iff::stringContains(name, XC7VX690_NAME))
			return XS7_DEVICE_XC7VX690;
		if(str::iff::stringContains(name, XC7VX980_NAME))
			return XS7_DEVICE_XC7VX980;
		if(str::iff::stringContains(name, XC7VX1140_NAME))
			return XS7_DEVICE_XC7VX1140;
	#endif
	//Series 7 Boards
	#ifdef XS7BOARDS
		if(str::iff::stringContains(name, "wizarde"))
			return XS7_DEVICE_WIZARDE;
		if(str::iff::stringContains(name, "kintex-7connectivity", "kintex7connectivity"))
			return XS7_DEVICE_KINTEX7CONNECTIVITY;
		if(str::iff::stringContains(name, "kc705", "kintex-7evaluation", "kintex7evaluation", "kintex-7embedded", "kintex7embedded"))
			return XS7_DEVICE_KC705;
		if(str::iff::stringContains(name, "kc724", "kintex-7characterization", "kintex7characterization"))
			return XS7_DEVICE_KC724;
		if(str::iff::stringContains(name, "sp701"))
			return XS7_DEVICE_SP701;
		if(str::iff::stringContains(name, "ac701"))
			return XS7_DEVICE_AC701;
		if(str::iff::stringContains(name, "artya7-35", "artya735", "arty35"))
			return XS7_DEVICE_ARTY_A35;
		if(str::iff::stringContains(name, "artya7-100", "artya7100", "arty100"))
			return XS7_DEVICE_ARTY_A100;
		if(str::iff::stringContains(name, "artyz7-10", "artyz710", "arty10"))
			return XS7_DEVICE_ARTY_Z10;
		if(str::iff::stringContains(name, "artyz7-20", "artyz720", "arty20"))
			return XS7_DEVICE_ARTY_Z20;
		if(str::iff::stringContains(name, "artys7-25", "artys725", "arty25"))
			return XS7_DEVICE_ARTY_S25;
		if(str::iff::stringContains(name, "artys7-50", "artys750", "arty50"))
			return XS7_DEVICE_ARTY_S50;
		if(str::iff::stringContains(name, "basys3"))
			return XS7_DEVICE_BASYS3;
		if(str::iff::stringContains(name, "pynq-z1", "pynqz1"))
			return XS7_DEVICE_PYNQ_Z1;
		if(str::iff::stringContains(name, "nexysa7-50", "nexysa750", "nexys50"))
			return XS7_DEVICE_NEXYS_A50;
		if(str::iff::stringContains(name, "nexysa7-100", "nexysa7100", "nexys100"))
			return XS7_DEVICE_NEXYS_A100;
		if(str::iff::stringContains(name, "zyboz7-10", "zyboz710", "zybo10"))
			return XS7_DEVICE_ZYBO_Z10;
		if(str::iff::stringContains(name, "zyboz7-20", "zyboz720", "zybo20"))
			return XS7_DEVICE_ZYBO_Z20;
		if(str::iff::stringContains(name, "cmoda7-15", "cmoda715", "cmod15"))
			return XS7_DEVICE_CMOD_A15;
		if(str::iff::stringContains(name, "cmoda7-35", "cmoda735", "cmod35"))
			return XS7_DEVICE_CMOD_A35;
		if(str::iff::stringContains(name, "cmods7-25", "cmods725", "cmod25", "cmods"))
			return XS7_DEVICE_CMOD_S25;
		if(str::iff::stringContains(name, "zedboard"))
			return XS7_DEVICE_ZEDBOARD;
		if(str::iff::stringContains(name, "coraz7-07", "coraz707", "cora07", "cora7"))
			return XS7_DEVICE_CORA_Z7;
		if(str::iff::stringContains(name, "coraz7-10", "coraz710", "cora10"))
			return XS7_DEVICE_CORA_Z10;
		if(str::iff::stringContains(name, "nexysvideo"))
			return XS7_DEVICE_NEXYS_VIDEO;
		if(str::iff::stringContains(name, "genesys2"))
			return XS7_DEVICE_ARTY_GENESYS2;
		if(str::iff::stringContains(name, "eclypsez7"))
			return XS7_DEVICE_ECLYPSE_Z7;
		if(str::iff::stringContains(name, "usb104"))
			return XS7_DEVICE_USB104;
		if(str::iff::stringContains(name, "netfpga-sume", "netfpgasume"))
			return XS7_DEVICE_NETFPGA_SUME;
		if(str::iff::stringContains(name, "vc707"))
			return XS7_DEVICE_VC707;
		if(str::iff::stringContains(name, "vc709"))
			return XS7_DEVICE_VC709;
		if(str::iff::stringContains(name, "vc7215"))
			return XS7_DEVICE_VC7215;
		if(str::iff::stringContains(name, "vc7203"))
			return XS7_DEVICE_VC7203;
		if(str::iff::stringContains(name, "vc7222"))
			return XS7_DEVICE_VC7222;
		if(str::iff::stringContains(name, "zc702"))
			return XS7_DEVICE_ZC702;
		if(str::iff::stringContains(name, "zc706"))
			return XS7_DEVICE_ZC706;
		
		
	#endif
	return XS7_DEVICE_NULL;
}
	
/************************************************************************** /**
 * setPartName if available. Otherwise will put a default name.
 *****************************************************************************/
void XilinxSeries7::setDevice(int deviceID, string customPartName)
{
	clearResourceStringArrays();
	bool customNameSet = false;
	#define setCustomPartName(x) {customNameSet = true; partName = x;}
	switch(deviceID){
		//Series 7 Spartan
		#ifdef XS7SPARTAN
		case XS7_DEVICE_XC7S6:
			XC7S6();
			break;
		case XS7_DEVICE_XC7S15:
			XC7S15();
			break;
		case XS7_DEVICE_XC7S25:
			XC7S25();
			break;
		case XS7_DEVICE_XC7S50:
			XC7S50();
			break;
		case XS7_DEVICE_XC7S75:
			XC7S75();
			break;
		case XS7_DEVICE_XC7S100:
			XC7S100();
			break;
		#endif
		//Series 7 Artix
		#ifdef XS7ARTIX
		case XS7_DEVICE_XC7A12:
			XC7A12();
			break;
		case XS7_DEVICE_XC7A15:
			XC7A15();
			break;
		case XS7_DEVICE_XC7A25:
			XC7A25();
			break;
		case XS7_DEVICE_XC7A35:
			XC7A35();
			break;
		case XS7_DEVICE_XC7A50:
			XC7A50();
			break;
		case XS7_DEVICE_XC7A75:
			XC7A75();
			break;
		case XS7_DEVICE_XC7A100:
			XC7A100();
			break;
		case XS7_DEVICE_XC7A200:
			XC7A200();
			break;
		#endif
		//Series 7 Zynq
		#ifdef XS7ZYNQ
		case XS7_DEVICE_XC7Z007:
			XC7Z007();
			break;
		case XS7_DEVICE_XC7Z010:
			XC7Z010();
			break;
		case XS7_DEVICE_XC7Z012:
			XC7Z012();
			break;
		case XS7_DEVICE_XC7Z014:
			XC7Z014();
			break;
		case XS7_DEVICE_XC7Z015:
			XC7Z015();
			break;
		case XS7_DEVICE_XC7Z020:
			XC7Z020();
			break;
		case XS7_DEVICE_XC7Z030:
			XC7Z030();
			break;
		case XS7_DEVICE_XC7Z035:
			XC7Z035();
			break;
		case XS7_DEVICE_XC7Z045:
			XC7Z045();
			break;
		case XS7_DEVICE_XC7Z100:
			XC7Z100();
			break;
		#endif
		//Series 7 Kintex
		#ifdef XS7KINTEX
		case XS7_DEVICE_XC7K70:
			XC7K70();
			break;
		case XS7_DEVICE_XC7K160:
			XC7K160();
			break;
		case XS7_DEVICE_XC7K325:
			XC7K325();
			break;
		case XS7_DEVICE_XC7K355:
			XC7K355();
			break;
		case XS7_DEVICE_XC7K410:
			XC7K410();
			break;
		case XS7_DEVICE_XC7K420:
			XC7K420();
			break;
		case XS7_DEVICE_XC7K480:
			XC7K480();
			break;
		#endif
		//Series 7 Virtex
		#ifdef XS7VIRTEX
		case XS7_DEVICE_XC7V585:
			XC7V585();
			break;
		case XS7_DEVICE_XC7V2000:
			XC7V2000();
			break;
		case XS7_DEVICE_XC7VH580:
			throw runtime_error("XC7VH580 needs special handling for GTZ SLRs, which is not yet implemented.");
			XC7VH580();
			break;
		case XS7_DEVICE_XC7VH870:
			throw runtime_error("XCV7VH870 needs special handling for GTZ SLRs, which is not yet implemented.");
			XC7VH870();
			break;
		case XS7_DEVICE_XC7VX330:
			XC7VX330();
			break;
		case XS7_DEVICE_XC7VX415:
			XC7VX415();
			break;
		case XS7_DEVICE_XC7VX485:
			XC7VX485();
			break;
		case XS7_DEVICE_XC7VX550:
			XC7VX550();
			break;
		case XS7_DEVICE_XC7VX690:
			XC7VX690();
			break;
		case XS7_DEVICE_XC7VX980:
			XC7VX980();
			break;
		case XS7_DEVICE_XC7VX1140:
			XC7VX1140();
			break;
		#endif
		//Series 7 Boards
		#ifdef XS7BOARDS
		case XS7_DEVICE_WIZARDE:
			XC7Z045();
			setCustomPartName("xc7z045ffg900-2");
			break;
		case XS7_DEVICE_KINTEX7CONNECTIVITY:
			XC7K325();
			setCustomPartName("xc7k325t-2ffg900c");
			break;
		case XS7_DEVICE_KC705:
			XC7K325();
			setCustomPartName("xc7k325t-2ffg900c");
			break;
		case XS7_DEVICE_KC724:
			XC7K325();
			setCustomPartName("xc7k325t-3ffg900e");
			break;
		case XS7_DEVICE_SP701:
			XC7S100();
			setCustomPartName("xc7s100-2fgga676c");
			break;
		case XS7_DEVICE_AC701:
			XC7A200();
			setCustomPartName("xc7a200t-2fbg676c");
			break;
		case XS7_DEVICE_ARTY_A35:
			XC7A35();
			setCustomPartName("xc7a35ticsg324-1l");
			break;
		case XS7_DEVICE_ARTY_A100:
			XC7A100();
			setCustomPartName("xc7a100tcsg324-1");
			break;
		case XS7_DEVICE_ARTY_Z10:
			XC7Z010();
			setCustomPartName("xc7z010-1clg400c");
			break;
		case XS7_DEVICE_ARTY_Z20:
			XC7Z020();
			setCustomPartName("xc7z020-1clg400c");
			break;
		case XS7_DEVICE_ARTY_S25:
			XC7S25();
			setCustomPartName("xc7s25-csga324");
			break;
		case XS7_DEVICE_ARTY_S50:
			XC7S50();
			setCustomPartName("xc7s50-csga324");
			break;
		case XS7_DEVICE_BASYS3:
			XC7A35();
			setCustomPartName("xc7a35t-1cpg236c");
			break;
		case XS7_DEVICE_PYNQ_Z1:
			XC7Z020();
			setCustomPartName("xc7z020-1clg400c");
			break;
		case XS7_DEVICE_NEXYS_A50:
			XC7A50();
			setCustomPartName("xc7a50t-1csg324c");
			break;
		case XS7_DEVICE_NEXYS_A100:
			XC7A100();
			setCustomPartName("xc7a100t-1csg324c");
			break;
		case XS7_DEVICE_ZYBO_Z10:
			XC7Z010();
			setCustomPartName("xc7z010-1clg400c");
			break;
		case XS7_DEVICE_ZYBO_Z20:
			XC7Z020();
			setCustomPartName("xc7z020-1clg400c");
			break;
		case XS7_DEVICE_CMOD_A15:
			XC7A15();
			setCustomPartName("xc7a15t-1cpg236c");
			break;
		case XS7_DEVICE_CMOD_A35:
			XC7A35();
			setCustomPartName("xc7a35t-1cpg236c");
			break;
		case XS7_DEVICE_CMOD_S25:
			XC7S25();
			setCustomPartName("xc7s25-1csga225c");
			break;
		case XS7_DEVICE_ZEDBOARD:
			XC7Z020();
			setCustomPartName("xc7z020-clg484");
			break;
		case XS7_DEVICE_CORA_Z7:
			XC7Z007();
			setCustomPartName("xc7z007s-1clg400c");
			break;
		case XS7_DEVICE_CORA_Z10:
			XC7Z010();
			setCustomPartName("xc7z010-1clg400c");
			break;
		case XS7_DEVICE_NEXYS_VIDEO:
			XC7A200();
			setCustomPartName("xc7a200t-1sbg484c");
			break;
		case XS7_DEVICE_ARTY_GENESYS2:
			XC7K325();
			setCustomPartName("xc7k325t-2ffg900c");
			break;
		case XS7_DEVICE_ECLYPSE_Z7:
			XC7Z020();
			setCustomPartName("xc7z020-1clg484c");
			break;
		case XS7_DEVICE_USB104:
			XC7A100();
			setCustomPartName("xc7a100t-1csg324i");
			break;
		case XS7_DEVICE_NETFPGA_SUME:
			XC7VX690();
			setCustomPartName("xc7vx690t-3ffg1761c");
			break;
		case XS7_DEVICE_VC707:
			XC7VX485();
			setCustomPartName("xc7vx485t-2ffg1761");
			break;
		case XS7_DEVICE_VC709:
			XC7VX690();
			setCustomPartName("xc7vx690t-2ffg1761c");
			break;
		case XS7_DEVICE_VC7215:
			XC7VX690();
			setCustomPartName("xc7vx690t-3ffg1927e");
			break;
		case XS7_DEVICE_VC7203:
			XC7VX485();
			setCustomPartName("xc7vx485t-3ffg1761e");
			break;
		case XS7_DEVICE_VC7222:
			XC7VH580();
			setCustomPartName("xc7vh580t-g2hcg1155e");
			break;
		case XS7_DEVICE_ZC702:
			XC7Z020();
			setCustomPartName("xc7z020-clg484-1");
			break;
		case XS7_DEVICE_ZC706:
			XC7Z045();
			setCustomPartName("xc7z045-2ffg900c");
			break;
		#endif
		default:throw runtime_error("Unknown device ID.");
	}
	#undef setCustomPartName
	if(!customNameSet) // if we didn't set already a good full name
		if(customPartName != "")// if not empty string
			partName = customPartName;
	
	log("Initialized device as \"" + partName + "\""); 
}

void XilinxSeries7::deviceHelp()
{
	cout << "A list of currently recognized Series 7 devices: " << endl;
	#ifdef XS7SPARTAN
	cout << "\tSeries 7 Spartan: " << endl;
	cout << "\t\tXC7S6, XA7S6, XC7S15, XA7S15, XC7S25, XA7S25, XC7S50, XA7S50, XC7S75, XA7S75, XC7S100, XA7S100" << endl;
	#endif
	#ifdef XS7ARTIX
	cout << "\tSeries 7 Artix: " << endl;
	cout << "\t\tXC7A12, XA7A12, XC7A15, XA7A15, XC7A25, XA7A25, XC7A35, XA7A35, XC7A50, XQ7A50, XA7A50," << endl;
	cout << "\t\tXC7A75, XA7A75, XC7A100, XQ7A100, XA7A100, XC7A200, XQ7A200" << endl;
	#endif
	#ifdef XS7ZYNQ
	cout << "\tSeries 7 Zynq: " << endl;
	cout << "\t\tXC7Z007, XC7Z010, XA7Z010, XC7Z012, XC7Z014, XC7Z015, XC7Z020, XQ7Z020, XA7Z020," << endl;
	cout << "\t\tXC7Z030, XQ7Z030, XA7Z030, XC7Z035, XC7Z045, XQ7Z045, XC7Z100, XQ7Z100" << endl;
	#endif
	#ifdef XS7KINTEX
	cout << "\tSeries 7 Kintex: " << endl;
	cout << "\t\tXC7K70, XC7K160, XA7K160, XC7K325, XQ7K325, XC7K355, XC7K410, XQ7K410, XC7K420, XC7K480" << endl;
	#endif
	#ifdef XS7VIRTEX
	cout << "\tSeries 7 Virtex: " << endl;
	cout << "\t\tXC7V585, XQ7V585, XC7V2000, XC7VX330, XQ7VX330, XC7VX415, XC7VX485, XQ7VX485, XC7VX550," << endl;
	cout << "\t\tXC7VX690, XQ7VX690, XC7VX980, XQ7VX980, XC7VX1140" << endl;
	#endif
	#ifdef XS7BOARDS
	cout << "\tSeries 7 Boards: " << endl;
	cout << "\t\tWizarde, Kintex-7 Connectivity Kit, KC705, KC724, SP701, AC701, Arty A7-35, Arty A7-100, Arty Z7-10, Arty Z7-20," << endl;
	cout << "\t\tArty S7-25, Arty S7-50, Basys 3, Pynq Z1, Nexys A7-50, Nexys A7-100, Zybo Z7-10, Zybo Z7-20, Cmod A7-15," << endl;
	cout << "\t\tCmod A7-35, Cmod S7, ZedBoard, Cora Z7-07, Cora Z7-10, Nexys Video, Genesys 2, Eclypse Z7, USB104, NetFPGA-SUME," << endl;
	cout << "\t\tVC707, VC709, VC7203, VC7215, VC7222, ZC702, ZC706" << endl;
	#endif
}