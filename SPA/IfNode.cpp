#include "IfNode.h"

// constructors
IfNode::IfNode(int stmtNum)
	: StmtNode(IF_STMT_, stmtNum) {

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
	if (this->_children.size() < 1) {
		this->_children.push_back(varNode);
	} else {
		this->_children.at(0) = varNode;
	}
	varNode->setParent(this);
}

void IfNode::linkThenStmtLstNode(StmtLstNode *thenNode) {
	if (this->_children.size() < 2) {
		this->_children.push_back(thenNode);
	} else {
		this->_children.at(1) = thenNode;
	}
	thenNode->setParent(this);
}
void IfNode::linkElseStmtLstNode(StmtLstNode *elseNode) {
	if (this->_children.size() < 3) {
		this->_children.push_back(elseNode);
	} else {
		this->_children.at(2) = elseNode;
	}
	elseNode->setParent(this);
}
