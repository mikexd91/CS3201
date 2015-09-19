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
	if (multiInsertSyns.empty() || multiInsertValues.empty()) {
		resultTable.synList.clear();
		resultTable.rows.clear();
	}
	//get number of syns present
	vector<string> existingSyns = resultTable.synList;
	int numSynInTable = getNumSynsInTable(resultTable);
	if (numSynInTable == 0) {
		//add all into table
		for (auto synIter = multiInsertSyns.begin(); synIter != multiInsertSyns.end(); synIter++) {
			resultTable.synList.push_back(*synIter);
		}
		if (resultTable.rows.empty()) {
			addNewRows(resultTable);
		} else {
			addSynToExistingRows(resultTable);
		}
	} else if (numSynInTable == multiInsertSyns.size()) {
		//all syns are present in table
		//remove rows as needed
		removeRows(resultTable);
	} else {
		//some syns are present in table
		combineRows(resultTable);
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
void MultiSynInsert::addNewRows(ResultTable& resultTable){
	list<Row> rowList;
	for (auto valueIter = multiInsertValues.begin(); valueIter != multiInsertValues.end(); ++valueIter) {
		Row row = (*valueIter);
		rowList.push_back(row);
	}
	resultTable.rows = rowList;
}

void MultiSynInsert::addSynToExistingRows(ResultTable& resultTable) {
	list<Row> rowList;
	for (auto rowIter = resultTable.rows.begin(); rowIter != resultTable.rows.end(); ++rowIter) {
		for (auto valueIter = multiInsertValues.begin(); valueIter != multiInsertValues.end(); ++valueIter) {
			Row row = *rowIter;
			row.insert(row.end(), (*valueIter).begin(), (*valueIter).end());
			rowList.push_back(row);
		}
	}
	resultTable.rows = rowList;
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

void MultiSynInsert::combineRows(ResultTable& resultTable) {
	//get indexes of common syns in result table and insert
	vector<int> commonSynsInTable = vector<int>();
	vector<int> commonSynsInInsert = vector<int>();
	vector<int> uniqueSynsInInsert = vector<int>();
	categoriseSyns(resultTable, commonSynsInTable, commonSynsInInsert, uniqueSynsInInsert);
	//insert syns
	insertSyns(resultTable, uniqueSynsInInsert);
	//check rows
	list<Row> rowList = list<Row>(); 
	//for each values to be inserted
	for (auto iter = multiInsertValues.begin(); iter != multiInsertValues.end(); iter++) {
		vector<string> insertValues = *iter;
		//for each row in the result table
		for (auto tableIter = resultTable.rows.begin(); tableIter != resultTable.rows.end(); tableIter++) {
			Row tableRow = *tableIter;
			bool toInsert = toInsertRow(tableRow, insertValues, commonSynsInTable, commonSynsInInsert);
			//check if row should be inserted or not
			if (toInsert) {
				//add all the unique syns into the row, and insert into result table
				addToRow(tableRow, uniqueSynsInInsert, insertValues);
				rowList.push_back(tableRow);
			}
		}
	}
	resultTable.rows = rowList;
}

void MultiSynInsert::categoriseSyns(ResultTable resultTable, vector<int>& commonSynsInTable, vector<int>& commonSynsInInsert, vector<int>& uniqueSynsInInsert) {
	for (auto synIter = multiInsertSyns.begin(); synIter != multiInsertSyns.end(); synIter++) {
		auto position = find(resultTable.synList.begin(), resultTable.synList.end(), *synIter);
		if (find(resultTable.synList.begin(), resultTable.synList.end(), *synIter) != resultTable.synList.end()) {
			commonSynsInTable.push_back(position - resultTable.synList.begin());
			commonSynsInInsert.push_back(synIter - multiInsertSyns.begin());
		} else {
			uniqueSynsInInsert.push_back(synIter - multiInsertSyns.begin());
		}
	}
}

bool MultiSynInsert::toInsertRow(Row tableRow, vector<string> insertValues, vector<int> commonSynsInTable, vector<int> commonSynsInInsert) {
	auto commonSynsInTableIter = commonSynsInTable.begin();
	auto commonSynsInInsertIter = commonSynsInInsert.begin();
	bool toInsert = true;
	for (; commonSynsInTableIter != commonSynsInTable.end() && commonSynsInInsertIter != commonSynsInInsert.end();
		commonSynsInTableIter++, commonSynsInInsertIter++) {
		if (tableRow.at(*commonSynsInTableIter) != insertValues.at(*commonSynsInInsertIter)) {
			toInsert = false;
			break;
		}
	}
	return toInsert;
}

void MultiSynInsert::insertSyns(ResultTable& resultTable, vector<int> uniqueSynsInInsert) {
	for (auto uniqueSynsInInsertIter = uniqueSynsInInsert.begin(); 
		uniqueSynsInInsertIter != uniqueSynsInInsert.end();
		uniqueSynsInInsertIter++) {
		resultTable.synList.push_back(multiInsertSyns.at(*uniqueSynsInInsertIter));
	}
}

void MultiSynInsert::addToRow(Row& tableRow, vector<int> uniqueSynsInInsert, vector<string> insertValues){
	for (auto uniqueSynsInInsertIter = uniqueSynsInInsert.begin();
					uniqueSynsInInsertIter != uniqueSynsInInsert.end();
					uniqueSynsInInsertIter++) {
		tableRow.push_back(insertValues.at(*uniqueSynsInInsertIter));
	}
}


