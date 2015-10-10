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
	static string removeFrontSpaces(string);
	static bool containsOperator(string);
	static string getFirstOperator(string);
	static int getOperatorIndex(string);
	static queue<string> exprBuilder(string);
	static string removeSpace(string);
	static vector<string> split(string, char, vector<string>*);
	static queue<string> queueBuilder(string);
	static string queueToString(queue<string>);
	static vector<string> tokeniser(string, char);
	static bool containsAny(string, vector<string>);
	static bool containsDeclarationType(string);
	static bool containsClauseType(string);
	static bool containsKeyword(string);
	static string getClauseString(string);
	static SuchThatClauseBuilder* createCorrectClause(string);
	static vector<string> splitByDelims(vector<string>, string);
	static void unexpectedEndCheck(queue<string>);

	static void parseDeclarations(Query*, vector<string>);
	static void parseSelectSynonyms(Query*, queue<string>);
	static void parseClause(Query*, queue<string>);
	static void parsePattern(Query*, queue<string>);
	static void parsePatternOther(Query*, queue<string>, string);
	static void parsePatternIf(Query*, queue<string>, string);
	static void parseWith(Query*, queue<string>);

	static Query parseQuery(string);
};

