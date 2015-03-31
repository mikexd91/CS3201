#include "ProcNode.h"

// constructor

ProcNode::ProcNode(const string &name) 
	: TNode(PROCEDURE_, name) {

}

// getters

StmtLstNode* ProcNode::getStmtLst() {
	return (StmtLstNode*)(this->_children.at(0));
}

// setters

void ProcNode::linkStmtLstNode(StmtLstNode *stmtLst) {
	this->linkChild(stmtLst);
}