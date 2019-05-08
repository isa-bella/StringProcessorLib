/*Main.cpp*/

#include <iostream>
#include <string.h>
#include <fstream>
#include "StringProcessorLib.h"



int main()
{
	std::ifstream inputFile("Input.txt");
	std::ofstream outFile("Output.txt");
	std::string line;
	std::vector<std::string> strings, results;
	StringProcessor processor;

	namespace time = std::chrono;
	
	if (inputFile.is_open())
	{
		while (getline(inputFile, line))
		{
			strings.push_back(line);
		}
		inputFile.close();
	}

	

	/*processor.enqueueStageOps(1, { Operation::lowercase, Operation::uppercase });
	processor.enqueueStageOps(2, { Operation::invert, Operation::sort });
	processor.enqueueStageOps(3, { Operation::invert, Operation::lowercase, Operation::uppercase });*/
	
	processor.enqueueStageOps(1, { Operation::lowercase});
	processor.enqueueStageOps(2, { Operation::uppercase });
	processor.enqueueStageOps(1, { Operation::sort, Operation::invert });

	//std::string test = "giGi Are MERRre";
	//processor.procLowercase(test);

	//std::cout << "test = " << test;


	auto startTime = time::high_resolution_clock::now();

	processor.start( strings );
	while (!processor.done()) {
		std::this_thread::sleep_for(std::chrono::seconds(1));
		std::cout << "Main thread: waiting for results...\n";
	}
	auto endTime = time::high_resolution_clock::now();

	std::cout << "Processing time: " << time::duration_cast<time::milliseconds>(endTime - startTime).count() << " ms\n";

	if (!processor.getResults(results))
		return -1;

	for (auto&& line : results)
	{
		std::cout << "ISADEBUG line " << line << std::endl;
		outFile << line;
	}	

	return 0;
}