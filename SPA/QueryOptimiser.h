#pragma once

#include "Query.h"
#include "Clause.h"
#include "SynGraph.h"

class QueryOptimiser
{
private:

public:
	QueryOptimiser(void);
	~QueryOptimiser(void);
	vector<int> optimizeQuery(Query*);
};

