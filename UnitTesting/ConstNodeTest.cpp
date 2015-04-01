#include <cppunit/config/SourcePrefix.h>
#include "ConstNodeTest.h"
#include "../SPA/ConstNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
ConstNodeTest::setUp() {
}

void 
ConstNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ConstNodeTest );

void ConstNodeTest::testProperties() { 

	// constructor
	ConstNode cnode("1");
	CPPUNIT_ASSERT_EQUAL(NodeType::CONSTANT_, cnode.getNodeType());
	string expname = "1";
	CPPUNIT_ASSERT_EQUAL(expname, cnode.getName());

	return;
}

void ConstNodeTest::testLinks() {

	// const node is a leaf.

	return;
}