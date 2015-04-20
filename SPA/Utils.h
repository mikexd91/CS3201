#pragma once
#include <string>
#include <vector>
#include <regex>
#include <queue>
#include <stack>
#include <set>

#include "Statement.h"

class Utils
{
public:
	
	static std::string sanitise(string str);
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
	static string convertNodeTypeToArgType(NodeType);
	static set<int> filterStatements(set<int>, NodeType);
	static NodeType convertArgTypeToNodeType(string);
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
	string const TYPE_PATTERN = "pattern";
	string const TYPE_FOLLOWS_STAR = "Follows*";
	string const TYPE_PARENT_STAR = "Parent*";

	string const ARG_WHILE = "while";
	string const ARG_STATEMENT = "stmt";
	string const ARG_ASSIGN = "assign";
	string const ARG_VARIABLE = "variable";
	string const ARG_PROGLINE = "prog_line";
	string const ARG_BOOLEAN = "boolean";
	string const ARG_CONSTANT = "constant";
	string const ARG_GENERIC = "underscore";

	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};