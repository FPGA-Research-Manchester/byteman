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

#include "../../../Common/str.h"
#include "XilinxUltraScalePlus.h"
#include "XilinxUltraScalePlusDevices.h"

using namespace std;

int XilinxUltraScalePlus::getDeviceByIDCODEorThrow(int IDCODE)
{
	int deviceID = XilinxUltraScalePlus::getDeviceByIDCODE(IDCODE);
	if(deviceID == XUSP_DEVICE_NULL)
		throw runtime_error(string("Unknown device with IDCODE = ").append(to_string(IDCODE)).append(" ."));
	return deviceID;
}
int XilinxUltraScalePlus::getDeviceByNameOrThrow(string name)
{
	int deviceID = XilinxUltraScalePlus::getDeviceByName(name);
	if(deviceID == XUSP_DEVICE_NULL)
		throw runtime_error(string("Unknown device: ").append(name).append("."));
	return deviceID;
}
int XilinxUltraScalePlus::getDeviceByIDCODE(int IDCODE)
{
	//US+ Artix
	#ifdef XUSPARTIX
	if(IDCODE == XCAU10P_IDCODE)
		return XUSP_DEVICE_XCAU10P;
	if(IDCODE == XCAU15P_IDCODE)
		return XUSP_DEVICE_XCAU15P;
	if(IDCODE == XCAU20P_IDCODE)
		return XUSP_DEVICE_XCAU20P;
	if(IDCODE == XCAU25P_IDCODE)
		return XUSP_DEVICE_XCAU25P;
	#endif
	//US+ Kintex
	#ifdef XUSPKINTEX
	if(IDCODE == XCKU3P_IDCODE)
		return XUSP_DEVICE_XCKU3P;
	if(IDCODE == XCKU5P_IDCODE)
		return XUSP_DEVICE_XCKU5P;
	if(IDCODE == XCKU9P_IDCODE)
		return XUSP_DEVICE_XCKU9P;
	if(IDCODE == XCKU11P_IDCODE)
		return XUSP_DEVICE_XCKU11P;
	if(IDCODE == XCKU13P_IDCODE)
		return XUSP_DEVICE_XCKU13P;
	if(IDCODE == XCKU15P_IDCODE)
		return XUSP_DEVICE_XCKU15P;
	if(IDCODE == XCKU19P_IDCODE)
		return XUSP_DEVICE_XCKU19P;
	#endif
	//US+ Virtex
	#ifdef XUSPVIRTEX
	if(IDCODE == XCVU3P_IDCODE)
		return XUSP_DEVICE_XCVU3P;
	if(IDCODE == XCVU5P_IDCODE)
		return XUSP_DEVICE_XCVU5P;
	if(IDCODE == XCVU7P_IDCODE)
		return XUSP_DEVICE_XCVU7P;
	if(IDCODE == XCVU9P_IDCODE)
		return XUSP_DEVICE_XCVU9P;
	if(IDCODE == XCVU11P_IDCODE)
		return XUSP_DEVICE_XCVU11P;
	if(IDCODE == XCVU13P_IDCODE)
		return XUSP_DEVICE_XCVU13P;
	if(IDCODE == XCVU19P_IDCODE)
		return XUSP_DEVICE_XCVU19P;
	if(IDCODE == XCVU23P_IDCODE)
		return XUSP_DEVICE_XCVU23P;
	if(IDCODE == XCVU27P_IDCODE)
		return XUSP_DEVICE_XCVU27P;
	if(IDCODE == XCVU29P_IDCODE)
		return XUSP_DEVICE_XCVU29P;
	if(IDCODE == XCVU31P_IDCODE)
		return XUSP_DEVICE_XCVU31P;
	if(IDCODE == XCVU33P_IDCODE)
		return XUSP_DEVICE_XCVU33P;
	if(IDCODE == XCVU35P_IDCODE)
		return XUSP_DEVICE_XCVU35P;
	if(IDCODE == XCVU37P_IDCODE)
		return XUSP_DEVICE_XCVU37P;
	if(IDCODE == XCVU45P_IDCODE)
		return XUSP_DEVICE_XCVU45P;
	if(IDCODE == XCVU47P_IDCODE)
		return XUSP_DEVICE_XCVU47P;
	if(IDCODE == XCVU57P_IDCODE)
		return XUSP_DEVICE_XCVU57P;
	#endif
	//US+ RFSoC (RFSOC BEFORE MPSOC, since names collide!)
	#ifdef XUSPZYNQ
	if(IDCODE == XCZU21DR_IDCODE)
		return XUSP_DEVICE_XCZU21DR;
	if(IDCODE == XCZU25DR_IDCODE)
		return XUSP_DEVICE_XCZU25DR;
	if(IDCODE == XCZU27DR_IDCODE)
		return XUSP_DEVICE_XCZU27DR;
	if(IDCODE == XCZU28DR_IDCODE)
		return XUSP_DEVICE_XCZU28DR;
	if(IDCODE == XCZU29DR_IDCODE)
		return XUSP_DEVICE_XCZU29DR;
	if(IDCODE == XCZU39DR_IDCODE)
		return XUSP_DEVICE_XCZU39DR;
	if(IDCODE == XCZU42DR_IDCODE)
		return XUSP_DEVICE_XCZU42DR;
	if(IDCODE == XCZU43DR_IDCODE)
		return XUSP_DEVICE_XCZU43DR;
	if(IDCODE == XCZU46DR_IDCODE)
		return XUSP_DEVICE_XCZU46DR;
	if(IDCODE == XCZU47DR_IDCODE)
		return XUSP_DEVICE_XCZU47DR;
	if(IDCODE == XCZU48DR_IDCODE)
		return XUSP_DEVICE_XCZU48DR;
	if(IDCODE == XCZU49DR_IDCODE)
		return XUSP_DEVICE_XCZU49DR;
	if(IDCODE == XCZU65DR_IDCODE)
		return XUSP_DEVICE_XCZU65DR;
	if(IDCODE == XCZU67DR_IDCODE)
		return XUSP_DEVICE_XCZU67DR;
	#endif
	//US+ Zynq
	#ifdef XUSPZYNQ
	if(IDCODE == XCZU1_IDCODE)
		return XUSP_DEVICE_XCZU1;
	if(IDCODE == XCZU2_IDCODE)
		return XUSP_DEVICE_XCZU2;
	if(IDCODE == XCZU3_IDCODE)
		return XUSP_DEVICE_XCZU3;
	if(IDCODE == XCZU4_IDCODE)
		return XUSP_DEVICE_XCZU4;
	if(IDCODE == XCZU5_IDCODE)
		return XUSP_DEVICE_XCZU5;
	if(IDCODE == XCZU6_IDCODE)
		return XUSP_DEVICE_XCZU6;
	if(IDCODE == XCZU7_IDCODE)
		return XUSP_DEVICE_XCZU7;
	if(IDCODE == XCZU9_IDCODE)
		return XUSP_DEVICE_XCZU9;
	if(IDCODE == XCZU11_IDCODE)
		return XUSP_DEVICE_XCZU11;
	if(IDCODE == XCZU15_IDCODE)
		return XUSP_DEVICE_XCZU15;
	if(IDCODE == XCZU17_IDCODE)
		return XUSP_DEVICE_XCZU17;
	if(IDCODE == XCZU19_IDCODE)
		return XUSP_DEVICE_XCZU19;
	#endif


	//Alveo rebranding
	#ifdef XUSPALVEO
	if(IDCODE == XCU200_IDCODE)
		return XUSP_DEVICE_XCU200;
	if(IDCODE == XCU250_IDCODE)
		return XUSP_DEVICE_XCU250;
	if(IDCODE == XCU26_IDCODE)
		return XUSP_DEVICE_XCU26;
	if(IDCODE == XCUX35_IDCODE)
		return XUSP_DEVICE_XCUX35;
	if(IDCODE == XCU50_IDCODE)
		return XUSP_DEVICE_XCU50;
	if(IDCODE == XCU55N_IDCODE)
		return XUSP_DEVICE_XCU55N;
	if(IDCODE == XCU55C_IDCODE)
		return XUSP_DEVICE_XCU55C;
	if(IDCODE == XCU280_IDCODE)
		return XUSP_DEVICE_XCU280;
	if(IDCODE == XCU30_IDCODE)
		return XUSP_DEVICE_XCU30;
	if(IDCODE == XCU25_IDCODE)
		return XUSP_DEVICE_XCU25;
	#endif
	
	return XUSP_DEVICE_NULL;
}
int XilinxUltraScalePlus::getDeviceByName(string name)
{
	name = str::removeSpaces(name);
	name = str::stringToLower(name);
	
	//US+ Artix
	#ifdef XUSPARTIX
		if(str::iff::stringContains(name, XCAU10P_NAME))
			return XUSP_DEVICE_XCAU10P;
		if(str::iff::stringContains(name, XCAU15P_NAME))
			return XUSP_DEVICE_XCAU15P;
		if(str::iff::stringContains(name, XCAU20P_NAME))
			return XUSP_DEVICE_XCAU20P;
		if(str::iff::stringContains(name, XCAU25P_NAME))
			return XUSP_DEVICE_XCAU25P;
	#endif
	//US+ Kintex
	#ifdef XUSPKINTEX
		if(str::iff::stringContains(name, XCKU3P_NAME,  "xqku3p"))
			return XUSP_DEVICE_XCKU3P;
		if(str::iff::stringContains(name, XCKU5P_NAME,  "xqku5p"))
			return XUSP_DEVICE_XCKU5P;
		if(str::iff::stringContains(name, XCKU9P_NAME,  "xqku9p"))
			return XUSP_DEVICE_XCKU9P;
		if(str::iff::stringContains(name, XCKU11P_NAME, "xqku11p"))
			return XUSP_DEVICE_XCKU11P;
		if(str::iff::stringContains(name, XCKU13P_NAME, "xqku13p"))
			return XUSP_DEVICE_XCKU13P;
		if(str::iff::stringContains(name, XCKU15P_NAME, "xqku15p"))
			return XUSP_DEVICE_XCKU15P;
		if(str::iff::stringContains(name, XCKU19P_NAME, "xqku19p"))
			return XUSP_DEVICE_XCKU19P;
	#endif
	//US+ Virtex
	#ifdef XUSPVIRTEX
		if(str::iff::stringContains(name, XCVU3P_NAME,  "xqvu3p"))
			return XUSP_DEVICE_XCVU3P;
		if(str::iff::stringContains(name, XCVU5P_NAME,  "xqvu5p"))
			return XUSP_DEVICE_XCVU5P;
		if(str::iff::stringContains(name, XCVU7P_NAME,  "xqvu7p"))
			return XUSP_DEVICE_XCVU7P;
		if(str::iff::stringContains(name, XCVU9P_NAME,  "xqvu9p"))
			return XUSP_DEVICE_XCVU9P;
		if(str::iff::stringContains(name, XCVU11P_NAME, "xqvu11p"))
			return XUSP_DEVICE_XCVU11P;
		if(str::iff::stringContains(name, XCVU13P_NAME, "xqvu13p"))
			return XUSP_DEVICE_XCVU13P;
		if(str::iff::stringContains(name, XCVU19P_NAME, "xqvu19p"))
			return XUSP_DEVICE_XCVU19P;
		if(str::iff::stringContains(name, XCVU23P_NAME, "xqvu23p"))
			return XUSP_DEVICE_XCVU23P;
		if(str::iff::stringContains(name, XCVU27P_NAME, "xqvu27p"))
			return XUSP_DEVICE_XCVU27P;
		if(str::iff::stringContains(name, XCVU29P_NAME, "xqvu29p"))
			return XUSP_DEVICE_XCVU29P;
		if(str::iff::stringContains(name, XCVU31P_NAME, "xqvu31p"))
			return XUSP_DEVICE_XCVU31P;
		if(str::iff::stringContains(name, XCVU33P_NAME, "xqvu33p"))
			return XUSP_DEVICE_XCVU33P;
		if(str::iff::stringContains(name, XCVU35P_NAME, "xqvu35p"))
			return XUSP_DEVICE_XCVU35P;
		if(str::iff::stringContains(name, XCVU37P_NAME, "xqvu37p"))
			return XUSP_DEVICE_XCVU37P;
		if(str::iff::stringContains(name, XCVU45P_NAME, "xqvu45p"))
			return XUSP_DEVICE_XCVU45P;
		if(str::iff::stringContains(name, XCVU47P_NAME, "xqvu47p"))
			return XUSP_DEVICE_XCVU47P;
		if(str::iff::stringContains(name, XCVU57P_NAME, "xqvu57p"))
			return XUSP_DEVICE_XCVU57P;
	#endif
	//US+ RFSoC (RFSOC BEFORE MPSOC, since names collide!)
	#ifdef XUSPZYNQRF
		if(str::iff::stringContains(name, XCZU21DR_NAME, "xqzu21dr"))
			return XUSP_DEVICE_XCZU21DR;
		if(str::iff::stringContains(name, XCZU25DR_NAME))
			return XUSP_DEVICE_XCZU25DR;
		if(str::iff::stringContains(name, XCZU27DR_NAME))
			return XUSP_DEVICE_XCZU27DR;
		if(str::iff::stringContains(name, XCZU28DR_NAME, "xqzu28dr"))
			return XUSP_DEVICE_XCZU28DR;
		if(str::iff::stringContains(name, XCZU29DR_NAME, "xqzu29dr"))
			return XUSP_DEVICE_XCZU29DR;
		if(str::iff::stringContains(name, XCZU39DR_NAME))
			return XUSP_DEVICE_XCZU39DR;
		if(str::iff::stringContains(name, XCZU42DR_NAME))
			return XUSP_DEVICE_XCZU42DR;
		if(str::iff::stringContains(name, XCZU43DR_NAME))
			return XUSP_DEVICE_XCZU43DR;
		if(str::iff::stringContains(name, XCZU46DR_NAME, "xqzu48dr", "xqzu49dr"))
			return XUSP_DEVICE_XCZU46DR; // Yes, this row is correct.
		if(str::iff::stringContains(name, XCZU47DR_NAME))
			return XUSP_DEVICE_XCZU47DR;
		if(str::iff::stringContains(name, XCZU48DR_NAME))
			return XUSP_DEVICE_XCZU48DR;
		if(str::iff::stringContains(name, XCZU49DR_NAME))
			return XUSP_DEVICE_XCZU49DR;
		if(str::iff::stringContains(name, XCZU65DR_NAME))
			return XUSP_DEVICE_XCZU65DR;
		if(str::iff::stringContains(name, XCZU67DR_NAME))
			return XUSP_DEVICE_XCZU67DR;
	#endif
	//US+ Zynq
	#ifdef XUSPZYNQMP
		if(str::iff::stringContains(name, XCZU1_NAME,  "xazu1",  "xqzu1"))
			return XUSP_DEVICE_XCZU1;
		if(str::iff::stringContains(name, XCZU2_NAME,  "xazu2",  "xqzu2"))
			return XUSP_DEVICE_XCZU2;
		if(str::iff::stringContains(name, XCZU3_NAME,  "xazu3",  "xqzu3"))
			return XUSP_DEVICE_XCZU3;
		if(str::iff::stringContains(name, XCZU4_NAME,  "xazu4",  "xqzu4"))
			return XUSP_DEVICE_XCZU4;
		if(str::iff::stringContains(name, XCZU5_NAME,  "xazu5",  "xqzu5"))
			return XUSP_DEVICE_XCZU5;
		if(str::iff::stringContains(name, XCZU6_NAME,  "xazu6",  "xqzu6"))
			return XUSP_DEVICE_XCZU6;
		if(str::iff::stringContains(name, XCZU7_NAME,  "xazu7",  "xqzu7"))
			return XUSP_DEVICE_XCZU7;
		if(str::iff::stringContains(name, XCZU9_NAME,  "xazu9",  "xqzu9"))
			return XUSP_DEVICE_XCZU9;
		if(str::iff::stringContains(name, XCZU11_NAME, "xazu11", "xqzu11"))
			return XUSP_DEVICE_XCZU11;
		if(str::iff::stringContains(name, XCZU15_NAME, "xazu15", "xqzu15"))
			return XUSP_DEVICE_XCZU15;
		if(str::iff::stringContains(name, XCZU17_NAME, "xazu17", "xqzu17"))
			return XUSP_DEVICE_XCZU17;
		if(str::iff::stringContains(name, XCZU19_NAME, "xazu19", "xqzu19"))
			return XUSP_DEVICE_XCZU19;
	#endif


	//Alveo rebranding
	#ifdef XUSPALVEO
		if(str::iff::stringContains(name, XCU200_NAME, "alveou200"))
			return XUSP_DEVICE_XCU200;
		if(str::iff::stringContains(name, XCU250_NAME, "alveou250"))
			return XUSP_DEVICE_XCU250;
		if(str::iff::stringContains(name, XCU26_NAME, "alveou26"))
			return XUSP_DEVICE_XCU26;
		if(str::iff::stringContains(name, XCUX35_NAME, "alveou35"))
			return XUSP_DEVICE_XCUX35;
		if(str::iff::stringContains(name, XCU50_NAME, "alveou50"))
			return XUSP_DEVICE_XCU50;
		if(str::iff::stringContains(name, XCU55N_NAME, "alveou55n"))
			return XUSP_DEVICE_XCU55N;
		if(str::iff::stringContains(name, XCU55C_NAME, "alveou55c"))
			return XUSP_DEVICE_XCU55C;
		if(str::iff::stringContains(name, XCU280_NAME, "alveou280"))
			return XUSP_DEVICE_XCU280;
		if(str::iff::stringContains(name, XCU30_NAME, "alveou30"))
			return XUSP_DEVICE_XCU30;
		if(str::iff::stringContains(name, XCU25_NAME, "alveou25"))
			return XUSP_DEVICE_XCU25;
	#endif

	//Some boards for easier use
	#ifdef XUSPBOARDS
		if(str::iff::stringContains(name, "zcu102", "zu-gmsl2"))
			return XUSP_DEVICE_ZCU102;
		if(str::iff::stringContains(name, "zcu104"))
			return XUSP_DEVICE_ZCU104;
		if(str::iff::stringContains(name, "zcu106"))
			return XUSP_DEVICE_ZCU106;
		if(str::iff::stringContains(name, "axu15eg"))
			return XUSP_DEVICE_AXU15EG;
		if(str::iff::stringContains(name, "axu2cg"))
			return XUSP_DEVICE_AXU2CG;
		if(str::iff::stringContains(name, "axu3eg"))
			return XUSP_DEVICE_AXU3EG;
		if(str::iff::stringContains(name, "axu4ev"))
			return XUSP_DEVICE_AXU4EV;
		if(str::iff::stringContains(name, "axu5ev"))
			return XUSP_DEVICE_AXU5EV;
		if(str::iff::stringContains(name, "axu9eg"))
			return XUSP_DEVICE_AXU9EG;
		if(str::iff::stringContains(name, "ultrazed-ev"))
			return XUSP_DEVICE_ULTRAZED_EV;
		if(str::iff::stringContains(name, "champ-xd1s"))
			return XUSP_DEVICE_CHAMP_XD1S;
		if(str::iff::stringContains(name, "zu-3eg"))
			return XUSP_DEVICE_ZU_3EG;
		if(str::iff::stringContains(name, "zu-5ev"))
			return XUSP_DEVICE_ZU_5EV;
		if(str::iff::stringContains(name, "htg-z999-06"))
			return XUSP_DEVICE_HTG_Z999_06;
		if(str::iff::stringContains(name, "htg-z999-09"))
			return XUSP_DEVICE_HTG_Z999_09;
		if(str::iff::stringContains(name, "htg-z999-15"))
			return XUSP_DEVICE_HTG_Z999_15;
		if(str::iff::stringContains(name, "htg-930-9-3"))
			return XUSP_DEVICE_HTG_930_9_3;
		if(str::iff::stringContains(name, "htg-930-9"))
			return XUSP_DEVICE_HTG_930_9;
		if(str::iff::stringContains(name, "htg-930-13-3"))
			return XUSP_DEVICE_HTG_930_13_3;
		if(str::iff::stringContains(name, "htg-930-13"))
			return XUSP_DEVICE_HTG_930_13;
		if(str::iff::stringContains(name, "htg-937-37p"))
			return XUSP_DEVICE_HTG_937_37P;
		if(str::iff::stringContains(name, "htg-937-47p"))
			return XUSP_DEVICE_HTG_937_47P;
		if(str::iff::stringContains(name, "htg-z922-11"))
			return XUSP_DEVICE_HTG_Z922_11;
		if(str::iff::stringContains(name, "htg-z922-19-dg"))
			return XUSP_DEVICE_HTG_Z922_19_DG;
		if(str::iff::stringContains(name, "htg-z922-19"))
			return XUSP_DEVICE_HTG_Z922_19;
		if(str::iff::stringContains(name, "czu3eg"))
			return XUSP_DEVICE_CZU3EG;
		if(str::iff::stringContains(name, "czu4ev"))
			return XUSP_DEVICE_CZU4EV;
		if(str::iff::stringContains(name, "te0802"))
			return XUSP_DEVICE_TE0802;
		if(str::iff::stringContains(name, "te0803-04-3be11"))
			return XUSP_DEVICE_TE0803_04_3BE11;
		if(str::iff::stringContains(name, "te0807-03-7de21"))
			return XUSP_DEVICE_TE0807_03_7DE21;
		if(str::iff::stringContains(name, "te0808-05-9be21"))
			return XUSP_DEVICE_TE0808_05_9BE21;
		if(str::iff::stringContains(name, "te0808-05-9gi21"))
			return XUSP_DEVICE_TE0808_05_9GI21;
		if(str::iff::stringContains(name, "te0808-05-bbe21"))
			return XUSP_DEVICE_TE0808_05_BBE21;
		if(str::iff::stringContains(name, "te0812"))
			return XUSP_DEVICE_TE0812;
		if(str::iff::stringContains(name, "teb0911"))
			return XUSP_DEVICE_TEB0911;
		if(str::iff::stringContains(name, "teb0912"))
			return XUSP_DEVICE_TEB0912;
		if(str::iff::stringContains(name, "tysom-3-zu7ev"))
			return XUSP_DEVICE_TYSOM_3_ZU7EV;
		if(str::iff::stringContains(name, "xpedite2600"))
			return XUSP_DEVICE_XPEDITE2600;
		if(str::iff::stringContains(name, "amc574"))
			return XUSP_DEVICE_AMC574;
		if(str::iff::stringContains(name, "zcu208"))
			return XUSP_DEVICE_ZCU208;
		if(str::iff::stringContains(name, "zcu216"))
			return XUSP_DEVICE_ZCU216;
		if(str::iff::stringContains(name, "zcu670"))
			return XUSP_DEVICE_ZCU670;
		if(str::iff::stringContains(name, "ultra96"))
			return XUSP_DEVICE_ULTRA96;
		if(str::iff::stringContains(name, "k26c"))
			return XUSP_DEVICE_K26C;
		if(str::iff::stringContains(name, "k26i"))
			return XUSP_DEVICE_K26I;
		if(str::iff::stringContains(name, "kv260"))
			return XUSP_DEVICE_KV260;
		if(str::iff::stringContains(name, "amc584"))
			return XUSP_DEVICE_AMC584;
		if(str::iff::stringContains(name, "htg-960"))
			return XUSP_DEVICE_HTG_960;
		if(str::iff::stringContains(name, "vcu1525"))
			return XUSP_DEVICE_VCU1525;
		if(str::iff::stringContains(name, "vcu118"))
			return XUSP_DEVICE_VCU118;
		if(str::iff::stringContains(name, "vcu128"))
			return XUSP_DEVICE_VCU128;
		if(str::iff::stringContains(name, "vcu129"))
			return XUSP_DEVICE_VCU129;
		if(str::iff::stringContains(name, "htg-940-9-3"))
			return XUSP_DEVICE_HTG_940_9_3;
		if(str::iff::stringContains(name, "htg-940-9"))
			return XUSP_DEVICE_HTG_940_9;
		if(str::iff::stringContains(name, "htg-940-13-3"))
			return XUSP_DEVICE_HTG_940_13_3;
		if(str::iff::stringContains(name, "htg-940-13"))
			return XUSP_DEVICE_HTG_940_13;
		if(str::iff::stringContains(name, "htg-9200-5p"))
			return XUSP_DEVICE_HTG_9200_5P;
		if(str::iff::stringContains(name, "htg-9200-9p"))
			return XUSP_DEVICE_HTG_9200_9P;
		if(str::iff::stringContains(name, "htg-9200-13p"))
			return XUSP_DEVICE_HTG_9200_13P;
		if(str::iff::stringContains(name, "htg-vup-pcie-hh-9p", "htg-910-9"))
			return XUSP_DEVICE_HTG_VUP_PCIE_HH_9P;
		if(str::iff::stringContains(name, "htg-vup-pcie-hh-13p", "htg-910-13"))
			return XUSP_DEVICE_HTG_VUP_PCIE_HH_13P;
		if(str::iff::stringContains(name, "htg-zrf-fmc-28"))
			return XUSP_DEVICE_HTG_ZRF_FMC_28;
		if(str::iff::stringContains(name, "htg-zrf-fmc-48"))
			return XUSP_DEVICE_HTG_ZRF_FMC_48;
		if(str::iff::stringContains(name, "htg-zrf16-29"))
			return XUSP_DEVICE_HTG_ZRF16_29;
		if(str::iff::stringContains(name, "htg-zrf16-49"))
			return XUSP_DEVICE_HTG_ZRF16_49;
		if(str::iff::stringContains(name, "htg-zrf-hh-28"))
			return XUSP_DEVICE_HTG_ZRF_HH_28;
		if(str::iff::stringContains(name, "htg-zrf-hh-48"))
			return XUSP_DEVICE_HTG_ZRF_HH_48;
		if(str::iff::stringContains(name, "htg-zusp-pcie-11-3"))
			return XUSP_DEVICE_HTG_ZUSP_PCIE_11_3;
		if(str::iff::stringContains(name, "htg-zusp-pcie-19-2"))
			return XUSP_DEVICE_HTG_ZUSP_PCIE_19_2;
		if(str::iff::stringContains(name, "htg-zrf8-r2-28", "htg-zrf8-28"))
			return XUSP_DEVICE_HTG_ZRF8_R2_28;
		if(str::iff::stringContains(name, "htg-zrf8-r2-48-I", "htg-zrf8-48-i"))
			return XUSP_DEVICE_HTG_ZRF8_R2_48_I;
		if(str::iff::stringContains(name, "htg-zrf8-r2-48", "htg-zrf8-48"))
			return XUSP_DEVICE_HTG_ZRF8_R2_48;
		if(str::iff::stringContains(name, "zuboard1cg"))
			return XUSP_DEVICE_ZUBOARD_1CG;
		if(str::iff::stringContains(name, "kcu116"))
			return XUSP_DEVICE_KCU116;
		if(str::iff::stringContains(name, "vermeot1mp"))
			return XUSP_DEVICE_VERMEO_T1_MP;
		if(str::iff::stringContains(name, "vermeot1rf"))
			return XUSP_DEVICE_VERMEO_T1_RF;
		if(str::iff::stringContains(name, "zcu1275"))
			return XUSP_DEVICE_ZCU1275;
		if(str::iff::stringContains(name, "zcu1285"))
			return XUSP_DEVICE_ZCU1285;
		if(str::iff::stringContains(name, "zcu111"))
			return XUSP_DEVICE_ZCU111;
	#endif
	return XUSP_DEVICE_NULL;
}
/************************************************************************** /**
 * setPartName if available. Otherwise will put a default name.
 *****************************************************************************/
