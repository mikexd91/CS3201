#include "QueryEvaluator.h"
#include "Clause.h"
#include "StringPair.h"
#include <vector>

using namespace std;

QueryEvaluator::QueryEvaluator(void)
{

}

QueryEvaluator::~QueryEvaluator(void)
{
}

set<string> getAllSynValues(vector<StringPair> selectList) {
	// check the syn type
	// get from the respective tables
	// convert all to string (if required)
	// return set of strings
}

set<string> QueryEvaluator::evaluateQuery(Query q) {
	vector<Clause> clauseList = q.getClauseList();
	vector<StringPair> selectList = q.getSelectList();
	
	if (clauseList.empty()) {
		set<string> result = getAllSynValues(selectList);
		return result;
	} else {

	}
	/*
	1.check if clauseList is empty
	if empty, 
		getSelectSynonym(0)
		populate all synonym with possible values
		put them in set<string> and return
2.if not empty
	for all clauses in getClauseList
		clause.evaluate()
		all to resultsList
3.if resultsList.size == 1 // there is only 1 query
	if selectSynonym is used in resultObj // helper function to compare
		return getSingleResults

	else // if selectSynonym is not used in resultObj
		if (resultObj.isClauseTrue())
			return all possible values of selectSyn
		else
			return none
4.else // resultsList.size > 1
	if both clauses use the same syn AND a clause uses select syn
		reduce result set via elimination and return result
	if both clauses use the same syn AND no clause uses select syn
		check if both clauses are valid
			if both are valid, return all possible select syn
			if 1 clause is invalid, return none
	if clauses do not use the same syn, 
		check if both are valid
			if both are valid, return all possible select syn
			if 1 clause is invalid, return none
	*/
	return set<string>();
}