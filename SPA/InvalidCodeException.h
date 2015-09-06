#include <iostream>
#include <exception>
#include <string>
#include <sstream>


class InvalidCodeException : public std::exception {
	
public:
	InvalidCodeException(const std::string m) : message(m) { }

	virtual const char * what() const throw() { 
		return message.c_str(); 
	}

private:
	int lineNumber;
	std::string message;
};