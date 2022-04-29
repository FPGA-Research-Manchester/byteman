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
#include "XilinxUltraScalePlus.h"
#include "XilinxUltraScalePlusDevices.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <cstring>
using namespace std;

int XilinxUltraScalePlus::getDeviceByIDCODEorThrow(int IDCODE){
	int deviceID = XilinxUltraScalePlus::getDeviceByIDCODE(IDCODE);
	if(deviceID == XUSP_DEVICE_NULL)
		throw runtime_error(string("Unknown device with IDCODE = ").append(to_string(IDCODE)).append(" ."));
	return deviceID;
}
int XilinxUltraScalePlus::getDeviceByNameOrThrow(string name){
	int deviceID = XilinxUltraScalePlus::getDeviceByName(name);
	if(deviceID == XUSP_DEVICE_NULL)
		throw runtime_error(string("Unknown device: ").append(name).append("."));
	return deviceID;
}
int XilinxUltraScalePlus::getDeviceByIDCODE(int IDCODE)
{
    //US+ Artix
	#ifdef XUSPARTIX
    if(IDCODE == XCAU10P_IDCODE)return XUSP_DEVICE_XCAU10P;
	if(IDCODE == XCAU15P_IDCODE)return XUSP_DEVICE_XCAU15P;
    if(IDCODE == XCAU20P_IDCODE)return XUSP_DEVICE_XCAU20P;
    if(IDCODE == XCAU25P_IDCODE)return XUSP_DEVICE_XCAU25P;
	#endif
    //US+ Kintex
	#ifdef XUSPKINTEX
    if(IDCODE == XCKU3P_IDCODE)return XUSP_DEVICE_XCKU3P;
	if(IDCODE == XCKU5P_IDCODE)return XUSP_DEVICE_XCKU5P;
    if(IDCODE == XCKU9P_IDCODE)return XUSP_DEVICE_XCKU9P;
    if(IDCODE == XCKU11P_IDCODE)return XUSP_DEVICE_XCKU11P;
    if(IDCODE == XCKU13P_IDCODE)return XUSP_DEVICE_XCKU13P;
    if(IDCODE == XCKU15P_IDCODE)return XUSP_DEVICE_XCKU15P;
    if(IDCODE == XCKU19P_IDCODE)return XUSP_DEVICE_XCKU19P;
	#endif
    //US+ Virtex
	#ifdef XUSPVIRTEX
    if(IDCODE == XCVU3P_IDCODE)return XUSP_DEVICE_XCVU3P;
    if(IDCODE == XCVU5P_IDCODE)return XUSP_DEVICE_XCVU5P;
    if(IDCODE == XCVU7P_IDCODE)return XUSP_DEVICE_XCVU7P;
    if(IDCODE == XCVU9P_IDCODE)return XUSP_DEVICE_XCVU9P;
    if(IDCODE == XCVU11P_IDCODE)return XUSP_DEVICE_XCVU11P;
    if(IDCODE == XCVU13P_IDCODE)return XUSP_DEVICE_XCVU13P;
    if(IDCODE == XCVU19P_IDCODE)return XUSP_DEVICE_XCVU19P;
    if(IDCODE == XCVU23P_IDCODE)return XUSP_DEVICE_XCVU23P;
    if(IDCODE == XCVU27P_IDCODE)return XUSP_DEVICE_XCVU27P;
    if(IDCODE == XCVU29P_IDCODE)return XUSP_DEVICE_XCVU29P;
    if(IDCODE == XCVU31P_IDCODE)return XUSP_DEVICE_XCVU31P;
    if(IDCODE == XCVU33P_IDCODE)return XUSP_DEVICE_XCVU33P;
    if(IDCODE == XCVU35P_IDCODE)return XUSP_DEVICE_XCVU35P;
    if(IDCODE == XCVU37P_IDCODE)return XUSP_DEVICE_XCVU37P;
    if(IDCODE == XCVU45P_IDCODE)return XUSP_DEVICE_XCVU45P;
    if(IDCODE == XCVU47P_IDCODE)return XUSP_DEVICE_XCVU47P;
    if(IDCODE == XCVU57P_IDCODE)return XUSP_DEVICE_XCVU57P;
	#endif
	//US+ RFSoC (RFSOC BEFORE MPSOC, since names collide!)
	#ifdef XUSPZYNQ
    if(IDCODE == XCZU21DR_IDCODE)return XUSP_DEVICE_XCZU21DR;
    if(IDCODE == XCZU25DR_IDCODE)return XUSP_DEVICE_XCZU25DR;
    if(IDCODE == XCZU27DR_IDCODE)return XUSP_DEVICE_XCZU27DR;
    if(IDCODE == XCZU28DR_IDCODE)return XUSP_DEVICE_XCZU28DR;
    if(IDCODE == XCZU29DR_IDCODE)return XUSP_DEVICE_XCZU29DR;
    if(IDCODE == XCZU39DR_IDCODE)return XUSP_DEVICE_XCZU39DR;
    if(IDCODE == XCZU42DR_IDCODE)return XUSP_DEVICE_XCZU42DR;
    if(IDCODE == XCZU43DR_IDCODE)return XUSP_DEVICE_XCZU43DR;
    if(IDCODE == XCZU46DR_IDCODE)return XUSP_DEVICE_XCZU46DR;
    if(IDCODE == XCZU47DR_IDCODE)return XUSP_DEVICE_XCZU47DR;
    if(IDCODE == XCZU48DR_IDCODE)return XUSP_DEVICE_XCZU48DR;
    if(IDCODE == XCZU49DR_IDCODE)return XUSP_DEVICE_XCZU49DR;
    if(IDCODE == XCZU65DR_IDCODE)return XUSP_DEVICE_XCZU65DR;
    if(IDCODE == XCZU67DR_IDCODE)return XUSP_DEVICE_XCZU67DR;
	#endif
	//US+ Zynq
	#ifdef XUSPZYNQ
    if(IDCODE == XCZU1_IDCODE)return XUSP_DEVICE_XCZU1;
    if(IDCODE == XCZU2_IDCODE)return XUSP_DEVICE_XCZU2;
    if(IDCODE == XCZU3_IDCODE)return XUSP_DEVICE_XCZU3;
    if(IDCODE == XCZU4_IDCODE)return XUSP_DEVICE_XCZU4;
    if(IDCODE == XCZU5_IDCODE)return XUSP_DEVICE_XCZU5;
    if(IDCODE == XCZU6_IDCODE)return XUSP_DEVICE_XCZU6;
    if(IDCODE == XCZU7_IDCODE)return XUSP_DEVICE_XCZU7;
    if(IDCODE == XCZU9_IDCODE)return XUSP_DEVICE_XCZU9;
    if(IDCODE == XCZU11_IDCODE)return XUSP_DEVICE_XCZU11;
    if(IDCODE == XCZU15_IDCODE)return XUSP_DEVICE_XCZU15;
    if(IDCODE == XCZU17_IDCODE)return XUSP_DEVICE_XCZU17;
    if(IDCODE == XCZU19_IDCODE)return XUSP_DEVICE_XCZU19;
	#endif


	//Alveo rebranding
	#ifdef XUSPALVEO
    if(IDCODE == XCU200_IDCODE)return XUSP_DEVICE_XCU200;
    if(IDCODE == XCU250_IDCODE)return XUSP_DEVICE_XCU250;
    if(IDCODE == XCU26_IDCODE)return XUSP_DEVICE_XCU26;
    if(IDCODE == XCUX35_IDCODE)return XUSP_DEVICE_XCUX35;
    if(IDCODE == XCU50_IDCODE)return XUSP_DEVICE_XCU50;
    if(IDCODE == XCU55N_IDCODE)return XUSP_DEVICE_XCU55N;
    if(IDCODE == XCU55C_IDCODE)return XUSP_DEVICE_XCU55C;
    if(IDCODE == XCU280_IDCODE)return XUSP_DEVICE_XCU280;
    if(IDCODE == XCU30_IDCODE)return XUSP_DEVICE_XCU30;
    if(IDCODE == XCU25_IDCODE)return XUSP_DEVICE_XCU25;
	#endif
	
	return XUSP_DEVICE_NULL;
}
int XilinxUltraScalePlus::getDeviceByName(string name)
{
	name.erase(remove_if(name.begin(), name.end(), ::isspace), name.end());
	transform(name.begin(), name.end(),name.begin(), ::toupper);
	#define deviceIs(x) (name.find(x)!=string::npos)
    //US+ Artix
	#ifdef XUSPARTIX
    if(deviceIs("XCAU10P"))return XUSP_DEVICE_XCAU10P;
    if(deviceIs("XCAU15P"))return XUSP_DEVICE_XCAU15P;
    if(deviceIs("XCAU20P"))return XUSP_DEVICE_XCAU20P;
    if(deviceIs("XCAU25P"))return XUSP_DEVICE_XCAU25P;
	#endif
    //US+ Kintex
	#ifdef XUSPKINTEX
    if(deviceIs("XCKU3P") || deviceIs("XQKU3P"))return XUSP_DEVICE_XCKU3P;
    if(deviceIs("XCKU5P") || deviceIs("XQKU5P"))return XUSP_DEVICE_XCKU5P;
    if(deviceIs("XCKU9P") || deviceIs("XQKU9P"))return XUSP_DEVICE_XCKU9P;
    if(deviceIs("XCKU11P") || deviceIs("XQKU11P"))return XUSP_DEVICE_XCKU11P;
    if(deviceIs("XCKU13P") || deviceIs("XQKU13P"))return XUSP_DEVICE_XCKU13P;
    if(deviceIs("XCKU15P") || deviceIs("XQKU15P"))return XUSP_DEVICE_XCKU15P;
    if(deviceIs("XCKU19P") || deviceIs("XQKU19P"))return XUSP_DEVICE_XCKU19P;
	#endif
    //US+ Virtex
	#ifdef XUSPVIRTEX
    if(deviceIs("XCVU3P") || deviceIs("XQVU3P") )return XUSP_DEVICE_XCVU3P;
    if(deviceIs("XCVU5P") || deviceIs("XQVU5P") )return XUSP_DEVICE_XCVU5P;
    if(deviceIs("XCVU7P") || deviceIs("XQVU7P") )return XUSP_DEVICE_XCVU7P;
    if(deviceIs("XCVU9P") || deviceIs("XQVU9P") )return XUSP_DEVICE_XCVU9P;
    if(deviceIs("XCVU11P") || deviceIs("XQVU11P"))return XUSP_DEVICE_XCVU11P;
    if(deviceIs("XCVU13P") || deviceIs("XQVU13P"))return XUSP_DEVICE_XCVU13P;
    if(deviceIs("XCVU19P") || deviceIs("XQVU19P"))return XUSP_DEVICE_XCVU19P;
    if(deviceIs("XCVU23P") || deviceIs("XQVU23P"))return XUSP_DEVICE_XCVU23P;
    if(deviceIs("XCVU27P") || deviceIs("XQVU27P"))return XUSP_DEVICE_XCVU27P;
    if(deviceIs("XCVU29P") || deviceIs("XQVU29P"))return XUSP_DEVICE_XCVU29P;
    if(deviceIs("XCVU31P") || deviceIs("XQVU31P"))return XUSP_DEVICE_XCVU31P;
    if(deviceIs("XCVU33P") || deviceIs("XQVU33P"))return XUSP_DEVICE_XCVU33P;
    if(deviceIs("XCVU35P") || deviceIs("XQVU35P"))return XUSP_DEVICE_XCVU35P;
    if(deviceIs("XCVU37P") || deviceIs("XQVU37P"))return XUSP_DEVICE_XCVU37P;
    if(deviceIs("XCVU45P") || deviceIs("XQVU45P"))return XUSP_DEVICE_XCVU45P;
    if(deviceIs("XCVU47P") || deviceIs("XQVU47P"))return XUSP_DEVICE_XCVU47P;
    if(deviceIs("XCVU57P") || deviceIs("XQVU57P"))return XUSP_DEVICE_XCVU57P;
	#endif
	//US+ RFSoC (RFSOC BEFORE MPSOC, since names collide!)
	#ifdef XUSPZYNQRF
    if(deviceIs("XCZU21DR") || deviceIs("XQZU21DR"))return XUSP_DEVICE_XCZU21DR;
    if(deviceIs("XCZU25DR"))return XUSP_DEVICE_XCZU25DR;
    if(deviceIs("XCZU27DR"))return XUSP_DEVICE_XCZU27DR;
    if(deviceIs("XCZU28DR") || deviceIs("XQZU28DR"))return XUSP_DEVICE_XCZU28DR;
    if(deviceIs("XCZU29DR") || deviceIs("XQZU29DR"))return XUSP_DEVICE_XCZU29DR;
    if(deviceIs("XCZU39DR"))return XUSP_DEVICE_XCZU39DR;
    if(deviceIs("XCZU42DR"))return XUSP_DEVICE_XCZU42DR;
    if(deviceIs("XCZU43DR"))return XUSP_DEVICE_XCZU43DR;
    if(deviceIs("XCZU46DR") || deviceIs("XQZU48DR") || deviceIs("XQZU49DR"))return XUSP_DEVICE_XCZU46DR; // Yes, this row is correct.
    if(deviceIs("XCZU47DR"))return XUSP_DEVICE_XCZU47DR;
    if(deviceIs("XCZU48DR"))return XUSP_DEVICE_XCZU48DR;
    if(deviceIs("XCZU49DR"))return XUSP_DEVICE_XCZU49DR;
    if(deviceIs("XCZU65DR"))return XUSP_DEVICE_XCZU65DR;
    if(deviceIs("XCZU67DR"))return XUSP_DEVICE_XCZU67DR;
	#endif
	//US+ Zynq
	#ifdef XUSPZYNQMP
    if(deviceIs("XCZU1") || deviceIs(  "XAZU1") || deviceIs(  "XQZU1"))return XUSP_DEVICE_XCZU1;
    if(deviceIs("XCZU2") || deviceIs(  "XAZU2") || deviceIs(  "XQZU2"))return XUSP_DEVICE_XCZU2;
    if(deviceIs("XCZU3") || deviceIs(  "XAZU3") || deviceIs(  "XQZU3"))return XUSP_DEVICE_XCZU3;
    if(deviceIs("XCZU4") || deviceIs(  "XAZU4") || deviceIs(  "XQZU4"))return XUSP_DEVICE_XCZU4;
    if(deviceIs("XCZU5") || deviceIs(  "XAZU5") || deviceIs(  "XQZU5"))return XUSP_DEVICE_XCZU5;
    if(deviceIs("XCZU6") || deviceIs(  "XAZU6") || deviceIs(  "XQZU6"))return XUSP_DEVICE_XCZU6;
    if(deviceIs("XCZU7") || deviceIs(  "XAZU7") || deviceIs(  "XQZU7"))return XUSP_DEVICE_XCZU7;
    if(deviceIs("XCZU9") || deviceIs(  "XAZU9") || deviceIs(  "XQZU9"))return XUSP_DEVICE_XCZU9;
    if(deviceIs("XCZU11") || deviceIs("XAZU11") || deviceIs("XQZU11"))return XUSP_DEVICE_XCZU11;
    if(deviceIs("XCZU15") || deviceIs("XAZU15") || deviceIs("XQZU15"))return XUSP_DEVICE_XCZU15;
    if(deviceIs("XCZU17") || deviceIs("XAZU17") || deviceIs("XQZU17"))return XUSP_DEVICE_XCZU17;
    if(deviceIs("XCZU19") || deviceIs("XAZU19") || deviceIs("XQZU19"))return XUSP_DEVICE_XCZU19;
	#endif


	//Alveo rebranding
	#ifdef XUSPALVEO
    if(deviceIs("XCU200") || deviceIs("ALVEOU200"))return XUSP_DEVICE_XCU200;
    if(deviceIs("XCU250") || deviceIs("ALVEOU250"))return XUSP_DEVICE_XCU250;
    if(deviceIs("XCU26") || deviceIs("ALVEOU26"))return XUSP_DEVICE_XCU26;
    if(deviceIs("XCUX35") || deviceIs("ALVEOU35"))return XUSP_DEVICE_XCUX35;
    if(deviceIs("XCU50") || deviceIs("ALVEOU50"))return XUSP_DEVICE_XCU50;
    if(deviceIs("XCU55N") || deviceIs("ALVEOU55N"))return XUSP_DEVICE_XCU55N;
    if(deviceIs("XCU55C") || deviceIs("ALVEOU55C"))return XUSP_DEVICE_XCU55C;
    if(deviceIs("XCU280") || deviceIs("ALVEOU280"))return XUSP_DEVICE_XCU280;
    if(deviceIs("XCU30") || deviceIs("ALVEOU30"))return XUSP_DEVICE_XCU30;
    if(deviceIs("XCU25") || deviceIs("ALVEOU25"))return XUSP_DEVICE_XCU25;
	#endif

    //Some boards for easier use
	#ifdef XUSPBOARDS
    if(deviceIs("ZCU102") || deviceIs("ZU-GMSL2"))return XUSP_DEVICE_ZCU102;
    if(deviceIs("ZCU104"))return XUSP_DEVICE_ZCU104;
    if(deviceIs("ZCU106"))return XUSP_DEVICE_ZCU106;
    if(deviceIs("AXU15EG"))return XUSP_DEVICE_AXU15EG;
    if(deviceIs("AXU2CG"))return XUSP_DEVICE_AXU2CG;
    if(deviceIs("AXU3EG"))return XUSP_DEVICE_AXU3EG;
    if(deviceIs("AXU4EV"))return XUSP_DEVICE_AXU4EV;
    if(deviceIs("AXU5EV"))return XUSP_DEVICE_AXU5EV;
    if(deviceIs("AXU9EG"))return XUSP_DEVICE_AXU9EG;
    if(deviceIs("ULTRAZED-EV"))return XUSP_DEVICE_ULTRAZED_EV;
    if(deviceIs("CHAMP-XD1S"))return XUSP_DEVICE_CHAMP_XD1S;
    if(deviceIs("ZU-3EG"))return XUSP_DEVICE_ZU_3EG;
    if(deviceIs("ZU-5EV"))return XUSP_DEVICE_ZU_5EV;
    if(deviceIs("HTG-Z999-06"))return XUSP_DEVICE_HTG_Z999_06;
    if(deviceIs("HTG-Z999-09"))return XUSP_DEVICE_HTG_Z999_09;
    if(deviceIs("HTG-Z999-15"))return XUSP_DEVICE_HTG_Z999_15;
    if(deviceIs("HTG-930-9-3"))return XUSP_DEVICE_HTG_930_9_3;
    if(deviceIs("HTG-930-9"))return XUSP_DEVICE_HTG_930_9;
    if(deviceIs("HTG-930-13-3"))return XUSP_DEVICE_HTG_930_13_3;
    if(deviceIs("HTG-930-13"))return XUSP_DEVICE_HTG_930_13;
    if(deviceIs("HTG-937-37P"))return XUSP_DEVICE_HTG_937_37P;
    if(deviceIs("HTG-937-47P"))return XUSP_DEVICE_HTG_937_47P;
    if(deviceIs("HTG-Z922-11"))return XUSP_DEVICE_HTG_Z922_11;
    if(deviceIs("HTG-Z922-19-DG"))return XUSP_DEVICE_HTG_Z922_19_DG;
    if(deviceIs("HTG-Z922-19"))return XUSP_DEVICE_HTG_Z922_19;
    if(deviceIs("CZU3EG"))return XUSP_DEVICE_CZU3EG;
    if(deviceIs("CZU4EV"))return XUSP_DEVICE_CZU4EV;
    if(deviceIs("TE0802"))return XUSP_DEVICE_TE0802;
    if(deviceIs("TE0803-04-3BE11"))return XUSP_DEVICE_TE0803_04_3BE11;
    if(deviceIs("TE0807-03-7DE21"))return XUSP_DEVICE_TE0807_03_7DE21;
    if(deviceIs("TE0808-05-9BE21"))return XUSP_DEVICE_TE0808_05_9BE21;
    if(deviceIs("TE0808-05-9GI21"))return XUSP_DEVICE_TE0808_05_9GI21;
    if(deviceIs("TE0808-05-BBE21"))return XUSP_DEVICE_TE0808_05_BBE21;
    if(deviceIs("TE0812"))return XUSP_DEVICE_TE0812;
    if(deviceIs("TEB0911"))return XUSP_DEVICE_TEB0911;
    if(deviceIs("TEB0912"))return XUSP_DEVICE_TEB0912;
    if(deviceIs("TYSOM-3-ZU7EV"))return XUSP_DEVICE_TYSOM_3_ZU7EV;
    if(deviceIs("XPEDITE2600"))return XUSP_DEVICE_XPEDITE2600;
    if(deviceIs("AMC574"))return XUSP_DEVICE_AMC574;
    if(deviceIs("ZCU208"))return XUSP_DEVICE_ZCU208;
    if(deviceIs("ZCU216"))return XUSP_DEVICE_ZCU216;
    if(deviceIs("ZCU670"))return XUSP_DEVICE_ZCU670;
    if(deviceIs("ULTRA96"))return XUSP_DEVICE_ULTRA96;
    if(deviceIs("K26C"))return XUSP_DEVICE_K26C;
    if(deviceIs("K26I"))return XUSP_DEVICE_K26I;
    if(deviceIs("KV260"))return XUSP_DEVICE_KV260;
    if(deviceIs("AMC584"))return XUSP_DEVICE_AMC584;
    if(deviceIs("HTG-960"))return XUSP_DEVICE_HTG_960;
    if(deviceIs("VCU1525"))return XUSP_DEVICE_VCU1525;
    if(deviceIs("VCU118"))return XUSP_DEVICE_VCU118;
    if(deviceIs("VCU128"))return XUSP_DEVICE_VCU128;
    if(deviceIs("VCU129"))return XUSP_DEVICE_VCU129;
    if(deviceIs("HTG-940-9-3"))return XUSP_DEVICE_HTG_940_9_3;
    if(deviceIs("HTG-940-9"))return XUSP_DEVICE_HTG_940_9;
    if(deviceIs("HTG-940-13-3"))return XUSP_DEVICE_HTG_940_13_3;
    if(deviceIs("HTG-940-13"))return XUSP_DEVICE_HTG_940_13;
    if(deviceIs("HTG-9200-5P"))return XUSP_DEVICE_HTG_9200_5P;
    if(deviceIs("HTG-9200-9P"))return XUSP_DEVICE_HTG_9200_9P;
    if(deviceIs("HTG-9200-13P"))return XUSP_DEVICE_HTG_9200_13P;
    if(deviceIs("HTG-VUP-PCIE-HH-9P") || deviceIs("HTG-910-9"))return XUSP_DEVICE_HTG_VUP_PCIE_HH_9P;
    if(deviceIs("HTG-VUP-PCIE-HH-13P") || deviceIs("HTG-910-13"))return XUSP_DEVICE_HTG_VUP_PCIE_HH_13P;
    if(deviceIs("HTG-ZRF-FMC-28"))return XUSP_DEVICE_HTG_ZRF_FMC_28;
    if(deviceIs("HTG-ZRF-FMC-48"))return XUSP_DEVICE_HTG_ZRF_FMC_48;
    if(deviceIs("HTG-ZRF16-29"))return XUSP_DEVICE_HTG_ZRF16_29;
    if(deviceIs("HTG-ZRF16-49"))return XUSP_DEVICE_HTG_ZRF16_49;
    if(deviceIs("HTG-ZRF-HH-28"))return XUSP_DEVICE_HTG_ZRF_HH_28;
    if(deviceIs("HTG-ZRF-HH-48"))return XUSP_DEVICE_HTG_ZRF_HH_48;
    if(deviceIs("HTG-ZUSP-PCIE-11-3"))return XUSP_DEVICE_HTG_ZUSP_PCIE_11_3;
    if(deviceIs("HTG-ZUSP-PCIE-19-2"))return XUSP_DEVICE_HTG_ZUSP_PCIE_19_2;
    if(deviceIs("HTG-ZRF8-R2-28") || deviceIs("HTG-ZRF8-28"))return XUSP_DEVICE_HTG_ZRF8_R2_28;
    if(deviceIs("HTG-ZRF8-R2-48-I") || deviceIs("HTG-ZRF8-48-I"))return XUSP_DEVICE_HTG_ZRF8_R2_48_I;
    if(deviceIs("HTG-ZRF8-R2-48") || deviceIs("HTG-ZRF8-48"))return XUSP_DEVICE_HTG_ZRF8_R2_48;
	#endif
	return XUSP_DEVICE_NULL;
	#undef deviceIs
}
/************************************************************************** /**
 * setPartName if available. Otherwise will put a default name.
 *****************************************************************************/
