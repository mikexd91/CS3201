#pragma once
#include "Query.h"
#include "Clause.h"
#include "Results.h"
#include "StmtTable.h"
#include "VarTable.h"
#include <vector>
//Query object to be passed between parser and evaluator
using namespace std;

class QueryEvaluator
{
public:
	QueryEvaluator(void);
	~QueryEvaluator(void);
	
	void setQuery(Query);
	Results getClauseResult(Clause,string, Results);
	Results oneClauseOneSyn();
	Results manyClauseOneSyn();
	Results evaluateQuery(Query);

private:
	Query queryObj;

};