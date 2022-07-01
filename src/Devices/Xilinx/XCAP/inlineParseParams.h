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

/**************************************************************************//**
 * Parses argument options for multiple operations.
 * @arg @c params A list of parameters to control behavior:
 * - "clock" or "clk" : Selects the clocks for operation.
 * - "logic" or "clb" : Selects the blocktype 0 for operation.
 * - "blockram" or "bram" : Selects the blockram contents for operation.
 * - If neither clock, logic, or blockram are chosen, chooses all.
 * - "blank" : Selects bitstream blanking (zeroing) operation.
 * - "full" : Disabling partial selection.
 * - "partial" : Writes only selected regions. Selected by default.
 * - "set" : Selects SET operation.
 * - "xor" : Selects bitwise XOR operation.
 * - "or" : Selects bitwise OR operation.
 * - "and" : Selects bitwise AND operation.
 *****************************************************************************/
inline void parseParams(string params){
	selectedOptions = SelectedOptions();
	selectedOptions.clk = false;
	selectedOptions.clb = false;
	selectedOptions.bram = false;
	selectedOptions.blank = false;
	selectedOptions.partialNotFull = true;
	selectedOptions.op = MergeOP::SET;
	selectedOptions.skipUnused = true;
	selectedOptions.intParam = 0;
	replace(params.begin(), params.end(), ',', ' ');
	stringstream ss(params);
	string param;
	while (!ss.eof()) {
		ss >> param;
		if(param == "clock" || param == "clk")selectedOptions.clk = true;
		if(param == "logic" || param == "clb")selectedOptions.clb = true;
		if(param == "blockram" || param == "bram")selectedOptions.bram = true;
		if(param == "blank")selectedOptions.blank++;
		if(param == "full")selectedOptions.partialNotFull = false;
		if(param == "partial")selectedOptions.partialNotFull = true;
		if(param == "set")selectedOptions.op = MergeOP::SET;
		if(param == "xor")selectedOptions.op = MergeOP::XOR;
		if(param == "or")selectedOptions.op = MergeOP::OR;
		if(param == "and")selectedOptions.op = MergeOP::AND;
		param.clear();
	}
	if(!str::parse::multipleInts(params, selectedOptions.intParam))
		selectedOptions.intParam = 0; //default 0 if there is nothing
	if(!selectedOptions.clk && !selectedOptions.clb && !selectedOptions.bram){ // by default, choose all
		selectedOptions.clk = selectedOptions.bram = selectedOptions.clb = true;
	}
}

