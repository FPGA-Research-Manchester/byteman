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

#ifndef XILINXSERIES7DEVICES_H
#define XILINXSERIES7DEVICES_H

#include<cstring>//strcpy

enum XS7_DEVICE_ENUM    ///< Enumeration of recognized Xilinx Series 7 devices and boards.
{
    XS7_DEVICE_NULL = 0,
    //S7 Spartan
	XS7_DEVICE_XC7S6,
	XS7_DEVICE_XC7S15,
	XS7_DEVICE_XC7S25,
	XS7_DEVICE_XC7S50,
	XS7_DEVICE_XC7S75,
	XS7_DEVICE_XC7S100,
    //S7 Artix
	XS7_DEVICE_XC7A12,
	XS7_DEVICE_XC7A15,
	XS7_DEVICE_XC7A25,
	XS7_DEVICE_XC7A35,
	XS7_DEVICE_XC7A50,
	XS7_DEVICE_XC7A75,
	XS7_DEVICE_XC7A100,
	XS7_DEVICE_XC7A200,
    //S7 Zynq
	XS7_DEVICE_XC7Z007,
	XS7_DEVICE_XC7Z010,
	XS7_DEVICE_XC7Z012,
	XS7_DEVICE_XC7Z014,
	XS7_DEVICE_XC7Z015,
	XS7_DEVICE_XC7Z020,
	XS7_DEVICE_XC7Z030,
	XS7_DEVICE_XC7Z035,
	XS7_DEVICE_XC7Z045,
	XS7_DEVICE_XC7Z100,
    //S7 Kintex
	XS7_DEVICE_XC7K70,
	XS7_DEVICE_XC7K160,
	XS7_DEVICE_XC7K325,
	XS7_DEVICE_XC7K355,
	XS7_DEVICE_XC7K410,
	XS7_DEVICE_XC7K420,
	XS7_DEVICE_XC7K480,
    //S7 Virtex
	XS7_DEVICE_XC7V585,
	XS7_DEVICE_XC7V2000,
	XS7_DEVICE_XC7VH580,
	XS7_DEVICE_XC7VH870,
	XS7_DEVICE_XC7VX330,
	XS7_DEVICE_XC7VX415,
	XS7_DEVICE_XC7VX485,
	XS7_DEVICE_XC7VX550,
	XS7_DEVICE_XC7VX690,
	XS7_DEVICE_XC7VX980,
	XS7_DEVICE_XC7VX1140,
    //S7 Boards
	XS7_DEVICE_WIZARDE,
	XS7_DEVICE_KINTEX7CONNECTIVITY,
	XS7_DEVICE_KC705,
	XS7_DEVICE_KC724,
	XS7_DEVICE_SP701,
	XS7_DEVICE_AC701,
	XS7_DEVICE_ARTY_A35,
	XS7_DEVICE_ARTY_A100,
	XS7_DEVICE_ARTY_Z10,
	XS7_DEVICE_ARTY_Z20,
	XS7_DEVICE_ARTY_S25,
	XS7_DEVICE_ARTY_S50,
	XS7_DEVICE_BASYS3,
	XS7_DEVICE_PYNQ_Z1,
	XS7_DEVICE_NEXYS_A50,
	XS7_DEVICE_NEXYS_A100,
	XS7_DEVICE_ZYBO_Z10,
	XS7_DEVICE_ZYBO_Z20,
	XS7_DEVICE_CMOD_A15,
	XS7_DEVICE_CMOD_A35,
	XS7_DEVICE_CMOD_S25,
	XS7_DEVICE_ZEDBOARD,
	XS7_DEVICE_CORA_Z7,
	XS7_DEVICE_CORA_Z10,
	XS7_DEVICE_NEXYS_VIDEO,
	XS7_DEVICE_ARTY_GENESYS2,
	XS7_DEVICE_ECLYPSE_Z7,
	XS7_DEVICE_USB104,
	XS7_DEVICE_NETFPGA_SUME,
	XS7_DEVICE_VC707,
	XS7_DEVICE_VC709,
	XS7_DEVICE_VC7215,
	XS7_DEVICE_VC7203,
	XS7_DEVICE_VC7222,
	XS7_DEVICE_ZC702,
	XS7_DEVICE_ZC706,
    
    XS7_DEVICE_MAX
};

#define TODO 0
//Functions how to build devices
#define XS7_TOPROWS(slrID,value) {SLRinfo[slrID].rowsInTopHalf = value;}
#define XS7_BOTROWS(slrID,value) {SLRinfo[slrID].rowsInBottomHalf = value;}
#define XS7_PARTNAME(value) {partName = value;}
#define XS7_SLRFROMROW(slrID,value) {SLRinfo[slrID].fromRow = value;}
#define XS7_SLRTOROW(slrID,value) {SLRinfo[slrID].toRow = value;}
#define XS7_ROWNUM(value) {numberOfRows = value;}
#define XS7_SLRNUM(value) {numberOfSLRs = value;}
#define XS7_IDCODE(slrID,value) {SLRinfo[slrID].IDCODE = value;}
#define XS7_STRING(rowID,value) {strcpy(resourceString[rowID],value);}
#define XS7_STRING_CAT(x,y)  {XS7_STRING(y, x##STRING##y);}
#define XS7_STRING_CAT_1(x)  {XS7_STRING_CAT(x,0);}
#define XS7_STRING_CAT_2(x)  {XS7_STRING_CAT(x,0);  XS7_STRING_CAT(x,1);}
#define XS7_STRING_CAT_3(x)  {XS7_STRING_CAT_2(x);  XS7_STRING_CAT(x,2);}
#define XS7_STRING_CAT_4(x)  {XS7_STRING_CAT_3(x);  XS7_STRING_CAT(x,3);}
#define XS7_STRING_CAT_5(x)  {XS7_STRING_CAT_4(x);  XS7_STRING_CAT(x,4);}
#define XS7_STRING_CAT_6(x)  {XS7_STRING_CAT_5(x);  XS7_STRING_CAT(x,5);}
#define XS7_STRING_CAT_7(x)  {XS7_STRING_CAT_6(x);  XS7_STRING_CAT(x,6);}
#define XS7_STRING_CAT_8(x)  {XS7_STRING_CAT_7(x);  XS7_STRING_CAT(x,7);}
#define XS7_STRING_CAT_9(x)  {XS7_STRING_CAT_8(x);  XS7_STRING_CAT(x,8);}
#define XS7_STRING_CAT_10(x) {XS7_STRING_CAT_9(x);  XS7_STRING_CAT(x,9);}
#define XS7_STRING_CAT_11(x) {XS7_STRING_CAT_10(x); XS7_STRING_CAT(x,10);}
#define XS7_STRING_CAT_12(x) {XS7_STRING_CAT_11(x); XS7_STRING_CAT(x,11);}

