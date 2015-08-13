#include <cppunit/config/SourcePrefix.h>
#include "VarNodeTest.h"
#include "../SPA/VarNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
VarNodeTest::setUp() {
}

void 
VarNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( VarNodeTest );

void VarNodeTest::testProperties() { 

	// constructor
	VarNode vnode("x");
	CPPUNIT_ASSERT_EQUAL(VARIABLE_, vnode.getNodeType());
	string expname = "x";
	CPPUNIT_ASSERT_EQUAL(expname, vnode.getName());

	return;
}

void VarNodeTest::testLinks() {

	// var node is a leaf.

	return;
}