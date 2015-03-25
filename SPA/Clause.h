#pragma once
#include <string>
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

	bool checkClauseValidity();
};

namespace stringconst{
	string const STRING_SELECT = "Select";
	string const STRING_SUCH = "such";
	string const STRING_THAT = "that";
	string const STRING_WITH = "with";
	string const STRING_AND = "and";

	string const QUERY_NONE = "NONE";
	string const QUERY_ERROR = "ERROR";
	string const QUERY_INVALID = "INVALID";
	string const QUERY_VALID = "VALID";

	string const TYPE_FOLLOWS = "FOLLOWS";
	string const TYPE_PARENT = "PARENT";
	string const TYPE_MODIFIES = "MODIFIES";
	string const TYPE_USES = "USES";
	string const TYPE_PATTERN = "PATTERN";
	string const TYPE_CALLS = "CALLS";

	string const ARG_STATEMENT = "STATEMENT";
	string const ARG_ASSIGN = "ASSIGN";
	string const ARG_VARIABLE = "VARIABLE";
};
