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
#ifndef XILINXULTRASCALEPLUSCAP_H
#define XILINXULTRASCALEPLUSCAP_H

#define XUSP_BLOCKTYPE_LOGIC 0
#define XUSP_BLOCKTYPE_BRAM 1

#define XUSP_CAP_CRC 0
#define XUSP_CAP_FAR 1
#define XUSP_CAP_FDRI 2
#define XUSP_CAP_FDRO 3
#define XUSP_CAP_CMD 4
#define XUSP_CAP_CTRL0 5
#define XUSP_CAP_MASK 6
#define XUSP_CAP_STAT 7
#define XUSP_CAP_LOUT 8
#define XUSP_CAP_COR0 9
#define XUSP_CAP_MFWR 10
#define XUSP_CAP_CBC 11
#define XUSP_CAP_IDCODE 12
#define XUSP_CAP_AXSS 13
#define XUSP_CAP_COR1 14
#define XUSP_CAP_WBSTAR 16
#define XUSP_CAP_TIMER 17
#define XUSP_CAP_UNDOC0 19
#define XUSP_CAP_BOOTSTS 22
#define XUSP_CAP_CTRL1 24
#define XUSP_CAP_UNDOC1 30
#define XUSP_CAP_BSPI 31

#define XUSP_CAP_CMD_NULL 0
#define XUSP_CAP_CMD_WCFG 1
#define XUSP_CAP_CMD_MFW 2
#define XUSP_CAP_CMD_DGHIGH 3
#define XUSP_CAP_CMD_LFRM 3
#define XUSP_CAP_CMD_RCFG 4
#define XUSP_CAP_CMD_START 5
#define XUSP_CAP_CMD_URAM 6
#define XUSP_CAP_CMD_RCRC 7
#define XUSP_CAP_CMD_AGHIGH 8
#define XUSP_CAP_CMD_SWITCH 9
#define XUSP_CAP_CMD_GRESTORE 10
#define XUSP_CAP_CMD_SHUTDOWN 11
#define XUSP_CAP_CMD_DESYNC 13
#define XUSP_CAP_CMD_IPROG 15
#define XUSP_CAP_CMD_CRCC 16
#define XUSP_CAP_CMD_LTIMER 17
#define XUSP_CAP_CMD_BSPI_READ 18
#define XUSP_CAP_CMD_FALL_EDGE 19

#define writeSync(ostream) fileWriteBE(ostream, 32, 0xAA995566);
#define writeNOP(ostream) fileWriteBE(ostream, 32, 0x20000000);
#define writeNOPs(ostream,n) for(int itemp=0;itemp<n;itemp++) writeNOP(ostream);
#define writeReg(ostream, reg, val) fileWriteBE(ostream, 32, (0x30000001+(reg<<13)));fileWriteBE(ostream, 32, val);
#define writeFAR(ostream, btype, row, maj, min) writeReg(ostream,XUSP_CAP_FAR,((btype&0x7)<<24 || (row&0x3F)<<18 || (maj&0x3FF)<<8 || (min&0xFF)))
#endif // XILINXULTRASCALEPLUSCAP_H
