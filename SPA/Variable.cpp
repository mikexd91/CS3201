#include "Variable.h"


Variable::Variable(const string& varName) {
	this->_name = varName;
	_usedBy = *new set<int>();
	_modifiedBy = *new set<int>();
}

string Variable::getName() {
	return this->_name;
}

set<int>& Variable::getUsedByStmts() {
	return this->_usedBy;
}

set<int>& Variable::getModifiedByStmts() {
	return this->_modifiedBy;
}

set<string>& Variable::getUsedByProc() {
	return this->_usedByProc;
}

set<string>& Variable::getModifiedByProc() {
	return this->_modifiedByProc;
}

set<TNode*>& Variable::getTNodes() {
	return this->_nodes;
}

void Variable::addUsingStmt(int stmt) {
	this->_usedBy.insert(stmt);
}

void Variable::addModifyingStmt(int stmt) {
	this->_modifiedBy.insert(stmt);
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
