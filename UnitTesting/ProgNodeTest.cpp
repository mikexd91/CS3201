#include <cppunit/config/SourcePrefix.h>
#include "ProgNodeTest.h"
#include "../SPA/ProgNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
ProgNodeTest::setUp() {
}

void 
ProgNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ProgNodeTest );

void ProgNodeTest::testProperties() { 

	// constructor
	ProgNode pnode("Alice");
	CPPUNIT_ASSERT_EQUAL(PROGRAM_, pnode.getNodeType());
	string expname = "Alice";
	CPPUNIT_ASSERT_EQUAL(expname, pnode.getName());

	return;
}

void ProgNodeTest::testLinks() {

	ProgNode pnode("Alice");
	ProcNode wnode("BobProc");

	// linkprocnode
	pnode.linkProcNode(&wnode);

	// getprocnodes
	CPPUNIT_ASSERT_EQUAL(&wnode, (ProcNode*)pnode.getProcNodes().at(0));
	CPPUNIT_ASSERT_EQUAL(&pnode, (ProgNode*)wnode.getParent());
	ProcNode proc = *((ProcNode*)(pnode.getProcNodes().at(0)));
	string expname = "BobProc";
	CPPUNIT_ASSERT_EQUAL(expname, proc.getName());

	return;
}