#define XS7_SETUP_SLR_EXPANDED(slrID,idcodeVal,fromRowVal,toRowVal) {XS7_IDCODE(slrID,idcodeVal);XS7_SLRFROMROW(slrID,fromRowVal);XS7_SLRTOROW(slrID,toRowVal);}
#define XS7_SETUP_SLR0(prefix) {XS7_SETUP_SLR_EXPANDED(0, prefix##IDCODE, prefix##SLR0_START, prefix##SLR0_END);}
#define XS7_SETUP_SLR(slrID, prefix) {XS7_TOPROWS(slrID,prefix##TOPROWS);XS7_BOTROWS(slrID,prefix##BOTROWS);XS7_SETUP_SLR_EXPANDED(slrID, prefix##IDCODE##slrID, prefix##SLR##slrID##_START, prefix##SLR##slrID##_END);}
#define XS7_SETUP_SLRs_2(prefix) {XS7_SETUP_SLR0(prefix); XS7_SETUP_SLR(1,prefix);}
#define XS7_SETUP_SLRs_3(prefix) {XS7_SETUP_SLRs_2(prefix); XS7_SETUP_SLR(2,prefix);}
#define XS7_SETUP_SLRs_4(prefix) {XS7_SETUP_SLRs_3(prefix); XS7_SETUP_SLR(3,prefix);}
#define XS7_SETUP_DEVICE(prefix) {                                                                \
    XS7_PARTNAME(prefix##NAME);                                                                   \
    XS7_SLRNUM(prefix##SLRNUM);                                                                   \
    XS7_ROWNUM(prefix##ROWNUM);                                                                   \
    XS7_TOPROWS(0,prefix##TOPROWS);                                                                   \
    XS7_BOTROWS(0,prefix##BOTROWS);                                                                   \
    XS7_SLRFROMROW(0,0);                                                                          \
    XS7_SLRTOROW(0,prefix##ROWNUM-1);                                                             \
    XS7_IDCODE(0,prefix##IDCODE);                                                                 \
}




//Xilinx Series7 Spartan
	//XC7S6 is software-limitted version of XC7S15
    #define XC7S6_NAME "7s6"
    #define XC7S6_SLRNUM 1
    #define XC7S6_ROWNUM 1
    #define XC7S6_IDCODE 0x3622093
    #define XC7S6_TOPROWS 1
    #define XC7S6_BOTROWS 0
    #define XC7S6_STRING0 "abLMLMLMLMLcNOdddddfgMNMHMNALMLMhi\x01"
    #define XC7S6() {                                                                            \
        XS7_SETUP_DEVICE(XC7S6_);                                                                \
        XS7_STRING_CAT_1(XC7S6_);                                                                \
    }
	
    #define XC7S15_NAME "7s15"
    #define XC7S15_SLRNUM 1
    #define XC7S15_ROWNUM 1
    #define XC7S15_IDCODE 0x3620093
    #define XC7S15_TOPROWS 1
    #define XC7S15_BOTROWS 0
    #define XC7S15_STRING0 XC7S6_STRING0
    #define XC7S15() {                                                                            \
        XS7_SETUP_DEVICE(XC7S15_);                                                                \
        XS7_STRING_CAT_1(XC7S15_);                                                                \
    }
	
	//XC7S25 is XC7A25 with disabled transceivers 
    #define XC7S25_NAME "7s25"
    #define XC7S25_SLRNUM 1
    #define XC7S25_ROWNUM 2
    #define XC7S25_IDCODE 0x37C4093
    #define XC7S25_TOPROWS 1
    #define XC7S25_BOTROWS 1
    #define XC7S25_STRING0 "abLMLMBMNINMdddddfgjLMLMBONMHMNALMLMhi\x01"
    #define XC7S25_STRING1 "abLMLMBMNINMdddddkglLPQRCONMHMNm\x01"
    #define XC7S25() {                                                                            \
        XS7_SETUP_DEVICE(XC7S25_);                                                                \
        XS7_STRING_CAT_2(XC7S25_);                                                                \
    }
	
	//XC7S50 is XC7A50 with disabled transceivers 
    #define XC7S50_NAME "7s50"
    #define XC7S50_SLRNUM 1
    #define XC7S50_ROWNUM 3
    #define XC7S50_IDCODE 0x362F093
    #define XC7S50_TOPROWS 2
    #define XC7S50_BOTROWS 1
    #define XC7S50_STRING0 "abLMLMBMNINMdddddfgONONjLMLMLMBONMHMNALMLMhi\x01"
    #define XC7S50_STRING1 "abLMLMBMNINMdddddngONONlLMLMLMBONMHMNALMLMhi\x01"
    #define XC7S50_STRING2 "abLMLMBMNINMLOLOLOgONONoLMLPQRCONMHMNm\x01"
    #define XC7S50() {                                                                            \
        XS7_SETUP_DEVICE(XC7S50_);                                                                \
        XS7_STRING_CAT_3(XC7S50_);                                                                \
    }
	
	//XC7S75 is software-limitted version of XC7S100
    #define XC7S75_NAME "7s75"
    #define XC7S75_SLRNUM 1
    #define XC7S75_ROWNUM 4
    #define XC7S75_IDCODE 0x37C8093
    #define XC7S75_TOPROWS 2
    #define XC7S75_BOTROWS 2
    #define XC7S75_STRING0 "abLMLMBMNINMLOLOLOgONONONONONpLMLMLMLMLMBONMHMNALMLMhi\x01"
    #define XC7S75_STRING1 "abLMLMBMNINMdddddfgONONONONONjLMLMLMLMLMBONMHMNALMLMhi\x01"
    #define XC7S75_STRING2 "abLMLMBMNINMdddddngONONONONONlLMLMLMLMLMBONMHMNALMLMhi\x01"
    #define XC7S75_STRING3 "abLMLMBMNINMLOLOLOgONONONONONoLMLMLMLMLMBONMHMNALMLMhi\x01"
    #define XC7S75() {                                                                            \
        XS7_SETUP_DEVICE(XC7S75_);                                                                \
        XS7_STRING_CAT_4(XC7S75_);                                                                \
    }
	
    #define XC7S100_NAME "7s100"
    #define XC7S100_SLRNUM 1
    #define XC7S100_ROWNUM 4
    #define XC7S100_IDCODE 0x37C7093
    #define XC7S100_TOPROWS 2
    #define XC7S100_BOTROWS 2
    #define XC7S100_STRING0 XC7S75_STRING0
    #define XC7S100_STRING1 XC7S75_STRING1
    #define XC7S100_STRING2 XC7S75_STRING2
    #define XC7S100_STRING3 XC7S75_STRING3
    #define XC7S100() {                                                                           \
        XS7_SETUP_DEVICE(XC7S100_);                                                               \
        XS7_STRING_CAT_4(XC7S100_);                                                               \
    }

