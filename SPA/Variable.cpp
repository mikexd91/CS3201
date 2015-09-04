#include "Variable.h"
#include <boost\lexical_cast.hpp>

using namespace boost;

Variable::Variable(const string& varName) {
	this->_name = varName;
	_usedBy = *new unordered_set<int>();
	_modifiedBy = *new unordered_set<int>();
}

string Variable::getName() {
	return this->_name;
}

unordered_set<int>& Variable::getUsedByStmts() {
	return this->_usedBy;
}

unordered_set<string>&  Variable::getUsedByStmtsAsString() {
	return this->_usedByAsString;
}

unordered_set<int>& Variable::getModifiedByStmts() {
	return this->_modifiedBy;
}

unordered_set<string>& Variable::getModifiedByStmtsAsString() {
	return this->_modifiedByAsString;
}

unordered_set<string>& Variable::getUsedByProc() {
	return this->_usedByProc;
}

unordered_set<string>& Variable::getModifiedByProc() {
	return this->_modifiedByProc;
}

unordered_set<TNode*>& Variable::getTNodes() {
	return this->_nodes;
}

void Variable::addUsingStmt(int stmt) {
	this->_usedBy.insert(stmt);
	this->_usedByAsString.insert(lexical_cast<string>(stmt));
}

void Variable::addModifyingStmt(int stmt) {
	this->_modifiedBy.insert(stmt);
	this->_modifiedByAsString.insert(lexical_cast<string>(stmt));
}

void Variable::addUsingProc(string proc) {
	this->_usedByProc.insert(proc);
}

void Variable::addModifyingProc(string proc) {
	this->_modifiedByProc.insert(proc);
}

void Variable::addTNode(TNode *node) {
	this->_nodes.insert(node);
}
