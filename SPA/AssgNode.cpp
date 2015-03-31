#include "AssgNode.h"

// constructor

AssgNode::AssgNode(int stmtNum) 
	: StmtNode(NodeType::ASSIGN_STMT_, stmtNum) {

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
	if (this->_children.size() < 1) {
		this->_children.push_back(varNode);
	} else {
		this->_children.at(0) = varNode;
	}
}

void AssgNode::linkExprNode(TNode *exprNode) {
	if (this->_children.size() < 2) {
		this->_children.push_back(exprNode);
	} else {
		this->_children.at(1) = exprNode;
	}
}