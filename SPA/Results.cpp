#pragma once
#include "Results.h"

Results::Results(void)
{
}

Results::~Results(void)
{
}

void Results::addStmt(int stmt) {
	stmtResultSet.insert(stmt);
}
	
void Results::addVar(string var) {
	varResultSet.insert(var);
}

void Results::setBool(bool res) {
	this->exist = res;
}

void Results::setResultType(Type type) {
	this->resultType = type;
}

set<int> Results::getStmtResults() {
	return this->stmtResultSet;
}
	
set<string> Results::getVarResults() {
	return this->varResultSet;
}

bool Results::getBoolResults() {
	return this->exist;
}

void removeEndComma(string *result) {
	result->pop_back();
	result->pop_back();
}

string formatVarResult(set<string> varSet) {
	string result;
	set<string>::iterator setIter;	
	for (setIter=varSet.begin(); setIter!=varSet.end(); setIter++) {
		string v = *setIter;
		result = v + ", ";
	}
	removeEndComma(&result);
	return result;
}

string formatStmtResult(set<int> stmtSet) {
	string result;
	set<int>::iterator setIter;	
	for (setIter=stmtSet.begin(); setIter!=stmtSet.end(); setIter++) {
		int n = *setIter;
		result = n + ", ";
	}
	removeEndComma(&result);
	return result;
}

string formatBoolResult(bool res) {
	return res + "";
}

string Results::toString() {
	Type type = this->resultType;
	string result;
	switch (type) {
	case Letter:
		result = formatVarResult(this->varResultSet);
		break;
	case Digit:
		result = formatStmtResult(this->stmtResultSet);
		break;
	case Tuple:
		break;
	case Boolean:
		result = formatBoolResult(this->exist);
		break;
	case None:
		result = "none";
		break;
	default:
		break;
	}
	return result;
}
