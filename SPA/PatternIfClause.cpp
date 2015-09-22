#include "PatternIfClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"

#include <set>
#include <stack>
#include <iostream>
#include "boost\lexical_cast.hpp"
#include "boost\algorithm\string.hpp"
#include "boost\unordered_map.hpp"
#include "boost\unordered\unordered_map.hpp"
#include "boost\foreach.hpp"

PatternIfClause::PatternIfClause(const string& syn, const string& var, const string& thenList, const string& elseList) {
	synType = stringconst::ARG_IF;
	this->syn = syn;
	this->var = var;
	// parser must set vartype and varfixed.
	this->_thenList = thenList;
	this->_elseList = elseList;
}

PatternIfClause::~PatternIfClause(void) {
}

string PatternIfClause::getThenList() {
	return this->_thenList;
}

string PatternIfClause::getElseList() {
	return this->_elseList;
}

bool PatternIfClause::isThenListWild() {
	return getThenList() == stringconst::STRING_EMPTY;
}

bool PatternIfClause::isElseListWild() {
	return getElseList() == stringconst::STRING_EMPTY;
}
bool PatternIfClause::isValid() {
	string varType = this->getVarType();
	bool checkVar = (varType == stringconst::ARG_VARIABLE) || (varType == stringconst::ARG_GENERIC);
	bool checkThenList = isThenListWild();
	bool checkElseList = isElseListWild();
	bool valid = checkVar && checkThenList && checkElseList;
	return valid;
}

bool PatternIfClause::matchPattern(string stmtNumStr, string varName) {
	int stmtNum = stoi(stmtNumStr);
	IfNode* ifNode = (IfNode*) this->stmtTable->getStmtObj(stmtNum)->getTNodeRef();
	return matchVar(ifNode, varName) && matchThenList(ifNode, getThenList()) && matchElseList(ifNode, getElseList());
}

unordered_set<string> PatternIfClause::getAllSynValues() {
	unordered_set<Statement*> allStmtObjs = stmtTable->getIfStmts();
	unordered_set<string> allSynValues;
	BOOST_FOREACH(Statement* stmtObj, allStmtObjs) {
		allSynValues.insert(lexical_cast<string>(stmtObj->getStmtNum()));
	}
	return allSynValues;
}

// helpers

bool PatternIfClause::matchThenList(IfNode* ifNode, string thenStr) {
	return thenStr == stringconst::STRING_EMPTY;
}

bool PatternIfClause::matchElseList(IfNode* ifNode, string elseStr) {
	return elseStr == stringconst::STRING_EMPTY;
}

bool PatternIfClause::matchVar(IfNode* ifNode, string varStr) {
	return (varStr == stringconst::STRING_EMPTY) || ifNode->getVarNode()->getName() == varStr;
}
