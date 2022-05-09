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
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <stdio.h>
#include <istream>
#include <ostream>
#include <string>
#include "byteman.h"
#include "Common/parseString.h"
using namespace std;

#include "Devices/Xilinx/UltraScalePlus/XilinxUltraScalePlus.h"

Arch selectedArch = ARCH_NULL;
#ifdef XUSP
XilinxUltraScalePlus mainXUSP, tempXUSP;
#endif //XUSP
void ArchDeviceHelp(){
	#ifdef XUSP
	if(selectedArch == ARCH_NULL || selectedArch == XIL_USP)
		XilinxUltraScalePlus::deviceHelp();
	#endif
}
void ArchAssemblyHelp(){
	#ifdef XUSP
	if(selectedArch == ARCH_NULL || selectedArch == XIL_USP)
		XilinxUltraScalePlus::assemblerHelp();
	#endif
}
#define paramHas(x) (params.find(x) != string::npos)
void parseVerbose(string verboseCmd){
	int verboseValue;
	if(!parseString::getMultipleInts(verboseCmd, verboseValue))verboseValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.verbose = verboseValue;
		tempXUSP.verbose = verboseValue;
	}
	#endif
}
void parseWarn(string warnCmd){
	int warnValue;
	if(!parseString::getMultipleInts(warnCmd, warnValue))warnValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.warn = warnValue;
		tempXUSP.warn = warnValue;
	}
	#endif
}
void parseRegion(string regionCmd){
	string params = parseString::getAllWords(regionCmd);
	int srcRow, srcCol, sizeRow, sizeCol;
	parseString::getMultipleInts(regionCmd, srcRow, srcCol, sizeRow, sizeCol);
	//srcRow = getNthInt(regionCmd, 0);
	//srcCol = getNthInt(regionCmd, 1);
	//sizeRow = getNthInt(regionCmd, 2);
	//sizeCol = getNthInt(regionCmd, 3);
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.region(params, srcRow, srcCol, sizeRow, sizeCol);
		tempXUSP.region(params, srcRow, srcCol, sizeRow, sizeCol);
	}
	#endif
}
void parseBlank(string blankCmd){
	string params = parseString::getAllWords(blankCmd);
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		if(paramHas("main") || paramHas("first"))
			mainXUSP.blank(params);
		if(paramHas("temp") || paramHas("second"))
			tempXUSP.blank(params);
	}
	#endif
}
void parseChange(string changeCmd){
	//TODO lut,ff,wires,bram,uram modifications
	#ifdef XUSP
	;
	#endif
}
void parseDevice(string deviceCmd){
	string params = parseString::getAllButLastWord(deviceCmd);
	string deviceName = parseString::getLastWord(deviceCmd);
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		if(paramHas("main") || paramHas("first"))
			mainXUSP.setDevice(mainXUSP.getDeviceByNameOrThrow(deviceName), deviceName);
		if(paramHas("temp") || paramHas("second"))
			tempXUSP.setDevice(mainXUSP.getDeviceByNameOrThrow(deviceName), deviceName);
	}
	#endif
}
void parseInput(string inputCmd){
	string params = parseString::getAllButLastWord(inputCmd);
	string filename = parseString::getLastWord(inputCmd);
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		if(paramHas("main") || paramHas("first"))
			mainXUSP.readBitstream(filename);
		if(paramHas("temp") || paramHas("second"))
			tempXUSP.readBitstream(filename);
	}
	#endif
}
void parseMerge(string mergeCmd){
	string params = parseString::getAllWords(mergeCmd);
	int srcRow, srcCol, sizeRow, sizeCol, dstRow, dstCol;
	parseString::getMultipleInts(mergeCmd, srcRow, srcCol, sizeRow, sizeCol, dstRow, dstCol);
	//srcRow = getNthInt(mergeCmd, 0);
	//srcCol = getNthInt(mergeCmd, 1);
	//sizeRow = getNthInt(mergeCmd, 2);
	//sizeCol = getNthInt(mergeCmd, 3);
	//dstRow = getNthInt(mergeCmd, 4);
	//dstCol = getNthInt(mergeCmd, 5);
	#ifdef XUSP
	if(selectedArch == XIL_USP)
		mainXUSP.merge(&tempXUSP, params, srcRow, srcCol, sizeRow, sizeCol, dstRow, dstCol);
	#endif
}
void parseOutput(string outputCmd){
	string params = parseString::getAllButLastWord(outputCmd);
	string filename = parseString::getLastWord(outputCmd);
	int srcRow, srcCol, sizeRow, sizeCol;
	parseString::getMultipleInts(outputCmd, srcRow, srcCol, sizeRow, sizeCol);
	//srcRow = getNthInt(outputCmd, 0);
	//srcCol = getNthInt(outputCmd, 1);
	//sizeRow = getNthInt(outputCmd, 2);
	//sizeCol = getNthInt(outputCmd, 3);
	#ifdef XUSP
	if(selectedArch == XIL_USP)
		mainXUSP.writeBitstream(filename, params, srcRow, srcCol, sizeRow, sizeCol);
	#endif
}
void parseAssembly(string assemblyCmd){
	string filenameIn = parseString::getNthWord(assemblyCmd, 0);
	string filenameOut = parseString::getNthWord(assemblyCmd, 1);
	//TODO , text assembly -> bitstream and vice versa
	#ifdef XUSP
	mainXUSP.assembler(filenameIn, filenameOut);
	#endif
}
#undef paramHas
bool archSelected = false;
void setArch(string arch){
	archSelected = true; // it will either select it or throw an exception, so just mark it anyway who cares
	#define has(x) (arch.rfind(x) != string::npos)
	#define hOR(x,y) (has(x)||has(y))
	transform(arch.begin(), arch.end(),arch.begin(), ::toupper);
	#ifdef XUSP
	if(has("XUSP") || (has("XILINX") && hOR("ULTRASCALE", "US") && hOR("PLUS","+") ) ) {
		selectedArch = XIL_USP;
		return;
	}
	#endif
	#undef has
	#undef hOR
	
	throw runtime_error(string("Unknown architecture: \"").append(arch).append("\"."));
}
void parseCommand(string nextCmd){
	replace(nextCmd.begin(), nextCmd.end(), '=', ' ');
	if (nextCmd.at(0) == '-') nextCmd.erase(0, 1);
	#define cmdIs(x,y) (0 == nextCmd.rfind(x, 0))
	if(cmdIs("h", "help")){
		parseHelp(nextCmd);
		return;
	}
	if(!archSelected)setArch(nextCmd);
	if(cmdIs("v", "verbose"))
		parseVerbose(nextCmd);
	else if(cmdIs("w", "warn"))
		parseWarn(nextCmd);
	else if(cmdIs("r", "region"))
		parseRegion(nextCmd);
	else if(cmdIs("b", "blank"))
		parseBlank(nextCmd);
	else if(cmdIs("c", "change"))
		parseChange(nextCmd);
	else if(cmdIs("d", "device"))
		parseDevice(nextCmd);
	else if(cmdIs("i", "input"))
		parseInput(nextCmd);
	else if(cmdIs("m", "merge"))
		parseMerge(nextCmd);
	else if(cmdIs("o", "output"))
		parseOutput(nextCmd);
	else if(cmdIs("a", "assembly"))
		parseAssembly(nextCmd);
	else if(cmdIs("e", "end"))
		exit(0);
	#undef cmdIs
}


