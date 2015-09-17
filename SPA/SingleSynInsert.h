#pragma once
#include <string>
#include <set>
#include <vector>
#include "InsertType.h"
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;

class SingleSynInsert : InsertType
{
public:
	//inserts the rows into the results table
	void execute(Result::ResultTable&);
	// for clauses with 1 synonym
	bool insertSingleSyn(string syn, unordered_set<string> values);

private:
	string singleValue;
	unordered_set<vector<string>> singleInsertValues;
}