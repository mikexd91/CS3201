#include "WhileNode.h"


// constructors
WhileNode::WhileNode(int stmtNum)
	: StmtNode(NodeType::WHILE_STMT_, stmtNum) {

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
	if (this->_children.size() < 1) {
		this->_children.push_back(varNode);
	} else {
		this->_children.at(0) = varNode;
	}
	varNode->setParent(this);
}

void WhileNode::linkStmtLstNode(StmtLstNode *stmtLst) {
	if (this->_children.size() < 2) {
		this->_children.push_back(stmtLst);
	} else {
		this->_children.at(1) = stmtLst;
	}
	stmtLst->setParent(this);
}