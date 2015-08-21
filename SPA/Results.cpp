#pragma once
#include "Results.h"

using namespace std;

Results::Results(void)
{
}

Results::~Results(void)
{
}

bool Results::isClausePass() {
	return this->clausePassed;
}

void Results::setClausePass() {
	this->clausePassed = true;
}

void Results::resetClausePass() {
	this->clausePassed = false;
}

/*
This is a helper function used by selectMultiSyn.
It helps retrieve all values in row that have the same synonym
as that in synList.
*/
unordered_map<string, string> Results::getSynResults(unordered_map<string, string> row, vector<string> synList) {
	unordered_map<string, string> results;
	string value;

	for (vector<string>::iterator j = synList.begin(); j != synList.end(); ++j) {
		value = row[*j];
		
		if (value.length() > 0) {
			results[*j] = value;
		} 
	}
	
	return results;
}

/*
This is a helper function used by hasResults.
It checks if resultsTable contains any results.
*/
bool Results::isResultsTableEmpty() {
	if (this->resultsTable.empty() || 
		this->resultsTable.at(0).empty()) {
		return true;
	} else {
		return false;
	}
}

/*
This function checks if the synonym contains any results.
*/
bool Results::hasResults(string syn) {
	if (!isResultsTableEmpty()) {
		unordered_map<string, string> row = this->resultsTable.at(0);
		string value = row[syn];
		
		if (value.length() > 0) {
			return true;
		} else {
			return false;
		}

	} else {
		return false;
	}
}


/*
This function returns paired results. If results for a synonym in 
the list of synonyms in synList does not exist, results returned will
not contain missing synonym's results. Hence, check for the synonym's
existance before using select functions.
*/
vector<unordered_map<string, string>>* Results::selectMultiSyn(vector<string> synList) {
	unordered_map<string, string> row;
	unordered_map<string, string> rowResults;
	set<unordered_map<string, string>> uniqueValues;
	vector<unordered_map<string, string>>* results = new vector<unordered_map<string, string>>();
	
	for (vector<unordered_map<string, string>>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		row = *i;
		rowResults = getSynResults(row, synList);	

		if (!rowResults.empty()) {
			uniqueValues.insert(rowResults);
		}
	}

	for (set<unordered_map<string, string>>::iterator i = uniqueValues.begin(); i != uniqueValues.end(); ++i) {
		row = *i;
		results->push_back(row);
	}

	return results;
}

/*
This function returns results paired with synonym syn. Results returned
are distinct. If there are no results associated with that synonym,
results returned will be an empty vector.
*/
vector<string>* Results::selectSyn(string syn) {
	unordered_map<string, string> row;
	string value;
	set<string> uniqueValues;
	vector<string>* results = new vector<string>();

	for (vector<unordered_map<string, string>>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		row = *i;
		value = row[syn];
		
		if (value.length() > 0) {
			uniqueValues.insert(value);
		}
	}

	for (set<string>::iterator i = uniqueValues.begin(); i != uniqueValues.end(); i++) {
		value = *i;
		results->push_back(value);
	}

	return results;
}

/*
This function gets results row that are associated with the following
synonym and value from the vector of unordered_map it is provided with.
*/
vector<unordered_map<string, string>>* Results::selectSynWhere(string syn, string val, vector<unordered_map<string, string>> res) {
	vector<unordered_map<string, string>>* results = new vector<unordered_map<string, string>>();
	if (syn.length() > 0 && val.length() > 0) {
		unordered_map<string, string> row;
		string value;

		for (vector<unordered_map<string, string>>::iterator i = res.begin(); i != res.end(); ++i) {
			row = *i;
			value = row[syn];
			if (value == val) {
				results->push_back(row);
			}
		}

	} 
	
	return results;
}

/*
When there are results in insertStack, insertBufferFlag will
be true.
*/
void Results::setInsertBufferFlag() {
	insertBufferFlag = true;
}

/*
When the results object is returned to the evaluator, the
insert flag will be reset
*/
void Results::resetInsertBufferFlag() {
	insertBufferFlag = false;
}

