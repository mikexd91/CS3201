#include "OpNode.h"


OpNode::OpNode(const string &varName)
	: TNode(OPERATOR_, varName) {

}

// getters
TNode* OpNode::getLeftNode() {
	if (_children.size() < 1) {
		return NULL;
	}
	return this->_children.at(0);
} 
TNode* OpNode::getRightNode() {
	if (_children.size() < 2) {
		return NULL;
	}
	return this->_children.at(1);
}

// setters
void OpNode::linkLeftNode(TNode *left) {
	if (this->_children.size() == 0) {
		this->_children.push_back(left);
	} else {
		this->_children.at(0) = left;
	}
	left->setParent(this);
}

void OpNode::linkRightNode(TNode *right) {
	if (this->_children.size() == 0) {
		this->_children.push_back(NULL);
		this->_children.push_back(right);
	} else if (this->_children.size() == 1) {
		this->_children.push_back(right);
	} else {
		this->_children.at(1) = right;
	}
	right->setParent(this);
}