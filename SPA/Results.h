#pragma once
#include <string>
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include <set>

using namespace std;
using boost::unordered_map;
using boost::unordered_set;

namespace ResultsConstants {
	const enum Category {
		EMPTY_TABLE = 0,
		BOTH_IN_TABLE = 1,
		ONE_IN_TABLE = 2,
		NONE_IN_TABLE = 3
	};
}

class Results
{
public:
	typedef unordered_map<string, string> Row;
	typedef unordered_set<unordered_map<string, string>*> ResultsTable;
	Results(void);
	~Results(void); // how to clear all results, especially resultsTable.
	bool test();
	bool test2();
	bool moveResultsToSet();
	bool isClausePass();

	// checks if syn exist in resultsTable
	bool hasResults(string syn);

	// we are using database query language as a guide, hence select instead of get
	
	// for clauses with 2 or more synonyms
	ResultsTable selectMultiSyn(unordered_set<string> synList); 
	// for clauses with 1 synonym
	unordered_set<string> selectSyn(string syn);
	
	// for clauses with 2 or more synonyms
	bool insertMultiResult(Row* results);
	// for clauses with 1 synonym
	bool insertResult(string syn, string value);
	// called after all results have been inserted. push tells me what to delete
	bool push();

	// Testing
	int getResultsTableSize();



private:
	/*
	struct Row {
		unordered_map<string, string> element;
	};

	struct ResultsTable {
		unordered_set<Row*> element;
	};
	*/

	/*
	ResTb testResultsTable;
	ResTb testResultsTableTemp;
	*/

	//to indicate to the query evaluator that the clause failed
	bool clausePassed;
	bool singleInsertFlag;
	bool multiInsertFlag;
	ResultsTable resultsTable;
	ResultsTable resultsTableTemp;
	ResultsTable multiInsertSet;
	set<pair<string, string>> singleInsertSet;
	set<string> constrainSyn;
	set<string> toAddSyn;

	void pushSingleSet();
	void pushMultiSet();
	void resetClauseFlags();
	bool isSynMatch(string key, string value, Row row);
	bool isConstrainFulfilled(Row synRow, Row resultsRow);
	ResultsConstants::Category getCategory();
	void filterNonResults();
	void combineWithRestrictions();
	void combineNewSyns();
	void createNewRows();
	void fillConstrainAndToAddSynSet();
	void addToResults(Row synRow, Row resultsRow);
	void setClauseFail();
	void setClausePass();
	Row* getDuplicateRow(Row row);
};


