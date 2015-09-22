#include "Result.h"

Result::Result() {
	resultTable = ResultTable();
}

Result::~Result() {
}


void Result::push(InsertType& insert) {
	insert.execute(resultTable);
	pass = !resultTable.synList.empty() && !resultTable.rows.empty();
}

ResultTable Result::getResultTable() {
	return resultTable;
}
void Result::setResultTable(ResultTable& table) {
	resultTable = table;
}

// if a synonym in synList cannot be found in resultTable, 
// an empty unordered_set<vector<string>> will be returned
unordered_set<vector<string>> Result::getMultiSyn(vector<string> synList) {
	vector<int> synPosition = vector<int>();
	unordered_set<vector<string>> res = unordered_set<vector<string>>();
	for (auto iter = synList.begin(); iter != synList.end(); ++iter) {
		string syn = *iter;
		if (syn.length() > 0 && isSynPresent(syn)) {
			int position = getSynPosition(syn);
			synPosition.push_back(position);
		} else {
			return res;
		}
	}

	for (auto rowIter = resultTable.rows.begin(); rowIter != resultTable.rows.end(); ++rowIter) {
		Row row = *rowIter;
		vector<string> valueRow = vector<string>();
		for (auto posIter = synPosition.begin(); posIter != synPosition.end(); ++posIter) {
			int position = *posIter;
			string value = row.at(position);
			valueRow.push_back(value);
		}
		res.insert(valueRow);
	}
	return res;
}

// if syn cannoth be found in resultTable, 
// an empty unordered_set<string> will be returned
unordered_set<string> Result::getSyn(string syn) {
	unordered_set<string> res = unordered_set<string>();
	if (syn.length() > 0 && isSynPresent(syn)) {
		int position = getSynPosition(syn);
		for (auto rowIter = resultTable.rows.begin(); rowIter != resultTable.rows.end(); ++rowIter) {
			Row row = *rowIter;
			string value = row.at(position);
			res.insert(value);
		}
	}
	return res;
}

bool Result::isPass() {
	return pass;
}

bool Result::isSynPresent(string syn) {
	size_t pos = getSynPosition(syn);
	bool isPresent = pos < resultTable.synList.size();
	return isPresent;
}

int Result::getSynPosition(string syn) {
	size_t pos = find(resultTable.synList.begin(), resultTable.synList.end(), syn) - resultTable.synList.begin();
	return pos;
}
