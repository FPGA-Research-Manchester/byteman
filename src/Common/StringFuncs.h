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
#include<algorithm> //replace
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
			
		// fix for error LNK2019: unresolved external symbol, keep this function in the header
		template<typename ... Rest> inline bool multipleInts(stringstream & ss)
		{
			return true;
		}
		template<typename ... Rest> inline bool multipleInts(stringstream & ss, int & x, Rest & ... args)	///< Parses stringstream @c ss for integer values, that are returned into @c x, @c args
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
				return multipleInts(ss, args...);
		}
		template<typename ... Args> inline bool multipleInts(string s, Args & ... args)	///< Parses string @c s for integer values, that are returned into @c args
		{
			stringstream ss(s);
			return multipleInts(ss, args...);
		}
	}
	namespace checkIf {
		template<typename ... Rest> inline bool stringEndsWith(string checkedString)	///< Returns false. End of recursion for template
		{
			return false;
		}
		template<typename ... Rest> inline bool stringEndsWith(string checkedString, string nextString, Rest ... restStrings)	///< Returns true if string @c checkedString's final characters match fully any of strings @c nextString or @c restStrings
		{
			if(checkedString.length() >= nextString.length())
				if(0 == checkedString.compare (checkedString.length() - nextString.length(), nextString.length(), nextString))
					return true;
			return stringEndsWith(checkedString, restStrings...);
		}
		template<typename ... Rest> inline bool stringIs(string checkedString)	///< Returns false. End of recursion for template
		{
			return false;
		}
		template<typename ... Rest> inline bool stringIs(string checkedString, string nextString, Rest ... restStrings)	///< Returns true if string @c checkedString matches fully any of strings @c nextString or @c restStrings
		{
			if(checkedString == nextString)
				return true;
			return stringEndsWith(checkedString, restStrings...);
		}
		template<typename ... Rest> inline bool stringContains(string checkedString)	///< Returns false. End of recursion for template
		{
			return false;
		}
		template<typename ... Rest> inline bool stringContains(string checkedString, string nextString, Rest ... restStrings)	///< Returns true if string @c checkedString contains any of strings @c nextString or @c restStrings
		{
			if(string::npos != checkedString.rfind(nextString))
				return true;
			return stringContains(checkedString, restStrings...);
		}
		template<typename ... Rest> inline bool stringBeginsWith(string checkedString)	///< Returns false. End of recursion for template
		{
			return false;
		}
		template<typename ... Rest> inline bool stringBeginsWith(string checkedString, string nextString, Rest ... restStrings)	///< Returns true if string @c checkedString's first characters match fully any of strings @c nextString or @c restStrings
		{
			if(0 == checkedString.rfind(nextString, 0))
				return true;
			return stringBeginsWith(checkedString, restStrings...);
		}
		template<typename ... Rest> inline bool stringWordIs(string checkedString)	///< Returns false. End of recursion for template
		{
			return false;
		}
		template<typename ... Rest> inline bool stringWordIs(string checkedString, string nextString, Rest ... restStrings)	///< Returns true if string @c checkedString matches fully any of strings @c nextString or @c restStrings
		{
			if(checkedString == nextString)
				return true;
			return stringBeginsWith(checkedString, restStrings...);
		}
		template<typename ... Rest> inline bool firstStringWordIs(string checkedString, string nextString, Rest ... restStrings)	///< Returns true if string @c checkedString's first word matches fully any of strings @c nextString or @c restStrings
		{
			stringstream ss(checkedString);
			string firstWord;
			ss >> firstWord;
			return stringWordIs(firstWord, nextString, restStrings...);
		}
	}
	inline string findStringAndGetAllAfter(string checkedString, string searchString)///< Finds string @c searchString inside @c checkedString and returns all to the right inside @c checkedString. If can't be found, returns empty string.
	{
		size_t loc = checkedString.find(searchString);
		if(string::npos == loc)
			return string("");
		return checkedString.substr(loc + searchString.size());
	}
	inline string replace(string str, char oldChar, char newChar)	///< Replaces all instances of @c oldChar in string @c str with @c newChar and returns the resulting string.
	{
		replace(str.begin(), str.end(), '=', ' ');
		return str;
	}
	inline string removeSpaces(string str)	///< Removes all space chars of @c str returns the resulting string.
	{
		str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
		return str;
	}
	inline string stringToUpper(string str)	///< Replaces all lowercase characters in @c str with uppercase and returns the resulting string.
	{
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		return str;
	}
	inline string stringToLower(string str)	///< Replaces all uppercase characters in @c str with lowercase and returns the resulting string.
	{
		transform(str.begin(), str.end(),str.begin(), ::tolower);
		return str;
	}
	inline string removeExternalQuotes(string str)	///< Removes double quotes from start and end of string @c str and returns the resulting string.
	{
		if(str.empty())
			return str;
		if(str.front() == '"')
			str.erase(str.begin());
		if(str.empty())
			return str;
		if(str.back() == '"')
			str.erase(str.end()-1);
		return str;
	}
}
#endif //STRINGFUNCS_H
