#include "TNode.h"

using namespace std;

// constructors

TNode::TNode(NodeType type) {
	this->_nodeType = type;
	this->_leftSibling = NULL;
	this->_rightSibling = NULL;
	this->_parent = NULL;
}

TNode::TNode(NodeType type, const string& name) {
	this->_nodeType = type;
	this->_nodeName = name;
	this->_leftSibling = NULL;
	this->_rightSibling = NULL;
	this->_parent = NULL;
}

//--------------------

// getters

NodeType TNode::getNodeType() {
	return this->_nodeType;
}

string TNode::getName() {
	// the name that comes before the colon e.g. else:stmtlst
	return this->_nodeName;
}

vector<TNode*>& TNode::getChildren() {
	return this->_children;
}

TNode* TNode::getParent() {
	return this->_parent;
}

TNode* TNode::getRightSibling() {
	return this->_rightSibling;
}
TNode* TNode::getLeftSibling() {
	return this->_leftSibling;
}

//--------------------

// setters

void TNode::setName(const string& name) {
	this->_nodeName = name;
}

void TNode::addChild(TNode *child) {
	this->_children.push_back(child);
}

void TNode::setParent(TNode *parent) {
	this->_parent = parent;
}

void TNode::linkParent(TNode *parent) {
	this->setParent(parent);
	parent->addChild(this);
}

void TNode::linkChild(TNode *child) {
	child->setParent(this);
	this->addChild(child);
}

void TNode::linkRightSibling(TNode *right) {
	right->_leftSibling = this;
	this->_rightSibling = right;
}

void TNode::linkLeftSibling(TNode *left) {
	left->_rightSibling = this;
	this->_leftSibling = left;
}

//---------------------

// checks
bool TNode::isNodeType(NodeType type) {
	return this->_nodeType == type;
}

bool TNode::hasChildren() {
	return this->_children.size() != 0;
}

bool TNode::hasParent() {
	return this->_parent != NULL;
}

bool TNode::hasRightSibling() {
	return this->_rightSibling != NULL;
}

bool TNode::hasLeftSibling() {
	return this->_leftSibling != NULL;
}