//Xilinx Series7 Artix
	//XC7A12 is software-limitted version of XC7A25
    #define XC7A12_NAME "7a12"
    #define XC7A12_SLRNUM 1
    #define XC7A12_ROWNUM 2
    #define XC7A12_IDCODE 0x37C3093
    #define XC7A12_TOPROWS 1
    #define XC7A12_BOTROWS 1
    #define XC7A12_STRING0 "abLMLMBMNINMdddddfgjLMLMBONMHMNALMLMhi\x01"
    #define XC7A12_STRING1 "abLMLMBMNINMdddddkglLPQRCONMHMNm\x01"
    #define XC7A12() {                                                                            \
        XS7_SETUP_DEVICE(XC7A12_);                                                                \
        XS7_STRING_CAT_2(XC7A12_);                                                                \
    }
	
	//XC7A15 is software-limitted version of XC7A50
    #define XC7A15_NAME "7a15"
    #define XC7A15_SLRNUM 1
    #define XC7A15_ROWNUM 3
    #define XC7A15_IDCODE 0x362E093
    #define XC7A15_TOPROWS 2
    #define XC7A15_BOTROWS 1
    #define XC7A15_STRING0 "abLMLMBMNINMdddddfgONONjLMLMLMBONMHMNALMLMhi\x01"
    #define XC7A15_STRING1 "abLMLMBMNINMdddddngONONlLMLMLMBONMHMNALMLMhi\x01"
    #define XC7A15_STRING2 "abLMLMBMNINMLOLOLOgONONoLMLPQRCONMHMNm\x01"
    #define XC7A15() {                                                                            \
        XS7_SETUP_DEVICE(XC7A15_);                                                                \
        XS7_STRING_CAT_3(XC7A15_);                                                                \
    }
	
    #define XC7A25_NAME "7a25"
    #define XC7A25_SLRNUM 1
    #define XC7A25_ROWNUM 2
    #define XC7A25_IDCODE 0x37C2093
    #define XC7A25_TOPROWS 1
    #define XC7A25_BOTROWS 1
    #define XC7A25_STRING0 XC7A12_STRING0
    #define XC7A25_STRING1 XC7A12_STRING1
    #define XC7A25() {                                                                            \
        XS7_SETUP_DEVICE(XC7A25_);                                                                \
        XS7_STRING_CAT_2(XC7A25_);                                                                \
    }
	
	//XC7A35 is software-limitted version of XC7A50
    #define XC7A35_NAME "7a35"
    #define XC7A35_SLRNUM 1
    #define XC7A35_ROWNUM 3
    #define XC7A35_IDCODE 0x362D093
    #define XC7A35_TOPROWS 2
    #define XC7A35_BOTROWS 1
    #define XC7A35_STRING0 XC7A15_STRING0
    #define XC7A35_STRING1 XC7A15_STRING1
    #define XC7A35_STRING2 XC7A15_STRING2
    #define XC7A35() {                                                                            \
        XS7_SETUP_DEVICE(XC7A35_);                                                                \
        XS7_STRING_CAT_3(XC7A35_);                                                                \
    }
	
    #define XC7A50_NAME "7a50"
    #define XC7A50_SLRNUM 1
    #define XC7A50_ROWNUM 3
    #define XC7A50_IDCODE 0x362C093
    #define XC7A50_TOPROWS 2
    #define XC7A50_BOTROWS 1
    #define XC7A50_STRING0 XC7A15_STRING0
    #define XC7A50_STRING1 XC7A15_STRING1
    #define XC7A50_STRING2 XC7A15_STRING2
    #define XC7A50() {                                                                            \
        XS7_SETUP_DEVICE(XC7A50_);                                                                \
        XS7_STRING_CAT_3(XC7A50_);                                                                \
    }
	
	//XC7A75 is software-limitted version of XC7A100
    #define XC7A75_NAME "7a75"
    #define XC7A75_SLRNUM 1
    #define XC7A75_ROWNUM 4
    #define XC7A75_IDCODE 0x3632093
    #define XC7A75_TOPROWS 2
    #define XC7A75_BOTROWS 2
    #define XC7A75_STRING0 "abLMLMBMNINMLOLOLOgONONONONONONpLMNINMBMLMLMBONMHMNm\x01"
    #define XC7A75_STRING1 "abLMLMBMNINMdddddfgONONONONONONjLMNINMBMLMLMBONMHMNALMLMhi\x01"
    #define XC7A75_STRING2 "abLMLMBMNINMdddddngONONONONONONlLMNINMBMLMLMBONMHMNALMLMhi\x01"
    #define XC7A75_STRING3 "abLMLMBMNINMLOLOLOgONONONONONONoLMNINMBMLPQRCONMHMNm\x01"
    #define XC7A75() {                                                                            \
        XS7_SETUP_DEVICE(XC7A75_);                                                                \
        XS7_STRING_CAT_4(XC7A75_);                                                                \
    }
	
    #define XC7A100_NAME "7a100"
    #define XC7A100_SLRNUM 1
    #define XC7A100_ROWNUM 4
    #define XC7A100_IDCODE 0x3631093
    #define XC7A100_TOPROWS 2
    #define XC7A100_BOTROWS 2
    #define XC7A100_STRING0 XC7A75_STRING0
    #define XC7A100_STRING1 XC7A75_STRING1
    #define XC7A100_STRING2 XC7A75_STRING2
    #define XC7A100_STRING3 XC7A75_STRING3
    #define XC7A100() {                                                                           \
        XS7_SETUP_DEVICE(XC7A100_);                                                               \
        XS7_STRING_CAT_4(XC7A100_);                                                               \
    }
	
    #define XC7A200_NAME "7a200"
    #define XC7A200_SLRNUM 1
    #define XC7A200_ROWNUM 5
    #define XC7A200_IDCODE 0x3636093
    #define XC7A200_TOPROWS 2
    #define XC7A200_BOTROWS 3
    #define XC7A200_STRING0 "abLMLMBMNINMNMHMNALOLOLOgMLMBMNINMLqL000400400004004000prO4NO4NONO4NO4NONO00000000000000BMNINMNMHMNALMLMhi\x01"
    #define XC7A200_STRING1 "abLMLMBMNINMNMHMNALOLOLOgMLMBMNINMLMLMLMBMNINMNMHMNALMLpNOBMNINMNMHMNANONONONONONONONONOBMNINMNMHMNALMLMhi\x01"
    #define XC7A200_STRING2 "abLMLMBMNINMNMHMNAdddddfgMLMBMNINMLMLMLMBMNINMNMHMNALMLjNOBMNINMNMHMNANONONONONONONONONOBMNINMNMHMNALMLMhi\x01"
    #define XC7A200_STRING3 "abLMLMBMNINMNMHMNAdddddngMLMBMNINMLMLMLMBMNINMNMHMNALMLlNOBMNINMNMHMNANONONONONONONONONOBMNINMNMHMNALMLMhi\x01"
    #define XC7A200_STRING4 "abLMLMBMNINMNMHMNALOLOLOgPQRCMNINMLqL000400400004004000orO4NO4NONO4NO4NONO00000000000000BMNINMNMHMNALMLMhi\x01"
    #define XC7A200() {                                                                           \
        XS7_SETUP_DEVICE(XC7A200_);                                                               \
        XS7_STRING_CAT_5(XC7A200_);                                                               \
    }

