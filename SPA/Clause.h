#pragma once
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::map;
using std::string;

enum ClauseType{
	FOLLOWS_, 
	FOLLOWSSTAR_, 
	PARENT_, 
	PARENTSTAR_, 
	USES_, 
	USESSTAR_, 
	MODIFIES_, 
	MODIFIESSTAR_, 
	ASSIGN_, 
	ASSIGNSTAR_  
};

class Clause
{
private:
	string firstArg;
	bool firstArgFixed;
	string firstArgType;
	string secondArg;
	bool secondArgFixed;
	string secondArgType;
	string patternArg;
public:
	Clause(void);
	~Clause(void);

	void setFirstArg(string);
	void setSecondArg(string);
	void setFirstArgType(string);
	void setSecondArgType(string);
	void setFirstArgFixed(bool);
	void setSecondArgFixed(bool);

	string getFirstArg();
	string getSecondArg();
	string getFirstArgType();
	string getSecondArgType();
	bool getFirstArgFixed();
	bool getSecondArgFixed();

};

namespace stringconst{
	string const STRING_SELECT = "Select";
	string const STRING_SUCHTHAT = "such that";
	string const STRING_SUCH = "such";
	string const STRING_THAT = "that";
	string const STRING_WITH = "with";
	string const STRING_AND = "and";
	string const STRING_EMPTY = "_";

	string const QUERY_NONE = "NONE";
	string const QUERY_ERROR = "ERROR";
	string const QUERY_INVALID = "INVALID";
	string const QUERY_VALID = "VALID";

	string const TYPE_FOLLOWS = "Follows";
	string const TYPE_PARENT = "Parent";
	string const TYPE_MODIFIES = "Modifies";
	string const TYPE_USES = "Uses";
	string const TYPE_PATTERN = "Pattern";
	string const TYPE_CALLS = "Calls";

	string const ARG_WHILE = "while";
	string const ARG_STATEMENT = "statement";
	string const ARG_ASSIGN = "assign";
	string const ARG_VARIABLE = "variable";
	string const ARG_BOOLEAN = "boolean";

	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};
