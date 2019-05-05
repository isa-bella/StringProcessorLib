#include <iostream>
#include <string>

	class StringProcessor
	{
	public:
		static void Start();
	private:
		// Returns the string in lowercase format
		static std::string Lowercase( std::string& str);

		// Returns the string in uppercase format
		static std::string Uppercase( std::string& str);

		// Returns the string sorted
		static std::string Sort( std::string& str);

		// Returns the string inverted
		static std::string Invert( std::string& str);
	};
