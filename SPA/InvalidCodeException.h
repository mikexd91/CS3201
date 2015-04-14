#include <iostream>
#include <exception>
#include <string>
#include <sstream>

using namespace std;

	
class InvalidCodeException : public exception {
	
public:
	InvalidCodeException(const string m) : message(m) { }

	virtual const char * what() const throw() { 
		return message.c_str(); 
	}

private:
	int lineNumber;
	string message;
};