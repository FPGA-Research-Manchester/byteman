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
 * commands (instructions) and feeds them into parse(). Catches any thrown
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
				bytemanInstance.parse(command);
				command.clear();
			}
			if(command.empty())
				command.append(argv[argi]);
			else 
				command.append(" ").append(argv[argi]);
			if(command == "-stdin" || command == "-s"){
				readSTDIN = true;
				command.clear();
			}
		}
		if(!command.empty())
			bytemanInstance.parse(command);
		while(readSTDIN){
			getline(cin,command);
			if(command.at(command.find_first_not_of(" \t")) != '#'){//parse the command only if it doesnt start with '#'
				if(string::npos != command.find("#"))
					command = command.substr(0, command.find("#"));//if there is a comment in the current line, remove it
				bytemanInstance.parse(command);
			}
		}
	} catch (const exception &e){
		cout << "The program was terminated with message: \n\t'" << e.what() << "'\nWhile trying to execute command: \n\t'" << command << "'" << endl;
		return 1;
	}
	return 0;
}

byteman::byteman(){
	#ifdef XS7
		mainXS7.instanceName = "Main Xil S7";
		tempXS7.instanceName = "Temp Xil S7";
	#endif //XS7
	
	#ifdef XUS
		mainXUS.instanceName = "Main Xil US";
		tempXUS.instanceName = "Temp Xil US";
	#endif //XUS
	
	#ifdef XUSP
		mainXUSP.instanceName = "Main Xil US+";
		tempXUSP.instanceName = "Temp Xil US+";
	#endif //XUSP
}
byteman::~byteman(){
	
}

