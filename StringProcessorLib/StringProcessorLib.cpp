// StringProcessorLib.cpp
// compile with: cl /c /EHsc StringProcessorLib.cpp
// post-build command: lib StringProcessorLib.obj
/*Any of following operations are possible to be added on each stage:
 - Lowercase � will convert all letters to lowercase
 - Uppercase � will convert all letters to uppercase
 - Sort � will sort letters within the line
 - Invert � will inverse the whole string line
 - Other operations can be added by the candidate */
#include "StringProcessorLib.h"
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <vector>

StringProcessor::StringProcessor( unsigned threadsNumber )
{
	for (unsigned i = 0; i < threadsNumber; i++)
		threads_.emplace_back(std::thread(&StringProcessor::waitForJobs, this));
}

StringProcessor::~StringProcessor()
{
	stop_ = true;
	condition_.notify_all();

	try {
		for (auto&& thread : threads_)
			thread.join();
	}
	catch (...) {
		;
	}
}

void StringProcessor::waitForJobs()
{
	do {
			if (jobReady())
				process();
	} while (!stop_);
}

bool StringProcessor::jobReady()
{
	std::unique_lock<std::mutex> guard(mutex_);

	condition_.wait(guard, [this]() { return !strings_.empty() || stop_; });
		
	return !stop_ && !strings_.empty() && !done_;
}

void StringProcessor::process()
{
	std::string stringJob;
	bool allProcessed = true;
	int pos;

	// Look for strings available for processing
	auto readyToProcess = [&allProcessed]( const StringData &string ) {
		if (!string.processed_) {
			allProcessed = false;
			return !string.data_.empty();
		}

		return false;
	};

	{
		std::lock_guard<std::mutex> guard(mutex_);

		auto it = std::find_if(strings_.begin(), strings_.end(), readyToProcess);

		if (it != strings_.end()) {
			stringJob = std::move((*it).data_);
			pos = it - strings_.begin();
		}
	}

	if (stringJob.empty()) {
		if (allProcessed)
			done_ = true;

		return;
	}

	auto processFn = [&stringJob]( Operation op ) {
		switch (op)
		{
		case Operation::lowercase:
			StringProcessor::procLowercase(stringJob);
			break;
		case Operation::uppercase:
			StringProcessor::procUppercase(stringJob);
			break;
		case Operation::sort:
			StringProcessor::procSort(stringJob);
			break;
		case Operation::invert:
			StringProcessor::procInvert(stringJob);
			break;
		case Operation::removespace:
			StringProcessor::procRemoveSpace(stringJob);
			break;
		default:
			break;
		}
	};
	//debug

	// Process all stages
	std::for_each(stages_.begin(), stages_.end(), [&processFn](const StageOperations & stage) {
		std::for_each(stage.second.begin(), stage.second.end(), processFn);
	});

	std::lock_guard<std::mutex> guard(mutex_);
	
	// Move the string back
	strings_[pos].data_ = std::move(stringJob);
	strings_[pos].processed_ = true;
}

bool StringProcessor::start( const std::vector<std::string> &strings )
{
	if (strings.empty())
		return false;

	{
		std::lock_guard<std::mutex> guard(mutex_);

		if (!strings_.empty())
			return false;

		std::for_each(strings.begin(), strings.end(), [this](const std::string & str) {
			strings_.emplace_back(str);
			});

		for (auto it = stages_.begin(); it != stages_.end(); ++it)
		{
			std::iter_swap(it, std::min_element(it, stages_.end()));
		}
		process_ = true;
	}

	// DBG
	std::for_each(stages_.begin(), stages_.end(), [](const StageOperations & stage) {
		std::cout << "ISADEBUG stage: " << stage.first << ", operations: ";
		std::for_each(stage.second.begin(), stage.second.end(), [](Operation op) {
			std::cout << (int)op << " ";

			});

		std::cout << std::endl;

		});

	condition_.notify_all();

	return true;
}

bool StringProcessor::getResults( std::vector<std::string> &result )
{
	std::lock_guard<std::mutex> guard(mutex_);

	std::for_each(strings_.begin(), strings_.end(), [&result](StringData &string) {
		result.emplace_back(std::move(string.data_));
	});

	// reset
	stages_.clear();
	strings_.clear();
	process_ = false;
	
	return !result.empty();
}

bool StringProcessor::enqueueStageOps(int stage, const std::vector<Operation>& operations)
{
	if (operations.size() > 5)
		return false;

	std::lock_guard<std::mutex> guard(mutex_);
	
	if (process_)
		return false;

	auto it = std::find_if(stages_.begin(), stages_.end(), [stage](const StageOperations & op) {
		return op.first == stage;
	});

	if ( it == stages_.end() )
		stages_.emplace_back(std::make_pair(stage, operations));

	return true;
}

bool StringProcessor::dequeueStageOps(int stage, std::vector<Operation> &operations)
{
	std::lock_guard<std::mutex> guard(mutex_);

	if (process_)
		return false;

	auto it = std::find_if(stages_.begin(), stages_.end(), [stage](const StageOperations & op) {
		return op.first == stage;
	});

	if (it == stages_.end())
		return false;

	operations = ( *it ).second;
	stages_.erase(it);

	return true;
}

void StringProcessor::procLowercase(std::string& s)
{	
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

void StringProcessor::procUppercase(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

void StringProcessor::procSort( std::string& str)
{
	std::sort(str.begin(), str.end());
}

void StringProcessor::procInvert( std::string& str)
{
	std::reverse(str.begin(), str.end());
}

void StringProcessor::procRemoveSpace(std::string& str)
{
	str.erase(remove_if(str.begin(), str.end(), isspace), str.end());
}


