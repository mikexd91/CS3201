#pragma once
#include <string>
#include <set>
#include <list>
#include <vector>
#include "boost/unordered_set.hpp"
#include "InsertType.h"
#include "ResultTable.h"

using namespace std;
using boost::unordered_set;

class InsertType;

class Result
{
public:

	Result(void);
	~Result(void); // how to clear all results, especially resultsTable.
	bool isPass(); 
	// checks if syn exist in resultsTable
	bool isSynPresent(string syn);

	// for clauses with 2 or more synonyms
	// the values in each vector returned follows the same order as that dictacted by the synList
	unordered_set<vector<string>> getMultiSyn(vector<string>); 
	// for clauses with 1 synonym
	unordered_set<string> getSyn(string);

	// called after all results have been inserted. push tells me what to delete
	void push(InsertType&);
	void setFail();
	void setPass();

	//for testing
	ResultTable getResultTable();
	void setResultTable(ResultTable&);

	//for food! :D

private:
	ResultTable resultTable;
	ResultTable resultTableTemp;
	bool pass;
 
	int getSynPosition(string);
};