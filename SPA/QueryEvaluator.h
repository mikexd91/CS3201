#pragma once
#include "Results.h"
#include "Query.h"

//Query object to be passed between parser and evaluator
using namespace std;

class QueryEvaluator
{
public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	Results evaluateQuery(Query q);

private:
	
};