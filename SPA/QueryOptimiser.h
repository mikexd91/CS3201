#pragma once

#include "Query.h"
#include "Clause.h"

class QueryOptimiser
{
private:
	bool isRelated(string, string);
	void addRelation(string, string);
	void removeRelation(string, string);
	void setWeight(Clause, int);
	int getWeight(Clause);

public:
	QueryOptimiser(void);
	~QueryOptimiser(void);
	void OptimiseQuery(Query);
};

