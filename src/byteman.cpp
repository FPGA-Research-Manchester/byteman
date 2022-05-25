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
#include<fstream>

#include "byteman.h"
#include "Common/str.h"
#include "Common/Coords.h"

using namespace std;

/**************************************************************************//**
 * Main function for byteman.
 * Parses command line arguments and eventual streamed script files as individual
 * commands (instructions) and feeds them into parseCommand(). Catches any thrown
 * exceptions by the subsystems in byteman.
 *
 * @arg @c argc @c argv[] Command line arguments
 *****************************************************************************/
int main(int argc, char * argv[])
{
	byteman bytemanInstance;
	string command;
	if(argc <= 1) 
		bytemanInstance.help("",1);//show usage and terminate with an error code
	try {
		bool readSTDIN = false;
		for(int argi = 1 ; argi < argc ; argi++) {
			if(argv[argi][0] == '-' && !command.empty()){ //new cmd and old cmd not empty
				bytemanInstance.parseCommand(command);
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
			bytemanInstance.parseCommand(command);
		while(readSTDIN){
			getline(cin,command);
			if(command.at(command.find_first_not_of(" \t")) != '#'){//parse the command only if it doesnt start with '#'
				if(string::npos != command.find("#"))
					command = command.substr(0, command.find("#"));//if there is a comment in the current line, remove it
				bytemanInstance.parseCommand(command);
			}
		}
	} catch (const exception &e){
        cout << "The program was terminated with message: \n\t'" << e.what() << "'\nWhile trying to execute command: \n\t'" << command << "'" << endl;
		return 1;
	}
    return 0;
}

byteman::byteman()
{
	mainXUSP.instanceName = "Main Xil US+";
	tempXUSP.instanceName = "Temp Xil US+";
}

void byteman::parseCommand(string command)
{
	command = str::replace(command, '=', ' ');
	command = str::replace(command, ':', ' ');
	if (command.at(0) == '-') command.erase(0, 1);
	string params = str::findStringAndGetAllAfter(command, " ");
	//cout <<"params is \""<<params<<"\""<<endl;
	SelectedOptions options = parseParams(params);
	
	if(str::iff::firstStringWordIs(command, "h", "help"))//check if help first
		byteman::help(params, 0);
	else if(Architecture::Unknown == selectedArchitecture) //then check if architecture
		byteman::setArchitecture(command);
	else if(str::iff::firstStringWordIs(command, "v", "verbose")) //check the rest of commands
		byteman::parseVerbose(params, options);
	else if(str::iff::firstStringWordIs(command, "w", "warn"))
		byteman::parseWarn(params, options);
	else if(str::iff::firstStringWordIs(command, "r", "region"))
		byteman::parseRegion(params, options);
	else if(str::iff::firstStringWordIs(command, "b", "blank"))
		byteman::parseBlank(params, options);
	else if(str::iff::firstStringWordIs(command, "c", "change"))
		byteman::parseChange(params, options);
	else if(str::iff::firstStringWordIs(command, "d", "device"))
		byteman::parseDevice(params, options);
	else if(str::iff::firstStringWordIs(command, "i", "input"))
		byteman::parseInput(params, options);
	else if(str::iff::firstStringWordIs(command, "m", "merge"))
		byteman::parseMerge(params, options);
	else if(str::iff::firstStringWordIs(command, "o", "output"))
		byteman::parseOutput(params, options);
	else if(str::iff::firstStringWordIs(command, "a", "assembly"))
		byteman::parseAssembly(params, options);
	else if(str::iff::firstStringWordIs(command, "e", "end"))
		exit(0);
	else
		throw runtime_error("Could not parse command. Consider checking out \"bytemap -help\".");
}

/**************************************************************************//**
 * Parses argument options for byteman
 *
 * @arg @c params A list of parameters to control behavior:
 * - "main" or "first" : Selects the main bitstream buffer as target for the operation
 * - "temp" or "second" : Selects the temp bitstream buffer as target for the operation
 *
 *****************************************************************************/
byteman::SelectedOptions byteman::parseParams(string params)
{
	SelectedOptions options = SelectedOptions();
	params = str::replace(params, ',', ' ');
	stringstream ss(params);
	string param;
	while (!ss.eof()) {
		ss >> param;
		if("main" == param || "first"  == param)options.mainBufferSelected++;
		if("temp" == param || "second" == param)options.tempBufferSelected++;
		param.clear();
	}
	if(0 == options.mainBufferSelected && 0 == options.tempBufferSelected) // by default, choose the main buffer
		options.mainBufferSelected = 1;
	return options;
}

void byteman::parseVerbose(string verboseCmd, SelectedOptions options)
{
	int verboseValue;
	if(!str::parse::multipleInts(verboseCmd, verboseValue))verboseValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			mainXUSP.enableLog = verboseValue;
			tempXUSP.enableLog = verboseValue;
		}
	#endif
}
void byteman::parseWarn(string warnCmd, SelectedOptions options)
{
	int warnValue;
	if(!str::parse::multipleInts(warnCmd, warnValue))warnValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			mainXUSP.enableWarn = warnValue;
			tempXUSP.enableWarn = warnValue;
		}
	#endif
}
void byteman::parseRegion(string regionCmd, SelectedOptions options)
{
	string params = str::parse::allStringWords(regionCmd);
	Rect2D rect;
	str::parse::multipleInts(regionCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.region(params, rect);
			if(options.tempBufferSelected)
				tempXUSP.region(params, rect);
		}
	#endif
}
void byteman::parseBlank(string blankCmd, SelectedOptions options)
{
	string params = str::parse::allStringWords(blankCmd);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.blank(params);
			if(options.tempBufferSelected)
				tempXUSP.blank(params);
		}
	#endif
}
void byteman::parseChange(string changeCmd, SelectedOptions options)
{
	//TODO lut,ff,wires,bram,uram modifications
	#ifdef XUSP
		;
	#endif
}
void byteman::parseDevice(string deviceCmd, SelectedOptions options)
{
	string params = str::parse::allStringWordsWithoutLastStringWord(deviceCmd);
	string deviceName = str::parse::lastStringWord(deviceCmd);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.setDevice(mainXUSP.getDeviceByNameOrThrow(deviceName), deviceName);
			if(options.tempBufferSelected)
				tempXUSP.setDevice(mainXUSP.getDeviceByNameOrThrow(deviceName), deviceName);
		}
	#endif
}
void byteman::parseInput(string inputCmd, SelectedOptions options)
{
	string params = str::parse::allStringWordsWithoutLastStringWord(inputCmd);
	string filename = str::parse::lastStringWord(inputCmd);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.readBitstream(filename);
			if(options.tempBufferSelected)
				tempXUSP.readBitstream(filename);
		}
	#endif
}
void byteman::parseMerge(string mergeCmd, SelectedOptions options)
{
	string params = str::parse::allStringWords(mergeCmd);
	Rect2D rect;
	Coord2D dst;
	str::parse::multipleInts(mergeCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col, dst.row, dst.col);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.merge(&tempXUSP, params, rect, dst);
			if(options.tempBufferSelected)
				tempXUSP.merge(&mainXUSP, params, rect, dst);
		}
	#endif
}
void byteman::parseOutput(string outputCmd, SelectedOptions options)
{
	string params = str::parse::allStringWordsWithoutLastStringWord(outputCmd);
	string filename = str::parse::lastStringWord(outputCmd);
	Rect2D rect;
	str::parse::multipleInts(outputCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.writeBitstream(filename, params, rect);
			if(options.tempBufferSelected)
				tempXUSP.writeBitstream(filename, params, rect);
		}
	#endif
}
void byteman::parseAssembly(string assemblyCmd, SelectedOptions options)
{
	string filenameIn = str::parse::nthStringWord(assemblyCmd, 0);
	string filenameOut = str::parse::nthStringWord(assemblyCmd, 1);
	#ifdef XUSP
		mainXUSP.assembler(filenameIn, filenameOut);
	#endif
}

void byteman::setArchitecture(string arch)
{
	arch = str::stringToLower(arch);
	#ifdef XUSP
		if(str::iff::stringContains(arch, "xusp")
		||	(	str::iff::stringContains(arch, "xilinx")
			&&	str::iff::stringContains(arch, "ultrascale", "us")
			&&	str::iff::stringContains(arch, "plus","+")
			)
		) {
			selectedArchitecture = Architecture::Xilinx_UltraScalePlus;
			return;
		}
	#endif
	
	throw runtime_error(string("Unknown architecture: \"").append(arch).append("\"."));
}
