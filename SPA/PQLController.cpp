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

set<string> PQLController::parse(string query) {
	// Parse query to query preprocessor (try catch?)
	// if query is valid, evaluate query with query evaluator
	// else return none
	// call query evaluator to format query
	// display query result
	try {
		Query q = *new Query();
		try {
			q = QueryParser::parseQuery(query);
		} catch (exception e) {
			cout << "parser not ok" << endl;
			cout << e.what() << endl;
		}

		QueryEvaluator* qe = new QueryEvaluator();
		set<string> results = set<string>();

		try {
			results = qe->evaluateQuery(q);
		} catch (exception e) {
			cout << "eval not ok" << endl;
			cout << e.what() << endl;
		}

		postProcess(results);

		return results;
	} catch (exception e) {
		//cout << "omg spoil" << endl;
		//cout << e.what() << endl;
		set<string>* empty = new set<string>();
		return *empty;
	}
}

void PQLController::postProcess(set<string>& results) {
	/*if (results.size() == 0) {
		results.emplace("none");
	}*/
}