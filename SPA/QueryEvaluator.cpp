#include "QueryEvaluator.h"
#include "Clause.h"
#include "StringPair.h"
#include "Utils.h"
#include <vector>

using namespace std;

QueryEvaluator::QueryEvaluator(void)
{

}

QueryEvaluator::~QueryEvaluator(void)
{
}

set<string> getAllSynValues(vector<StringPair> selectList) {
	StringPair syn = selectList.front();
	string synType = syn.getSecond();
	
	if (synType == stringconst::ARG_WHILE) {

	} else if (synType == stringconst::ARG_STATEMENT) {

	} else if (synType == stringconst::ARG_ASSIGN) {

	} else if (synType == stringconst::ARG_VARIABLE) {

	} else {
		//error
		return set<string>();
	}
	// check the syn type
	// get from the respective tables
	// convert all to string (if required)
	// return set of strings
	
}

int getSameClause(Results obj1, Results obj2) {
	// return the NUMBER of syn that appear in both obj1 and obj2 has
	return 0;
}

bool usesSelectSyn(Results obj) {
	// checks if this obj uses the select synonym
	return false;
}

// get the results of select syn. Have to pull results from both pair and single results
set<string> getSelectSynResult(Results mergedResult, vector<StringPair> selectList) {
	return set<string>();
}

set<string> evaluateOneClause(Results res, vector<StringPair> selectList) {
	if (usesSelectSyn(res)) {
		set<string> result = getSelectSynResult(res, selectList);
		return result;
	}

	if (res.isClauseTrue()) {
		set<string> result = getAllSynValues(selectList);
		return result;
	}

	return set<string>();
	/*
	3.if resultsList.size == 1 // there is only 1 query
	if selectSynonym is used in resultObj // helper function to compare
		return getSingleResults

	else // if selectSynonym is not used in resultObj
		if (resultObj.isClauseTrue())
			return all possible values of selectSyn
		else
			return none
	*/
}

set<string> evaluateManyClause(vector<Results> resultList, vector<StringPair> selectList) {
	Results obj1 = resultList.front();
	Results obj2 = resultList.back();
	int numRepeatingClause = getSameClause(obj1, obj2);
	
	switch (numRepeatingClause) {
		case 0 : 
			if (obj1.isClauseTrue() && obj2.isClauseTrue()) {
				set<string> result = getAllSynValues(selectList);
				return result;
			} else {
				return set<string>();
			}
			break;

		case 1 :
			if (obj1.isClauseTrue() && obj2.isClauseTrue()) {
				Results mergedResult = obj1.getIntercept(obj2);
				vector<string> res = mergedResult.getSinglesResults();

				if (usesSelectSyn(obj1) || usesSelectSyn(obj2)) {
					set<string> result = getSelectSynResult(mergedResult, selectList);
					return result;
				} 

				if (!res.empty()) {
					set<string> result = getAllSynValues(selectList);
					return result;
				} 
			} 
			return set<string>();

		case 2 :
			if (obj1.isClauseTrue() && obj2.isClauseTrue()) {
				Results mergedResult = obj1.getIntercept(obj2);
				vector<pair<string, string>> res = mergedResult.getPairResults();

				if (usesSelectSyn(obj1) || usesSelectSyn(obj2)) {
					set<string> result = getSelectSynResult(mergedResult, selectList);
					return result;
				}
				
				if (!res.empty()) {
					set<string> result = getAllSynValues(selectList);
					return result;
				} 
			}
			return set<string>();

		default :
			// error
			return set<string>();
	}
	/*
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
	
}

set<string> QueryEvaluator::evaluateQuery(Query q) {
	vector<Clause> clauseList = q.getClauseList();
	vector<StringPair> selectList = q.getSelectList();
	
	if (clauseList.empty()) {
		set<string> result = getAllSynValues(selectList);
		return result;

	} else {
		vector<Results> resultsList;

		for (vector<Clause>::iterator iter = clauseList.begin() ; iter != clauseList.end(); iter++) {
			Clause c = *iter;
			Results res = c.evaluate();
			resultsList.push_back(res);
		}

		if (resultsList.size() == 1) {
			Results resultObj= resultsList.front();
			set<string> result = evaluateOneClause(resultObj, selectList);
			return result;

		} else {
			set<string> result = evaluateManyClause(resultsList, selectList);
			return result;
		}
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
}