#pragma once
#include "Results.h"
#include "Query.h"
#include <set>

//Query object to be passed between parser and evaluator
using namespace std;

class QueryEvaluator
{
public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	set<string> evaluateQuery(Query q);

	int getSameClause(Results obj1, Results obj2);
	set<string> getAllSynValues(vector<StringPair> selectList);
	set<string> evaluateOneClause(Results res, vector<StringPair> selectList);
	set<string> evaluateManyClause(vector<Results> resultList, vector<StringPair> selectList);

private:

};