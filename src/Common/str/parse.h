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

#ifndef STR_PARSE_H
#define STR_PARSE_H

#include<cstdint> //uint
#include<string>
#include<iomanip> //quoted
#include<sstream>

/**************************************************************************//**
 * The str::parse:: namespace holds some custom and/or shorter functions for string
 * parsing.
 *
 *****************************************************************************/
namespace str{
	namespace parse {
		/// Parses a string @c s, returns the n-th string word that is not an integer
		inline std::string nthStringWord(std::string s, int n)	
		{
			std::stringstream ss(s);
			std::string temp;
			int intFound, i = 0;
			while (!ss.eof()) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					intFound = stoi(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
				} catch (const std::exception &e) {
					if(i == n)
						return temp;
					i++;
					e;
				}
				temp.clear();
			}
			return std::string("");
		}
		/// Parses a string @c s, removes all integers and returns the rest
		inline std::string allStringWords(std::string s)	
		{
			std::stringstream ss(s);
			std::string temp, ret = "";
			int intFound;
			while (!ss.eof()) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					intFound = stoi(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
				} catch (const std::exception &e) {
					if(!ret.empty())
						ret.append(" ");
					ret.append(temp);
					e;
				}
				temp.clear();
			}
			return ret;
		}
		/// Parses a string @c s, removes all integers and returns the last of all string words
		inline std::string lastStringWord(std::string s)	
		{
			std::stringstream ss(s);
			std::string ret;
			ret.clear();
			std::string temp;
			int intFound;
			while (!ss.eof()) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					intFound = stoi(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
				} catch (const std::exception &e) {
					ret = temp;
					e;
				}
				temp.clear();
			}
			return ret;
		}
		/// Parses a string @c s, removes all integers and the last string word. Returns the rest
		inline std::string allStringWordsWithoutLastStringWord(std::string s)	
		{
			std::stringstream ss(s);
			std::string ret;
			ret.clear();
			std::string temp;
			std::string temp2;
			int intFound;
			while (!ss.eof()) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					intFound = stoi(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
				} catch (const std::exception &e) {
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
		/// Parses a string @c s, returns the n-th integer
		inline bool nthInteger(std::string s, int n, int &x)	
		{
			std::stringstream ss(s);
			std::string temp;
			int intFound;
			int i = 0;
			while (!ss.eof()) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					intFound = stoi(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
					if(i == n) {
						x = intFound;
						return true;
					}
					i++;
				} catch (const std::exception &e) {
					e;
				}
				temp.clear();
			}
			return false;
		}
		/// Removes all string words from a given string @c s and returns the parsed @c arrsize number of integers into @c arr
		inline bool arrayOfUints(std::string s, int arrsize, uint32_t* arr)	
		{
			if (arrsize == 0)
				return true;
			std::stringstream ss(s);
			std::string temp;
			uint32_t intFound;
			bool intWasFound = false;
			for (int i = 0 ; i < arrsize ; i++)
			{
				intWasFound = false;
				while ((!ss.eof()) && (!intWasFound)) {
					ss >> quoted(temp);
					if(temp.empty()) break;
					try{
						size_t sz;
						intFound = (uint32_t)(stoll(temp, &sz, 0));
						if(temp.size() != sz)
							throw std::exception();
						arr[i] = intFound;
						intWasFound = true;
					} catch (const std::exception &e) {
						e;
					}
					temp.clear();
				}
				if(ss.eof() && (!intWasFound))
					return false;
			}
			return true;   //success
		}
		
		template<typename ... Rest> inline bool multipleInts(std::stringstream & ss)
		{
			return true;
		}
		/// Parses stringstream @c ss for integer values, that are returned into @c x, @c args
		template<typename ... Rest> inline bool multipleInts(std::stringstream & ss, int & x, Rest & ... args)	
		{
			int numArgs = sizeof...(args);
			std::string temp;
			int intFound;
			bool intWasFound = false;
			while ((!ss.eof()) && (!intWasFound)) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					intFound = stoi(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
					x = intFound;
					intWasFound = true;
				} catch (const std::exception &e) {
					e;
				}
				temp.clear();
			}
			if(ss.eof() && (!intWasFound))
				return false;
			return multipleInts(ss, args...);
		}
		/// Parses string @c s for integer values, that are returned into @c args
		template<typename ... Args> inline bool multipleInts(std::string s, Args & ... args)	
		{
			std::stringstream ss(s);
			return multipleInts(ss, args...);
		}
		
		template<typename ... Rest> inline bool multipleUints(std::stringstream & ss)
		{
			return true;
		}
		/// Parses stringstream @c ss for uint32_t values, that are returned into @c x, @c args
		template<typename ... Rest> inline bool multipleUints(std::stringstream & ss, uint32_t & x, Rest & ... args)	
		{
			int numArgs = sizeof...(args);
			std::string temp;
			unsigned long uintFound;
			bool intWasFound = false;
			while ((!ss.eof()) && (!intWasFound)) {
				ss >> quoted(temp);
				if(temp.empty()) break;
				try{
					size_t sz;
					uintFound = stoul(temp, &sz, 0);
					if(temp.size() != sz)
						throw std::exception();
					x = (uint32_t)uintFound;
					intWasFound = true;
				} catch (const std::exception &e) {
					e;
				}
				temp.clear();
			}
			if(ss.eof() && (!intWasFound))
				return false;
			return multipleUints(ss, args...);
		}
		/// Parses string @c s for uint32_t values, that are returned into @c args
		template<typename ... Args> inline bool multipleUints(std::string s, Args & ... args)	
		{
			std::stringstream ss(s);
			return multipleUints(ss, args...);
		}
	}
}
#endif //STR_PARSE_H
