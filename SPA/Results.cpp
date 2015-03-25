#pragma once
#include "Results.h"

Results::Results(void)
{
}


Results::~Results(void)
{
}

void Results::addStmt(int stmt) {
	stmtResultSet.push_back(stmt);
}
	
void Results::addVar(string var) {
	varResultSet.push_back(var);
}

void Results::setResultType(string type) {
	this->resultType = type;
}

vector<int> Results::getStmtResults() {
	return this->stmtResultSet;
}
	
vector<string> Results::getVarResults() {
	return this->varResultSet;
}
	
string Results::toString() {
	
	/*
	if (this->resultType.compare("stmt")) {

	} else if (this->resultType.compare("var")) {

	} else {

	}
	*/
}