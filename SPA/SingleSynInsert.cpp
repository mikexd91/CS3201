#include "SingleSynInsert.h"

void SingleSynInsert::execute(ResultTable& resultTable) {
	size_t pos = find(resultTable.synList.begin(), resultTable.synList.end(), singleSyn) - resultTable.synList.begin();
	bool isSynPresent = pos < resultTable.synList.size();
	if (isSynPresent) {
		//remove row without value in single insert value
		resultTable.rows.remove_if([&](Row row ){ return singleInsertValues.find(row.at(pos)) == singleInsertValues.end(); });
	} else {
		//add syn to row
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