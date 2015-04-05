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
		Results res = getClauseResult(c, "none", res);
		map<string, string> declarationList = q.getDeclarationList();
		dlIter = declarationList.find(selectSyn);
		string synonymType = dlIter->second;
		res.setResultType(synonymType);
		return res;
	}
}

Results QueryEvaluator::manyClauseOneSyn() {

}