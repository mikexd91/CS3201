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
	//existing row in results table
	Row* resultsRow;
	//row from clause
	Row synRow;
	string key;
	string value;
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
	for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		resultsRow = *i;
		for (unordered_set<Row*>::iterator j = multiInsertSet.begin(); j != multiInsertSet.end(); ++j) {
			bool toInsert = true;
			synRow = *(*j);
			for (unordered_map<string, string>::iterator k = synRow.begin(); k != synRow.end(); ++k) {
				// check if syn matches in resultsRow
				key = k->first;
				value = k->second;
				//if the current result does not match the one provided by the clause (e.g. a =2 in current table and a = 3 from clause) 
				//we know that we should not add the resultsRow in for this iteration (we shouldn't add a=2,b=3 in for a=3,b=3 for an example)
				//so we do not need to continue checking for the rest of the the other synonyms in that set (like c and so on)
				if (!isSynMatch(key, value, *resultsRow)) {
					toInsert = false;
					break;
				}
			}
			//if we have already inserted the row once
			//we do not need to check if it matches the other results given from the clause
			if (toInsert) {
				resultsTableTemp.insert(resultsRow);
				break;
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

void Results::createNewRows() {
	for (unordered_set<Row*>::iterator j = multiInsertSet.begin(); j != multiInsertSet.end(); ++j) {
		Row* resultsRow = new Row();
		Row synRow = *(*j);
		for (unordered_map<string, string>::iterator k = synRow.begin(); k != synRow.end(); ++k) {
			string key = k->first;
			string value = k->second;
			(*resultsRow)[key] = value;
		}
		resultsTableTemp.insert(resultsRow);
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
			Row* newRow = getDuplicateRow(resultsRow);
			for (unordered_map<string, string>::iterator k = synRow.begin(); k != synRow.end(); ++k) {
				key = k->first;
				value = k->second;
				(*newRow)[key] = value;
			}
			resultsTableTemp.insert(newRow);
		}
	}
}

ResultsConstants::Category Results::getCategory() {
	unordered_set<Row*>::iterator iter = multiInsertSet.begin();
	Row firstRow = *(*iter);
	string synonym;
	int synListSize = firstRow.size();
	int numSynNotInResultsTable = 0;
	if(resultsTable.empty()) {
		return ResultsConstants::EMPTY_TABLE;
	}

	for (unordered_map<string, string>::iterator i = firstRow.begin(); i != firstRow.end(); ++i) {
		synonym = i->first;

		if (!hasResults(synonym)) {
			numSynNotInResultsTable++;
		}
	}

	if (numSynNotInResultsTable == 0) {
		return ResultsConstants::BOTH_IN_TABLE;
	} else if (numSynNotInResultsTable == synListSize) {
		return ResultsConstants::NONE_IN_TABLE;
	} else {
		return ResultsConstants::ONE_IN_TABLE;
	}
}

void Results::pushSingleSet() {
	string key;
	string value;

	if (resultsTable.empty()) {
		//initialise new rows
		for (set<pair<string, string>>::iterator j = singleInsertSet.begin(); j != singleInsertSet.end(); ++j) {
			Row* row = new Row();
			key = j->first;
			value = j->second;
			(*row)[key] = value;
			resultsTableTemp.insert(row);
		}
	} else {
		for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
			Row* row = *i;
			for (set<pair<string, string>>::iterator j = singleInsertSet.begin(); j != singleInsertSet.end(); ++j) {
				key = j->first;
				value = j->second;
				if (hasResults(key)) {
					if (isSynMatch(key, value, *row)) {
						resultsTableTemp.insert(row);
					} 
				} else {
					//Scenario where one needs to duplicate the row to add new synonym
					//e.g.: <a:2, b:3> existing, we want to add s: 2 and s: 3, need to duplicate <a: 2, b: 3>
					Row* newRow = getDuplicateRow(*row);
					//synonym is not present in the row yet. 
					(*newRow)[key] = value;
					resultsTableTemp.insert(newRow);
				}
			}
		}

		resultsTable.clear();
		resultsTable = resultsTableTemp;
		resultsTableTemp.clear();
		singleInsertSet.clear();
	}
}

