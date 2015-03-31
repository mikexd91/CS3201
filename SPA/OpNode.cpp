#include "OpNode.h"


OpNode::OpNode(const string &varName)
	: TNode(OPERATOR_) {

}

// getters
TNode* OpNode::getLeftNode() {
	return this->_children.at(0);
} 
TNode* OpNode::getRightNode() {
	return this->_children.at(1);
}

// setters
void OpNode::linkLeftNode(TNode *left) {
	this->_children.at(0) = left;
}

void OpNode::linkRightNode(TNode *right) {
	this->_children.at(1) = right;
}