#include "StmtLstNode.h"

// constructor
StmtLstNode::StmtLstNode() 
	: TNode(NodeType::STMTLST_) {

}

StmtLstNode::StmtLstNode(const string &nodeName)
	: TNode(NodeType::STMTLST_, nodeName) {

}

// setters
void StmtLstNode::linkStmtNode(StmtNode *stmtNode) {
	this->linkChild(stmtNode);
}
