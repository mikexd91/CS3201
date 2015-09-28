#pragma once
#include <string>
#include <set>
#include <vector>
#include "InsertType.h"
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;
class SingleSynInsert : public InsertType
{
public:
	SingleSynInsert(void);
	//inserts the rows into the results table
	bool execute(ResultTable&);
	// for clauses with 1 synonym
	void setSyn(string);
	void insertValue(string);

private:
	string singleSyn;
	unordered_set<string> singleInsertValues;
};