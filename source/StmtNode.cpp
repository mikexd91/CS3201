#include "StmtNode.h"


StmtNode::StmtNode(NodeType type, int stmtNum)
	: TNode(type) {
		this->_stmtNum = stmtNum;
}

StmtNode::StmtNode(NodeType type, int stmtNum, const string& name)
	: TNode(type, name) {
		this->_stmtNum = stmtNum;
}

// getters 

int StmtNode::getStmtNum() {
	return this->_stmtNum;
}