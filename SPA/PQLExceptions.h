#include <iostream>
#include <exception>

using std::exception;

struct MissingDeclarationException : public exception{
	const char * what() const throw(){
		return "Declaration not found!";
	}
}