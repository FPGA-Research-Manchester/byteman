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

#ifndef STR_IFF_H
#define STR_IFF_H

#include<iostream>
#include<string>
#include<sstream>

using namespace std;

/**************************************************************************//**
 * The str::iff:: namespace holds some custom and/or shorter functions for string
 * conditionals (searching/matching).
 * 
 *****************************************************************************/
namespace str{
	namespace iff {
		/// Returns false. End of recursion for template
		template<typename ... Rest> inline bool stringEndsWith(string checkedString)	
		{
			return false;
		}
		/// Returns true if string @c checkedString's final characters match fully any of strings @c nextString or @c restStrings
		template<typename ... Rest> inline bool stringEndsWith(string checkedString, string nextString, Rest ... restStrings)	
		{
			if(checkedString.length() >= nextString.length())
				if(0 == checkedString.compare (checkedString.length() - nextString.length(), nextString.length(), nextString))
					return true;
			return stringEndsWith(checkedString, restStrings...);
		}
		/// Returns false. End of recursion for template
		template<typename ... Rest> inline bool stringIs(string checkedString)	
		{
			return false;
		}
		/// Returns true if string @c checkedString matches fully any of strings @c nextString or @c restStrings
		template<typename ... Rest> inline bool stringIs(string checkedString, string nextString, Rest ... restStrings)	
		{
			if(checkedString == nextString)
				return true;
			return stringIs(checkedString, restStrings...);
		}
		/// Returns false. End of recursion for template
		template<typename ... Rest> inline bool stringContains(string checkedString)	
		{
			return false;
		}
		/// Returns true if string @c checkedString contains any of strings @c nextString or @c restStrings
		template<typename ... Rest> inline bool stringContains(string checkedString, string nextString, Rest ... restStrings)	
		{
			if(string::npos != checkedString.rfind(nextString))
				return true;
			return stringContains(checkedString, restStrings...);
		}
		/// Returns false. End of recursion for template
		template<typename ... Rest> inline bool stringBeginsWith(string checkedString)	
		{
			return false;
		}
		/// Returns true if string @c checkedString's first characters match fully any of strings @c nextString or @c restStrings
		template<typename ... Rest> inline bool stringBeginsWith(string checkedString, string nextString, Rest ... restStrings)	
		{
			if(0 == checkedString.rfind(nextString, 0))
				return true;
			return stringBeginsWith(checkedString, restStrings...);
		}
		/// Returns false. End of recursion for template
		template<typename ... Rest> inline bool stringWordIs(string checkedString)	
		{
			return false;
		}
		/// Returns true if string @c checkedString matches fully any of strings @c nextString or @c restStrings
		template<typename ... Rest> inline bool stringWordIs(string checkedString, string nextString, Rest ... restStrings)	
		{
			if(checkedString == nextString)
				return true;
			return stringBeginsWith(checkedString, restStrings...);
		}
		/// Returns true if string @c checkedString's first word matches fully any of strings @c nextString or @c restStrings
		template<typename ... Rest> inline bool firstStringWordIs(string checkedString, string nextString, Rest ... restStrings)	
		{
			stringstream ss(checkedString);
			string firstWord;
			ss >> firstWord;
			return stringWordIs(firstWord, nextString, restStrings...);
		}
		/// Returns false. End of recursion for template
		template<typename ... Rest> inline bool charIs(char checkedChar)	
		{
			return false;
		}
		/// Returns true if char @c checkedChar matches any of chars @c nextChar or @c restChars
		template<typename ... Rest> inline bool charIs(char checkedChar, char nextChar, Rest ... restChars)	
		{
			if(checkedChar == nextChar)
				return true;
			return charIs(checkedChar, restChars...);
		}
	}
}
#endif //STR_IFF_H
