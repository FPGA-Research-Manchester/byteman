#ifndef PARSESTRING_H
#define PARSESTRING_H

#include <iostream>
#include <string>
#include <sstream>

using namespace std;
namespace parseString{
	string getNthWord(string, int);
	string getLastWord(string);
	string getAllButLastWord(string);
	string getAllWords(string);
	bool getNthInt(string, int, int&);
	bool getArrayOfUints(string, int, uint32_t*);
	string removeQuotes(string);

	// fix for error LNK2019: unresolved external symbol, keep this function in the header
	template<typename First, typename ... Rest> bool getMultipleIntsRecursive(stringstream & ss)
	{
		return true;
	}
	template<typename First, typename ... Rest> bool getMultipleIntsRecursive(stringstream & ss, int & x, Rest & ... args)
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
			return getMultipleIntsRecursive<int>(ss, args...);
	}
	template<typename ... Args> bool getMultipleInts(string s, Args & ... args)
	{
		stringstream ss(s);
		return getMultipleIntsRecursive<int>(ss, args...);
	}
}
#endif //PARSESTRING_H
