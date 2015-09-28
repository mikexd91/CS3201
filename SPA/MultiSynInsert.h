#pragma once
#include <string>
#include <set>
#include <vector>
#include "InsertType.h"
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;

class MultiSynInsert : public InsertType
{
public:
	MultiSynInsert();
	//inserts the rows into the results table
	bool execute(ResultTable&);
	// for clauses with 2 or more synonyms
	// the values follows the same order as that dictacted by the synList
	void setSyns(vector<string>);
	void insertValues(vector<string>);

private:
	vector<string> multiInsertSyns;
	unordered_set<vector<string>> multiInsertValues;
	int getNumSynsInTable(ResultTable);
	vector<int> getSynPositionsInTable(ResultTable);
	void addNewRows(ResultTable&);
	void addSynToExistingRows(ResultTable&);
	void removeRows(ResultTable&);
	void combineRows(ResultTable&);
	void categoriseSyns(ResultTable, vector<int>&, vector<int>&, vector<int>&);
	void insertSyns(ResultTable&, vector<int>);
	void addToRow(Row&, vector<int>, vector<string>);
	bool toInsertRow(Row, vector<string>, vector<int>, vector<int>);		
};