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
	firstArgType = stringconst::ARG_WHILE;
	firstArg = syn;
	firstArgFixed = false;
	secondArg = var;
	// parser must set vartype and varfixed.
	this->_stmtList = stmtList;
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

// generic == underscore
// fixed == "1"
// allS1 == synonym

//e.g. Pattern "a"("a", ?, ?)
bool PatternWhileClause::evaluateS1FixedS2Fixed(string a, string b) {
	// always return false because s1 cannot be fixed!
	//cout << "s1fs2f";
	return false;
}

//e.g. Pattern _(_, ?, ?)
bool PatternWhileClause::evaluateS1GenericS2Generic() {
	// always return false because s1 cannot be generic!
	//cout << "s1gs2g";
	return false;
}

//e.g. Pattern _("a", ?, ?)
bool PatternWhileClause::evaluateS1GenericS2Fixed(string a) {
	// always return false because s1 cannot be generic!
	//cout << "s1gs2f";
	return false;
}

//e.g. Pattern "a"(_, ?, ?)
bool PatternWhileClause::evaluateS1FixedS2Generic(string a) {
	// always return false because s1 cannot be fixed!
	//cout << "s1fs2g";
	return false;
}

//e.g. Pattern "a"(a, ?, ?)
unordered_set<string> PatternWhileClause::getAllS2WithS1Fixed(string a) {
	// always return empty set because s1 cannot be fixed!
	//cout << "s1fs2n";
	return unordered_set<string>();
}

//e.g. Pattern _(a, ?, ?)
unordered_set<string> PatternWhileClause::getAllS2() {
	// always return empty set because s1 cannot be underscore!
	//cout << "s2n";
	return unordered_set<string>();
}

//e.g. Pattern a("a", ?, ?)
unordered_set<string> PatternWhileClause::getAllS1WithS2Fixed(string var) {
	//cout << "s1ns2f";
	// TODO
	// choices:
	//	var fixed expr wild
	//	var fixed expr fixed
	return evaluateVarFixed();
}

//e.g. Pattern a(_, ?, ?)
unordered_set<string> PatternWhileClause::getAllS1() {
	//cout << "s1n";
	// TODO
	// choices:
	//	var wild expr wild
	//	var wild expr fixed
	return evaluateVarWild();
}

//e.g. Pattern a(a, ?, ?)
Results::ResultsTable* PatternWhileClause::getAllS1AndS2() {
	//cout << "s1ns2n";
	// TODO
	// choices:
	//	var syn expr wild
	//	var syn expr fixed
	return evaluateVarSyn();
}

// my own evaluators

unordered_set<string> PatternWhileClause::evaluateVarWild() {
	unordered_set<string> resultsSet = unordered_set<string>();

	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> whileStmts = stable->getWhileStmts();
	BOOST_FOREACH(Statement* whileStmt, whileStmts) {
		resultsSet.insert(boost::lexical_cast<string>(whileStmt->getStmtNum()));
	}

	return resultsSet;
}

unordered_set<string> PatternWhileClause::evaluateVarFixed() {
	unordered_set<string> resultsSet = unordered_set<string>();

	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> whileStmts = stable->getWhileStmts();
	BOOST_FOREACH(Statement* whileStmt, whileStmts) {
		WhileNode* whileNode = (WhileNode*) whileStmt->getTNodeRef();
		resultsSet.insert(boost::lexical_cast<string>(whileStmt->getStmtNum()));
	}

	return resultsSet;
}

Results::ResultsTable* PatternWhileClause::evaluateVarSyn() {
	Results::ResultsTable* res = new Results::ResultsTable();

	// simply insert all assgs
	StmtTable* stable = StmtTable::getInstance();
	unordered_set<Statement*> whileStmts = stable->getWhileStmts();
	VarTable* vtable = VarTable::getInstance();
	vector<string>* varNames = vtable->getAllVarNames();

	BOOST_FOREACH(Statement* whileStmt, whileStmts) {
		WhileNode* whileNode = (WhileNode*) whileStmt->getTNodeRef();
		for (size_t j = 0; j < varNames->size(); j++) {
			string var = varNames->at(j);
			if (matchVar(whileNode, var)) {
				string stmtNumStr = boost::lexical_cast<string>(whileNode->getStmtNum());
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

bool PatternWhileClause::matchStmtList(WhileNode* whileNode, string thenStr) {
	return thenStr == stringconst::STRING_EMPTY;
}

bool PatternWhileClause::matchVar(WhileNode* whileNode, string varStr) {
	return whileNode->getVarNode()->getName() == varStr;
}
