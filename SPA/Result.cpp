#include "Result.h"

Result::Result() {
	resultTable = ResultTable();
}

Result::~Result() {
}


void Result::push(InsertType& insert) {
	insert.execute(resultTable);
}

ResultTable Result::getResultTable() {
	return resultTable;
}
void Result::setResultTable(ResultTable& table) {
	resultTable = table;
}

