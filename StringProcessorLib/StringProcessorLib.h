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


	class StringProcessor
	{
	public:
		enum Operation { lowercase, uppercase, sort, invert };

		using StringList = std::vector<std::string>;
		using StageOperations = std::pair<int, std::vector<Operation>>;

	public:
		StringProcessor( unsigned threadsNumber = 1 );
		virtual ~StringProcessor();

		virtual bool start( const StringList &string );

		virtual bool enqueueStageOps(int stage, const std::vector<Operation> &operations);
		virtual bool dequeueStageOps(int stage, std::vector<Operation>& operations);
		
		bool jobReady();
		std::vector<std::string> getResults();

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
		StringList				 results_;
		std::vector<std::thread> threads_;
		unsigned threadsNumber_{ 2 };
		std::mutex mutex_;
		std::condition_variable condition_;
		
		bool process_{ false };
		std::atomic_bool done_{ false };
		std::atomic_bool stop_{ false };
	};
