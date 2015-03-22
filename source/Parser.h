#pragma once

#include <string>
#include <regex>

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

	vector<string> explode(const string &s);
	void match(string token);
	void getNextToken();
	string getWord();

	void program();
	void procedure();
	void stmtLst();
	void stmt();
	void assign();
};