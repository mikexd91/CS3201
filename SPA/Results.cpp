#pragma once
#include "Results.h"
#include <iostream>

using namespace std;

/* HELPER FUNCTIONS */
bool Results::isSynMatch(string key, string value, Row row) {
	string val = row[key];
	if (value == val) {
		return true;
	} else {
		return false;
	}
}

bool Results::isConstrainFulfilled(Row synRow, Row resultsRow) {
	string syn;
	string value;

	for (set<string>::iterator i = constrainSyn.begin(); i != constrainSyn.end(); ++i) {
		syn = *i;
		value = synRow[syn];
		if (!isSynMatch(syn, value, resultsRow)) {
			return false;
		}
	}
	return true;
}

void Results::addToResults(Row synRow, Row resultsRow) {
	Row row;
	string syn;
	string value;

	for (set<string>::iterator i = toAddSyn.begin(); i != toAddSyn.end(); ++i) {
		row = resultsRow;
		syn = *i;
		value = synRow[syn];
		row[syn] = value;
		resultsTableTemp.insert(&row);
	}
}

void Results::fillConstrainAndToAddSynSet() {
	constrainSyn = set<string>();
	unordered_set<Row*>::iterator j = multiInsertSet.begin();
	Row insertRow = *(*j);
	string syn;

	for (unordered_map<string, string>::iterator iter = insertRow.begin(); iter != insertRow.end(); ++iter) {
		syn = iter->first;
		if (hasResults(syn)) {
			constrainSyn.insert(syn);
		} else {
			toAddSyn.insert(syn);
		}
	}
}

void Results::filterNonResults() {
	Row resultsRow;
	Row synRow;
	int count = 0;
	int size;
	string key;
	string value;

	for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		resultsRow = *(*i);
		for (unordered_set<Row*>::iterator j = multiInsertSet.begin(); j != multiInsertSet.end(); ++j) {
			synRow = *(*j);
			size = synRow.size();
			for (unordered_map<string, string>::iterator k = synRow.begin(); k != synRow.end(); ++k) {
				// check if syn matches in resultsRow
				key = k->first;
				value = k->second;
				if (isSynMatch(key, value, resultsRow)) {
					count++;
				}
			}
			
			//check if matches number == j.size
			if (count == size) {
				resultsTableTemp.insert(&resultsRow);
			}
		}
	}


}

void Results::combineWithRestrictions() {
	fillConstrainAndToAddSynSet();
	Row resultsRow;
	Row synRow;
	string syn;
	string value;

	for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		resultsRow = *(*i);
		for (unordered_set<Row*>::iterator j = multiInsertSet.begin(); j != multiInsertSet.end(); ++j) {
			synRow = *(*j);
			if (isConstrainFulfilled(synRow, resultsRow)) {
				addToResults(synRow, resultsRow);
			}
		}
	}
}

void Results::combineNewSyns() {
	Row resultsRow;
	Row synRow;
	string key;
	string value;

	for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		resultsRow = *(*i);
		for (unordered_set<Row*>::iterator j = multiInsertSet.begin(); j != multiInsertSet.end(); ++j) {
			synRow = *(*j);
			for (unordered_map<string, string>::iterator k = synRow.begin(); k != synRow.end(); ++k) {
				key = k->first;
				value = k->second;
				resultsRow[key] = value;
				resultsTableTemp.insert(&resultsRow);
			}
		}
	}
}

int Results::getCategory() {
	unordered_set<Row*>::iterator iter = multiInsertSet.begin();
	Row firstRow = *(*iter);
	string synonym;
	int synListSize = firstRow.size();
	int numSynNotInResultsTable = 0;

	for (unordered_map<string, string>::iterator i = firstRow.begin(); i != firstRow.end(); ++i) {
		synonym = i->first;

		if (!hasResults(synonym)) {
			numSynNotInResultsTable++;
		}
	}

	if (numSynNotInResultsTable == 0) {
		return 0;
	} else if (numSynNotInResultsTable == synListSize) {
		return 2;
	} else {
		return 3;
	}
}

void Results::pushSingleSet() {
	Row row;
	string key;
	string value;

	for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		row = *(*i);
		for (set<pair<string, string>>::iterator j = singleInsertSet.begin(); j != singleInsertSet.end(); ++j) {
			key = j->first;
			value = j->second;
			if (hasResults(key)) {
				if (isSynMatch(key, value, row)) {
					resultsTableTemp.insert(&row);
				} 
			} else {
				row[key] = value;
				resultsTableTemp.insert(&row);
			}
		}
	}

	resultsTable.clear();
	resultsTable = resultsTableTemp;
	resultsTableTemp.clear();
}