void XilinxUltraScalePlus::setDevice(int deviceID, string customPartName)
{
	bool customNameSet = false;
	#define setCustomPartName(x) {customNameSet = true; partName = x;}
	switch(deviceID){
		//XUSP Artix
		#ifdef XUSPARTIX
		case XUSP_DEVICE_XCAU10P:
			XCAU10P();
			break;
		case XUSP_DEVICE_XCAU15P:
			XCAU15P();
			break;
		case XUSP_DEVICE_XCAU20P:
			XCAU20P();
			break;
		case XUSP_DEVICE_XCAU25P:
			XCAU25P();
			break;
		#endif
		//XUSP Kintex
		#ifdef XUSPKINTEX
		case XUSP_DEVICE_XCKU3P:
			XCKU3P();
			break;
		case XUSP_DEVICE_XCKU5P:
			XCKU5P();
			break;
		case XUSP_DEVICE_XCKU9P:
			XCKU9P();
			break;
		case XUSP_DEVICE_XCKU11P:
			XCKU11P();
			break;
		case XUSP_DEVICE_XCKU13P:
			XCKU13P();
			break;
		case XUSP_DEVICE_XCKU15P:
			XCKU15P();
			break;
		case XUSP_DEVICE_XCKU19P:
			XCKU19P();
			break;
		#endif
		//XUSP Virtex
		#ifdef XUSPVIRTEX
		case XUSP_DEVICE_XCVU3P:
			XCVU3P();
			break;
		case XUSP_DEVICE_XCVU5P:
			XCVU5P();
			break;
		case XUSP_DEVICE_XCVU7P:
			XCVU7P();
			break;
		case XUSP_DEVICE_XCVU9P:
			XCVU9P();
			break;
		case XUSP_DEVICE_XCVU11P:
			XCVU11P();
			break;
		case XUSP_DEVICE_XCVU13P:
			XCVU13P();
			break;
		case XUSP_DEVICE_XCVU19P:
			XCVU19P();
			break;
		case XUSP_DEVICE_XCVU23P:
			XCVU23P();
			break;
		case XUSP_DEVICE_XCVU27P:
			XCVU27P();
			break;
		case XUSP_DEVICE_XCVU29P:
			XCVU29P();
			break;
		case XUSP_DEVICE_XCVU31P:
			XCVU31P();
			break;
		case XUSP_DEVICE_XCVU33P:
			XCVU33P();
			break;
		case XUSP_DEVICE_XCVU35P:
			XCVU35P();
			break;
		case XUSP_DEVICE_XCVU37P:
			XCVU37P();
			break;
		case XUSP_DEVICE_XCVU45P:
			XCVU45P();
			break;
		case XUSP_DEVICE_XCVU47P:
			XCVU47P();
			break;
		case XUSP_DEVICE_XCVU57P:
			XCVU57P();
			break;
		#endif
		//XUSP RFSoC
		#ifdef XUSPZYNQRF
		case XUSP_DEVICE_XCZU21DR:
			XCZU21DR();
			break;
		case XUSP_DEVICE_XCZU25DR:
			XCZU25DR();
			break;
		case XUSP_DEVICE_XCZU27DR:
			XCZU27DR();
			break;
		case XUSP_DEVICE_XCZU28DR:
			XCZU28DR();
			break;
		case XUSP_DEVICE_XCZU29DR:
			XCZU29DR();
			break;
		case XUSP_DEVICE_XCZU39DR:
			XCZU39DR();
			break;
		case XUSP_DEVICE_XCZU42DR:
			XCZU42DR();
			break;
		case XUSP_DEVICE_XCZU43DR:
			XCZU43DR();
			break;
		case XUSP_DEVICE_XCZU46DR:
			XCZU46DR();
			break;
		case XUSP_DEVICE_XCZU47DR:
			XCZU47DR();
			break;
		case XUSP_DEVICE_XCZU48DR:
			XCZU48DR();
			break;
		case XUSP_DEVICE_XCZU49DR:
			XCZU49DR();
			break;
		case XUSP_DEVICE_XCZU65DR:
			XCZU65DR();
			break;
		case XUSP_DEVICE_XCZU67DR:
			XCZU67DR();
			break;
		#endif
		//XUSP MPSoC
		#ifdef XUSPZYNQMP
		case XUSP_DEVICE_XCZU1:
			XCZU1();
			break;
		case XUSP_DEVICE_XCZU2:
			XCZU2();
			break;
		case XUSP_DEVICE_XCZU3:
			XCZU3();
			break;
		case XUSP_DEVICE_XCZU4:
			XCZU4();
			break;
		case XUSP_DEVICE_XCZU5:
			XCZU5();
			break;
		case XUSP_DEVICE_XCZU6:
			XCZU6();
			break;
		case XUSP_DEVICE_XCZU7:
			XCZU7();
			break;
		case XUSP_DEVICE_XCZU9:
			XCZU9();
			break;
		case XUSP_DEVICE_XCZU11:
			XCZU11();
			break;
		case XUSP_DEVICE_XCZU15:
			XCZU15();
			break;
		case XUSP_DEVICE_XCZU17:
			XCZU17();
			break;
		case XUSP_DEVICE_XCZU19:
			XCZU19();
			break;
		#endif
		//XUSP Alveo
		#ifdef XUSPALVEO
		case XUSP_DEVICE_XCU200:
			XCU200(); 
			setCustomPartName("xcu200-fsgd2104-2-e");
			break;
		case XUSP_DEVICE_XCU250:
			XCU250(); 
			setCustomPartName("xcu250-figd2104-2l-e");
			break;
		case XUSP_DEVICE_XCU26:
			XCU26(); 
			setCustomPartName("xcu26-l2vsva1365e");
			break;
		case XUSP_DEVICE_XCUX35:
			XCUX35();
			break;
		case XUSP_DEVICE_XCU50:
			XCU50(); 
			setCustomPartName("xcu50-fsvh2104-2lv-e");
			break;
		case XUSP_DEVICE_XCU55N:
			XCU55N();
			break;
		case XUSP_DEVICE_XCU55C:
			XCU55C(); 
			setCustomPartName("xcu55c-l2fsvh2892e");
			break;
		case XUSP_DEVICE_XCU280:
			XCU280();
			break;
		case XUSP_DEVICE_XCU30:
			XCU30();
			break;
		case XUSP_DEVICE_XCU25:
			XCU25();
			break;
		#endif
		//XUSP Boards
		#ifdef XUSPBOARDS
		case XUSP_DEVICE_ZCU102:
			XCZU9(); 
			setCustomPartName("xczu9eg-2ffvb1156");
			break;
		case XUSP_DEVICE_ZCU104:
			XCZU7(); 
			setCustomPartName("xczu7ev-2ffvc1156");
			break;
		case XUSP_DEVICE_ZCU106:
			XCZU7(); 
			setCustomPartName("xczu7ev-2ffvc1156");
			break;
		case XUSP_DEVICE_AXU15EG:
			XCZU15(); 
			setCustomPartName("xczu15eg-2sffvb1156i");
			break;
		case XUSP_DEVICE_AXU2CG:
			XCZU2(); 
			setCustomPartName("xczu2cg-1sfvc784e");
			break;
		case XUSP_DEVICE_AXU3EG:
			XCZU3(); 
			setCustomPartName("xczu3eg-1sfvc784i");
			break;
		case XUSP_DEVICE_AXU4EV:
			XCZU4(); 
			setCustomPartName("xczu4ev-1sfvc784i");
			break;
		case XUSP_DEVICE_AXU5EV:
			XCZU5(); 
			setCustomPartName("xczu5ev-2sfvc784i");
			break;
		case XUSP_DEVICE_AXU9EG:
			XCZU9(); 
			setCustomPartName("xczu9eg-2ffvb1156i");
			break;
		case XUSP_DEVICE_ULTRAZED_EV:
			XCZU7(); 
			setCustomPartName("xczu7ev-fbvb900");
			break;
		case XUSP_DEVICE_CHAMP_XD1S:
			XCZU4();
			break;
		case XUSP_DEVICE_ZU_3EG:
			XCZU3(); 
			setCustomPartName("xczu3eg-sfvc784-1-e");
			break;
		case XUSP_DEVICE_ZU_5EV:
			XCZU5(); 
			setCustomPartName("xczu5ev-sfvc784-1-e");
			break;
		case XUSP_DEVICE_HTG_Z999_06:
			XCZU6(); 
			setCustomPartName("xczu6eg-2ffvb1156e");
			break;
		case XUSP_DEVICE_HTG_Z999_09:
			XCZU9(); 
			setCustomPartName("xczu9eg-2ffvb1156e");
			break;
		case XUSP_DEVICE_HTG_Z999_15:
			XCZU15(); 
			setCustomPartName("xczu15eg-2ffvb1156e");
			break;
		case XUSP_DEVICE_HTG_930_9_3:
			XCVU9P(); 
			setCustomPartName("xcvu9p-3flgb2104e");
			break;
		case XUSP_DEVICE_HTG_930_9:
			XCVU9P(); 
			setCustomPartName("xcvu9p-2flgb2104e");
			break;
		case XUSP_DEVICE_HTG_930_13_3:
			XCVU13P(); 
			setCustomPartName("xcvu13p-3fhgb2104e");
			break;
		case XUSP_DEVICE_HTG_930_13:
			XCVU13P(); 
			setCustomPartName("xcvu13p-2fhgb2104e");
			break;
		case XUSP_DEVICE_HTG_937_37P:
			XCVU37P(); 
			setCustomPartName("xcvu37p-2fsvh2892e4539");
			break;
		case XUSP_DEVICE_HTG_937_47P:
			XCVU47P(); 
			setCustomPartName("xcvu47p-2fsvh2892e4539");
			break;
		case XUSP_DEVICE_HTG_Z922_11:
			XCZU11(); 
			setCustomPartName("xczu11eg-3ffvc1760e");
			break;
		case XUSP_DEVICE_HTG_Z922_19_DG:
			XCZU19(); 
			setCustomPartName("xqzu19eg-2ffvc1760e");
			break;
		case XUSP_DEVICE_HTG_Z922_19:
			XCZU19(); 
			setCustomPartName("xczu19eg-2ffvc1760e");
			break;
		case XUSP_DEVICE_CZU3EG:
			XCZU3(); 
			setCustomPartName("xczu3eg-1sfvc784e");
			break;
		case XUSP_DEVICE_CZU4EV:
			XCZU4(); 
			setCustomPartName("xczu3eg-1sfvc784e");
			break;
		case XUSP_DEVICE_TE0802:
			XCZU2(); 
			setCustomPartName("xczu2cg-1sbva484e");
			break;
		case XUSP_DEVICE_TE0803_04_3BE11:
			XCZU3(); 
			setCustomPartName("xczu3eg-1sfvc784e");
			break;
		case XUSP_DEVICE_TE0807_03_7DE21:
			XCZU7(); 
			setCustomPartName("xczu7ev-1fbvb900e");
			break;
		case XUSP_DEVICE_TE0808_05_9BE21:
			XCZU9(); 
			setCustomPartName("xczu9eg-1ffvc900e");
			break;
		case XUSP_DEVICE_TE0808_05_9GI21:
			XCZU9(); 
			setCustomPartName("xczu9eg-2ffvc900i");
			break;
		case XUSP_DEVICE_TE0808_05_BBE21:
			XCZU15(); 
			setCustomPartName("xczu15eg-1ffvc900e");
			break;
		case XUSP_DEVICE_TE0812:
			XCZU6(); 
			setCustomPartName("xczu6eg-1ffvc900i");
			break;
		case XUSP_DEVICE_TEB0911:
			XCZU9(); 
			setCustomPartName("xczu9eg-1ffvb1156e");
			break;
		case XUSP_DEVICE_TEB0912:
			XCZU11(); 
			setCustomPartName("xczu11eg-1ffvc1760i");
			break; 
		case XUSP_DEVICE_TYSOM_3_ZU7EV:
			XCZU7(); 
			setCustomPartName("zu7ev-ffvc1156");
			break;
		case XUSP_DEVICE_XPEDITE2600:
			XCZU11(); 
			setCustomPartName("xczu11eg");
			break;
		case XUSP_DEVICE_AMC574:
			XCZU29DR(); 
			setCustomPartName("xczu29dr");
			break;
		case XUSP_DEVICE_ZCU208:
			XCZU48DR(); 
			setCustomPartName("xczu48dr-2fsvg1517e5184");
			break;
		case XUSP_DEVICE_ZCU216:
			XCZU49DR(); 
			setCustomPartName("xczu49dr-2ffvf1760");
			break;
		case XUSP_DEVICE_ZCU670:
			XCZU67DR(); 
			setCustomPartName("xczu67dr-2fsve1156i");
			break;
		case XUSP_DEVICE_ULTRA96:
			XCZU3(); 
			setCustomPartName("xczu3eg-sbva484-1-i");
			break;
		case XUSP_DEVICE_K26C:
			XCK26(); 
			setCustomPartName("xck26-sfvc784-2lv-c");
			break;
		case XUSP_DEVICE_K26I:
			XCK26(); 
			setCustomPartName("xck26-sfvc784-2lv-i");
			break;
		case XUSP_DEVICE_KV260:
			XCK26(); 
			setCustomPartName("xck26-sfvc784-2lv-c");
			break;
		case XUSP_DEVICE_AMC584:
			XCVU13P();
			break;
		case XUSP_DEVICE_HTG_960:
			XCVU19P(); 
			setCustomPartName("xcvu19p-2fsvb3824e");
			break;
		case XUSP_DEVICE_VCU1525:
			XCVU9P(); 
			setCustomPartName("xcvu9p-l2fsgd2104e");
			break;
		case XUSP_DEVICE_VCU118:
			XCVU9P(); 
			setCustomPartName("xcvu9p-lga2104e");
			break;
		case XUSP_DEVICE_VCU128:
			XCVU37P(); 
			setCustomPartName("xcvu37p-l2fsvh2892e");
			break;
		case XUSP_DEVICE_VCU129:
			XCVU29P(); 
			setCustomPartName("xcvu29p-l2fsga2577e");
			break;
		case XUSP_DEVICE_HTG_940_9_3:
			XCVU9P(); 
			setCustomPartName("xcvu9p-3flgb2104e");
			break;
		case XUSP_DEVICE_HTG_940_9:
			XCVU9P(); 
			setCustomPartName("xcvu9p-2flgb2104e");
			break;
		case XUSP_DEVICE_HTG_940_13_3:
			XCVU13P(); 
			setCustomPartName("xcvu13p-3fhgb2104e");
			break;
		case XUSP_DEVICE_HTG_940_13:
			XCVU13P(); 
			setCustomPartName("xcvu13p-2fhgb2104e");
			break;
		case XUSP_DEVICE_HTG_9200_5P:
			XCVU5P(); 
			setCustomPartName("xcvu5p-2flgb2104e");
			break;
		case XUSP_DEVICE_HTG_9200_9P:
			XCVU9P(); 
			setCustomPartName("xcvu9p-2flgb2104e");
			break;
		case XUSP_DEVICE_HTG_9200_13P:
			XCVU13P(); 
			setCustomPartName("xcvu13p-2fhgb2104e");
			break;
		case XUSP_DEVICE_HTG_VUP_PCIE_HH_9P:
			XCVU9P(); 
			setCustomPartName("xcvu9p-2flgb2104e");
			break;
		case XUSP_DEVICE_HTG_VUP_PCIE_HH_13P:
			XCVU13P(); 
			setCustomPartName("xcvu13p-2fhgb2104e");
			break;
		case XUSP_DEVICE_HTG_ZRF_FMC_28:
			XCZU28DR(); 
			setCustomPartName("xczu28dr-2ffvg1517");
			break;
		case XUSP_DEVICE_HTG_ZRF_FMC_48:
			XCZU48DR(); 
			setCustomPartName("xczu48dr-2ffvg1517");
			break;
		case XUSP_DEVICE_HTG_ZRF16_29:
			XCZU29DR(); 
			setCustomPartName("xczu29dr-2ffvf1760e");
			break;
		case XUSP_DEVICE_HTG_ZRF16_49:
			XCZU49DR(); 
			setCustomPartName("xczu49dr-2ffvf1760e");
			break;
		case XUSP_DEVICE_HTG_ZRF_HH_28:
			XCZU28DR(); 
			setCustomPartName("xczu28dr-2ffvg1517");
			break;
		case XUSP_DEVICE_HTG_ZRF_HH_48:
			XCZU48DR(); 
			setCustomPartName("xczu48dr-2ffvg1517");
			break;
		case XUSP_DEVICE_HTG_ZUSP_PCIE_11_3:
			XCZU11(); 
			setCustomPartName("xczu11eg-3ffvc1760e");
			break;
		case XUSP_DEVICE_HTG_ZUSP_PCIE_19_2:
			XCZU19(); 
			setCustomPartName("xczu19eg-2ffvc1760e");
			break;
		case XUSP_DEVICE_HTG_ZRF8_R2_28:
			XCZU28DR(); 
			setCustomPartName("xczu28dr-2ffvg1517");
			break;
		case XUSP_DEVICE_HTG_ZRF8_R2_48_I:
			XCZU48DR(); 
			setCustomPartName("xczu48dr-ffvg1517-2-i");
			break;
		case XUSP_DEVICE_HTG_ZRF8_R2_48:
			XCZU48DR(); 
			setCustomPartName("xczu48dr-2ffvg1517");
			break;
		case XUSP_DEVICE_ZUBOARD_1CG:
			XCZU1();
			
			break;
		case XUSP_DEVICE_KCU116:
			XCKU5P();
			setCustomPartName("xcku5p-ffvb676-2-e");
			break;
		case XUSP_DEVICE_VERMEO_T1_MP:
			XCZU19();
			setCustomPartName("xczu19eg-ffvd1760-2L-e");
			break;
		case XUSP_DEVICE_VERMEO_T1_RF:
			XCZU21DR();
			setCustomPartName("xczu21dr-ffvd1156-2L-e");
			break;
		case XUSP_DEVICE_ZCU1275:
			XCZU29DR();
			setCustomPartName("xczu29dr-ffvf1760-2-e");
			break;
		case XUSP_DEVICE_ZCU1285:
			XCZU39DR();
			setCustomPartName("xczu39dr-ffvf1760-2-i");
			break;
		case XUSP_DEVICE_ZCU111:
			XCZU28DR();
			setCustomPartName("xczu28dr-ffvg1517-2-e");
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
void XilinxUltraScalePlus::deviceHelp()
{
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
	cout << "\t\tZUBoard 1CG, KCU 116, Vermeo T1 MPSoC, Vermeo T1 RFSoC, ZCU 1275, ZCU 1285, ZCU 111" << endl;
	#endif
}