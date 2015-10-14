#pragma once
#include "Query.h"
#include "Clause.h"
#include "StringPair.h"
#include "SuchThatClauseBuilder.h"
#include <queue>
#include <string>
#include <vector>
#include <sstream>

using std::queue;
using std::vector;
using std::stringstream;
using std::string;

class QueryParser
{
	
public:
	static QueryParser* getInstance();			// gets singleton instance

	string removeFrontSpaces(string);
	bool containsOperator(string);
	string getFirstOperator(string);
	int getOperatorIndex(string);
	queue<string> exprBuilder(string);
	string removeSpace(string);
	vector<string> split(string, char, vector<string>*);
	void queueBuilder(string, queue<string>*);
	string queueToString(queue<string>);
	void tokeniser(string, char, vector<string>*);
	bool containsAny(string, vector<string>*);
	bool containsDeclarationType(string);
	bool containsClauseType(string);
	bool containsKeyword(string);
	string getClauseString(string);
	SuchThatClauseBuilder* createCorrectClause(string, queue<string>*);
	void splitByDelims(vector<string>*, string, vector<string>);
	void unexpectedEndCheck(queue<string>*);

	void parseDeclarations(Query*, vector<string>*);
	void parseSelectSynonyms(Query*, queue<string>*);
	void parseClause(Query*, queue<string>*);
	void parsePattern(Query*, queue<string>*);
	void parsePatternOther(Query*, queue<string>*, string);
	void parsePatternIf(Query*, queue<string>*, string);
	void parseWith(Query*, queue<string>*);

	Query* parseQuery(string);

private:
	// SINGLETON
	static bool instanceFlag;
	static QueryParser* parser;

	// SINGLETON CONSTRUCTOR
	QueryParser();
};

