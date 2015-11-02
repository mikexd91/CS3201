#include "QueryOptimiser.h"


QueryOptimiser::QueryOptimiser(void)
{
}


QueryOptimiser::~QueryOptimiser(void)
{
}

vector<int> QueryOptimiser::optimizeQuery(Query* q) {
	vector<Clause*> clauseList = q->getClauseList();
	BOOST_FOREACH(auto i, clauseList) {
		if (!i->isValid()) {
			return vector<int>();
		}
	}

	SynGraph *graph = new SynGraph();
	vector<int> queryCompList = graph->reorderQuery(q);
	return queryCompList;
}
