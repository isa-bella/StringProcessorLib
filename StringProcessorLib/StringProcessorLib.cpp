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
#include <vector>

StringProcessor::StringProcessor( unsigned threadsNumber ) : threadsNumber_{ threadsNumber }
{
	for ( unsigned i = 0; i < threadsNumber_; i++ )
		threads_.emplace_back( std::thread(&StringProcessor::waitForJobs, this) );
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
		
	return !stop_ && !strings_.empty();
	
}

void StringProcessor::process()
{
	// iterate stages and apply ops to string
	// at the end make done_ = true;


	for (auto it = stages_.begin(); it != stages_.end(); ++it)
	{
		for (auto it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
		{
			switch (*it2)
			{
			case lowercase:
				//for (auto it3 = strings_.begin(); it3 != strings_.end(); ++it3)
				for	(std::vector<std::string>::size_type it3 = 0; it3 != strings_.size(); it3++)
				{
					results_[it3] = procLowercase(strings_[it3]);
				}
				break;
			case uppercase:
				for (std::vector<std::string>::size_type it3 = 0; it3 != strings_.size(); it3++)
				{
					results_[it3] = procUppercase(strings_[it3]);
				}
				break;
			case sort:
				for (std::vector<std::string>::size_type it3 = 0; it3 != strings_.size(); it3++)
				{
					results_[it3] = procSort(strings_[it3]);
				}
				break;
			case invert:
				for (std::vector<std::string>::size_type it3 = 0; it3 != strings_.size(); it3++)
				{
					results_[it3] = procInvert(strings_[it3]);
				}
				break;
			}
		}

	}

}

bool StringProcessor::start( const StringList &strings )
{
	if (strings.empty())
		return false;
	{
		std::lock_guard<std::mutex> guard(mutex_);

		if (!strings_.empty())
			return false;
	
		strings_ = strings;
		process_ = true;
	}

	condition_.notify_one();

	return true;
}

std::vector<std::string> StringProcessor::getResults()
{
	std::lock_guard<std::mutex> guard(mutex_);

	
	// reset
	process_ = false;
	strings_.clear();
	stages_.clear();

	return results_;
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
	StageOperations stageToDelete = make_pair(stage, operations);
	std::lock_guard<std::mutex> guard(mutex_);

	if (process_)
		return false;

	auto it = std::find_if(stages_.begin(), stages_.end(), [stage](const StageOperations & op) {
		return op.first == stage;
	});

	if (it == stages_.end())
		return false;

	stageToDelete = *it;
	stages_.erase(it);

	return true;
}

std::string StringProcessor::procLowercase( std::string& s)
{	
	// convert string to lower case

	std::for_each(s.begin(), s.end(), [](char c) {
		c = ::tolower(c);
	});

	std::cout << " ISADEBUG In Lowercase : " << s << std::endl;

	return s;

}

std::string StringProcessor::procUppercase( std::string& s)
{
	// convert string to lower case

	std::for_each(s.begin(), s.end(), [](char c) {
		c = ::toupper(c);
	});

	std::cout << " ISADEBUG In Upper Case : " << s << std::endl;

	return s;

}

std::string  StringProcessor::procSort( std::string& str)
{
	std::sort(str.begin(), str.end());
	return str;
}

std::string StringProcessor::procInvert( std::string& str)
{
	std::reverse(str.begin(), str.end());
	return str;
}

