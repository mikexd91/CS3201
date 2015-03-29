#pragma once

#include <string>
#include <regex>
#include "PDR.h"

using namespace std;

class Parser {
public: 
	Parser();
	void parse(string);

private:

	string nextToken;
	vector<string> tokens;
	vector<string>::iterator iter;
	int nestingLevel;
	PDR parsedDataReceiver;

	string sanitise(string str);
	vector<string> explode(const string &str);
	void match(string token);
	void getNextToken();
	string getWord();
	queue<string> getExpression();

	void program();
	void procedure();
	void stmtLst();
	void stmt();
	void assign();
	void parseWhile();
	void endParse();
};

namespace ParserConstants {
	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
}