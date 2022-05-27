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

#ifndef STR_H
#define STR_H

#include<algorithm> //replace
#include<string>

using namespace std;

#include "str/iff.h"
#include "str/parse.h"

/**************************************************************************//**
 * The str:: namespace holds some custom and/or shorter functions including string
 * string parsing and string manipulation.
 * 
 *****************************************************************************/
namespace str{
	/// Finds string @c searchString inside @c checkedString and returns all to the right inside @c checkedString. If can't be found, returns empty string.
	inline string findStringAndGetAllAfter(string checkedString, string searchString)
	{
		size_t loc = checkedString.find(searchString);
		if(string::npos == loc)
			return string("");
		return checkedString.substr(loc + searchString.size());
	}
	/// Replaces all instances of @c oldChar in string @c str with @c newChar and returns the resulting string.
	inline string replace(string str, char oldChar, char newChar)	
	{
		replace(str.begin(), str.end(), '=', ' ');
		return str;
	}
	/// Removes all space chars of @c str returns the resulting string.
	inline string removeSpaces(string str)	
	{
		str.erase(remove_if(str.begin(), str.end(), ::isspace), str.end());
		return str;
	}
	/// Replaces all lowercase characters in @c str with uppercase and returns the resulting string.
	inline string stringToUpper(string str)	
	{
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		return str;
	}
	/// Replaces all uppercase characters in @c str with lowercase and returns the resulting string.
	inline string stringToLower(string str)	
	{
		transform(str.begin(), str.end(),str.begin(), ::tolower);
		return str;
	}
	/// Removes double quotes from start and end of string @c str and returns the resulting string.
	inline string removeExternalQuotes(string str)	
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
#endif //STR_H
