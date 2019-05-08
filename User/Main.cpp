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
	
	processor.enqueueStageOps(1, { Operation::lowercase, Operation::uppercase });
	processor.enqueueStageOps(3, { Operation::sort, Operation::invert });
	processor.enqueueStageOps(3, { Operation::uppercase });
	processor.enqueueStageOps(2, { Operation::lowercase, Operation::removespace });

	auto startTime = time::high_resolution_clock::now();

	processor.start( strings );
	while (!processor.done())
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	auto endTime = time::high_resolution_clock::now();

	std::cout << "Processing time: " << time::duration_cast<time::milliseconds>(endTime - startTime).count() << " ms\n";

	if (!processor.getResults(results))
		return -1;

	for (auto&& line : results)
	{
		//std::cout << " line '" << line << "'" << std::endl;
		outFile << line << std::endl;
	}	
	
	return 0;
}