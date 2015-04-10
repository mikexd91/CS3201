#pragma once
#include "Query.h"
#include "Clause.h"
#include "StringPair.h"
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

private:
	string removeSpace(string);
	queue<string> queueBuilder(string, char);
	vector<string> split(string, char, vector<string>);
	vector<string> tokeniser(string, char);
	bool checkValidity();
	bool containsAny(string, vector<string>);
	bool containsClauseType(string);
	string getClauseString(string);
	void parseSelect(vector<string>, Query);

	Query queryProcessor(string);
	void parseDeclarations(Query, vector<string>);
	void parseSelectSynonyms(Query, queue<string>);
	void parseClause(Query, queue<string>);
	void parsePattern(Query, queue<string>);

public:
	QueryParser(void);
	~QueryParser(void);

	Query processQuery(string);
	
};

