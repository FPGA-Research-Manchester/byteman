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
#include<iostream>
#include<stdexcept>
#include<algorithm> //replace
#include<fstream>
#include<string>
#include "byteman.h"
#include "Common/StringFuncs.h"
#include "Common/Coords.h"
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
	if(!StringFuncs::parseMultipleInts(verboseCmd, verboseValue))verboseValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.verbose = verboseValue;
		tempXUSP.verbose = verboseValue;
	}
	#endif
}
void parseWarn(string warnCmd){
	int warnValue;
	if(!StringFuncs::parseMultipleInts(warnCmd, warnValue))warnValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.warn = warnValue;
		tempXUSP.warn = warnValue;
	}
	#endif
}
void parseRegion(string regionCmd){
	string params = StringFuncs::parse::allStringWords(regionCmd);
	Rect2D rect;
	StringFuncs::parseMultipleInts(regionCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.region(params, rect);
		tempXUSP.region(params, rect);
	}
	#endif
}
void parseBlank(string blankCmd){
	string params = StringFuncs::parse::allStringWords(blankCmd);
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
	string params = StringFuncs::parse::allStringWordsWithoutLastStringWord(deviceCmd);
	string deviceName = StringFuncs::parse::lastStringWord(deviceCmd);
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
	string params = StringFuncs::parse::allStringWordsWithoutLastStringWord(inputCmd);
	string filename = StringFuncs::parse::lastStringWord(inputCmd);
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
	string params = StringFuncs::parse::allStringWords(mergeCmd);
	Rect2D rect;
	Coord2D dst;
	StringFuncs::parseMultipleInts(mergeCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col, dst.row, dst.col);
	#ifdef XUSP
	if(selectedArch == XIL_USP)
		mainXUSP.merge(&tempXUSP, params, rect, dst);
	#endif
}
void parseOutput(string outputCmd){
	string params = StringFuncs::parse::allStringWordsWithoutLastStringWord(outputCmd);
	string filename = StringFuncs::parse::lastStringWord(outputCmd);
	Rect2D rect;
	StringFuncs::parseMultipleInts(outputCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XUSP
	if(selectedArch == XIL_USP)
		mainXUSP.writeBitstream(filename, params, rect);
	#endif
}
void parseAssembly(string assemblyCmd){
	string filenameIn = StringFuncs::parse::nthStringWord(assemblyCmd, 0);
	string filenameOut = StringFuncs::parse::nthStringWord(assemblyCmd, 1);
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
	replace(nextCmd.begin(), nextCmd.end(), ':', ' ');
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