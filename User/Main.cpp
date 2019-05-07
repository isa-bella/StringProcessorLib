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

	std::vector<std::string> strings{ "lkjflsd kdfjsk KSJHKA", "lkjfskdfjs kfd" };

	StringProcessor processor;

	processor.enqueueStageOps(1, { Operation::lowercase, Operation::uppercase });
	processor.enqueueStageOps(2, { Operation::invert, Operation::sort });
	
	namespace time = std::chrono;

	auto start = time::high_resolution_clock::now();
	processor.start( strings );
	while (!processor.done()) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Main thread: waiting for results...\n";
	}
	auto end = time::high_resolution_clock::now();

	std::cout << "Processing time: " << time::duration_cast<time::milliseconds>(end - start).count() << " ms\n";

	std::vector<std::string> results;
		
	if (!processor.getResults(results))
		return -1;

	std::ofstream outFile("out.txt");

	for (auto&& line : results)
	{
		std::cout << "ISADEBUG line " << line << std::endl;
		outFile << line;
	}	

	return 0;
}