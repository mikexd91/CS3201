#include <cppunit/config/SourcePrefix.h>
#include "StmtLstNodeTest.h"
#include "../SPA/StmtLstNode.h"
#include "../SPA/AssgNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
StmtLstNodeTest::setUp() {
}

void 
StmtLstNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( StmtLstNodeTest );

void StmtLstNodeTest::testProperties() { 

	// constructor
	StmtLstNode pnode("then");
	CPPUNIT_ASSERT_EQUAL(NodeType::STMTLST_, pnode.getNodeType());
	string expname = "then";
	CPPUNIT_ASSERT_EQUAL(expname, pnode.getName());

	return;
}

void StmtLstNodeTest::testLinks() {

	StmtLstNode pnode;
	AssgNode anode(1);

	// linkstmtnode
	pnode.linkStmtNode(&anode);

	// getStmtNodes
	AssgNode* get = (AssgNode*)(pnode.getChildren().at(0));
	CPPUNIT_ASSERT_EQUAL(&anode, get);
	CPPUNIT_ASSERT_EQUAL(1, get->getStmtNum());

	return;
}