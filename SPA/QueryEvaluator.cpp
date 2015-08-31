#include "QueryEvaluator.h"
#include "Clause.h"
#include "StringPair.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ConstTable.h"
#include "boost/unordered_set.hpp"

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

/*
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

*/

/*
1) checks if clauseList is empty
2a) empty clauseList, return all values in selectList
2b) clauseList is not empty, create a new Results obj and pass obj into clause.evaluate
3) after each clause, check if clausePass == true. if clausePass == true, continue.
if clausePass == false, stop and return empty set<string>
4) get select synonym from results obj 
5) return set<string>
*/
Results* QueryEvaluator::evaluateQuery(Query q) {
	Results *obj = new Results();
	vector<Clause*> clauseList = q.getClauseList();
	vector<StringPair> selectList = q.getSelectList();
	if (clauseList.empty()) {
		getValuesFromTables(selectList, *obj);
		return obj;

	} else {
		for (vector<Clause*>::iterator i = clauseList.begin(); i != clauseList.end(); ++i) {
			(*i)->evaluate(obj);

			if (!obj->isClausePass()) {
				return NULL;
			} else {
				obj->setClauseFail();
			}
		}
		Results* processedObj = getValuesFromResult(selectList, *obj);
		delete obj;
		return processedObj;
	}
}

// 1) for each synonym in selectList, it will get values of the synonym type
// 2) values will be inserted into results obj with insertSyn
void QueryEvaluator::getValuesFromTables(vector<StringPair> selectList,  Results &obj) {
	
}

// 1. check which synonym in selectList are not in results table
// 2. getValuesFromTables for 1.
// 3. get selectList synonyms from resultsTable
// 4. put results from 3. into a new results obj
// 5. return obj in 4.
Results* QueryEvaluator::getValuesFromResult(vector<StringPair> selectList, Results &obj) {
	vector<StringPair> synToAddToResults = getSynonymNotInResult(selectList, obj);
	if (synToAddToResults.size() > 0) {
		getValuesFromTables(synToAddToResults, obj);
	}
	
	unordered_set<string> synList = vectorToSet(selectList);
	Results::ResultsTable res = obj.selectMultiSyn(synList);
	Results* processedRes = new Results();
	insertProcessResults(res, *processedRes);

	return processedRes;
}

vector<StringPair> QueryEvaluator::getSynonymNotInResult(vector<StringPair> selectList,  Results &obj) {
	vector<StringPair> synNotInResults = vector<StringPair>();
	StringPair p;
	string synonym;
	
	for (vector<StringPair>::iterator i = selectList.begin(); i != selectList.end(); ++i) {
		p = *i;
		synonym = p.getFirst();
		if (!obj.hasResults(synonym)) {
			synNotInResults.push_back(p);
		}
	}
	return synNotInResults;
}

unordered_set<string> QueryEvaluator::vectorToSet(vector<StringPair> selectList) {
	unordered_set<string> synList = unordered_set<string>();
	string syn;

	for (vector<StringPair>::iterator i = selectList.begin(); i != selectList.end(); ++i) {
		syn = i->getFirst();
		synList.insert(syn);
	}
	return synList;
}

void QueryEvaluator::insertProcessResults(Results::ResultsTable resultsTble, Results &obj) {
	Results::Row resTbleRow;
	for (Results::ResultsTable::iterator i = resultsTble.begin(); i != resultsTble.end(); ++i) {
		Results::Row* newRow = new Results::Row();
		resTbleRow = *(*i);
		for (Results::Row::iterator j = resTbleRow.begin(); j != resTbleRow.end(); ++j) {
			string key = j->first;
			string value = j->second;
			(*newRow)[key] = value;
		}
		obj.insertMultiResult(newRow);
	}
}