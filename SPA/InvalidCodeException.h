#include <exception>
using namespace std;

class InvalidCodeException {
public:
	InvalidCodeException()
		: message("Invalid SIMPLE code input!") { }

	const char * what() const { return message; }
private:
	const char * message;
};
