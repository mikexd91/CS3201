#pragma once
#include <string>
#include <vector>
#include <regex>

using std::string;
using std::vector;
class Utils
{
public:
	Utils(void);
	~Utils(void);
	
	string sanitise(string str);
	vector<string> explode(const string &str);
	
};

namespace UtilsConstants {
	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
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
	string const TYPE_FOLLOWS_STAR = "Follows*";
	string const TYPE_PARENT_STAR = "Parent*";
	string const TYPE_MODIFIES_STAR = "Modifies*";
	string const TYPE_USES_STAR = "Uses*";

	string const ARG_WHILE = "while";
	string const ARG_STATEMENT = "statement";
	string const ARG_ASSIGN = "assign";
	string const ARG_VARIABLE = "variable";
	string const ARG_BOOLEAN = "boolean";

	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};