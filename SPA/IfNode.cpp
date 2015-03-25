#include "IfNode.h"

// constructors
IfNode::IfNode(int stmtNum)
	: StmtNode(NodeType::IF_STMT_, stmtNum) {

}

// getters
VarNode* IfNode::getVarNode() {
	// children[0]
	return (VarNode*)(this->_children.at(0));
}

StmtLstNode* IfNode::getThenStmtLstNode() {
	// children[1]
	return (StmtLstNode*)(this->_children.at(1));
}

StmtLstNode* IfNode::getElseStmtLstNode() {
	// children[2]
	return (StmtLstNode*)(this->_children.at(2));
}

// setters
void IfNode::linkVarNode(VarNode *varNode) {
	this->_children.at(0) = varNode;
}

void IfNode::linkThenStmtLstNode(StmtLstNode *thenNode) {
	this->_children.at(1) = thenNode;
}
void IfNode::linkElseStmtLstNode(StmtLstNode *elseNode) {
	this->_children.at(2) = elseNode;
}
