#include "QueryEvaluator.h"

using namespace std;

map<string, string>::iterator dlIter;						// map iterator

QueryEvaluator::QueryEvaluator(void)
{

}

QueryEvaluator::~QueryEvaluator(void)
{
}

void QueryEvaluator::setQuery(Query q) {
	this->queryObj = q;
}

Results QueryEvaluator::getClauseResult(Clause c,string syn, Results res) {
	Query q = this->queryObj;
}

Results QueryEvaluator::oneClauseOneSyn() {
	Query q = this->queryObj;
	Clause c = q.getClause(0);
	string selectSyn = q.getSelectSynonym(0);
	// check if synonym is used in clause
	string arg1 = c.getFirstArg();
	string arg2 = c.getSecondArg();

	if (arg1 == selectSyn || arg2 == selectSyn) { // synonym is used in clause
		Results res = getClauseResult(c, selectSyn, res);
		return res;

	} else { // synonym is not used in clause
		Results res = getClauseResult(c, "bool", res); // check if clause is true
		map<string, string> declarationList = q.getDeclarationList();
		dlIter = declarationList.find(selectSyn);
		string synonymType = dlIter->second;
		res.setResultType();
		return res;
	}
}

Results QueryEvaluator::manyClauseOneSyn() {
	Query q = this->queryObj;
	Clause c1 = q.getClause(1);
	Clause c2 = q.getClause(2);

	string arg1 = c1.getFirstArg();
	string arg2 = c1.getSecondArg();
	string arg3 = c2.getFirstArg();
	string arg4 = c2.getSecondArg();

}