/******************************************************************************
 * Copyright 2022 Kristiyan Manev, University of Manchester
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
#ifndef XILINXULTRASCALEPLUS_H
#define XILINXULTRASCALEPLUS_H
#include <cstdint> //uint32_t
#include <vector>
#include <string>
#include "XilinxUltraScalePlusFabric.h"
using namespace std;
class XilinxUltraScalePlus
{
    public:
        XilinxUltraScalePlus();
        virtual ~XilinxUltraScalePlus();
		
		//
		struct rect {
			int posRow, posCol, sizeRow, sizeCol;
		};
		
		//runtime params
        int verbose, warn;
		
		//XilinxUltraScalePlusDevices.cpp:
		int getDeviceByIDCODEorThrow(int);
		int getDeviceByNameOrThrow(string);
		int getDeviceByIDCODE(int);
		int getDeviceByName(string);
		void setDevice(int, string = "");
		
		//Info
		void printDeviceList();
		
		//region selector
		void region(string, int, int, int, int);
		//file IO
        void readBitstream(string);
        void writeBitstream(string, string, int, int, int, int);
		
        //merge (relocate)
        void merge(XilinxUltraScalePlus*, string, int, int, int, int, int, int);
		//functions
		void blank(string);
		
		//The ones below are not needed to be public, but we don't really care
		
		void outputBITheader(ofstream&);
		void outputBITfooter(ofstream&);
		void outputBITregion(ofstream&, int, int, int, int, int, int, int);
		vector<rect> regionsSelected;
		
		//Priv functions
        void readBitstreamBIT(string);
        void writeBitstreamBIT(string, string, int, int, int, int);
		
        //loaded bitstream data
        string initializedBitstreamPartName; ///< the partName of currently initialized bitstream buffers
        int numberOfBRAMCols;
        int numberOfCols;
        int numberOfFramesBeforeCol[XUSP_MAX_COLS];
        int numberOfBRAMsBeforeCol[XUSP_MAX_COLS];
        int numberOfFramesPerRow;
        int numberOfWordsPerRow;
        void ensureInitializedBitstreamArrays();

		void* blankFrame;//helper frame
        uint32_t* bitstreamGlobal;
        uint32_t* bitstreamCLB[XUSP_MAX_ROWS][XUSP_MAX_COLS];

        uint32_t* bitstreamBRAM[XUSP_MAX_ROWS][XUSP_MAX_BRAM_COLS];
		
		uint32_t* bitstreamEnd;
		
		//selected device params
        int numberOfSLRs;
        int numberOfRows;
        int IDCODEs[XUSP_MAX_SLRS];
        string designName;
		string partName;
        char resourceString[XUSP_MAX_ROWS][XUSP_MAX_COLS];
    protected:
    private:
		//constant arrays defining US+ fabric
        int const numberOfFramesPerResourceLetter[256] = XUSP_LOOKUP_FRAME_SIZES;
        char const *typeOfFramesPerResourceLetter[256] = XUSP_LOOKUP_FRAME_NAMES;
};

#endif // XILINXULTRASCALEPLUS_H
