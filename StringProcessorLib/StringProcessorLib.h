#include <string>
#include <map>
#include <vector>
#include <list>
#include <queue>
#include <thread>
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>

enum class Operation { lowercase, uppercase, sort, invert };

	class StringProcessor
	{
		struct StringData {
			std::string data_;
			bool processed_{ false };
		};
	public:

		using StringList = std::vector<StringData>;
		using StageOperations = std::pair<int, std::vector<Operation>>;

	public:
		StringProcessor( unsigned threadsNumber = 2 );
		virtual ~StringProcessor();

		virtual bool start( const std::vector<std::string> &string );

		virtual bool enqueueStageOps(int stage, const std::vector<Operation> &operations);
		virtual bool dequeueStageOps(int stage, std::vector<Operation>& operations);
		
		bool getResults(std::vector<std::string>& result);
		bool jobReady();
		bool done() const
		{
			return done_.load();
		}

	private:
		virtual void waitForJobs();

		void process();

		// Returns the string in lowercase format
		static std::string procLowercase( std::string& str);

		// Returns the string in uppercase format
		static std::string procUppercase( std::string& str);

		// Returns the string sorted
		static std::string procSort( std::string& str);

		// Returns the string inverted
		static std::string procInvert( std::string& str);

	private:
		std::list<StageOperations> stages_;
		StringList				 strings_;
		std::vector<std::thread> threads_;
		std::mutex mutex_;
		std::condition_variable condition_;
		
		bool process_{ false };
		std::atomic_bool done_{ false };
		std::atomic_bool stop_{ false };
	};
