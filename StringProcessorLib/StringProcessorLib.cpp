// StringProcessorLib.cpp
// compile with: cl /c /EHsc StringProcessorLib.cpp
// post-build command: lib StringProcessorLib.obj
/*Any of following operations are possible to be added on each stage:
 - Lowercase – will convert all letters to lowercase
 - Uppercase – will convert all letters to uppercase
 - Sort – will sort letters within the line
 - Invert – will inverse the whole string line
 - Other operations can be added by the candidate */
#include "StringProcessorLib.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <locale>



std::string StringProcessor::Lowercase( std::string& s)
{	
	// convert string to lower case

	std::for_each(s.begin(), s.end(), [](char& c) {
		c = ::tolower(c);
	});

	std::cout << "In Lowercase : " << s << std::endl;

	return s;

}

std::string StringProcessor::Uppercase( std::string& s)
{
	// convert string to lower case

	std::for_each(s.begin(), s.end(), [](char& c) {
		c = ::toupper(c);
	});

	std::cout << "In Upper Case : " << s << std::endl;

	return s;

}

std::string  StringProcessor::Sort( std::string& str)
{
	std::sort(str.begin(), str.end());
	return str;
}

std::string StringProcessor::Invert( std::string& str)
{
	std::reverse(str.begin(), str.end());
	return str;
}

void Start()
{
	//Start the processing

}