/*
This function inserts the value of the given list of synonyms into 
the resultsTable. It does not require the synonyms to exist beforehand
for the insertion to work. It acts as a buffer and
requires the function pushChanges to enter results into the resultsTable.
*/
bool Results::insertMultiResult(unordered_map<string, string> results) {
	if (results.size() > 0) {
		insertStack.insert(results);
		setInsertBufferFlag();
		return true;
	} else {
		return false;
	}
}

/*
This function inserts the value of the single given synonym into 
the resultsTable. It does not require the synonym to exist beforehand
for the insertion to work. It acts as a buffer and
requires the function pushChanges to enter results into the resultsTable.
*/
bool Results::insertResult(string syn, string value) {
	if (syn.length() > 0 && value.length() > 0) {
		unordered_map<string, string> result;
		result[syn] = value;
		insertStack.insert(result);
		setInsertBufferFlag();
		return true;
	} else {
		return false;
	}
}

/*
This is a helper function to push inserted results into resultsTable.
It is called by pushChanges
*/
bool Results::pushInsertResults() {
	unordered_map<string, string> row;
	unordered_map<string, string> stackRow;
	string key;
	string value;
	int initialSize = resultsTable.size();
		
	for (int i = 0; i < initialSize; ++i) {
		row = resultsTable.at(i);
		for (set<unordered_map<string, string>>::iterator j = insertStack.begin(); j != insertStack.end(); ++j) {
			stackRow = *j;
			for (unordered_map<string, string>::iterator k = stackRow.begin(); k != stackRow.end(); ++k) {
				key = k->first;
				value = k->second;
				row[key] = value;
			}

			resultsTable.push_back(row);
		}
	}

	resultsTable.erase(resultsTable.begin(), resultsTable.begin() + initialSize);
	return true;
}

/*
This is a helper function to push deleted results changes into resultsTable.
It is called by pushChanges
*/
bool Results::pushDelResults() {
	unordered_map<string, string> row;
	unordered_map<string, string> stackRow;
	vector<int> rowNumber;
	int position;
	string key;
	string value;
	string resultValue;

	for (vector<unordered_map<string, string>>::iterator i = resultsTable.begin(); i != resultsTable.end(); ++i) {
		row = *i;
		for (set<unordered_map<string, string>>::iterator j = delStack.begin(); j != delStack.end(); ++j) {
			stackRow = *j;
			for (unordered_map<string, string>::iterator k = stackRow.begin(); k != stackRow.end(); ++k) {
				key = k->first;
				value = k->second;
				resultValue = row[key];

				if (resultValue == value) {
					position = i - resultsTable.begin();
					rowNumber.push_back(position);
				}
			}
		}
	}

	for (vector<int>::iterator i = rowNumber.begin(); i != rowNumber.end(); ++i) {
		resultsTable.erase(resultsTable.begin() + *i);
	}

	return true;
}

/*
This function is called after insertResult or insertMultiResult.
It enters the results into the resultsTable.
*/
bool Results::pushChanges() {
	if (insertBufferFlag) {
		pushInsertResults();
		return true;
	} else if (delBufferFlag) {
		pushDelResults();
		return true;
	} else {
		return false;
	}
}

/*
When there are results in delStack, delBufferFlag will
be true.
*/
void Results::setDelBufferFlag() {
	delBufferFlag = true;
}

/*
When the results object is returned to the evaluator, the
delete flag will be reset
*/
void Results::resetDelBufferFlag() {
	delBufferFlag = false;
}

/*
This function deletes the associated results of the given list of synonyms in 
the resultsTable. It does not require the synonyms to exist beforehand
for the deletion to work. It acts as a buffer and
requires the function pushChanges to enter results into the resultsTable.
*/
bool Results::delMultiResult(unordered_map<string, string> results) {
	if (results.size() > 0) {
		delStack.insert(results);
		setDelBufferFlag();
		return true;
	} else {
		return false;
	}
}

/*
This function deletes the value of the single given synonym in 
the resultsTable. It does not require the synonym to exist beforehand
for the deletion to work. It acts as a buffer and
requires the function pushChanges to enter results into the resultsTable.
*/
bool Results::delResult(string syn, string value) {
	if (syn.length() > 0 && value.length() > 0) {
		unordered_map<string, string> result;
		result[syn] = value;
		delStack.insert(result);
		setDelBufferFlag();
		return true;
	} else {
		return false;
	}
}
