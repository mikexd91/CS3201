#include "QueryEvaluator.h"
#include "Clause.h"
#include "StringPair.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ConstTable.h"

#include <boost\foreach.hpp>
#include <vector>
#include <sstream>
#include <iostream>

using namespace std;

QueryEvaluator::QueryEvaluator(void)
{
}

QueryEvaluator::~QueryEvaluator(void)
{
}

set<string> QueryEvaluator::getAllSynValues(vector<StringPair> selectList) {
	StringPair syn = selectList.at(0);
	string synType = syn.getSecond();
	set<string>* results = new set<string>();

	if (synType == stringconst::ARG_WHILE) {
		StmtTable* stmtTable = StmtTable::getInstance();
		set<Statement*> whileList = stmtTable->getWhileStmts();
		for (set<Statement*>::iterator iter = whileList.begin() ; iter != whileList.end(); iter++) {
			Statement* stmtObj = *iter;
			int stmtNum = stmtObj->getStmtNum();
			
			string s;
			ostringstream convert; // stringstream int to string
			convert << stmtNum;
			s = convert.str();
			results->insert(s);
		}
		return *results;

	} else if (synType == stringconst::ARG_STATEMENT) {
		StmtTable* stmtTable = StmtTable::getInstance();
		boost::unordered_map<int, Statement*>::iterator iter;
		for (iter = stmtTable->getIterator(); iter != stmtTable->getEnd(); iter++) {
			int stmtNum = iter->first;
			
			string s;
			ostringstream convert; // stringstream int to string
			convert << stmtNum;
			s = convert.str();
			results->insert(s); // might need to flush. currently not flushing
		}
		return *results;

	} else if (synType == stringconst::ARG_ASSIGN) {
		StmtTable* stmtTable = StmtTable::getInstance();
		set<Statement*> assignList = stmtTable->getAssgStmts();
		for (set<Statement*>::iterator iter = assignList.begin() ; iter != assignList.end(); iter++) {
			Statement* stmtObj = *iter;
			int stmtNum = stmtObj->getStmtNum();
			
			string s;
			ostringstream convert; // stringstream int to string
			convert << stmtNum;
			s = convert.str();
			results->insert(s);
		}
		return *results;

	} else if (synType == stringconst::ARG_VARIABLE) {
		VarTable* varTable = VarTable::getInstance();
		boost::unordered_map<string, Variable*>::iterator iter;
		for (iter = varTable->getIterator(); iter != varTable->getEnd(); iter++) {
			Variable v = *iter->second;
			string varName = v.getName();
			results->insert(varName); // might need to flush. currently not flushing
		}
		return *results;

	} else if (synType == stringconst::ARG_CONSTANT) {

		ConstTable* ctable = ConstTable::getInstance();
		vector<string> allConstName = ctable->getAllConstNames();
		
		BOOST_FOREACH(auto p, allConstName) {
			results->insert(p);
		}

		return *results;

	} else {
		//error
		return set<string>();
	}
	
}

// return the NUMBER of times syn appear in both obj1 and obj2
int QueryEvaluator::getSameClause(Results obj1, Results obj2) {
	int numSynObj1 = obj1.getNumOfSyn();
	int numSynObj2 = obj2.getNumOfSyn();

	if (numSynObj1 == 2 && numSynObj2 == 1) {
		int countr = getSameClause(obj2, obj1);
		return countr;
	}
	
	string firstSyn;
	string secondSyn;
	string thirdSyn;
	string forthSyn;
	int count = 0;

	if (numSynObj1 == 1 && numSynObj2 == 1) {
		firstSyn = obj1.getFirstClauseSyn();
		secondSyn = obj2.getFirstClauseSyn();
		if (firstSyn == secondSyn) {
			count++;
		} 
		return count;
	}

	if (numSynObj1 == 1 && numSynObj2 == 2) {
		firstSyn = obj1.getFirstClauseSyn();
		secondSyn = obj2.getFirstClauseSyn();
		thirdSyn = obj2.getSecondClauseSyn();

		if (firstSyn == secondSyn) {
			count++;
		}

		if (firstSyn == thirdSyn) {
			count++;
		}
		return count;
	}

	if (numSynObj1 == 2 && numSynObj2 == 2) {
		firstSyn = obj1.getFirstClauseSyn();
		secondSyn = obj1.getSecondClauseSyn();
		thirdSyn = obj2.getFirstClauseSyn();
		forthSyn = obj2.getSecondClauseSyn();

		if (firstSyn == thirdSyn) {
			count++;
		}

		if (secondSyn == thirdSyn) {
			count++;
		}

		if (secondSyn == forthSyn) {
			count++;
		}
		return count;
	}

	return count;
}

set<string> QueryEvaluator::evaluateOneClause(Results res, vector<StringPair> selectList) {
	string syn = selectList.at(0).getFirst();

	if (res.usesSyn(syn) && res.isClausePassed()) {

		set<string> result = res.getSelectSynResult(syn);
		return result;
	}

	if (!res.usesSyn(syn) && res.isClausePassed()) {

		set<string> result = getAllSynValues(selectList);
		return result;
	}

	return set<string>();
	
}

set<string> QueryEvaluator::evaluateManyClause(vector<Results> resultList, vector<StringPair> selectList) {
	Results obj1 = resultList.at(0);
	Results obj2 = resultList.at(1);
	int numRepeatingClause = getSameClause(obj1, obj2);
	string syn = selectList.at(0).getFirst();
	
	switch (numRepeatingClause) {
		case 0 : 
			if (obj1.isClausePassed() && obj2.isClausePassed()) {
				set<string> result = getAllSynValues(selectList);
				return result;

			} else {
				return set<string>();

			}

		case 1 :
			if (obj1.isClausePassed() && obj2.isClausePassed()) {
				obj1.getIntersect(obj2);

				if (obj1.usesSyn(syn)) {
					set<string> result = obj1.getSelectSynResult(syn);
					return result;
				}

				if (obj2.usesSyn(syn)) {
					set<string> result = obj2.getSelectSynResult(syn);
					return result;
				}

				if (!obj1.usesSyn(syn) && !obj2.usesSyn(syn)) {
					set<string> result = getAllSynValues(selectList);
					return result;
				}

				return set<string>();
			}
			
		case 2 :
			if (obj1.isClausePassed() && obj2.isClausePassed()) {
				obj1.getIntersect(obj2);

				if (obj1.usesSyn(syn)) {
					set<string> result = obj1.getSelectSynResult(syn);
					return result;
				
				} else {
					set<string> result = getAllSynValues(selectList);
					return result;
				}
				
			}
			return set<string>();

		default :
			// error
			return set<string>();
	}
	
}

set<string> QueryEvaluator::evaluateQuery(Query q) {
	vector<Clause*> clauseList = q.getClauseList();
	vector<StringPair> selectList = q.getSelectList();
	
	if (clauseList.empty()) {
		set<string> result = getAllSynValues(selectList);
		return result;

	} else {
		vector<Results> resultsList;

		for (vector<Clause*>::iterator iter = clauseList.begin() ; iter != clauseList.end(); iter++) {
			Clause* c = *iter;
			Results res = c->evaluate();
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
	
}