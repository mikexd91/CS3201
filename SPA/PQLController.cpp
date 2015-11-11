#include "PQLController.h"
#include "QueryParser.h"
#include "QueryEvaluator.h"
#include "QueryOptimiser.h"
#include "Result.h"
#include <string>
#include <iostream>

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
		QueryParser* parser = QueryParser::getInstance();
		Query* q= parser->parseQuery(query);

		QueryOptimiser* qo = new QueryOptimiser();
		vector<int>* optimisedQ = qo->optimizeQuery(q);
		
		boost::unordered_set<string> valueSet;
		QueryEvaluator* qe = new QueryEvaluator();
		if (optimisedQ->size() > 0) { 
			Result* resObj = qe->evalOptimisedQuery(q, optimisedQ);
			vector<StringPair> selectList = q->getSelectList();
			valueSet = qe->printValues(resObj, selectList);
		
		} else {
			valueSet = qe->printInvalid(q);
		}

		return valueSet;

	} catch (std::exception& e) {
		cout << "Evalutaion Failed...";
		cout << e.what() << endl;
		unordered_set<string> empty;
		return empty;
	}
}
