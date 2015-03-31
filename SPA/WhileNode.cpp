#include "WhileNode.h"


// constructors
WhileNode::WhileNode(int stmtNum)
	: StmtNode(WHILE_STMT_, stmtNum) {

}

// getters
VarNode* WhileNode::getVarNode() {
	// children[0]
	return (VarNode*)(this->_children.at(0));
}

StmtLstNode* WhileNode::getStmtLstNode() {
	// children[1]
	return (StmtLstNode*)(this->_children.at(1));
}

// setters
void WhileNode::linkVarNode(VarNode *varNode) {
	this->_children.at(0) = varNode;
}

void WhileNode::linkStmtLstNode(StmtLstNode *stmtLst) {
	this->_children.at(1) = stmtLst;
}