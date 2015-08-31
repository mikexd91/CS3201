#include <iostream>
#include <exception>

using namespace std;

struct MissingDeclarationException : public exception{
	const char * what() const throw(){
		return "Declaration not found!";
	}
};

struct InvalidDeclarationException : public exception{
	const char * what() const throw(){
		return "Declaration type mismatch!";
	}
};

struct InvalidSelectException : public exception{
	const char * what() const throw(){
		return "Query invalid!";
	}
};

struct InvalidArgumentException : public exception{
	const char * what() const throw(){
		return "Argument invalid";
	}
};

struct UnexpectedEndException : public exception{
	const char * what() const throw(){
		return "Unexpected end of query!";
	}
};

struct UnexpectedClauseException : public exception{
	const char * what() const throw(){
		return "Clause type mismatch";
	}
};

struct DebugHelpException : public exception{
	const char * what() const throw(){
		return "Debug";
	}
};

struct InvalidClauseException : public exception{
	const char* what () const throw(){
		return "Clause: Invalid parameters";
	}
};

struct InvalidSyntaxException : public exception{
	const char* what() const throw(){
		return "Clause: Invalid syntax";
	}
};