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

private:
	
};