//Xilinx Series7 Zynq
	//XC7Z007 is software-limitted version of XC7Z010 with one ARM core disabled 
    #define XC7Z007_NAME "7z007"
    #define XC7Z007_SLRNUM 1
    #define XC7Z007_ROWNUM 2
    #define XC7Z007_IDCODE 0x3723093
    #define XC7Z007_TOPROWS 1
    #define XC7Z007_BOTROWS 1
    #define XC7Z007_STRING0 "230000400100001004tMLMBMNINMdddddfgONONjLMBONMHMNALMLMhi\x01"
    #define XC7Z007_STRING1 "230000400100001004uMLMBMNINMdddddvgONONlLMBONMHMNALMLMhi\x01"
    #define XC7Z007() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z007_);                                                               \
        XS7_STRING_CAT_2(XC7Z007_);                                                               \
    }
	
    #define XC7Z010_NAME "7z010"
    #define XC7Z010_SLRNUM 1
    #define XC7Z010_ROWNUM 2
    #define XC7Z010_IDCODE 0x3722093
    #define XC7Z010_TOPROWS 1
    #define XC7Z010_BOTROWS 1
    #define XC7Z010_STRING0 XC7Z007_STRING0
    #define XC7Z010_STRING1 XC7Z007_STRING1
    #define XC7Z010() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z010_);                                                               \
        XS7_STRING_CAT_2(XC7Z010_);                                                               \
    }
	
	//XC7Z012 is software-limitted version of XC7Z015 with one ARM core disabled 
    #define XC7Z012_NAME "7z012"
    #define XC7Z012_SLRNUM 1
    #define XC7Z012_ROWNUM 3
    #define XC7Z012_IDCODE 0x373C093
    #define XC7Z012_TOPROWS 1
    #define XC7Z012_BOTROWS 2
    #define XC7Z012_STRING0 "abNMNMBMNINMNMHMNALMLMLMLMLOLOLOgONONjLMLMLMNINMBMLPQRCMNINMLm\x01"
    #define XC7Z012_STRING1 "230000400100001004tMLMLMLMdddddfgONONlLMLMLMNINMBMLMLMBMNINMLALMLMhi\x01"
    #define XC7Z012_STRING2 "230000400100001004uMLMLMLMdddddvgONONoLMLMLMNINMBMLMLMBMNINMLALMLMhi\x01"
    #define XC7Z012() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z012_);                                                               \
        XS7_STRING_CAT_3(XC7Z012_);                                                               \
    }
	
	//XC7Z014 is software-limitted version of XC7Z020 with one ARM core disabled 
    #define XC7Z014_NAME "7z014"
    #define XC7Z014_SLRNUM 1
    #define XC7Z014_ROWNUM 3
    #define XC7Z014_IDCODE 0x3728093
    #define XC7Z014_TOPROWS 1
    #define XC7Z014_BOTROWS 2
    #define XC7Z014_STRING0 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMLjNOBMLMLMLMLOLOLOgONONOBMNINMNMHMNALMLMhi\x01"
    #define XC7Z014_STRING1 "230000400100001004tMLMBMNINMLMLMLlNOBMLMLMLMdddddfgONONOBMNINMNMHMNALMLMhi\x01"
    #define XC7Z014_STRING2 "230000400100001004uMLMBMNINMLMLMLoNOBMLMLMLMdddddvgONONOBMNINMNMHMNALMLMhi\x01"
    #define XC7Z014() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z014_);                                                               \
        XS7_STRING_CAT_3(XC7Z014_);                                                               \
    }
	
    #define XC7Z015_NAME "7z015"
    #define XC7Z015_SLRNUM 1
    #define XC7Z015_ROWNUM 3
    #define XC7Z015_IDCODE 0x373B093
    #define XC7Z015_TOPROWS 1
    #define XC7Z015_BOTROWS 2
    #define XC7Z015_STRING0 XC7Z012_STRING0
    #define XC7Z015_STRING1 XC7Z012_STRING1
    #define XC7Z015_STRING2 XC7Z012_STRING2
    #define XC7Z015() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z015_);                                                               \
        XS7_STRING_CAT_3(XC7Z015_);                                                               \
    }
	
    #define XC7Z020_NAME "7z020"
    #define XC7Z020_SLRNUM 1
    #define XC7Z020_ROWNUM 3
    #define XC7Z020_IDCODE 0x3727093
    #define XC7Z020_TOPROWS 1
    #define XC7Z020_BOTROWS 2
    #define XC7Z020_STRING0 XC7Z014_STRING0
    #define XC7Z020_STRING1 XC7Z014_STRING1
    #define XC7Z020_STRING2 XC7Z014_STRING2
    #define XC7Z020() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z020_);                                                               \
        XS7_STRING_CAT_3(XC7Z020_);                                                               \
    }
	
    #define XC7Z030_NAME "7z030"
    #define XC7Z030_SLRNUM 1
    #define XC7Z030_ROWNUM 4
    #define XC7Z030_IDCODE 0x372C093
    #define XC7Z030_TOPROWS 1
    #define XC7Z030_BOTROWS 3
    #define XC7Z030_STRING0 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMNMHMNALpNONONONALOLOLOgMNINMBOLPQRCMNINMLw\x01"
    #define XC7Z030_STRING1 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMNMHMNALjNONONONALOLOLOgMNINMBOLMLMBMNINMLALMLMhx\x01"
    #define XC7Z030_STRING2 "230000400100001004tMLMBMNINMLMLMNMHMNALlNONONONAdddddfgMNINMBOLMLMBMNINMLALMLMhx\x01"
    #define XC7Z030_STRING3 "230000400100001004uMLMBMNINMLMLMNMHMNALoNONONONAdddddvgMNINMBOLMLMBMNINMLALMLMhx\x01"
    #define XC7Z030() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z030_);                                                               \
        XS7_STRING_CAT_4(XC7Z030_);                                                               \
    }
	
	//XC7Z035 is software-limitted version of XC7Z045
    #define XC7Z035_NAME "7z035"
    #define XC7Z035_SLRNUM 1
    #define XC7Z035_ROWNUM 7
    #define XC7Z035_IDCODE 0x3732093
    #define XC7Z035_TOPROWS 1
    #define XC7Z035_BOTROWS 6
    #define XC7Z035_STRING0 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMNMHMNALMLMLMLMLMLMLMLMLpNOBMNINMNONONONONONALOLOLOgMNINMBOLMLMBMNINMLw\x01"
    #define XC7Z035_STRING1 XC7Z035_STRING0
    #define XC7Z035_STRING2 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMNMHMNALMLMLMLMLMLMLMLMLjNOBMNINMNONONONONONALOLOLOgMNINMBOLMLMBMNINMLw\x01"
    #define XC7Z035_STRING3 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMNMHMNALMLMLMLMLMLMLMLMLlNOBMNINMNONONONONONALOLOLOgMNINMBOLPQRCMNINMLw\x01"
    #define XC7Z035_STRING4 "abNMNMBMNINMNMHMNALMLMBMNINMLMLMNMHMNALMLMLMLMLMLMLMLMLoNOBMNINMNONONONONONALOLOLOgMNINMBOLMLMBMNINMLALMLMhx\x01"
    #define XC7Z035_STRING5 "230000400100001004tMLMBMNINMLMLMNMHMNALMLMLMLMLMLMLMLMLoNOBMNINMNONONONONONAdddddfgMNINMBOLMLMBMNINMLALMLMhx\x01"
    #define XC7Z035_STRING6 "230000400100001004uMLMBMNINMLMLMNMHMNALMLMLMLMLMLMLMLMLoNOBMNINMNONONONONONAdddddvgMNINMBOLMLMBMNINMLALMLMhx\x01"
    #define XC7Z035() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z035_);                                                               \
        XS7_STRING_CAT_7(XC7Z035_);                                                               \
    }
	
    #define XC7Z045_NAME "7z045"
    #define XC7Z045_SLRNUM 1
    #define XC7Z045_ROWNUM 7
    #define XC7Z045_IDCODE 0x3731093
    #define XC7Z045_TOPROWS 1
    #define XC7Z045_BOTROWS 6
    #define XC7Z045_STRING0 XC7Z035_STRING0
    #define XC7Z045_STRING1 XC7Z035_STRING1
    #define XC7Z045_STRING2 XC7Z035_STRING2
    #define XC7Z045_STRING3 XC7Z035_STRING3
    #define XC7Z045_STRING4 XC7Z035_STRING4
    #define XC7Z045_STRING5 XC7Z035_STRING5
    #define XC7Z045_STRING6 XC7Z035_STRING6
    #define XC7Z045() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z045_);                                                               \
        XS7_STRING_CAT_7(XC7Z045_);                                                               \
    }
	
    #define XC7Z100_NAME "7z100"
    #define XC7Z100_SLRNUM 1
    #define XC7Z100_ROWNUM 7
    #define XC7Z100_IDCODE 0x3736093
    #define XC7Z100_TOPROWS 1
    #define XC7Z100_BOTROWS 6
    #define XC7Z100_STRING0 "abNMNMBMNINMNMHMNALMLMBMNINMLMNINMBMNINMNMHMNALOLOLOgMLMNINMBMNINMNONpLMLMLMLMNINMBMNINMNONONONMHMNANMHMNMLMNMHMNALMNINMBMLMLMBMNINMLw\x01"
    #define XC7Z100_STRING1 XC7Z100_STRING0
    #define XC7Z100_STRING2 "abNMNMBMNINMNMHMNALMLMBMNINMLMNINMBMNINMNMHMNALOLOLOgMLMNINMBMNINMNONjLMLMLMLMNINMBMNINMNONONONMHMNANMHMNMLMNMHMNALMNINMBMLMLMBMNINMLw\x01"
    #define XC7Z100_STRING3 "abNMNMBMNINMNMHMNALMLMBMNINMLMNINMBMNINMNMHMNALOLOLOgMLMNINMBMNINMNONlLMLMLMLMNINMBMNINMNONONONMHMNANMHMNMLMNMHMNALMNINMBMLPQRCMNINMLw\x01"
    #define XC7Z100_STRING4 "abNMNMBMNINMNMHMNALMLMBMNINMLMNINMBMNINMNMHMNALOLOLOgMLMNINMBMNINMNONoLMLMLMLMNINMBMNINMNONONONMHMNANMHMNMLMNMHMNALMNINMBMLMLMBMNINMLALMLMhx\x01"
    #define XC7Z100_STRING5 "230000400100001004tMLMBMNINMLMNINMBMNINMNMHMNAdddddfgMLMNINMBMNINMNONoLMLMLMLMNINMBMNINMNONONONMHMNANMHMNMLMNMHMNALMNINMBMLMLMBMNINMLALMLMhx\x01"
    #define XC7Z100_STRING6 "230000400100001004uMLMBMNINMLMNINMBMNINMNMHMNAdddddvgMLMNINMBMNINMNONoLMLMLMLMNINMBMNINMNONONONMHMNANMHMNMLMNMHMNALMNINMBMLMLMBMNINMLALMLMhx\x01"
    #define XC7Z100() {                                                                           \
        XS7_SETUP_DEVICE(XC7Z100_);                                                               \
        XS7_STRING_CAT_7(XC7Z100_);                                                               \
    }