Results::Row* Results::getDuplicateRow(Row row) {
	Row* newRow = new Row(); 
	for (Row::iterator k = row.begin(); k != row.end(); ++k) {
		string existingKey = k->first;
		string existingValue = k -> second;
		(*newRow)[existingKey] = existingValue;
	}
	return newRow;
}

void Results::pushMultiSet() {
	ResultsConstants::Category category = getCategory();
	string categoryArr[] = {"know all", "know some", "know none"};
	
	switch (category) {
		case ResultsConstants::BOTH_IN_TABLE: 
			filterNonResults();
			break;
		case ResultsConstants::ONE_IN_TABLE:
			combineWithRestrictions();
			break;
		case ResultsConstants::NONE_IN_TABLE:
			combineNewSyns();
			break;
		case ResultsConstants::EMPTY_TABLE:
			createNewRows();
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
	(*row)["a"] = "99";
	resultsTable.insert(row);
	if (resultsTable.size() > 0) {
		ResultsTable::iterator iter = resultsTable.begin();
		Row row1 = *(*iter);
		string value = row1["a"];
		if (value.length() > 0) {
			return true;
		}
		
	}
	return true;
}

bool Results::test2() {
	
	return false;
}

/* CORE FUNCTIONS */

Results::Results(void)
{
	//will be set to false by the Results class if no result is inserted by the clause
	clausePassed = true;
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
	
void Results::setClauseFail() {
	clausePassed = false;
}

void Results::resetClauseFlags() {
	singleInsertFlag = false;
	multiInsertFlag = false;
}

// checks if syn exist in resultsTable
bool Results::hasResults(string syn) {
	if (resultsTable.size() > 0) {
		unordered_set<Row*>::iterator iter = resultsTable.begin();
		Row row = *(*iter);
		string value = row[syn];
		if (value.length() > 0) {
			return true;
		}	
	}
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
unordered_set<string> Results::selectSyn(string syn) {
	unordered_set<string> rtnSet = unordered_set<string>();
	
	if (resultsTable.size() > 0 && syn.length() > 0) {
		Row results;
		string value;

		for (unordered_set<Row*>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
			results = *(*i);
			value = results[syn];
			rtnSet.insert(value);
		}
		
	}
	return rtnSet;
}
	
// for clauses with 2 or more synonyms
bool Results::insertMultiResult(Row* results) {
	if ((*results).size() > 0) {
		multiInsertSet.insert(results);
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
		//no result is inserted -> clause returned null
		resultsTable.clear();
		setClauseFail();
		return false;
	}
}

int Results::getResultsTableSize() {
	return resultsTable.size();
}

//For testing

int main() {

	Results r = Results();
	//r.insertResult("s", "2");
	//r.insertResult("s", "3");
	//Test initalising results row with multi-syn
	Results::Row* row = new Results::Row();
	(*row)["a"] = "2";
	(*row)["b"] = "3";
	Results::Row* row1 = new Results::Row();
	(*row1)["a"] = "1";
	(*row1)["b"] = "3";
	Results::Row* row2 = new Results::Row();
	(*row2)["a"] = "3";
	(*row2)["b"] = "3";
	Results::Row* row3 = new Results::Row();
	(*row3)["a"] = "2";
	(*row3)["b"] = "4";
	r.insertMultiResult(row);
	r.insertMultiResult(row1);
	r.insertMultiResult(row2);
	r.insertMultiResult(row3);
	r.push();
	//Test appending results to rows (need to duplicate)
	r.insertResult("s", "2");
	r.insertResult("s", "3");
	r.push();
	/**
	//Test elimination of results
	r.insertResult("b", "3");
	r.push();
	**/
	//Test elimination of results for multi-syn
	Results::Row* row4 = new Results::Row();
	(*row4)["a"] = "2";
	(*row4)["b"] = "3";
	Results::Row* row5 = new Results::Row();
	(*row5)["a"] = "1";
	(*row5)["b"] = "3";
	r.insertMultiResult(row4);
	r.insertMultiResult(row5);
	r.push();
	//Test elimination of results for multi-syn
	Results::Row* row6 = new Results::Row();
	(*row6)["c"] = "2";
	(*row6)["d"] = "4";
	r.insertMultiResult(row6);
	r.push();
	unordered_set<string> test = r.selectSyn("s");
}

