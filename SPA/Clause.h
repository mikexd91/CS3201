#pragma once
#include <string>
#include <map>

using std::map;
using std::string;
class Clause
{
private:
	string clauseType;
	string firstArg;
	string firstArgType;
	string secondArg;
	string secondArgType;
	string patternArg;
public:
	Clause(void);
	~Clause(void);

	void setClauseType(string);
	void setFirstArg(string);
	void setSecondArg(string);
	void setFirstArgType(string);
	void setSecondArgType(string);
	void setPatternArg(string);

	string getClauseType();
	string getFirstArg();
	string getSecondArg();
	string getFirstArgType();
	string getSecondArgType();
	string getPatternArg();

	bool checkClauseValidity(map<string, string>);
};

namespace stringconst{
	string const STRING_SELECT = "Select";
	string const STRING_SUCHTHAT = "such that";
	string const STRING_SUCH = "such";
	string const STRING_THAT = "that";
	string const STRING_WITH = "with";
	string const STRING_AND = "and";
	string const STRING_EMPTY = "";

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

	string const ARG_STATEMENT = "statement";
	string const ARG_ASSIGN = "assign";
	string const ARG_VARIABLE = "variable";
	string const ARG_BOOLEAN = "boolean";

	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};