//Xilinx Series7 Kintex
    #define XC7K70_NAME "7k70"
    #define XC7K70_SLRNUM 1
    #define XC7K70_ROWNUM 4
    #define XC7K70_IDCODE 0x3647093
    #define XC7K70_TOPROWS 2
    #define XC7K70_BOTROWS 2
    #define XC7K70_STRING0 "abLMLMBMNINMNMHMNALOLOLOgpLMLMBMNINMLALMLMhx\x01"
    #define XC7K70_STRING1 "abLMLMBMNINMNMHMNAdddddfgjLMLMBMNINMLALMLMhx\x01"
    #define XC7K70_STRING2 "abLMLMBMNINMNMHMNAdddddkglLPQRCMNINMLw\x01"
    #define XC7K70_STRING3 "abLMLMBMNINMNMHMNALOLOLOgoLMLMBMNINMLw\x01"
    #define XC7K70() {                                                                            \
        XS7_SETUP_DEVICE(XC7K70_);                                                                \
        XS7_STRING_CAT_4(XC7K70_);                                                                \
    }
	
    #define XC7K160_NAME "7k160"
    #define XC7K160_SLRNUM 1
    #define XC7K160_ROWNUM 5
    #define XC7K160_IDCODE 0x364C093
    #define XC7K160_TOPROWS 2
    #define XC7K160_BOTROWS 3
    #define XC7K160_STRING0 "abLMLMBMNINMNMHMNALOLOLOgONMNINMBONONpLMLANMHMNMNINMBMLMLMBMNINMLMLALMLMhx\x01"
    #define XC7K160_STRING1 XC7K160_STRING0
    #define XC7K160_STRING2 "abLMLMBMNINMNMHMNAdddddfgONMNINMBONONjLMLANMHMNMNINMBMLMLMBMNINMLMLALMLMhx\x01"
    #define XC7K160_STRING3 "abLMLMBMNINMNMHMNAdddddkgONMNINMBONONlLMLANMHMNMNINMBMLPQRCMNINMLMLw\x01"
    #define XC7K160_STRING4 "abLMLMBMNINMNMHMNALOLOLOgONMNINMBONONoLMLANMHMNMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K160() {                                                                           \
        XS7_SETUP_DEVICE(XC7K160_);                                                               \
        XS7_STRING_CAT_5(XC7K160_);                                                               \
    }
	
    #define XC7K325_NAME "7k325"
    #define XC7K325_SLRNUM 1
    #define XC7K325_ROWNUM 7
    #define XC7K325_IDCODE 0x3651093
    #define XC7K325_TOPROWS 4
    #define XC7K325_BOTROWS 3
    #define XC7K325_STRING0 "abLMLMBMNINMNMHMNALOLOLOgONONONOBMNINMNONONONONONpLMLMLMLMLMLMBMNINMLMNINMBMLMLMBMNINMLMLALMLMhx\x01"
    #define XC7K325_STRING1 XC7K325_STRING0
    #define XC7K325_STRING2 "abLMLMBMNINMNMHMNAdddddfgONONONOBMNINMNONONONONONpLMLMLMLMLMLMBMNINMLMNINMBMLMLMBMNINMLMLALMLMhx\x01"
    #define XC7K325_STRING3 "abLMLMBMNINMNMHMNAdddddkgONONONOBMNINMNONONONONONjLMLMLMLMLMLMBMNINMLMNINMBMLPQRCMNINMLMLw\x01"
    #define XC7K325_STRING4 "abLMLMBMNINMNMHMNALOLOLOgONONONOBMNINMNONONONONONlLMLMLMLMLMLMBMNINMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K325_STRING5 "abLMLMBMNINMNMHMNALOLOLOgONONONOBMNINMNONONONONONoLMLMLMLMLMLMBMNINMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K325_STRING6 XC7K325_STRING5
    #define XC7K325() {                                                                           \
        XS7_SETUP_DEVICE(XC7K325_);                                                               \
        XS7_STRING_CAT_7(XC7K325_);                                                               \
    }
	
    #define XC7K355_NAME "7k355"
    #define XC7K355_SLRNUM 1
    #define XC7K355_ROWNUM 6
    #define XC7K355_IDCODE 0x3747093
    #define XC7K355_TOPROWS 3
    #define XC7K355_BOTROWS 3
    #define XC7K355_STRING0 "abLMLMBMNINMNMHMNALOLOLOgOBMNINMNMHMNANONONONOBMNINMNMHMNANONpLMLMBMNINMNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K355_STRING1 XC7K355_STRING0
    #define XC7K355_STRING2 "abLMLMBMNINMNMHMNAdddddfgOBMNINMNMHMNANONONONOBMNINMNMHMNANONjLMLMBMNINMNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K355_STRING3 "abLMLMBMNINMNMHMNAdddddkgOBMNINMNMHMNANONONONOBMNINMNMHMNANONlLMLMBMNINMNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLPQRCMNINMLMLw\x01"
    #define XC7K355_STRING4 "abLMLMBMNINMNMHMNALOLOLOgOBMNINMNMHMNANONONONOBMNINMNMHMNANONoLMLMBMNINMNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K355_STRING5 XC7K355_STRING4
    #define XC7K355() {                                                                           \
        XS7_SETUP_DEVICE(XC7K355_);                                                               \
        XS7_STRING_CAT_6(XC7K355_);                                                               \
    }
	
    #define XC7K410_NAME "7k410"
    #define XC7K410_SLRNUM 1
    #define XC7K410_ROWNUM 7
    #define XC7K410_IDCODE 0x3656093
    #define XC7K410_TOPROWS 4
    #define XC7K410_BOTROWS 3
    #define XC7K410_STRING0 "abLMLMBMNINMNMHMNALOLOLOgONONONOBMNINMNMHMNANONONONOBMNINMNMHMNANpNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLALMLMhx\x01"
    #define XC7K410_STRING1 XC7K410_STRING0
    #define XC7K410_STRING2 "abLMLMBMNINMNMHMNAdddddfgONONONOBMNINMNMHMNANONONONOBMNINMNMHMNANpNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLALMLMhx\x01"
    #define XC7K410_STRING3 "abLMLMBMNINMNMHMNAdddddkgONONONOBMNINMNMHMNANONONONOBMNINMNMHMNANjNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLPQRCMNINMLMLw\x01"
    #define XC7K410_STRING4 "abLMLMBMNINMNMHMNALOLOLOgONONONOBMNINMNMHMNANONONONOBMNINMNMHMNANlNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K410_STRING5 "abLMLMBMNINMNMHMNALOLOLOgONONONOBMNINMNMHMNANONONONOBMNINMNMHMNANoNMHMNALMLMLMLMBMNINMNMHMNALMLMLMNINMBMLMLMBMNINMLMLw\x01"
    #define XC7K410_STRING6 XC7K410_STRING5
    #define XC7K410() {                                                                           \
        XS7_SETUP_DEVICE(XC7K410_);                                                               \
        XS7_STRING_CAT_7(XC7K410_);                                                               \
    }
	
	//XC7K420 is software-limitted version of XC7K480, which itself is XC7K355's rows but more
    #define XC7K420_NAME "7k420"
    #define XC7K420_SLRNUM 1
    #define XC7K420_ROWNUM 8
    #define XC7K420_IDCODE 0x3752093
    #define XC7K420_TOPROWS 4
    #define XC7K420_BOTROWS 4
    #define XC7K420_STRING0 XC7K355_STRING0
    #define XC7K420_STRING1 XC7K355_STRING0
    #define XC7K420_STRING2 XC7K355_STRING0
    #define XC7K420_STRING3 XC7K355_STRING2
    #define XC7K420_STRING4 XC7K355_STRING3
    #define XC7K420_STRING5 XC7K355_STRING4
    #define XC7K420_STRING6 XC7K355_STRING4
    #define XC7K420_STRING7 XC7K355_STRING4
    #define XC7K420() {                                                                           \
        XS7_SETUP_DEVICE(XC7K420_);                                                               \
        XS7_STRING_CAT_8(XC7K420_);                                                               \
    }
	
    #define XC7K480_NAME "7k480"
    #define XC7K480_SLRNUM 1
    #define XC7K480_ROWNUM 8
    #define XC7K480_IDCODE 0x3751093
    #define XC7K480_TOPROWS 4
    #define XC7K480_BOTROWS 4
    #define XC7K480_STRING0 XC7K420_STRING0
    #define XC7K480_STRING1 XC7K420_STRING1
    #define XC7K480_STRING2 XC7K420_STRING2
    #define XC7K480_STRING3 XC7K420_STRING3
    #define XC7K480_STRING4 XC7K420_STRING4
    #define XC7K480_STRING5 XC7K420_STRING5
    #define XC7K480_STRING6 XC7K420_STRING6
    #define XC7K480_STRING7 XC7K420_STRING7
    #define XC7K480() {                                                                           \
        XS7_SETUP_DEVICE(XC7K480_);                                                               \
        XS7_STRING_CAT_8(XC7K480_);                                                               \
    }

