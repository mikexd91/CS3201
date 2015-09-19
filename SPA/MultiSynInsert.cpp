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
		removeRows(resultTable);
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

vector<int> MultiSynInsert::getSynPositionsInTable(ResultTable resultTable){
	vector<int> synPositionsInTable = vector<int>();
	for (auto synIter = multiInsertSyns.begin(); synIter != multiInsertSyns.end(); synIter++) {
		auto position = find(resultTable.synList.begin(), resultTable.synList.end(), *synIter);
		if (find(resultTable.synList.begin(), resultTable.synList.end(), *synIter) != resultTable.synList.end()) {
			synPositionsInTable.push_back(position - resultTable.synList.begin());
		}
	}
	return synPositionsInTable;
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

void MultiSynInsert::removeRows(ResultTable &table) {
		//all syns already exist in table
		//get positions of syns in table in order of multiinsertsyn
		vector<int> synPositions = getSynPositionsInTable(table);
		//eliminate rows from table
		table.rows.remove_if([&](Row row )->bool { 
			//for each of the rows to be inserted
			for (auto insertRowIter = multiInsertValues.begin(); insertRowIter != multiInsertValues.end(); ++insertRowIter) {
				bool isRowInTable = true;
				vector<string> insertRow = *insertRowIter;
				auto tableIter = synPositions.begin();
				auto insertValueIter= insertRow.begin();
				//compare the values in the rows with those in the table
				for (; tableIter != synPositions.end() && insertValueIter != insertRow.end(); tableIter++, insertValueIter++) {
					int tableIndex = *tableIter;
					string insertValue = *insertValueIter;
					if (row.at(tableIndex) != insertValue) {
						isRowInTable = false;
						break;
					}
				}
				if (isRowInTable) {
					//if row is present in table do not remove the row
					return false;
				}
			}
			//row not present in table, remove
			return true;
		});
}
