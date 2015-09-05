#pragma once
#include <string>
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include <set>
#include <list>

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

	//hashing function for row in results table
	struct RowHash {
		size_t operator()(Row* const& r) const {
			size_t seed = 0;
			//first get keys of row
			list<string> keys;
			for (unordered_map<string, string>::const_iterator it = (*r).begin(); it != (*r).end(); ++it) {
				  keys.push_back(it->first);
			}
			keys.sort();

			for (list<string>::iterator keyIter = keys.begin(); keyIter != keys.end(); ++keyIter) {
				string currentKey = *keyIter;
				//cannot use r[currentKey] as [] operator is a non-const function
				const string value = (*r)[currentKey];
				boost::hash_combine(seed, currentKey);
				boost::hash_combine(seed, value);
			}
			return seed;
		};
	};

	//equality function for row in results table
	struct RowEquality {
		bool operator() (Row* const& r1, Row* const& r2) const {
			if (r1->size() != r2->size()) {
				return false;
			}
			list<string> keys;
			for (unordered_map<string, string>::const_iterator it = r1->begin(); it != r1->end(); ++it) {
				  keys.push_back(it->first);
			}
			keys.sort();

			for (list<string>::iterator keyIter = keys.begin(); keyIter != keys.end(); ++keyIter) {
				string currentKey = *keyIter;
				const string value1 = (*r1)[currentKey];
				const string value2 = (*r2)[currentKey];
				if (value1 != value2) {
					return false;
				}
			}
			return true;
		};
	};

	//for storing of rows in results tables
	typedef unordered_set<Row*, RowHash, RowEquality> ResultsTable;
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
	void setClauseFail();
	void setClausePass();

	// Testing
	int getResultsTableSize();
	ResultsTable getResultsTable();




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
	Row* getDuplicateRow(Row row);
};


