#include "PatternWhileClause.h"
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

PatternWhileClause::PatternWhileClause(const string& syn, const string& var, const string& stmtList) {
	this->synType = stringconst::ARG_WHILE;
	this->syn = syn;
	this->var = var;
	// parser must set vartype and varfixed.
	this->_stmtList = stmtList;
	this->clauseType = PATTERNWHILE_;
}

PatternWhileClause::~PatternWhileClause(void) {
}

string PatternWhileClause::getStmtList() {
	return this->_stmtList;
}

bool PatternWhileClause::isStmtListWild() {
	return getStmtList() == stringconst::STRING_EMPTY;
}

bool PatternWhileClause::isVarWild() {
	return (getVarType() == stringconst::ARG_GENERIC)
		&& (getVar() == stringconst::STRING_EMPTY);
}

bool PatternWhileClause::isValid() {
	string varType = this->getVarType();
	bool checkVar = (varType == stringconst::ARG_VARIABLE) || isVarWild();
	bool checkStmtList = isStmtListWild();
	bool valid = checkVar && checkStmtList;
	return valid;
}

bool PatternWhileClause::matchPattern(string stmtNumStr, string varName) {
	int stmtNum = stoi(stmtNumStr);
	WhileNode* whileNode = (WhileNode*) this->stmtTable->getStmtObj(stmtNum)->getTNodeRef();
	return matchVar(whileNode, varName) && matchStmtList(whileNode, getStmtList());
}

unordered_set<string> PatternWhileClause::getAllSynValues() {
	unordered_set<Statement*> allStmtObjs = stmtTable->getWhileStmts();
	unordered_set<string> allSynValues;
	BOOST_FOREACH(Statement* stmtObj, allStmtObjs) {
		allSynValues.insert(lexical_cast<string>(stmtObj->getStmtNum()));
	}
	return allSynValues;
}

// helpers

bool PatternWhileClause::matchStmtList(WhileNode* whileNode, string thenStr) {
	return thenStr == stringconst::STRING_EMPTY;
}

bool PatternWhileClause::matchVar(WhileNode* whileNode, string varStr) {
	return (varStr == stringconst::STRING_EMPTY) || whileNode->getVarNode()->getName() == varStr;
}
