#include "CallNode.h"


CallNode::CallNode(int stmtNum, const string &nodeName)
	: StmtNode(NodeType::CALL_STMT_, stmtNum, nodeName) {

}

