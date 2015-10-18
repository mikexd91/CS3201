#include <iostream>
#include <exception>
using namespace std;

struct MissingDeclarationException : public std::exception{
	const char * what() const throw(){
		return "Declaration not found!";
	}
};

struct InvalidDeclarationException : public std::exception{
	const char * what() const throw(){
		return "Declaration type mismatch!";
	}
};

struct InvalidSelectException : public std::exception{
	const char * what() const throw(){
		return "Query invalid!";
	}
};

struct InvalidArgumentException : public std::exception{
	const char * what() const throw(){
		return "Argument invalid";
	}
};

struct UnexpectedEndException : public std::exception{
	const char * what() const throw(){
		return "Unexpected end of query!";
	}
};

struct UnexpectedClauseException : public std::exception{
	const char * what() const throw(){
		return "Clause type mismatch";
	}
};

struct DebugHelpException : public std::exception{
	const char * what() const throw(){
		return "Debug";
	}
};

struct InvalidClauseException : public std::exception{
	const char* what () const throw(){
		return "Clause: Invalid parameters";
	}
};

struct InvalidSyntaxException : public std::exception{
	const char* what() const throw(){
		return "Clause: Invalid syntax";
	}
};

struct InvalidAttributeException : public std::exception{
	const char* what () const throw(){
		return "Attribute condition mismatch";
	}
};