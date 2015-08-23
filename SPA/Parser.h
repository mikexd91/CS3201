#pragma once

#include <string>
#include <regex>
#include "PDR.h"
#include "boost/assign.hpp"

using namespace std;

class Parser {
public: 
	Parser();
	void parse(string);

private:

	string currentProcName;
	string nextToken;
	vector<string> tokens;
	vector<string>::iterator iter;
	int nestingLevel;
	PDR* parsedDataReceiver;
	int stmtCount;
	vector<string> existingProcedures;
	vector<string> calledProcedures;

	string sanitise(string str);
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
	void call();
	void parseWhile();
	void parseIfBlock();
	void parseIf();
	void parseThen();
	void parseElse();
	void validateCallStmts();
	void endParse();
	string generateErrorMessage(int lineNumber);
	
	//void parseFactor(string, queue<string>&);
	//void parseSymbol(string, queue<string>&, stack<string>&);
};

namespace ParserConstants {
	const string DELIM_STRING = " ;={}+*-()";
	const vector<char> DELIMITERS = boost::assign::list_of(';')('=')('{')('}')('+')('*')('-')('(')(')');
	const string ERROR_MESSAGE = "Error near line ";
	
}