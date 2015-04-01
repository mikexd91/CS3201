#include "Variable.h"


Variable::Variable(const string& varName) {
	this->_name = varName;
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

set<TNode*>& Variable::getTNodes() {
	return this->_nodes;
}

void Variable::addUsingStmt(int stmt) {
	this->_usedBy.insert(stmt);
}

void Variable::addModifyingStmt(int stmt) {
	this->_modifiedBy.insert(stmt);
}

void Variable::addTNode(TNode *node) {
	this->_nodes.insert(node);
}