void XilinxUltraScalePlus::setDevice(int deviceID, string customPartName)
{
	bool customNameSet = false;
	#define setPart(x) customNameSet = true; partName = x;
	switch(deviceID){
		//XUSP Artix
		#ifdef XUSPARTIX
		case XUSP_DEVICE_XCAU10P: XCAU10P; break;
		case XUSP_DEVICE_XCAU15P: XCAU15P; break;
		case XUSP_DEVICE_XCAU20P: XCAU20P; break;
		case XUSP_DEVICE_XCAU25P: XCAU25P; break;
		#endif
		//XUSP Kintex
		#ifdef XUSPKINTEX
		case XUSP_DEVICE_XCKU3P: XCKU3P; break;
		case XUSP_DEVICE_XCKU5P: XCKU5P; break;
		case XUSP_DEVICE_XCKU9P: XCKU9P; break;
		case XUSP_DEVICE_XCKU11P: XCKU11P; break;
		case XUSP_DEVICE_XCKU13P: XCKU13P; break;
		case XUSP_DEVICE_XCKU15P: XCKU15P; break;
		case XUSP_DEVICE_XCKU19P: XCKU19P; break;
		#endif
		//XUSP Virtex
		#ifdef XUSPVIRTEX
		case XUSP_DEVICE_XCVU3P: XCVU3P; break;
		case XUSP_DEVICE_XCVU5P: XCVU5P; break;
		case XUSP_DEVICE_XCVU7P: XCVU7P; break;
		case XUSP_DEVICE_XCVU9P: XCVU9P; break;
		case XUSP_DEVICE_XCVU11P: XCVU11P; break;
		case XUSP_DEVICE_XCVU13P: XCVU13P; break;
		case XUSP_DEVICE_XCVU19P: XCVU19P; break;
		case XUSP_DEVICE_XCVU23P: XCVU23P; break;
		case XUSP_DEVICE_XCVU27P: XCVU27P; break;
		case XUSP_DEVICE_XCVU29P: XCVU29P; break;
		case XUSP_DEVICE_XCVU31P: XCVU31P; break;
		case XUSP_DEVICE_XCVU33P: XCVU33P; break;
		case XUSP_DEVICE_XCVU35P: XCVU35P; break;
		case XUSP_DEVICE_XCVU37P: XCVU37P; break;
		case XUSP_DEVICE_XCVU45P: XCVU45P; break;
		case XUSP_DEVICE_XCVU47P: XCVU47P; break;
		case XUSP_DEVICE_XCVU57P: XCVU57P; break;
		#endif
		//XUSP RFSoC
		#ifdef XUSPZYNQRF
		case XUSP_DEVICE_XCZU21DR: XCZU21DR; break;
		case XUSP_DEVICE_XCZU25DR: XCZU25DR; break;
		case XUSP_DEVICE_XCZU27DR: XCZU27DR; break;
		case XUSP_DEVICE_XCZU28DR: XCZU28DR; break;
		case XUSP_DEVICE_XCZU29DR: XCZU29DR; break;
		case XUSP_DEVICE_XCZU39DR: XCZU39DR; break;
		case XUSP_DEVICE_XCZU42DR: XCZU42DR; break;
		case XUSP_DEVICE_XCZU43DR: XCZU43DR; break;
		case XUSP_DEVICE_XCZU46DR: XCZU46DR; break;
		case XUSP_DEVICE_XCZU47DR: XCZU47DR; break;
		case XUSP_DEVICE_XCZU48DR: XCZU48DR; break;
		case XUSP_DEVICE_XCZU49DR: XCZU49DR; break;
		case XUSP_DEVICE_XCZU65DR: XCZU65DR; break;
		case XUSP_DEVICE_XCZU67DR: XCZU67DR; break;
		#endif
		//XUSP MPSoC
		#ifdef XUSPZYNQMP
		case XUSP_DEVICE_XCZU1: XCZU1; break;
		case XUSP_DEVICE_XCZU2: XCZU2; break;
		case XUSP_DEVICE_XCZU3: XCZU3; break;
		case XUSP_DEVICE_XCZU4: XCZU4; break;
		case XUSP_DEVICE_XCZU5: XCZU5; break;
		case XUSP_DEVICE_XCZU6: XCZU6; break;
		case XUSP_DEVICE_XCZU7: XCZU7; break;
		case XUSP_DEVICE_XCZU9: XCZU9; break;
		case XUSP_DEVICE_XCZU11: XCZU11; break;
		case XUSP_DEVICE_XCZU15: XCZU15; break;
		case XUSP_DEVICE_XCZU17: XCZU17; break;
		case XUSP_DEVICE_XCZU19: XCZU19; break;
		#endif
		//XUSP Alveo
		#ifdef XUSPALVEO
		case XUSP_DEVICE_XCU200: XCU200; setPart("XCU200-FSGD2104-2-E"); break;
		case XUSP_DEVICE_XCU250: XCU250; setPart("XCU250-FIGD2104-2L-E"); break;
		case XUSP_DEVICE_XCU26: XCU26; setPart("XCU26-L2VSVA1365E"); break;
		case XUSP_DEVICE_XCUX35: XCUX35; break;
		case XUSP_DEVICE_XCU50: XCU50; setPart("XCU50-FSVH2104-2LV-E"); break;
		case XUSP_DEVICE_XCU55N: XCU55N; break;
		case XUSP_DEVICE_XCU55C: XCU55C; setPart("XCU55C-L2FSVH2892E"); break;
		case XUSP_DEVICE_XCU280: XCU280; break;
		case XUSP_DEVICE_XCU30: XCU30; break;
		case XUSP_DEVICE_XCU25: XCU25; break;
		#endif
		//XUSP Boards
		#ifdef XUSPBOARDS
		case XUSP_DEVICE_ZCU102: XCZU9; setPart("XCZU9EG-2FFVB1156"); break;
		case XUSP_DEVICE_ZCU104: XCZU7;setPart("XCZU7EV-2FFVC1156");break;
		case XUSP_DEVICE_ZCU106: XCZU7;setPart("XCZU7EV-2FFVC1156");break;
		case XUSP_DEVICE_AXU15EG: XCZU15;setPart("XCZU15EG-2SFFVB1156I");break;
		case XUSP_DEVICE_AXU2CG: XCZU2;setPart("XCZU2CG-1SFVC784E");break;
		case XUSP_DEVICE_AXU3EG: XCZU3;setPart("XCZU3EG-1SFVC784I");break;
		case XUSP_DEVICE_AXU4EV: XCZU4;setPart("XCZU4EV-1SFVC784I");break;
		case XUSP_DEVICE_AXU5EV: XCZU5;setPart("XCZU5EV-2SFVC784I");break;
		case XUSP_DEVICE_AXU9EG: XCZU9;setPart("XCZU9EG-2FFVB1156I");break;
		case XUSP_DEVICE_ULTRAZED_EV: XCZU7;setPart("XCZU7EV-FBVB900");break;
		case XUSP_DEVICE_CHAMP_XD1S: XCZU4;break;
		case XUSP_DEVICE_ZU_3EG: XCZU3;setPart("XCZU3EG-SFVC784-1-E");break;
		case XUSP_DEVICE_ZU_5EV: XCZU5;setPart("XCZU5EV-SFVC784-1-E");break;
		case XUSP_DEVICE_HTG_Z999_06: XCZU6;setPart("XCZU6EG-2FFVB1156E");break;
		case XUSP_DEVICE_HTG_Z999_09: XCZU9;setPart("XCZU9EG-2FFVB1156E");break;
		case XUSP_DEVICE_HTG_Z999_15: XCZU15;setPart("XCZU15EG-2FFVB1156E");break;
		case XUSP_DEVICE_HTG_930_9_3: XCVU9P;setPart("XCVU9P-3FLGB2104E");break;
		case XUSP_DEVICE_HTG_930_9: XCVU9P;setPart("XCVU9P-2FLGB2104E");break;
		case XUSP_DEVICE_HTG_930_13_3: XCVU13P;setPart("XCVU13P-3FHGB2104E");break;
		case XUSP_DEVICE_HTG_930_13: XCVU13P;setPart("XCVU13P-2FHGB2104E");break;
		case XUSP_DEVICE_HTG_937_37P: XCVU37P;setPart("XCVU37P-2FSVH2892E4539");break;
		case XUSP_DEVICE_HTG_937_47P: XCVU47P;setPart("XCVU47P-2FSVH2892E4539");break;
		case XUSP_DEVICE_HTG_Z922_11: XCZU11;setPart("XCZU11EG-3FFVC1760E");break;
		case XUSP_DEVICE_HTG_Z922_19_DG: XCZU19;setPart("XQZU19EG-2FFVC1760E");break;
		case XUSP_DEVICE_HTG_Z922_19: XCZU19;setPart("XCZU19EG-2FFVC1760E");break;
		case XUSP_DEVICE_CZU3EG: XCZU3;setPart("XCZU3EG-1SFVC784E");break;
		case XUSP_DEVICE_CZU4EV: XCZU4;setPart("XCZU3EG-1SFVC784E");break;
		case XUSP_DEVICE_TE0802: XCZU2;setPart("XCZU2CG-1SBVA484E");break;
		case XUSP_DEVICE_TE0803_04_3BE11: XCZU3;setPart("XCZU3EG-1SFVC784E");break;
		case XUSP_DEVICE_TE0807_03_7DE21: XCZU7;setPart("XCZU7EV-1FBVB900E");break;
		case XUSP_DEVICE_TE0808_05_9BE21: XCZU9;setPart("XCZU9EG-1FFVC900E");break;
		case XUSP_DEVICE_TE0808_05_9GI21: XCZU9;setPart("XCZU9EG-2FFVC900I");break;
		case XUSP_DEVICE_TE0808_05_BBE21: XCZU15;setPart("XCZU15EG-1FFVC900E");break;
		case XUSP_DEVICE_TE0812: XCZU6;setPart("XCZU6EG-1FFVC900I");break;
		case XUSP_DEVICE_TEB0911: XCZU9;setPart("XCZU9EG-1FFVB1156E");break;
		case XUSP_DEVICE_TEB0912: XCZU11;setPart("XCZU11EG-1FFVC1760I");break; 
		case XUSP_DEVICE_TYSOM_3_ZU7EV: XCZU7;setPart("ZU7EV-FFVC1156");break;
		case XUSP_DEVICE_XPEDITE2600: XCZU11;setPart("XCZU11EG");break;
		case XUSP_DEVICE_AMC574: XCZU29DR;setPart("XCZU29DR");break;
		case XUSP_DEVICE_ZCU208: XCZU48DR;setPart("XCZU48DR-2FSVG1517E5184");break;
		case XUSP_DEVICE_ZCU216: XCZU49DR;setPart("XCZU49DR-2FFVF1760");break;
		case XUSP_DEVICE_ZCU670: XCZU67DR;setPart("XCZU67DR-2FSVE1156I");break;
		case XUSP_DEVICE_ULTRA96: XCZU3;setPart("XCZU3EG-SBVA484-1-I");break;
		case XUSP_DEVICE_K26C: XCK26;setPart("XCK26-SFVC784-2LV-C");break;
		case XUSP_DEVICE_K26I: XCK26;setPart("XCK26-SFVC784-2LV-I");break;
		case XUSP_DEVICE_KV260: XCK26;setPart("XCK26-SFVC784-2LV-C");break;
		case XUSP_DEVICE_AMC584: XCVU13P;break;
		case XUSP_DEVICE_HTG_960: XCVU19P;setPart("XCVU19P-2FSVB3824E");break;
		case XUSP_DEVICE_VCU1525: XCVU9P;setPart("XCVU9P-L2FSGD2104E");break;
		case XUSP_DEVICE_VCU118: XCVU9P;setPart("XCVU9P-LGA2104E");break;
		case XUSP_DEVICE_VCU128: XCVU37P;setPart("XCVU37P-L2FSVH2892E");break;
		case XUSP_DEVICE_VCU129: XCVU29P;setPart("XCVU29P-L2FSGA2577E");break;
		case XUSP_DEVICE_HTG_940_9_3: XCVU9P;setPart("XCVU9P-3FLGB2104E ");break;
		case XUSP_DEVICE_HTG_940_9: XCVU9P;setPart("XCVU9P-2FLGB2104E ");break;
		case XUSP_DEVICE_HTG_940_13_3: XCVU13P;setPart("XCVU13P-3FHGB2104E");break;
		case XUSP_DEVICE_HTG_940_13: XCVU13P;setPart("XCVU13P-2FHGB2104E");break;
		case XUSP_DEVICE_HTG_9200_5P: XCVU5P;setPart("XCVU5P-2FLGB2104E");break;
		case XUSP_DEVICE_HTG_9200_9P: XCVU9P;setPart("XCVU9P-2FLGB2104E");break;
		case XUSP_DEVICE_HTG_9200_13P: XCVU13P;setPart("XCVU13P-2FHGB2104E");break;
		case XUSP_DEVICE_HTG_VUP_PCIE_HH_9P: XCVU9P;setPart("XCVU9P-2FLGB2104E");break;
		case XUSP_DEVICE_HTG_VUP_PCIE_HH_13P: XCVU13P;setPart("XCVU13P-2FHGB2104E");break;
		case XUSP_DEVICE_HTG_ZRF_FMC_28: XCZU28DR;setPart("XCZU28DR-2FFVG1517");break;
		case XUSP_DEVICE_HTG_ZRF_FMC_48: XCZU48DR;setPart("XCZU48DR-2FFVG1517");break;
		case XUSP_DEVICE_HTG_ZRF16_29: XCZU29DR;setPart("XCZU29DR-2FFVF1760E");break;
		case XUSP_DEVICE_HTG_ZRF16_49: XCZU49DR;setPart("XCZU49DR-2FFVF1760E");break;
		case XUSP_DEVICE_HTG_ZRF_HH_28: XCZU28DR;setPart("XCZU28DR-2FFVG1517");break;
		case XUSP_DEVICE_HTG_ZRF_HH_48: XCZU48DR;setPart("XCZU48DR-2FFVG1517");break;
		case XUSP_DEVICE_HTG_ZUSP_PCIE_11_3: XCZU11;setPart("XCZU11EG-3FFVC1760E");break;
		case XUSP_DEVICE_HTG_ZUSP_PCIE_19_2: XCZU19;setPart("XCZU19EG-2FFVC1760E");break;
		case XUSP_DEVICE_HTG_ZRF8_R2_28: XCZU28DR;setPart("XCZU28DR-2FFVG1517");break;
		case XUSP_DEVICE_HTG_ZRF8_R2_48_I: XCZU48DR;setPart("XCZU48DR-FFVG1517-2-I");break;
		case XUSP_DEVICE_HTG_ZRF8_R2_48: XCZU48DR;setPart("XCZU48DR-2FFVG1517");break;
		#endif
		default:throw runtime_error("Unknown device ID.");
	}
	#undef setPart
    if(!customNameSet) // if we didn't set already a good full name
		if(customPartName != "")// if not empty string
			partName = customPartName;

}
void XilinxUltraScalePlus::printDeviceList(){
	cout << "A list of currently recognized US+ devices: " << endl;
	#ifdef XUSPARTIX
	cout << "\tUltraScale+ Artix: " << endl;
	cout << "\t\tXCAU10P, XCAU15P, XCAU20P, XCAU25P" << endl;
	#endif
	#ifdef XUSPKINTEX
	cout << "\tUltraScale+ Kintex: " << endl;
	cout << "\t\tXCKU3P, XQKU3P, XCKU5P, XQKU5P, XCKU9P, XQKU9P, XCKU11P, XQKU11P, XCKU13P, XQKU13P, XCKU15P, XQKU15P, XCKU19P, XQKU19P" << endl;
	#endif
	#ifdef XUSPVIRTEX
	cout << "\tUltraScale+ Virtex: " << endl;
	cout << "\t\tXCVU3P, XQVU3P, XCVU5P, XQVU5P, XCVU7P, XQVU7P, XCVU9P, XQVU9P, XCVU11P, XQVU11P, XCVU13P, XQVU13P," << endl;
	cout << "\t\tXCVU19P, XQVU19P, XCVU23P, XQVU23P, XCVU27P, XQVU27P, XCVU29P, XQVU29P, XCVU31P, XQVU31P, XCVU33P," << endl;
	cout << "\t\tXQVU33P, XCVU35P, XQVU35P, XCVU37P, XQVU37P, XCVU45P, XQVU45P, XCVU47P, XQVU47P, XCVU57P, XQVU57P" << endl;
	#endif
	#ifdef XUSPZYNQRF
	cout << "\tUltraScale+ Zynq RFSoC: " << endl;
	cout << "\t\tXCZU21DR, XQZU21DR, XCZU25DR, XCZU27DR, XCZU28DR, XQZU28DR, XCZU29DR, XQZU29DR, XCZU39DR, XCZU42DR," << endl;
	cout << "\t\tXCZU43DR, XCZU46DR, XQZU48DR, XQZU49DR, XCZU47DR, XCZU48DR, XCZU49DR, XCZU65DR, XCZU67DR" << endl;
	#endif
	#ifdef XUSPZYNQMP
	cout << "\tUltraScale+ Zynq MPSoC: " << endl;
	cout << "\t\tXCZU1, XAZU1, XQZU1, XCZU2, XAZU2, XQZU2, XCZU3, XAZU3, XQZU3, XCZU4, XAZU4, XQZU4," << endl;
	cout << "\t\tXCZU5, XAZU5, XQZU5, XCZU6, XAZU6, XQZU6, XCZU7, XAZU7, XQZU7, XCZU9, XAZU9, XQZU9," << endl;
	cout << "\t\tXCZU11, XAZU11, XQZU11, XCZU15, XAZU15, XQZU15, XCZU17, XAZU17, XQZU17, XCZU19, XAZU19, XQZU19" << endl;
	#endif
	#ifdef XUSPALVEO
	cout << "\tUltraScale+ Alveo: " << endl;
	cout << "\t\tXCU200, Alveo U200, XCU250, Alveo U250, XCU26, Alveo U26, XCUX35, Alveo U35, XCU50, Alveo U50," << endl;
	cout << "\t\tXCU55N, Alveo U55N, XCU55C, Alveo U55C, XCU280, Alveo U280, XCU30, Alveo U30, XCU25, Alveo U25" << endl;
	#endif
	#ifdef XUSPBOARDS
	cout << "\tMisc UltraScale+ Boards: " << endl;
	cout << "\t\tZCU 102, ZU-GMSL2, ZCU104, ZCU106, AXU15EG, AXU2CG, AXU3EG, AXU4EV, AXU5EV, AXU9EG, ULTRAZED-EV, CHAMP-XD1S, ZU-3EG," << endl;
	cout << "\t\tZU-5EV, HTG-Z999-06, HTG-Z999-09, HTG-Z999-15, HTG-930-9-3, HTG-930-9, HTG-930-13-3, HTG-930-13, HTG-937-37P, HTG-937-47P," << endl;
	cout << "\t\tHTG-Z922-11, HTG-Z922-19-DG, HTG-Z922-19, CZU3EG, CZU4EV, TE0802, TE0803-04-3BE11, TE0807-03-7DE21, TE0808-05-9BE21," << endl;
	cout << "\t\tTE0808-05-9GI21, TE0808-05-BBE21, TE0812, TEB0911, TEB0912, TYSOM-3-ZU7EV, XPEDITE2600, AMC574, ZCU208, ZCU216, ZCU670," << endl;
	cout << "\t\tUltra96, K26C, K26I, KV260, AMC584, HTG-960, VCU1525, VCU118, VCU128, VCU129, HTG-940-9-3, HTG-940-9, HTG-940-13-3," << endl;
	cout << "\t\tHTG-940-13, HTG-9200-5P, HTG-9200-9P, HTG-9200-13P, HTG-VUP-PCIE-HH-9P, HTG-910-9, HTG-VUP-PCIE-HH-13P, HTG-910-13," << endl;
	cout << "\t\tHTG-ZRF-FMC-28, HTG-ZRF-FMC-48, HTG-ZRF16-29, HTG-ZRF16-49, HTG-ZRF-HH-28, HTG-ZRF-HH-48, HTG-ZUSP-PCIE-11-3," << endl;
	cout << "\t\tHTG-ZUSP-PCIE-19-2, HTG-ZRF8-R2-28, HTG-ZRF8-28, HTG-ZRF8-R2-48-I, HTG-ZRF8-48-I, HTG-ZRF8-R2-48, HTG-ZRF8-48" << endl;
	#endif
}