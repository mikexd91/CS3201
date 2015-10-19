#pragma once

#include "Query.h"
#include "Clause.h"

struct cmp
{
    bool operator()(Clause* a, Clause* b)
    {
		return a->getWeight() < b->getWeight();
    }
};


class QueryOptimiser
{
private:
	/*
	bool isRelated(string, string);
	void addRelation(string, string);
	void removeRelation(string, string);
	void setWeight(Clause*, int);
	int getWeight(Clause*);
	*/

public:
	QueryOptimiser(void);
	~QueryOptimiser(void);
	void sortQuery(Query*);
	//void OptimiseQuery(Query);
};

