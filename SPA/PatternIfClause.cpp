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
	firstArgType = stringconst::ARG_IF;
	firstArg = syn;
	firstArgFixed = false;
	secondArg = var;
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

bool PatternIfClause::isVarWild() {
	return (getVarType() == stringconst::ARG_GENERIC)
		&& (getVar() == stringconst::STRING_EMPTY);
}

bool PatternIfClause::isValid() {
	string varType = this->getVarType();
	bool checkVar = (varType == stringconst::ARG_VARIABLE) || isVarWild();
	bool checkThenList = isThenListWild();
	bool checkElseList = isElseListWild();
	bool valid = checkVar && checkThenList && checkElseList;
	return valid;
}

// generic == underscore
// fixed == "1"
// allS1 == synonym

//e.g. Pattern "a"("a", ?, ?)
bool PatternIfClause::evaluateS1FixedS2Fixed(string a, string b) {
	// always return false because s1 cannot be fixed!
	//cout << "s1fs2f";
	return false;
}

//e.g. Pattern _(_, ?, ?)
bool PatternIfClause::evaluateS1GenericS2Generic() {
	// always return false because s1 cannot be generic!
	//cout << "s1gs2g";
	return false;
}

//e.g. Pattern _("a", ?, ?)
bool PatternIfClause::evaluateS1GenericS2Fixed(string a) {
	// always return false because s1 cannot be generic!
	//cout << "s1gs2f";
	return false;
}

//e.g. Pattern "a"(_, ?, ?)
bool PatternIfClause::evaluateS1FixedS2Generic(string a) {
	// always return false because s1 cannot be fixed!
	//cout << "s1fs2g";
	return false;
}

//e.g. Pattern "a"(a, ?, ?)
unordered_set<string> PatternIfClause::getAllS2WithS1Fixed(string a) {
	// always return empty set because s1 cannot be fixed!
	//cout << "s1fs2n";
	return unordered_set<string>();
}

//e.g. Pattern _(a, ?, ?)
unordered_set<string> PatternIfClause::getAllS2() {
	// always return empty set because s1 cannot be underscore!
	//cout << "s2n";
	return unordered_set<string>();
}

//e.g. Pattern a("a", ?, ?)
unordered_set<string> PatternIfClause::getAllS1WithS2Fixed(string var) {
	//cout << "s1ns2f";
	// TODO
	// choices:
	//	var fixed expr wild
	//	var fixed expr fixed
	return evaluateVarFixed();
}

//e.g. Pattern a(_, ?, ?)
unordered_set<string> PatternIfClause::getAllS1() {
	//cout << "s1n";
	// TODO
	// choices:
	//	var wild expr wild
	//	var wild expr fixed
	return evaluateVarWild();
}

//e.g. Pattern a(a, ?, ?)
Results::ResultsTable* PatternIfClause::getAllS1AndS2() {
	//cout << "s1ns2n";
	// TODO
	// choices:
	//	var syn expr wild
	//	var syn expr fixed
	return evaluateVarSyn();
}

// my own evaluators

unordered_set<string> PatternIfClause::evaluateVarWild() {
	unordered_set<string> resultsSet = unordered_set<string>();

	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> ifStmts = stable->getIfStmts();
	BOOST_FOREACH(Statement* ifStmt, ifStmts) {
		resultsSet.insert(boost::lexical_cast<string>(ifStmt->getStmtNum()));
	}

	return resultsSet;
}

unordered_set<string> PatternIfClause::evaluateVarFixed() {
	unordered_set<string> resultsSet = unordered_set<string>();

	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> ifStmts = stable->getIfStmts();
	BOOST_FOREACH(Statement* ifStmt, ifStmts) {
		IfNode* ifNode = (IfNode*) ifStmt->getTNodeRef();
		resultsSet.insert(boost::lexical_cast<string>(ifStmt->getStmtNum()));
	}

	return resultsSet;
}

Results::ResultsTable* PatternIfClause::evaluateVarSyn() {
	Results::ResultsTable* res = new Results::ResultsTable();

	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> ifStmts = stable->getIfStmts();
	VarTable* vtable = VarTable::getInstance();
	vector<string>* varNames = vtable->getAllVarNames();//->getAllVarNames();

	BOOST_FOREACH(Statement* ifStmt, ifStmts) {
		IfNode* ifNode = (IfNode*) ifStmt->getTNodeRef();
		for (size_t j = 0; j < varNames->size(); j++) {
			string var = varNames->at(j);
			if (matchVar(ifNode, var)) {
				string stmtNumStr = boost::lexical_cast<string>(ifNode->getStmtNum());
				// TODO add pair result;
				Results::Row* pair = new Results::Row();
				(*pair)[getSynonym()] = stmtNumStr;
				(*pair)[getVar()] = var;
				res->insert(pair);
			}
		}
	}

	return res;
}



// helpers

bool PatternIfClause::matchThenList(IfNode* ifNode, string thenStr) {
	return thenStr == stringconst::STRING_EMPTY;
}

bool PatternIfClause::matchElseList(IfNode* ifNode, string elseStr) {
	return elseStr == stringconst::STRING_EMPTY;
}

bool PatternIfClause::matchVar(IfNode* ifNode, string varStr) {
	return ifNode->getVarNode()->getName() == varStr;
}
