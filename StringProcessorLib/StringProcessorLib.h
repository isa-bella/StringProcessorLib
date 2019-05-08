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

enum class Operation { lowercase, uppercase, sort, invert, removespace };

	class StringProcessor
	{
		struct StringData {
			StringData(const std::string& data) : data_{ data }
			{
			}
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

		// Processes the string in lowercase format
		static void procLowercase(std::string& str);

		// Processes the string in uppercase format
		static void procUppercase(std::string& str);
		
		// Sorts the string
		static void procSort(std::string& str);

		// Inverts the string
		static void procInvert(std::string& str);

		//Removes the spaces from string
		static void procRemoveSpace(std::string& str);

	private:
		std::list<StageOperations> stages_;
		StringList				 strings_;
		std::priority_queue<std::string> resultQueue_;

		std::vector<std::thread> threads_;
		std::mutex mutex_;
		std::condition_variable condition_;
		
		bool process_{ false };
		std::atomic_bool done_{ false };
		std::atomic_bool stop_{ false };
	};