/**************************************************************************//**
 * @mainpage
 * byteman is a high-throughput bitstream relocation and manipulation tool.
 * 
 * 
 * A good starting point for exploring the code is byteman.cpp where 
 * the main() function parses the command line arguments.
 * 
 *****************************************************************************/

/**************************************************************************//**
 * Main function for byteman.
 * Parses command line arguments and eventual streamed script files as individual
 * commands (instructions) and feeds them into parseCommand(). Catches any thrown
 * exceptions by the subsystems in byteman.
 *
 * @arg @c argc @c argv[] Command line arguments
 *****************************************************************************/
int main(int argc, char * argv[]){
	string command;
	if(argc <= 1) 
		parseHelp("",1,1);//show usage and terminate with an error code
	try {
		bool readSTDIN = false;
		for(int argi = 1 ; argi < argc ; argi++) {
			if(argv[argi][0] == '-' && !command.empty()){ //new cmd and old cmd not empty
				parseCommand(command);
				command.clear();
			}
			if(command.empty())
				command.append(argv[argi]);
			else 
				command.append(" ").append(argv[argi]);
			if(command == "-stdin"){
				readSTDIN = true;
				command.clear();
			}
		}
		if(!command.empty())
			parseCommand(command);
		while(readSTDIN){
			getline(cin,command);
			if(command.at(command.find_first_not_of(" \t")) != '#'){//parse the command only if it doesnt start with '#'
				if(string::npos != command.find("#"))
					command = command.substr(0, command.find("#"));//if there is a comment in the current line, remove it
				parseCommand(command);
			}
		}
	} catch (const exception &e){
        cout << "The program was terminated with message: \n\t'" << e.what() << "'\nWhile trying to execute command: \n\t'" << command << "'" << endl;
		return 1;
	}
    return 0;
}