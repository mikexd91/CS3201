#pragma once
#include "Results.h"
#include "Query.h"
#include "Clause.h"

//Query object to be passed between parser and evaluator
using namespace std;

class QueryEvaluator
{
public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	Results evaluateQuery(Query q);

private:
	void setQueryObj(Query q);
	void setClauseObj(Clause c);
	void setResultObj(Results r);
	
	int isFixed(void);
	bool isSynInClause(void);
	void setResultSynNotInClause(bool isTrue);

	Results evaluateFollowsClause(void);
	
	Query queryObj;
	Clause clauseObj;
	Results resultsObj;
};