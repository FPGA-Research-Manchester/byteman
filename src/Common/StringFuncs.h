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

using namespace std;

namespace StringFuncs{
	namespace parse {
		inline string nthStringWord(string s, int n)	///< Parses a string @c s, returns the n-th string word that is not an integer
		{
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
		inline string allStringWords(string s)	///< Parses a string @c s, removes all integers and returns the rest
		{
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
		inline string lastStringWord(string s)	///< Parses a string @c s, removes all integers and returns the last of all string words
		{
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
		inline string allStringWordsWithoutLastStringWord(string s)	///< Parses a string @c s, removes all integers and the last string word. Returns the rest
		{
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
		inline bool nthInteger(string s, int n, int &x)	///< Parses a string @c s, returns the n-th integer
		{
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
		inline bool arrayOfUints(string s, int arrsize, uint32_t* arr)	///< Removes all string words from a given string @c s and returns the parsed @c arrsize number of integers into @c arr
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
		inline bool stringEndsWith(string checkedString, string searchString)	///< Returns true if string @c checkedString's final characters match fully string @c searchString
		{
			if(checkedString.length() >= searchString.length())
				if(0 == checkedString.compare (checkedString.length() - searchString.length(), searchString.length(), searchString))
					return true;
			return false;
		}
	}
	inline string removeExternalQuotes(string s)	///< Removes double quotes from start and end of string @c s and returns the resulting string.
	{
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
	template<typename First, typename ... Rest> inline bool parseMultipleIntsRecursive(stringstream & ss, int & x, Rest & ... args)	///< Parses stringstream @c ss for integer values, that are returned into @c x, @c args
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
	template<typename ... Args> inline bool parseMultipleInts(string s, Args & ... args)	///< Parses string @c s for integer values, that are returned into @c args
	{
		stringstream ss(s);
		return parseMultipleIntsRecursive<int>(ss, args...);
	}
}
#endif //STRINGFUNCS_H
