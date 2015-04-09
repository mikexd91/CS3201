#pragma once
#include <string>
#include <vector>
#include <regex>
#include <queue>
#include <stack>

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
	
};

namespace UtilsConstants {
	//const string DELIM_STRING = " ;={}";
	//const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};