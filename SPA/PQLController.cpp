#include "PQLController.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include <string>
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
		q = QueryParser::parseQuery(query);
		QueryEvaluator* qe = new QueryEvaluator();
		set<string> results = qe->evaluateQuery(q);

		postProcess(results);

		return results;
	} catch (...){
		set<string>* empty = new set<string>();
		return *empty;
	}
}

void PQLController::postProcess(set<string>& results) {
	/*if (results.size() == 0) {
		results.emplace("none");
	}*/
}