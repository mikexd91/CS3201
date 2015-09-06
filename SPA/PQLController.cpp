#include "PQLController.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include <string>
#include <iostream>
#include <set>

using namespace std;


PQLController::PQLController(void) {

}


PQLController::~PQLController(void) {
}

unordered_set<string> PQLController::parse(string query) {
	// Parse query to query preprocessor (try catch?)
	// if query is valid, evaluate query with query evaluator
	// else return none
	// call query evaluator to format query
	// display query result
	try {
		Query* q = new Query();
		q = &QueryParser::parseQuery(query);

		QueryEvaluator* qe = new QueryEvaluator();
		Results* resObj = qe->evaluateQuery(*q);
		vector<StringPair> selectList = q->getSelectList();
		unordered_set<string> valueSet = qe->getValuesToPrint(resObj, selectList);
		
		delete q;
		delete resObj;
		delete qe;

		return valueSet;

		} catch (std::exception& e) {
			cout << "eval not ok" << endl;
			cout << e.what() << endl;
			unordered_set<string> empty;
			return empty;
		}
}
