#include "StmtLstNode.h"

// constructor
StmtLstNode::StmtLstNode() 
	: TNode(STMTLST_) {

}

StmtLstNode::StmtLstNode(const string &nodeName)
	: TNode(STMTLST_, nodeName) {

}

// setters
void StmtLstNode::linkStmtNode(StmtNode *stmtNode) {
	this->linkChild(stmtNode);
}
