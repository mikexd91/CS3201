#include "AssgNode.h"

// constructor

AssgNode::AssgNode(int stmtNum) 
	: StmtNode(ASSIGN_STMT_, stmtNum) {

}

// getters 

VarNode* AssgNode::getVarNode() {
	// just children[0] and children[1]
	return (VarNode*)(this->_children.at(0));
}
TNode* AssgNode::getExprNode() {
	return this->_children.at(1);
}

// setters

void AssgNode::linkVarNode(VarNode *varNode) {
	this->_children.at(0) = varNode;
}
void AssgNode::linkExprNode(TNode *exprNode) {
	this->_children.at(1) = exprNode;
}