#include <cppunit/config/SourcePrefix.h>
#include "AssgNodeTest.h"
#include "../SPA/AssgNode.h"
#include "../SPA/OpNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
AssgNodeTest::setUp() {
}

void 
AssgNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( AssgNodeTest );

void AssgNodeTest::testProperties() { 

	// constructor
	AssgNode anode(95143);
	CPPUNIT_ASSERT_EQUAL(ASSIGN_STMT_, anode.getNodeType());
	CPPUNIT_ASSERT_EQUAL(95143, anode.getStmtNum());

	return;
}

void AssgNodeTest::testLinks() {

	AssgNode anode(1);
	VarNode vnode("x");
	OpNode onode("+");
	
	// linkexprnode
	anode.linkExprNode(&onode);

	// linkvarnode
	anode.linkVarNode(&vnode);

	// getvarnode
	VarNode* vget = anode.getVarNode();
	CPPUNIT_ASSERT_EQUAL(&vnode, vget);
	string expname = "x";
	CPPUNIT_ASSERT_EQUAL(expname, vget->getName());

	// getexprnode
	OpNode* oget = (OpNode*)(anode.getExprNode());
	CPPUNIT_ASSERT_EQUAL(&onode, oget);
	string exponame = "+";
	CPPUNIT_ASSERT_EQUAL(exponame, oget->getName());

	return;
}