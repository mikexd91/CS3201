#pragma once
#include <string>
#include <set>
#include <vector>
#include "InsertType.h"
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;

class MultiSynInsert : InsertType
{
public:
	//inserts the rows into the results table
	void execute(Result::ResultTable&);
	// for clauses with 2 or more synonyms
	// the values follows the same order as that dictacted by the synList
	bool setSyns(vector<string>);
	bool insertValues(vector<string>);

private:
	vector<string> multiInsertSyns;
	unordered_set<vector<string>> multiInsertValues;

};