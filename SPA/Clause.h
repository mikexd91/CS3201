#pragma once
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::map;
using std::string;
class Clause
{
private:
	string clauseType;
	bool isStar;
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

	void setClauseType(string);
	void setStar(bool);
	void setFirstArg(string);
	void setSecondArg(string);
	void setFirstArgType(string);
	void setSecondArgType(string);
	void setFirstArgFixed(bool);
	void setSecondArgFixed(bool);
	void setPatternArg(string);

	string getClauseType();
	bool getStar();
	string getFirstArg();
	string getSecondArg();
	string getFirstArgType();
	string getSecondArgType();
	bool getFirstArgFixed();
	bool getSecondArgFixed();
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

	string const STAR_FOLLOWS = "Follows*";
	string const STAR_PARENT = "Parent*";
	string const STAR_MODIFIES = "Modifies*";
	string const STAR_USES = "Uses*";
	string const STAR_PATTERN = "Pattern*";
	string const STAR_CALLS = "Calls*";

	string const ARG_WHILE = "while";
	string const ARG_STATEMENT = "statement";
	string const ARG_ASSIGN = "assign";
	string const ARG_VARIABLE = "variable";
	string const ARG_BOOLEAN = "boolean";

	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};