void Results::pushMultiSet() {
	int category = getCategory();
	string categoryArr[] = {"know all", "know some", "know none"};
	
	switch (category) {
		case 0: 
			filterNonResults();
			break;
		case 1:
			combineWithRestrictions();
			break;
		case 2:
			combineNewSyns();
			break;
		default:
			// category returned does not match any of the above
			// category in categoryArr[]. Throw error?
			break;
	}

	resultsTable = resultsTableTemp;
	resultsTableTemp.clear();
	multiInsertSet.clear();
	constrainSyn.clear();
	toAddSyn.clear();
}

bool Results::moveResultsToSet() {
	Row row;
	
	for (unordered_set<Row*>::iterator i = resultsTableTemp.begin(); i != resultsTableTemp.end(); ++i) {
		row = *(*i);
		resultsTable.insert(&row);
	}
	return true;
} 

bool Results::test() {
	resultsTable = ResultsTable();
	Row* row = new Row();
	row["a"] = "99";
	row["b"] = "lol";
	resultsTable.insert(&row);
	/*
	if (resultsTable.size() > 0) {
		ResultsTable::iterator iter = resultsTable.begin();
		Row row1 = *(*iter);
		string value = row1["a"];
		if (value.length() > 0) {
			return true;
		}
		
	}
	*/
	return true;
}

bool Results::test2() {
	
	return false;
}

/* CORE FUNCTIONS */

Results::Results(void)
{
	clausePassed = false;
	singleInsertFlag = false;
	multiInsertFlag = false;

	resultsTable = ResultsTable();
	resultsTableTemp = ResultsTable();
	multiInsertSet = ResultsTable();
	singleInsertSet = set<pair<string, string>>();
	constrainSyn = set<string>();
	toAddSyn = set<string>();
}

Results::~Results(void)
{
}

bool Results::isClausePass() {
	return clausePassed;
}

void Results::setClausePass() {
	clausePassed = true;
}
	
void Results::resetClausePass() {
	clausePassed = false;
}

void Results::resetClauseFlags() {
	singleInsertFlag = false;
	multiInsertFlag = false;
}

// checks if syn exist in resultsTable
bool Results::hasResults(string syn) {
	/*
	unordered_map<string, string>::iterator rowIter;
	string value;
	Row row;
	*/
	if (resultsTable.size() > 0) {
		ResultsTable::iterator iter = resultsTable.begin();
		Row* row1 = *iter;
		string value = row1->begin()->second;
		cout  << value;
		if (value.length() > 0) {
			return true;
		}
		
	}
	/*
	if (resultsTable.size() > 0) {
		unordered_set<Row*>::iterator iter = resultsTable.begin();
		row = *(*iter);
		value = row[syn];
		if (value.length() > 0) {
			return true;
		}
		
	}
	*/
	return false;
}

// for clauses with 2 or more synonyms
Results::ResultsTable Results::selectMultiSyn(unordered_set<string> synList) {
	ResultsTable rtnTable = ResultsTable();
	
	if (resultsTable.size() > 0 && synList.size() > 0) {
		string syn;
		Row results;
		Row row;
		
		for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
			for (unordered_set<string>::iterator j = synList.begin(); j != synList.end(); ++j) {
				syn = *j;
				results = *(*i);
				row[syn] = results[syn];
				rtnTable.insert(&row);
			}
		}
		
	}

	return rtnTable;
}
	
// for clauses with 1 synonym
unordered_set<string*> Results::selectSyn(string syn) {
	unordered_set<string*> rtnSet = unordered_set<string*>();
	
	if (resultsTable.size() > 0 && syn.length() > 0) {
		Row results;
		string value;

		for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
			results = *(*i);
			value = results[syn];
			rtnSet.insert(&value);
		}
		
	}
	
	return rtnSet;
}
	
// for clauses with 2 or more synonyms
bool Results::insertMultiResult(Row results) {
	if (results.size() > 0) {
		multiInsertSet.insert(&results);
		multiInsertFlag = true;
		return true;
	}
	return false;
}
// for clauses with 1 synonym
bool Results::insertResult(string syn, string value) {
	if (syn.length() > 0 && value.length() > 0) {
		pair<string, string> p = make_pair(syn, value);
		singleInsertSet.insert(p);
		singleInsertFlag = true;
		return true;
	}
	return false;
}
// called after all results have been inserted. push tells me what to delete
bool Results::push() {
	if (singleInsertFlag) {
		pushSingleSet();
		resetClauseFlags();
		return true;
	} else if (multiInsertFlag) {
		pushMultiSet();
		resetClauseFlags();
		return true;
	} else {
		return false;
	}
}

int Results::getResultsTableSize() {
	return resultsTable.size();
}
