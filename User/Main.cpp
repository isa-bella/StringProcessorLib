/*Main.cpp*/
/* Steps:
	- read from file and then create a std::vector
	- check for string to be less than 1000 and 1000 lines in file
	- ask user for input 
	- save map of options per stage
	- start the processing



*/
#include <iostream>
#include <string.h>
#include "StringProcessorLib.h"



int main()
{
	
	std::string test = "ana are mere";


	std::cout << "Lowercase: " <<
		StringProcessor::Lowercase(test) << std::endl;
	std::cout << "Uppercase: " <<
		StringProcessor::Uppercase(test) << std::endl;
	std::cout << "Sort:  " <<
		StringProcessor::Sort(test) << std::endl;
	std::cout << "Invert: " <<
		StringProcessor::Invert(test) << std::endl;

	return 0;
}