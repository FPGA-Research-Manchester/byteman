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

#ifndef STRINGFUNCS_H
#define STRINGFUNCS_H

#include<iostream>
#include<cstdint> //uint
#include<string>
#include<sstream>
#include<stdexcept> //TODO is it needed?

using namespace std;
namespace StringFuncs{
	namespace parse {
		inline string nthStringWord(string s, int n){
			stringstream ss(s);
			string ret;
			ret.clear();
			string temp;
			int intFound;
			int i = 0;
			ss >> temp;
			while (!ss.eof()) {
				ss >> temp;
				try{
					intFound = stoi(temp, nullptr, 0);
				} catch (const exception &e) {
					if(i == n)
						return temp;
					i++;
					e;
				}
				temp.clear();
			}
			return string("");
		}
		inline string allStringWords(string s){ // returns a string of all string components that are not ints
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
		inline string lastStringWord(string s){
			stringstream ss(s);
			string ret;
			ret.clear();
			string temp;
			int intFound;
			ss >> temp;
			while (!ss.eof()) {
				ss >> temp;
				try{
					intFound = stoi(temp, nullptr, 0);
				} catch (const exception &e) {
					ret = temp;
					e;
				}
				temp.clear();
			}
			return ret;
		}
		inline string allStringWordsWithoutLastStringWord(string s){
			stringstream ss(s);
			string ret;
			ret.clear();
			string temp;
			string temp2;
			int intFound;
			ss >> temp;
			while (!ss.eof()) {
				ss >> temp;
				try{
					intFound = stoi(temp, nullptr, 0);
				} catch (const exception &e) {
					if(!ret.empty())
						ret.append(" ");
					ret.append(temp2);
					temp2 = temp;
					e;
				}
				temp.clear();
			}
			return ret;
		}
		inline bool nthInteger(string s, int n, int &x){
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
		inline bool arrayOfUints(string s, int arrsize, uint32_t* arr)
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
	}
	namespace checkIf {
		inline bool stringEndsWith(string checkedString, string searchString){
			if(checkedString.length() >= searchString.length())
				if(0 == checkedString.compare (checkedString.length() - searchString.length(), searchString.length(), searchString))
					return true;
			return false;
		}
	}
	inline string removeExternalQuotes(string s){
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
	
	
	// fix for error LNK2019: unresolved external symbol, keep this function in the header
	template<typename First, typename ... Rest> inline bool parseMultipleIntsRecursive(stringstream & ss)
	{
		return true;
	}
	template<typename First, typename ... Rest> inline bool parseMultipleIntsRecursive(stringstream & ss, int & x, Rest & ... args)
	{
		int numArgs = sizeof...(args);
		string temp;
		int intFound;
		bool intWasFound = false;
		while ((!ss.eof()) && (!intWasFound)) {
			ss >> temp;
			try{
				intFound = stoi(temp, nullptr, 0);
				x = intFound;
				intWasFound = true;
			} catch (const exception &e) {
				e;
			}
			temp.clear();
		}
		if(ss.eof() && (!intWasFound))
			return false;
		if (numArgs == 0)
			return true;
		else 
			return parseMultipleIntsRecursive<int>(ss, args...);
	}
	template<typename ... Args> inline bool parseMultipleInts(string s, Args & ... args)
	{
		stringstream ss(s);
		return parseMultipleIntsRecursive<int>(ss, args...);
	}
}
#endif //STRINGFUNCS_H
