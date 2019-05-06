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
#include <fstream>
#include "StringProcessorLib.h"



int main()
{
	
	std::string test = "ana are mere";


	/*std::cout << "Lowercase: " <<
		StringProcessor::lowercase(test) << std::endl;
	std::cout << "Uppercase: " <<
		StringProcessor::uppercase(test) << std::endl;
	std::cout << "Sort:  " <<
		StringProcessor::sort(test) << std::endl;
	std::cout << "Invert: " <<
		StringProcessor::invert(test) << std::endl;*/

	StringProcessor::StringList strings{ "lkjflsd kdfjsk KSJHKA", "lkjfskdfjs kfd" };

	StringProcessor processor;

	processor.enqueueStageOps(1, { StringProcessor::lowercase, StringProcessor::uppercase });
	processor.enqueueStageOps(2, { StringProcessor::invert, StringProcessor::sort });

	processor.start( strings );

	while (!processor.done()) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}

	std::vector<std::string> results;

	results = processor.getResults();

	std::ofstream outFile("out.txt");

	for (auto&& line : results)
	{
		std::cout << "ISADEBUG line " << line << std::endl;
		outFile << line;
		outFile.close();
	}	

	return 0;
}