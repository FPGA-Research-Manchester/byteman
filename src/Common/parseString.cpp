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
#include "parseString.h"

namespace parseString{
	string getNthWord(string s, int n){
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
	string getLastWord(string s){
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
	string getAllButLastWord(string s){
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
	string getAllWords(string s){ // returns a string of all string components that are not ints
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
	bool getNthInt(string s, int n, int &x){
		stringstream ss(s);
		string temp;
		int intFound;
		int i = 0;
		while (!ss.eof()) {
			ss >> temp;
			try{
				intFound = stoi(temp, nullptr, 0);
				if(i == n) {
					x = intFound;
					return true;
				}
				i++;
			} catch (const exception &e) {
				e;
			}
			temp.clear();
		}
		return false;
	}

	bool getArrayOfUints(string s, int arrsize, uint32_t* arr)
	{
		if (arrsize == 0)
			return true;
		stringstream ss(s);
		string temp;
		uint32_t intFound;
		bool intWasFound = false;
		for (int i = 0 ; i < arrsize ; i++)
		{
			intWasFound = false;
			while ((!ss.eof()) && (!intWasFound)) {
				ss >> temp;
				try{
					intFound = (uint32_t)(stoll(temp, nullptr, 0));
					arr[i] = intFound;
					intWasFound = true;
				} catch (const exception &e) {
					e;
				}
				temp.clear();
			}
			if(ss.eof() && (!intWasFound))
				return false;
		}
		return true;   //success
	}
	string removeQuotes(string s){
		if(s.empty())
			return s;
		if(s.front() == '"')
			s.erase(s.begin());
		if(s.empty())
			return s;
		if(s.back() == '"')
			s.erase(s.end()-1);
		return s;
	}
}