//Xilinx Series7 Virtex
    #define XC7V585_NAME "7v585"
    #define XC7V585_SLRNUM 1
    #define XC7V585_ROWNUM 9
    #define XC7V585_IDCODE 0x3671093
    #define XC7V585_TOPROWS 5
    #define XC7V585_BOTROWS 4
    #define XC7V585_STRING0 "abLMLMBMNINMLMLMNMHMNALOLOLOgONONONONANMHMNONONMNINMBONpLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLMLMBONONw\x01"
    #define XC7V585_STRING1 "abLMLMBMNINMLMLMNMHMNALOLOLOgONONONONANMHMNONONMNINMBONpLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLPQRCONONw\x01"
    #define XC7V585_STRING2 XC7V585_STRING0
    #define XC7V585_STRING3 "ybLMLMBMNINMLMLMNMHMNAdddddfgONONONONANMHMNONONMNINMBONpLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLMLMBONONw\x01"
    #define XC7V585_STRING4 "ybLMLMBMNINMLMLMNMHMNAdddddngONONONONANMHMNONONMNINMBONjLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLPQRCONONw\x01"
    #define XC7V585_STRING5 "ybLMLMBMNINMLMLMNMHMNALOLOLOgONONONONANMHMNONONMNINMBONlLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLMLMBONONw\x01"
    #define XC7V585_STRING6 "ybLMLMBMNINMLMLMNMHMNALOLOLOgONONONONANMHMNONONMNINMBONoLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLMLMBONONw\x01"
    #define XC7V585_STRING7 "ybLMLMBMNINMLMLMNMHMNALOLOLOgONONONONANMHMNONONMNINMBONoLMLMLMLMLMLMLALMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMNINMBMLPQRCONONw\x01"
    #define XC7V585_STRING8 XC7V585_STRING6
    #define XC7V585() {                                                                           \
        XS7_SETUP_DEVICE(XC7V585_);                                                               \
        XS7_STRING_CAT_9(XC7V585_);                                                               \
    }

    #define XC7V2000_NAME "7v2000"
    #define XC7V2000_SLRNUM 4
    #define XC7V2000_ROWNUM 12
    #define XC7V2000_IDCODE 0x36B3093
    #define XC7V2000_IDCODE1 0x36A2093
    #define XC7V2000_IDCODE2 0x36A4093
    #define XC7V2000_IDCODE3 0x36A6093
    #define XC7V2000_TOPROWS 2
    #define XC7V2000_BOTROWS 1
	#define XC7V2000_SLR1_START 0
    #define XC7V2000_STRING0 "DMNINMLMLMLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMybLMLMBMNINMLMLMNMHMNAdddddzgONONONONONONONONONONONMNINMBONONONONONONONONONONONONONJLMLMLMLMLMLMLMLMLMLMLMLMLMLMLALMLMLMLMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMLMLMLMLMLMLMNINMBMLMLMBONONw\x01"
    #define XC7V2000_STRING1 "BMNINMLMLMLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMybLMLMBMNINMLMLMNMHMNAdddddKgONONONONONONONONONONONMNINMBONONONONONONONONONONONONONjLMLMLMLMLMLMLMLMLMLMLMLMLMLMLALMLMLMLMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMLMLMLMLMLMLMNINMBMLPQRCONONw\x01"
    #define XC7V2000_STRING2 "EMNINMLMLMLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMybLMLMBMNINMLMLMNMHMNALOLOLOgONONONONONONONONONONONMNINMBONONONONONONONONONONONONONeLMLMLMLMLMLMLMLMLMLMLMLMLMLMLALMLMLMLMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMLMLMLMLMLMLMNINMBMLMLMBONONw\x01"
    #define XC7V2000_SLR1_END 2
    #define XC7V2000_SLR0_START 3
	#define XC7V2000_STRING3 "DMNINMLMLMLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMybLMLMBMNINMLMLMNMHMNAdddddfgONONONONONONONONONONONMNINMBONONONONONONONONONONONONONJLMLMLMLMLMLMLMLMLMLMLMLMLMLMLALMLMLMLMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMLMLMLMLMLMLMNINMBMLMLMBONONw\x01"
    #define XC7V2000_STRING4 "BMNINMLMLMLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMybLMLMBMNINMLMLMNMHMNAdddddngONONONONONONONONONONONMNINMBONONONONONONONONONONONONONjLMLMLMLMLMLMLMLMLMLMLMLMLMLMLALMLMLMLMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMNMHMNALMLMhxLMLMLMLMLMLMLMLMLMLMLMLMBMNINMLMLMLMLMLMLMLMLMNINMBMLPQRCONONw\x01"
    #define XC7V2000_STRING5 XC7V2000_STRING2
	#define XC7V2000_SLR0_END 5
	#define XC7V2000_SLR2_START 6
    #define XC7V2000_STRING6 XC7V2000_STRING0
    #define XC7V2000_STRING7 XC7V2000_STRING1
    #define XC7V2000_STRING8 XC7V2000_STRING2
	#define XC7V2000_SLR2_END 8
	#define XC7V2000_SLR3_START 9
    #define XC7V2000_STRING9 XC7V2000_STRING0
    #define XC7V2000_STRING10 XC7V2000_STRING1
    #define XC7V2000_STRING11 XC7V2000_STRING2
	#define XC7V2000_SLR3_END 11
    #define XC7V2000() {                                                                          \
        XS7_SETUP_DEVICE(XC7V2000_);                                                              \
        XS7_SETUP_SLRs_4(XC7V2000_);                                                             \
        XS7_STRING_CAT_12(XC7V2000_);                                                             \
    }

