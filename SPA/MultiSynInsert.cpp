#include "MultiSynInsert.h"

MultiSynInsert::MultiSynInsert() {
	 multiInsertValues = unordered_set<vector<string>>();
}

void MultiSynInsert::setSyns(vector<string> syns) {
	multiInsertSyns = syns;
}
void MultiSynInsert::insertValues(vector<string> values) {
	multiInsertValues.insert(values);
}


void MultiSynInsert::execute(ResultTable& resultTable) {
	//get number of syns present
	vector<string> existingSyns = resultTable.synList;
	int numSynInTable = getNumSynsInTable(resultTable);
	if (numSynInTable == 0) {
		//add all into table
		for (auto synIter = multiInsertSyns.begin(); synIter != multiInsertSyns.end(); synIter++) {
			resultTable.synList.push_back(*synIter);
		}
		list<Row> temp;
		if (resultTable.rows.empty()) {
			addNewRows(temp);
		} else {
			addSynToExistingRows(resultTable, temp);
		}
		resultTable.rows = temp;
	} else if (numSynInTable == multiInsertSyns.size()) {
		//eliminate rows from table
	} else {
	}
}

int MultiSynInsert::getNumSynsInTable(ResultTable resultTable){
	int numSynInTable = 0;
	for (auto synIter = multiInsertSyns.begin(); synIter != multiInsertSyns.end(); synIter++) {
		if (find(resultTable.synList.begin(), resultTable.synList.end(), *synIter) != resultTable.synList.end()) {
			numSynInTable++;
		}
	}
	return numSynInTable;
}

void MultiSynInsert::addNewRows(list<Row>& rowList){
	for (auto valueIter = multiInsertValues.begin(); valueIter != multiInsertValues.end(); ++valueIter) {
		Row row = (*valueIter);
		rowList.push_back(row);
	}
}

void MultiSynInsert::addSynToExistingRows(ResultTable resultTable, list<Row>& rowList) {
	for (auto rowIter = resultTable.rows.begin(); rowIter != resultTable.rows.end(); ++rowIter) {
		for (auto valueIter = multiInsertValues.begin(); valueIter != multiInsertValues.end(); ++valueIter) {
			Row row = *rowIter;
			row.insert(row.end(), (*valueIter).begin(), (*valueIter).end());
			rowList.push_back(row);
		}
	}
}