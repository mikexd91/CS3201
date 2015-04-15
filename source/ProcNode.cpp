#include "ProcNode.h"

// constructor

ProcNode::ProcNode(const string &name) 
	: TNode(PROCEDURE_, name) {

}

// getters

StmtLstNode* ProcNode::getStmtLstNode() {
	if (_children.size() < 1) {
		return NULL;
	}
	return (StmtLstNode*)(this->_children.at(0));
}

// setters

void ProcNode::linkStmtLstNode(StmtLstNode *stmtLst) {
	if (this->_children.size() == 0) {
		this->_children.push_back(stmtLst);
	} else {
		this->_children.at(0) = stmtLst;
	}
	stmtLst->setParent(this);
}