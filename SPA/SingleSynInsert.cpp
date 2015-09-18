#include "SingleSynInsert.h"

SingleSynInsert::SingleSynInsert() {
	 singleInsertValues = unordered_set<string>();
}

void SingleSynInsert::setSyn(string syn) {
	singleSyn = syn;
}
void SingleSynInsert::insertValue(string syn) {
	singleInsertValues.insert(syn);
}


void SingleSynInsert::execute(ResultTable& resultTable) {
	size_t pos = find(resultTable.synList.begin(), resultTable.synList.end(), singleSyn) - resultTable.synList.begin();
	bool isSynPresent = pos < resultTable.synList.size();
	if (isSynPresent) {
		//syn is not present the list
		//remove row without value in the insert set
		resultTable.rows.remove_if([&](Row row ){ return singleInsertValues.find(row.at(pos)) == singleInsertValues.end(); });
	} else {
		//add syns to all rows
		list<Row> temp;
		resultTable.synList.push_back(singleSyn);
		Row row = Row();
		for (auto rowIter = resultTable.rows.begin(); rowIter != resultTable.rows.end(); ++rowIter) {
			for (auto valueIter = singleInsertValues.begin(); valueIter != singleInsertValues.end(); ++valueIter) {
				row = *rowIter;
				row.push_back(*valueIter);
				temp.push_back(row);
			}
		}
		resultTable.rows = temp;
	}
}