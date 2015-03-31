#pragma once
#include "Query.h"
#include "Clause.h"
#include "Results.h"
#include <vector>
//Query object to be passed between parser and evaluator
using namespace std;

class QueryEvaluator
{
public:
	QueryEvaluator(void);
	~QueryEvaluator(void);
	Results evaluateQuery(Query);

private:
	Query queryObj;
	
};