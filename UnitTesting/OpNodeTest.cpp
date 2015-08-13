#include <cppunit/config/SourcePrefix.h>
#include "OpNodeTest.h"
#include "../SPA/OpNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/VarNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
OpNodeTest::setUp() {
}

void 
OpNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( OpNodeTest );

void OpNodeTest::testProperties() { 

	// constructor
	OpNode onode("+");
	CPPUNIT_ASSERT_EQUAL(OPERATOR_, onode.getNodeType());
	string plus = "+";
	CPPUNIT_ASSERT_EQUAL(plus, onode.getName());

	return;
}

void OpNodeTest::testLinks() {

	VarNode vnode("x");
	OpNode onode("+");
	ConstNode cnode("1");

	// linkleftnode
	onode.linkLeftNode(&vnode);
	// linkrightnode
	onode.linkRightNode(&cnode);

	// getleftnode
	VarNode* vget = (VarNode*)onode.getLeftNode();
	CPPUNIT_ASSERT_EQUAL(&vnode, vget);
	string varname = "x";
	CPPUNIT_ASSERT_EQUAL(varname, vget->getName());
	

	// getrightnode
	ConstNode* cget = (ConstNode*)(onode.getRightNode());
	CPPUNIT_ASSERT_EQUAL(&cnode, cget);
	string constname = "1";
	CPPUNIT_ASSERT_EQUAL(constname, cget->getName());

	return;
}