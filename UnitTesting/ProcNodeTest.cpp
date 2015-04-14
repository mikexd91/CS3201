#include <cppunit/config/SourcePrefix.h>
#include "ProcNodeTest.h"
#include "../SPA/ProcNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
ProcNodeTest::setUp() {
}

void 
ProcNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ProcNodeTest );

void ProcNodeTest::testProperties() { 

	// constructor
	ProcNode pnode("Alice");
	CPPUNIT_ASSERT_EQUAL(PROCEDURE_, pnode.getNodeType());
	string expname = "Alice";
	CPPUNIT_ASSERT_EQUAL(expname, pnode.getName());

	return;
}

void ProcNodeTest::testLinks() {

	ProcNode pnode("Alice");
	StmtLstNode slnode("then");

	// linkstmtlstnode
	pnode.linkStmtLstNode(&slnode);

	// getprocnodes
	CPPUNIT_ASSERT_EQUAL(&slnode, pnode.getStmtLstNode());
	CPPUNIT_ASSERT_EQUAL(&pnode, (ProcNode*)slnode.getParent());
	string expname = "then";
	CPPUNIT_ASSERT_EQUAL(expname, pnode.getStmtLstNode()->getName());

	return;
}