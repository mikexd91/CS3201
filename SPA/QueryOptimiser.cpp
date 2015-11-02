#include "QueryOptimiser.h"


QueryOptimiser::QueryOptimiser(void)
{
}


QueryOptimiser::~QueryOptimiser(void)
{
}

vector<int>* QueryOptimiser::optimizeQuery(Query* q) {
	vector<int>* queryCompList = new vector<int>();
	vector<Clause*> clauseList = q->getClauseList();
	BOOST_FOREACH(auto i, clauseList) {
		if (!i->isValid()) {
			return queryCompList;
		}
	}

	SynGraph *graph = new SynGraph();
	queryCompList = graph->reorderQuery(q);
	return queryCompList;
}
