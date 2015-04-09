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
	PDR* parsedDataReceiver;
	int stmtCount;

	string sanitise(string str);
	vector<string> explode(const string &str);
	void match(string token);
	void getNextToken();
	string getWord();
	string getName();
	//string getFactor();
	queue<string> getExpression();
	void throwException(int);

	void program();
	void procedure();
	void stmtLst();
	void stmt();
	void assign();
	void parseWhile();
	void endParse();
	//void parseFactor(string, queue<string>&);
	//void parseSymbol(string, queue<string>&, stack<string>&);
};

namespace ParserConstants {
	const string DELIM_STRING = " ;={}+*-";
	const char DELIMITERS[] = { ';', '=', '{', '}', '+', '*', '-' };
	const string ERROR_MESSAGE = "Error at line ";
}