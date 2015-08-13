#include <cppunit/config/SourcePrefix.h>
#include "WhileNodeTest.h"
#include "../SPA/WhileNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
WhileNodeTest::setUp() {
}

void 
WhileNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( WhileNodeTest );

void WhileNodeTest::testProperties() { 

	// constructor
	WhileNode inode(999234234);
	CPPUNIT_ASSERT_EQUAL(WHILE_STMT_, inode.getNodeType());
	CPPUNIT_ASSERT_EQUAL(999234234, inode.getStmtNum());

	return;
}

void WhileNodeTest::testLinks() {

	WhileNode inode(1);
	StmtLstNode slnode;
	VarNode vnode("x");

	// linkvarnode
	inode.linkVarNode(&vnode);
	
	// linkstmtlstnode
	inode.linkStmtLstNode(&slnode);

	// getvarnode
	CPPUNIT_ASSERT_EQUAL(&vnode, inode.getVarNode());
	CPPUNIT_ASSERT_EQUAL(&inode, (WhileNode*)(vnode.getParent()));

	// getstmtlstnode
	CPPUNIT_ASSERT_EQUAL(&slnode, inode.getStmtLstNode());
	CPPUNIT_ASSERT_EQUAL(&inode, (WhileNode*)(slnode.getParent()));

	return;
}