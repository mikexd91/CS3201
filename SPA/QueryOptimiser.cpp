#include "QueryOptimiser.h"


QueryOptimiser::QueryOptimiser(void)
{
}


QueryOptimiser::~QueryOptimiser(void)
{
}

void QueryOptimiser::sortQuery(Query* q) {
	vector<Clause*> clauseList = q->getClauseList();
	std::sort(clauseList.begin(), clauseList.end(), cmp());
	q->setClauseList(clauseList);
}
