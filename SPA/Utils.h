#pragma once
#include <string>
#include <vector>
#include <regex>
#include <queue>
#include <stack>

#include "Statement.h"

using namespace std;

class Utils
{
public:
	
	static string sanitise(string str);
	static vector<string> explode(const string&, const string, const char[7]);
	static queue<string> getRPN(queue<string>);
	static string getWordAndPop(queue<string>&);
	static void parseFactor(string, queue<string>&);
	static void parseSymbol(string, queue<string>&, stack<string>&);
	static bool isValidFactor(string);
	static bool isValidSymbol(string);
	static bool isValidName(string);
	static bool isValidConstant(string);
	static bool isSameType(string, NodeType);
};

namespace UtilsConstants {
	//const string DELIM_STRING = " ;={}";
	//const char DELIMITERS[] = { ';', '=', '{', '}' };
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

	string const ARG_WHILE = "While";
	string const ARG_STATEMENT = "Statement";
	string const ARG_ASSIGN = "Assign";
	string const ARG_VARIABLE = "Variable";
	string const ARG_BOOLEAN = "boolean";

	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};