#pragma once
#include "Results.h"
#include "Query.h"
#include <set>
#include "boost/unordered_set.hpp"

//Query object to be passed between parser and evaluator
using namespace std;
using boost::unordered_set;

class QueryEvaluator
{
private:
	// get values from tables and store values in results obj
	void getValuesFromTables(vector<StringPair> selectList, Results &result); 

	// get values from results obj and create a new results obj with values of synonyms in selectList
	Results* getValuesFromResult(vector<StringPair> selectList, Results &result); 

	vector<StringPair> getSynonymNotInResult(vector<StringPair> selectList,  Results &obj);
	unordered_set<string> vectorToSet(vector<StringPair> selectList);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	Results* evaluateQuery(Query query);

};