#include <cppunit/config/SourcePrefix.h>
#include "CallNodeTest.h"
#include "../SPA/CallNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
CallNodeTest::setUp() {
}

void 
CallNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CallNodeTest );

void CallNodeTest::testProperties() { 

	// constructor
	CallNode cnode(1, "Alice");
	CPPUNIT_ASSERT_EQUAL(NodeType::CALL_STMT_, cnode.getNodeType());
	CPPUNIT_ASSERT_EQUAL(1, cnode.getStmtNum());
	string expname = "Alice";
	CPPUNIT_ASSERT_EQUAL(expname, cnode.getName());

	return;
}

void CallNodeTest::testLinks() {

	// nth here cos callnode is always a leaf

	return;
}