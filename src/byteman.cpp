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
#include "Common/StringFuncs.h"
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
	Xilinx_UltraScalePlus.mainBuffer.instanceName = "Main Xilinx US+";
	Xilinx_UltraScalePlus.tempBuffer.instanceName = "Temp Xilinx US+";
}

void byteman::parseCommand(string command)
{
	command = StringFuncs::replace(command, '=', ' ');
	command = StringFuncs::replace(command, ':', ' ');
	if (command.at(0) == '-') command.erase(0, 1);
	string params = StringFuncs::findStringAndGetAllAfter(command, " ");
	//cout <<"params is \""<<params<<"\""<<endl;
	SelectedOptions options = parseParams(params);
	
	if(StringFuncs::checkIf::firstStringWordIs(command, "h", "help"))//check if help first
		byteman::help(params, 0);
	else if(Architecture::Unknown == selectedArchitecture) //then check if architecture
		byteman::setArchitecture(command);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "v", "verbose")) //check the rest of commands
		byteman::parseVerbose(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "w", "warn"))
		byteman::parseWarn(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "r", "region"))
		byteman::parseRegion(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "b", "blank"))
		byteman::parseBlank(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "c", "change"))
		byteman::parseChange(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "d", "device"))
		byteman::parseDevice(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "i", "input"))
		byteman::parseInput(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "m", "merge"))
		byteman::parseMerge(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "o", "output"))
		byteman::parseOutput(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "a", "assembly"))
		byteman::parseAssembly(params, options);
	else if(StringFuncs::checkIf::firstStringWordIs(command, "e", "end"))
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
	params = StringFuncs::replace(params, ',', ' ');
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
	if(!StringFuncs::parse::multipleInts(verboseCmd, verboseValue))verboseValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			Xilinx_UltraScalePlus.mainBuffer.enableLog = verboseValue;
			Xilinx_UltraScalePlus.tempBuffer.enableLog = verboseValue;
		}
	#endif
}
void byteman::parseWarn(string warnCmd, SelectedOptions options)
{
	int warnValue;
	if(!StringFuncs::parse::multipleInts(warnCmd, warnValue))warnValue = 1; // if nothing was given, default to 1
	
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			Xilinx_UltraScalePlus.mainBuffer.enableWarn = warnValue;
			Xilinx_UltraScalePlus.tempBuffer.enableWarn = warnValue;
		}
	#endif
}
void byteman::parseRegion(string regionCmd, SelectedOptions options)
{
	string params = StringFuncs::parse::allStringWords(regionCmd);
	Rect2D rect;
	StringFuncs::parse::multipleInts(regionCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				Xilinx_UltraScalePlus.mainBuffer.region(params, rect);
			if(options.tempBufferSelected)
				Xilinx_UltraScalePlus.tempBuffer.region(params, rect);
		}
	#endif
}
void byteman::parseBlank(string blankCmd, SelectedOptions options)
{
	string params = StringFuncs::parse::allStringWords(blankCmd);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				Xilinx_UltraScalePlus.mainBuffer.blank(params);
			if(options.tempBufferSelected)
				Xilinx_UltraScalePlus.tempBuffer.blank(params);
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
	string params = StringFuncs::parse::allStringWordsWithoutLastStringWord(deviceCmd);
	string deviceName = StringFuncs::parse::lastStringWord(deviceCmd);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				Xilinx_UltraScalePlus.mainBuffer.setDevice(Xilinx_UltraScalePlus.mainBuffer.getDeviceByNameOrThrow(deviceName), deviceName);
			if(options.tempBufferSelected)
				Xilinx_UltraScalePlus.tempBuffer.setDevice(Xilinx_UltraScalePlus.mainBuffer.getDeviceByNameOrThrow(deviceName), deviceName);
		}
	#endif
}
void byteman::parseInput(string inputCmd, SelectedOptions options)
{
	string params = StringFuncs::parse::allStringWordsWithoutLastStringWord(inputCmd);
	string filename = StringFuncs::parse::lastStringWord(inputCmd);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				Xilinx_UltraScalePlus.mainBuffer.readBitstream(filename);
			if(options.tempBufferSelected)
				Xilinx_UltraScalePlus.tempBuffer.readBitstream(filename);
		}
	#endif
}
void byteman::parseMerge(string mergeCmd, SelectedOptions options)
{
	string params = StringFuncs::parse::allStringWords(mergeCmd);
	Rect2D rect;
	Coord2D dst;
	StringFuncs::parse::multipleInts(mergeCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col, dst.row, dst.col);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				Xilinx_UltraScalePlus.mainBuffer.merge(&Xilinx_UltraScalePlus.tempBuffer, params, rect, dst);
			if(options.tempBufferSelected)
				Xilinx_UltraScalePlus.tempBuffer.merge(&Xilinx_UltraScalePlus.mainBuffer, params, rect, dst);
		}
	#endif
}
void byteman::parseOutput(string outputCmd, SelectedOptions options)
{
	string params = StringFuncs::parse::allStringWordsWithoutLastStringWord(outputCmd);
	string filename = StringFuncs::parse::lastStringWord(outputCmd);
	Rect2D rect;
	StringFuncs::parse::multipleInts(outputCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				Xilinx_UltraScalePlus.mainBuffer.writeBitstream(filename, params, rect);
			if(options.tempBufferSelected)
				Xilinx_UltraScalePlus.tempBuffer.writeBitstream(filename, params, rect);
		}
	#endif
}
void byteman::parseAssembly(string assemblyCmd, SelectedOptions options)
{
	string filenameIn = StringFuncs::parse::nthStringWord(assemblyCmd, 0);
	string filenameOut = StringFuncs::parse::nthStringWord(assemblyCmd, 1);
	#ifdef XUSP
		Xilinx_UltraScalePlus.mainBuffer.assembler(filenameIn, filenameOut);
	#endif
}

void byteman::setArchitecture(string arch)
{
	arch = StringFuncs::stringToLower(arch);
	#ifdef XUSP
		if(StringFuncs::checkIf::stringContains(arch, "xusp")
		||	(	StringFuncs::checkIf::stringContains(arch, "xilinx")
			&&	StringFuncs::checkIf::stringContains(arch, "ultrascale", "us")
			&&	StringFuncs::checkIf::stringContains(arch, "plus","+")
			)
		) {
			selectedArchitecture = Architecture::Xilinx_UltraScalePlus;
			return;
		}
	#endif
	
	throw runtime_error(string("Unknown architecture: \"").append(arch).append("\"."));
}
