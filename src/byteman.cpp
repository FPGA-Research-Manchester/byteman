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
#include <string.h>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include "byteman.h"
using namespace std;

#include "Devices/Xilinx/UltraScalePlus/XilinxUltraScalePlus.h"

Arch selectedArch = ARCH_NULL;
#ifdef XUSP
XilinxUltraScalePlus mainXUSP, tempXUSP;
#endif //XUSP
void printDeviceList(){
	#ifdef XUSP
	if(selectedArch == ARCH_NULL || selectedArch == XIL_USP)
		mainXUSP.printDeviceList();
	#endif
}
string getNthStringArg(string s, int n){
	stringstream ss(s);
	string ret;
	ret.clear();
	string temp;
    int intFound;
	int i = 0;
	ss >> temp;
    while (!ss.eof()) {
        ss >> temp;
        if (!(stringstream(temp) >> intFound)){
            if(i == n)
				return temp;
			i++;
		}
        temp.clear();
    }
	return string("");
}
string getLastStringArg(string s){
	stringstream ss(s);
	string ret;
	ret.clear();
	string temp;
    int intFound;
	ss >> temp;
    while (!ss.eof()) {
        ss >> temp;
        if (!(stringstream(temp) >> intFound)){
            ret = temp;
		}
        temp.clear();
    }
	return ret;
}
string getAllButLastStringArg(string s){
	stringstream ss(s);
	string ret;
	ret.clear();
	string temp;
	string temp2;
    int intFound;
	ss >> temp;
    while (!ss.eof()) {
        ss >> temp;
        if (!(stringstream(temp) >> intFound)){
            ret.append(temp2).append(" ");
			temp2 = temp;
		}
    }
	return ret;
}
string getAllStrings(string s){ // returns a string of all string components that are not ints
	stringstream ss(s);
	string ret;
	ret.clear();
	string temp;
    int intFound;
	ss >> temp;
    while (!ss.eof()) {
        ss >> temp;
        if (!(stringstream(temp) >> intFound))
            ret.append(temp).append(" ");
        temp.clear();
    }
	return ret;
}
int getNthInt(string s, int n){
	stringstream ss(s);
	string temp;
    int intFound;
	int i = 0;
    while (!ss.eof()) {
        ss >> temp;
        if (stringstream(temp) >> intFound){
			if(i == n)
				return intFound;
			i++;
		}
        temp.clear();
    }
	return -0x7FFFFFFF;
}
#define paramHas(x) (params.find(x) != string::npos)
void parseVerbose(string verboseCmd){
	int verboseValue = getNthInt(verboseCmd, 0);
	if(verboseValue == 0)verboseValue = 0; // if 0 was given, then it's 0
	else verboseValue = 1; // if nothing was given or something else then 1
	cout<< "verboseValue now is "<< verboseValue << endl; 
	#ifdef XUSP
	cout<<"verboseValue" << verboseValue << endl;
	if(selectedArch == XIL_USP){
		mainXUSP.verbose = verboseValue;
		tempXUSP.verbose = verboseValue;
	}
	#endif
}
void parseWarn(string warnCmd){
	int warnValue = getNthInt(warnCmd, 0);
	if(warnValue == 0)warnValue = 0; // if 0 was given, then it's 0
	else warnValue = 1; // if nothing was given or something else then 1
	cout<< "warnValue is now "<< warnValue << endl; 
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.warn = warnValue;
		tempXUSP.warn = warnValue;
	}
	#endif
}
void parseRegion(string regionCmd){
	string params = getAllStrings(regionCmd);
	int srcRow, srcCol, sizeRow, sizeCol;
	srcRow = getNthInt(regionCmd, 0);
	srcCol = getNthInt(regionCmd, 1);
	sizeRow = getNthInt(regionCmd, 2);
	sizeCol = getNthInt(regionCmd, 3);
	#ifdef XUSP
	if(selectedArch == XIL_USP){
		mainXUSP.region(params, srcRow, srcCol, sizeRow, sizeCol);
		tempXUSP.region(params, srcRow, srcCol, sizeRow, sizeCol);
	}
	#endif
}
void parseBlank(string blankCmd){
	string params = getAllStrings(blankCmd);
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
	string params = getAllButLastStringArg(deviceCmd);
	string deviceName = getLastStringArg(deviceCmd);
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
	string params = getAllButLastStringArg(inputCmd);
	string filename = getLastStringArg(inputCmd);
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
	string params = getAllStrings(mergeCmd);
	int srcRow, srcCol, sizeRow, sizeCol, dstRow, dstCol;
	srcRow = getNthInt(mergeCmd, 0);
	srcCol = getNthInt(mergeCmd, 1);
	sizeRow = getNthInt(mergeCmd, 2);
	sizeCol = getNthInt(mergeCmd, 3);
	dstRow = getNthInt(mergeCmd, 4);
	dstCol = getNthInt(mergeCmd, 5);
	#ifdef XUSP
	if(selectedArch == XIL_USP)
		mainXUSP.merge(&tempXUSP, params, srcRow, srcCol, sizeRow, sizeCol, dstRow, dstCol);
	#endif
}
void parseOutput(string outputCmd){
	string params = getAllButLastStringArg(outputCmd);
	string filename = getLastStringArg(outputCmd);
	int srcRow, srcCol, sizeRow, sizeCol;
	srcRow = getNthInt(outputCmd, 0);
	srcCol = getNthInt(outputCmd, 1);
	sizeRow = getNthInt(outputCmd, 2);
	sizeCol = getNthInt(outputCmd, 3);
	#ifdef XUSP
	if(selectedArch == XIL_USP)
		mainXUSP.writeBitstream(filename, params, srcRow, srcCol, sizeRow, sizeCol);
	#endif
}
void parseAssembly(string assemblyCmd){
	//TODO , text assembly -> bitstream and vice versa
	#ifdef XUSP
	;
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
	#define cmdIs(x,y) (nextCmd.rfind(x, 0) == 0)
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
int main(int argc, char * argv[]){
	if(argc <= 1) 
		parseHelp("",1,1);//show usage and terminate with an error code
	try {
		string command;
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
			if(command.at(command.find_first_not_of(" \t")) != '#')// if #, skip that line as comment
				parseCommand(command);
		}
	} catch (const exception &e){
        cout << "The program was terminated with message: \n\t'" << e.what() << "'\n";
		return 1;
	}
    return 0;
}