void byteman::parse(string command)
{
	command = str::replace(command, '=', ' ');
	command = str::replace(command, ':', ' ');
	if (command.at(0) == '-') command.erase(0, 1);
	string params = str::findStringAndGetAllAfter(command, " ");
	SelectedOptions options = parseParams(params);
	
	if(str::iff::firstStringWordIs(command, "h", "help"))//check if help first
		help(params, 0);
	else if(Architecture::Unknown == selectedArchitecture) //then check if architecture
		setArchitecture(command);
	else if(str::iff::firstStringWordIs(command, "v", "verbose")) //check the rest of commands
		parseVerbose(params, options);
	else if(str::iff::firstStringWordIs(command, "w", "warn"))
		parseWarn(params, options);
	else if(str::iff::firstStringWordIs(command, "r", "region"))
		parseRegion(params, options);
	else if(str::iff::firstStringWordIs(command, "b", "blank"))
		parseBlank(params, options);
	else if(str::iff::firstStringWordIs(command, "c", "change"))
		parseChange(params, options);
	else if(str::iff::firstStringWordIs(command, "d", "device"))
		parseDevice(params, options);
	else if(str::iff::firstStringWordIs(command, "i", "input"))
		parseInput(params, options);
	else if(str::iff::firstStringWordIs(command, "m", "merge"))
		parseMerge(params, options);
	else if(str::iff::firstStringWordIs(command, "o", "output"))
		parseOutput(params, options);
	else if(str::iff::firstStringWordIs(command, "a", "assembly"))
		parseAssembly(params, options);
	#if !defined(NDEBUG)
	else if(str::iff::firstStringWordIs(command, "t", "test"))
		parseTest(params, options);
	#endif
	else if(str::iff::firstStringWordIs(command, "e", "exit"))
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
	
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			mainXS7.enableLog = verboseValue;
			tempXS7.enableLog = verboseValue;
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			mainXUS.enableLog = verboseValue;
			tempXUS.enableLog = verboseValue;
		}
	#endif
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
	
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			mainXS7.enableWarn = warnValue;
			tempXS7.enableWarn = warnValue;
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			mainXUS.enableWarn = warnValue;
			tempXUS.enableWarn = warnValue;
		}
	#endif
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
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			mainXS7.region(params, rect);
			tempXS7.region(params, rect);
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			mainXUS.region(params, rect);
			tempXUS.region(params, rect);
		}
	#endif
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			mainXUSP.region(params, rect);
			tempXUSP.region(params, rect);
		}
	#endif
}
void byteman::parseBlank(string blankCmd, SelectedOptions options)
{
	string params = blankCmd;
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXS7.blank(params);
			if(options.tempBufferSelected)
				tempXS7.blank(params);
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUS.blank(params);
			if(options.tempBufferSelected)
				tempXUS.blank(params);
		}
	#endif
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
	#ifdef XS7
		;
	#endif
	#ifdef XUS
		;
	#endif
	#ifdef XUSP
		;
	#endif
}
void byteman::parseDevice(string deviceCmd, SelectedOptions options)
{
	string deviceName = str::parse::lastStringWord(deviceCmd);
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXS7.setDeviceByNameOrThrow(deviceName);
			if(options.tempBufferSelected)
				tempXS7.setDeviceByNameOrThrow(deviceName);
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUS.setDeviceByNameOrThrow(deviceName);
			if(options.tempBufferSelected)
				tempXUS.setDeviceByNameOrThrow(deviceName);
		}
	#endif
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUSP.setDeviceByNameOrThrow(deviceName);
			if(options.tempBufferSelected)
				tempXUSP.setDeviceByNameOrThrow(deviceName);
		}
	#endif
}
void byteman::parseInput(string inputCmd, SelectedOptions options)
{
	string params = str::parse::allStringWordsWithoutLastStringWord(inputCmd);
	string filename = str::parse::lastStringWord(inputCmd);
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXS7.readBitstream(filename);
			if(options.tempBufferSelected)
				tempXS7.readBitstream(filename);
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			if(options.mainBufferSelected)
				mainXUS.readBitstream(filename);
			if(options.tempBufferSelected)
				tempXUS.readBitstream(filename);
		}
	#endif
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
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			mainXS7.merge(&tempXS7, params, rect, dst);
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			mainXUS.merge(&tempXUS, params, rect, dst);
		}
	#endif
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			mainXUSP.merge(&tempXUSP, params, rect, dst);
		}
	#endif
}
void byteman::parseOutput(string outputCmd, SelectedOptions options)
{
	string params = str::parse::allStringWordsWithoutLastStringWord(outputCmd);
	string filename = str::parse::lastStringWord(outputCmd);
	Rect2D rect;
	str::parse::multipleInts(outputCmd, rect.position.row, rect.position.col, rect.size.row, rect.size.col);
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture){
			mainXS7.writeBitstream(filename, params, rect);
		}
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture){
			mainXUS.writeBitstream(filename, params, rect);
		}
	#endif
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture){
			mainXUSP.writeBitstream(filename, params, rect);
		}
	#endif
}
void byteman::parseAssembly(string assemblyCmd, SelectedOptions options)
{
	string filenameIn = str::parse::nthStringWord(assemblyCmd, 0);
	string filenameOut = str::parse::nthStringWord(assemblyCmd, 1);
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture)
			mainXS7.assembler(filenameIn, filenameOut);
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture)
			mainXUS.assembler(filenameIn, filenameOut);
	#endif
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture)
			mainXUSP.assembler(filenameIn, filenameOut);
	#endif
}
#if !defined(NDEBUG)
void byteman::parseTest(string testCmd, SelectedOptions options)
{
	uint32_t testValue;
	if(!str::parse::multipleUints(testCmd, testValue))
		testValue = 0;
	
	testCmd = str::stringToLower(testCmd);
	bool usableFramesOnly = false;
	if(str::iff::stringContains(testCmd, "usableframes", "mappedframes"))
		usableFramesOnly = true;
	
	bool testValueEqual = true;
	if(str::iff::stringContains(testCmd, "notequal"))
		testValueEqual = false;
	#ifdef XS7
		if(Architecture::Xilinx_Series7 == selectedArchitecture)
			mainXS7.test(usableFramesOnly, testValueEqual, testValue);
	#endif
	#ifdef XUS
		if(Architecture::Xilinx_UltraScale == selectedArchitecture)
			mainXUS.test(usableFramesOnly, testValueEqual, testValue);
	#endif
	#ifdef XUSP
		if(Architecture::Xilinx_UltraScalePlus == selectedArchitecture)
			mainXUSP.test(usableFramesOnly, testValueEqual, testValue);
	#endif
}
#endif

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
	#endif//XUSP
	#ifdef XUS
		if(str::iff::stringContains(arch, "xus")
		||	(	str::iff::stringContains(arch, "xilinx")
			&&	str::iff::stringContains(arch, "ultrascale", "us")
			)
		) {
			selectedArchitecture = Architecture::Xilinx_UltraScale;
			return;
		}
	#endif//XUS
	#ifdef XS7
		if(str::iff::stringContains(arch, "xs7")
		||	(	str::iff::stringContains(arch, "xilinx")
			&&	str::iff::stringContains(arch, "series")
			&&	str::iff::stringContains(arch, "7")
			)
		) {
			selectedArchitecture = Architecture::Xilinx_Series7;
			return;
		}
	#endif//XS7
	throw runtime_error(string("Unknown architecture: \"").append(arch).append("\"."));
}
