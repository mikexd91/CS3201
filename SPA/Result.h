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
	~Result(void);

	void setFail();
	bool isPass(); 
	bool isSynPresent(string syn);
	unordered_set<vector<string>> getMultiSyn(vector<string>); 
	unordered_set<string> getSyn(string);
	bool push(InsertType&);

	//for testing
	ResultTable getResultTable();
	void setResultTable(ResultTable&);
	int getResultTableSize();

private:
	ResultTable resultTable;
	bool pass;
 
	int getSynPosition(string);
};