#include "QueryEvaluator.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "Follows.h"
#include <vector>

using namespace std;

map<string, string>::iterator dlIter;						// map iterator

QueryEvaluator::QueryEvaluator(void)
{

}

QueryEvaluator::~QueryEvaluator(void)
{
}

void QueryEvaluator::setQueryObj(Query q) {
	this->queryObj = q;
}

void QueryEvaluator::setClauseObj(Clause c) {
	this->clauseObj = c;
}

void QueryEvaluator::setResultObj(Results r) {
	this->resultsObj = r;
}

int QueryEvaluator::isFixed() {
	bool isArg1Fixed = this->clauseObj.getFirstArgFixed();
	bool isArg2Fixed = this->clauseObj.getSecondArgFixed();
	
	if (isArg1Fixed && isArg2Fixed) {
		return 1;
	} else if (isArg1Fixed && !isArg2Fixed) {
		return 2;
	} else if (!isArg1Fixed && isArg2Fixed) {
		return 3;
	} else if (!isArg1Fixed && !isArg2Fixed) {
		return 4;
	} else {
		// throw error
		return 0;
	}
}

bool QueryEvaluator::isSynInClause() {
	string synonym = this->queryObj.getSelectSynonym(0);
	string arg1 = this->clauseObj.getFirstArg();
	string arg2 = this->clauseObj.getSecondArg();

	if (synonym.compare(arg1) == 0 ||
		synonym.compare(arg2) == 0) {
		return true;
	} else {
		return false;
	}
}

void QueryEvaluator::setResultSynNotInClause(bool isTrue) {
	
}

Results QueryEvaluator::evaluateFollowsClause(void) {
	string arg1 = this->clauseObj.getFirstArg();
	string arg2 = this->clauseObj.getSecondArg();
	Follows f = Follows();

	int isFixedCase = isFixed();
	switch (isFixedCase) {
		case 1 :  // Follows(1,2)
			int stmt1 = stoi(arg1);
			int stmt2 = stoi(arg2);
			bool isFollows = f.isFollows(stmt1, stmt2);
			setResultSynNotInClause(isFollows);
			break;

		case 2 : // Follows(1,a)
			int stmt1 = stoi(arg1);
			int stmt2 = f.getFollows(stmt1);

			if (isSynInClause()) {

			} else {

			}
			break;
		case 3 : // Follows(a,1)
			break;
		case 4 : // Follows(s1,s2)
			break;
		default:
			// throw error
			break;
	}

	return this->resultsObj;
}

Results QueryEvaluator::evaluateQuery(Query q) {
	return Results();
}