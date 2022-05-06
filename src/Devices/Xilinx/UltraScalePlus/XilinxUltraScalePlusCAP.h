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

#define XUSP_CAP_REG_CRC 0
#define XUSP_CAP_REG_FAR 1
#define XUSP_CAP_REG_FDRI 2
#define XUSP_CAP_REG_FDRO 3
#define XUSP_CAP_REG_CMD 4
#define XUSP_CAP_REG_CTRL0 5
#define XUSP_CAP_REG_MASK 6
#define XUSP_CAP_REG_STAT 7
#define XUSP_CAP_REG_LOUT 8
#define XUSP_CAP_REG_COR0 9
#define XUSP_CAP_REG_MFWR 10
#define XUSP_CAP_REG_CBC 11
#define XUSP_CAP_REG_IDCODE 12
#define XUSP_CAP_REG_AXSS 13
#define XUSP_CAP_REG_COR1 14
#define XUSP_CAP_REG_WBSTAR 16
#define XUSP_CAP_REG_TIMER 17
#define XUSP_CAP_REG_MAGIC0 19
#define XUSP_CAP_REG_BOOTSTS 22
#define XUSP_CAP_REG_CTRL1 24
#define XUSP_CAP_REG_MAGIC1 30
#define XUSP_CAP_REG_BSPI 31
#define XUSP_CAP_REG_UNKNOWN 32

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
#define XUSP_CAP_CMD_UNKNOWN 32

#define OP_NOP 0
#define OP_READ 1
#define OP_WRITE 2
#define OP_RESERVED 3 

#define getInstructionType(instruction) ((instruction>>29) & 0x7)
#define getInstructionOPCODE(instruction) ((instruction>>27) & 0x3)
#define getInstructionPayload(instruction) (instruction & 0x07FFFFFF)
#define getInstructionType1Register(instruction) ((instruction>>13) & 0x1F)
#define getInstructionType1WordCount(instruction) (instruction & 0x7FF)

#define parseFAR(farVal, varBA, varRow, varCol, varMin) {varBA=(farVal>>24)&0x7;varRow=(farVal>>18)&0x3F;varCol=(farVal>>8)&0x3FF;varMin=farVal&0xFF;}

#define CAPwriteSync(ostream) {fileWriteBE(ostream, 32, 0xAA995566);}
#define CAPwriteNOP(ostream) {fileWriteBE(ostream, 32, 0x20000000);}
#define CAPwriteNOPwData(ostream, data) {fileWriteBE(ostream, 32, 0x20000000+data);}
#define CAPwriteRESERVEDwData(ostream, data) {fileWriteBE(ostream, 32, 0x38000000+data);}
#define CAPwriteNOPs(ostream,n) {for(int itemp=0;itemp<n;itemp++) CAPwriteNOP(ostream);}
#define CAPwriteReg(ostream, reg, val) {fileWriteBE(ostream, 32, (0x30000001+(reg<<13)));fileWriteBE(ostream, 32, val);}
#define CAPreadReg(ostream, reg, length) {fileWriteBE(ostream, 32, (0x28000000+(reg<<13)+length));}
#define CAPselectReg(ostream, reg) {fileWriteBE(ostream, 32, (0x30000000+(reg<<13)));}
#define CAPwriteFAR(ostream, btype, row, maj, min) {CAPwriteReg(ostream,XUSP_CAP_REG_FAR,(((btype&0x7)<<24) + ((row&0x3F)<<18) + ((maj&0x3FF)<<8) + (min&0xFF)));}
#define CAPwriteFDRI1(ostream, length) {fileWriteBE(ostream, 32, (0x30000000+(XUSP_CAP_REG_FDRI<<13)+length));}
#define CAPwriteFDRI2(ostream, length) {fileWriteBE(ostream, 32, (0x50000000+length));}
#define CAPselectAndWriteFDRI2(ostream, length) {CAPwriteFDRI1(ostream,0);CAPwriteFDRI2(ostream,length);}
#endif // XILINXULTRASCALEPLUSCAP_H