//TODO: VH virtex7 devices need some special support for the empty GTZ SLRs. Skip them for now
	//VH580 has a third SLR that is empty?!
    #define XC7VH580_NAME "7vh580"
    #define XC7VH580_SLRNUM 2
    #define XC7VH580_ROWNUM 6
    #define XC7VH580_IDCODE 0x36D9093
    #define XC7VH580_IDCODE1 0x36C2093
    #define XC7VH580_TOPROWS 1
    #define XC7VH580_BOTROWS 2
	#define XC7VH580_SLR0_START 0
    #define XC7VH580_STRING0 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONONANMHMNONONONONONONONONJLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VH580_STRING1 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddfgONONONONONONONONANMHMNONONONONONONONONjLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VH580_STRING2 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddngONONONONONONONONANMHMNONONONONONONONONeLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VH580_SLR0_END 2
	#define XC7VH580_SLR1_START 3
	#define XC7VH580_STRING3 XC7VH580_STRING0
    #define XC7VH580_STRING4 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddzgONONONONONONONONANMHMNONONONONONONONONjLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VH580_STRING5 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddKgONONONONONONONONANMHMNONONONONONONONONsLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
	#define XC7VH580_SLR1_END 5
	#define XC7VH580() {                                                                          \
        XS7_SETUP_DEVICE(XC7VH580_);                                                              \
        XS7_SETUP_SLRs_2(XC7VH580_);                                                             \
        XS7_STRING_CAT_6(XC7VH580_);                                                              \
    }

	//VH870 has first and fifth SLRs that are empty?!
    #define XC7VH870_NAME "7vh870"
    #define XC7VH870_SLRNUM 3
    #define XC7VH870_ROWNUM 9
    #define XC7VH870_IDCODE 0x36DB093
    #define XC7VH870_IDCODE1 0x36C2093
    #define XC7VH870_IDCODE2 0x36C4093
    #define XC7VH870_TOPROWS 1
    #define XC7VH870_BOTROWS 2
	//SLR '1' is under the chip
	#define XC7VH870_SLR1_START 0
    #define XC7VH870_STRING0 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONONANMHMNONONONONONONONON#LMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VH870_STRING1 XC7VH580_STRING4
    #define XC7VH870_STRING2 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddKgONONONONONONONONANMHMNONONONONONONONONeLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VH870_SLR1_END 2
	#define XC7VH870_SLR0_START 3
	#define XC7VH870_STRING3 XC7VH580_STRING0
    #define XC7VH870_STRING4 XC7VH580_STRING1
    #define XC7VH870_STRING5 XC7VH580_STRING2
	#define XC7VH870_SLR0_END 5
	#define XC7VH870_SLR2_START 6
    #define XC7VH870_STRING6 XC7VH580_STRING0
    #define XC7VH870_STRING7 XC7VH580_STRING4
    #define XC7VH870_STRING8 XC7VH580_STRING5
	#define XC7VH870_SLR2_END 8
	
    #define XC7VH870() {                                                                          \
        XS7_SETUP_DEVICE(XC7VH870_);                                                              \
        XS7_SETUP_SLRs_3(XC7VH870_);                                                             \
        XS7_STRING_CAT_9(XC7VH870_);                                                              \
    }

    #define XC7VX330_NAME "7vx330"
    #define XC7VX330_SLRNUM 1
    #define XC7VX330_ROWNUM 7
    #define XC7VX330_IDCODE 0x3667093
    #define XC7VX330_TOPROWS 5
    #define XC7VX330_BOTROWS 2
    #define XC7VX330_STRING0 "abLMLMBMNINMNMHMNALOLOLOgONONANMHMNALMLpNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX330_STRING1 "ybLMLMBMNINMNMHMNAdddddfgONONANMHMNALMLpNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX330_STRING2 "ybLMLMBMNINMNMHMNAdddddngONONANMHMNALMLpNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX330_STRING3 "ybLMLMBMNINMNMHMNALOLOLOgONONANMHMNALMLjNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX330_STRING4 "ybLMLMBMNINMNMHMNALOLOLOgONONANMHMNALMLlNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX330_STRING5 "ybLMLMBMNINMNMHMNALOLOLOgONONANMHMNALMLoNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX330_STRING6 "ybLMLMBMNINMNMHMNALOLOLOgONONANMHMNALMLoNONOBMNINMNMHMNANONOBMNINMNMHMNANONOhxNONOBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX330() {                                                                          \
        XS7_SETUP_DEVICE(XC7VX330_);                                                              \
        XS7_STRING_CAT_7(XC7VX330_);                                                              \
    }

    #define XC7VX415_NAME "7vx415"
    #define XC7VX415_SLRNUM 1
    #define XC7VX415_ROWNUM 6
    #define XC7VX415_IDCODE 0x3682093
    #define XC7VX415_TOPROWS 5
    #define XC7VX415_BOTROWS 1
    #define XC7VX415_STRING0 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOdddddfgONONANMHMNjNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX415_STRING1 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOdddddngONONANMHMNlNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX415_STRING2 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX415_STRING3 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX415_STRING4 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX415_STRING5 XC7VX415_STRING2
    #define XC7VX415() {                                                                          \
        XS7_SETUP_DEVICE(XC7VX415_);                                                              \
        XS7_STRING_CAT_6(XC7VX415_);                                                              \
    }

    #define XC7VX485_NAME "7vx485"
    #define XC7VX485_SLRNUM 1
    #define XC7VX485_ROWNUM 7
    #define XC7VX485_IDCODE 0x3687093
    #define XC7VX485_TOPROWS 5
    #define XC7VX485_BOTROWS 2
    #define XC7VX485_STRING0 "wONONALMNMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMLOLOLOgMNMHMNANMHMNpNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLMLMBONONw\x01"
    #define XC7VX485_STRING1 "wONONALMNMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMdddddfgMNMHMNANMHMNpNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLMLMBONONw\x01"
    #define XC7VX485_STRING2 "wONONGYRZMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMdddddngMNMHMNANMHMNpNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLPQRCONONw\x01"
    #define XC7VX485_STRING3 "wONONALMNMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMLOLOLOgMNMHMNANMHMNjNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLMLMBONONw\x01"
    #define XC7VX485_STRING4 "wONONALMNMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMLOLOLOgMNMHMNANMHMNlNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLMLMBONONw\x01"
    #define XC7VX485_STRING5 "wONONGYRZMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMLOLOLOgMNMHMNANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLPQRCONONw\x01"
    #define XC7VX485_STRING6 "wONONALMNMLANMHMNMNMHMNANMHMNALMybNMHMNANMHMNMNINMBMNINMLOLOLOgMNMHMNANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMBMNINMLMNINMBMLMLMBONONw\x01"
    #define XC7VX485() {                                                                          \
        XS7_SETUP_DEVICE(XC7VX485_);                                                              \
        XS7_STRING_CAT_7(XC7VX485_);                                                              \
    }

	//XC7VX550 is software-limitted version of XC7VX690
    #define XC7VX550_NAME "7vx550"
    #define XC7VX550_SLRNUM 1
    #define XC7VX550_ROWNUM 10
    #define XC7VX550_IDCODE 0x3692093
    #define XC7VX550_TOPROWS 5
    #define XC7VX550_BOTROWS 5
    #define XC7VX550_STRING0 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNpNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX550_STRING1 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNpNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX550_STRING2 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNpNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX550_STRING3 XC7VX550_STRING0
    #define XC7VX550_STRING4 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOdddddfgONONANMHMNjNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX550_STRING5 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOdddddngONONANMHMNlNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX550_STRING6 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX550_STRING7 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX550_STRING8 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMybNMHMNANMHMNMNINMBOLOLOLOgONONANMHMNoNMHMNANMHMNMNINMBMNINMNMHMNANMHMNMNINMBMNINMhxLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX550_STRING9 XC7VX550_STRING6
    #define XC7VX550() {                                                                          \
        XS7_SETUP_DEVICE(XC7VX550_);                                                              \
        XS7_STRING_CAT_10(XC7VX550_);                                                             \
    }

    #define XC7VX690_NAME "7vx690"
    #define XC7VX690_SLRNUM 1
    #define XC7VX690_ROWNUM 10
    #define XC7VX690_IDCODE 0x3691093
    #define XC7VX690_TOPROWS 5
    #define XC7VX690_BOTROWS 5
    #define XC7VX690_STRING0 XC7VX550_STRING0
    #define XC7VX690_STRING1 XC7VX550_STRING1
    #define XC7VX690_STRING2 XC7VX550_STRING2
    #define XC7VX690_STRING3 XC7VX550_STRING3
    #define XC7VX690_STRING4 XC7VX550_STRING4
    #define XC7VX690_STRING5 XC7VX550_STRING5
    #define XC7VX690_STRING6 XC7VX550_STRING6
    #define XC7VX690_STRING7 XC7VX550_STRING7
    #define XC7VX690_STRING8 XC7VX550_STRING8
    #define XC7VX690_STRING9 XC7VX550_STRING9
    #define XC7VX690() {                                                                          \
        XS7_SETUP_DEVICE(XC7VX690_);                                                              \
        XS7_STRING_CAT_10(XC7VX690_);                                                             \
    }

    #define XC7VX980_NAME "7vx980"
    #define XC7VX980_SLRNUM 1
    #define XC7VX980_ROWNUM 9
    #define XC7VX980_IDCODE 0x3696093
    #define XC7VX980_TOPROWS 4
    #define XC7VX980_BOTROWS 5
    #define XC7VX980_STRING0 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONMHMNANMHMNMLMLMLMLMLMLpLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX980_STRING1 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONMHMNANMHMNMLMLMLMLMLMLpLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX980_STRING2 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONMHMNANMHMNMLMLMLMLMLMLpLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX980_STRING3 XC7VX980_STRING0
    #define XC7VX980_STRING4 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddfgONONONONONONONMHMNANMHMNMLMLMLMLMLMLjLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX980_STRING5 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddngONONONONONONONMHMNANMHMNMLMLMLMLMLMLlLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX980_STRING6 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONMHMNANMHMNMLMLMLMLMLMLoLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX980_STRING7 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONMHMNANMHMNMLMLMLMLMLMLoLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX980_STRING8 "_ONONANMHMNALMLMNMLANMHMNMNINMBMNINMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONMHMNANMHMNMLMLMLMLMLMLoLMLMLMLMLMLMNMHMNANMHMNMNINMBMNINMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMNMHMNANMHMNMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX980() {                                                                          \
        XS7_SETUP_DEVICE(XC7VX980_);                                                              \
        XS7_STRING_CAT_9(XC7VX980_);                                                              \
    }

    #define XC7VX1140_NAME "7vx1140"
    #define XC7VX1140_SLRNUM 4
    #define XC7VX1140_ROWNUM 12
    #define XC7VX1140_IDCODE 0x36D5093
    #define XC7VX1140_IDCODE1 0x36C2093
    #define XC7VX1140_IDCODE2 0x36C4093
    #define XC7VX1140_IDCODE3 0x36C6093
    #define XC7VX1140_TOPROWS 1
    #define XC7VX1140_BOTROWS 2
	#define XC7VX1140_SLR1_START 0
    #define XC7VX1140_STRING0 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMLOLOLOgONONONONONONONONANMHMNONONONONONONONONJLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLMLMLMBMNINMBONON_\x01"
    #define XC7VX1140_STRING1 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddzgONONONONONONONONANMHMNONONONONONONONONjLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX1140_STRING2 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddKgONONONONONONONONANMHMNONONONONONONONONeLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX1140_SLR1_END 2
	#define XC7VX1140_SLR0_START 3
	#define XC7VX1140_STRING3 XC7VX1140_STRING0
    #define XC7VX1140_STRING4 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddfgONONONONONONONONANMHMNONONONONONONONONjLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLSTUVUFMNINMBONON_\x01"
    #define XC7VX1140_STRING5 "_ONONANMHMNALMLMNMLANMHMNONMHMNALMLMNMHMNALMLMybLMLMBMNINMNMHMNALMLMdddddngONONONONONONONONANMHMNONONONONONONONONeLMLMLMLMLMLMLMLMNMHMNALMLMLMLMLMLMLMLMLMLMLMLMLMBMNINMNMHMNALMLMhxLMLMBMNINMLMLMBMNINMLMNINMBMLWXRYRCMNINMBONON_\x01"
    #define XC7VX1140_SLR0_END 5
	#define XC7VX1140_SLR2_START 6
	#define XC7VX1140_STRING6 XC7VX1140_STRING0
    #define XC7VX1140_STRING7 XC7VX1140_STRING1
    #define XC7VX1140_STRING8 XC7VX1140_STRING2
    #define XC7VX1140_SLR2_END 8
	#define XC7VX1140_SLR3_START 9
	#define XC7VX1140_STRING9 XC7VX1140_STRING0
    #define XC7VX1140_STRING10 XC7VX1140_STRING1
    #define XC7VX1140_STRING11 XC7VX1140_STRING2
    #define XC7VX1140_SLR3_END 11
	#define XC7VX1140() {                                                                         \
        XS7_SETUP_DEVICE(XC7VX1140_);                                                             \
        XS7_SETUP_SLRs_4(XC7VX1140_);                                                            \
        XS7_STRING_CAT_12(XC7VX1140_);                                                            \
    }

#endif // XILINXSERIES7DEVICES_H
