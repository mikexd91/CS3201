#include <cppunit/config/SourcePrefix.h>
#include "IfNodeTest.h"
#include "../SPA/IfNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
IfNodeTest::setUp() {
}

void 
IfNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( IfNodeTest );

void IfNodeTest::testProperties() { 

	// constructor
	IfNode inode(1);
	CPPUNIT_ASSERT_EQUAL(IF_STMT_, inode.getNodeType());
	CPPUNIT_ASSERT_EQUAL(1, inode.getStmtNum());

	return;
}

void IfNodeTest::testLinks() {

	IfNode inode(1);
	StmtLstNode tnode("then");
	StmtLstNode enode("else");
	VarNode vnode("x");

	// linkvarnode
	inode.linkVarNode(&vnode);
	
	// linkthenstmtlstnode
	inode.linkThenStmtLstNode(&tnode);

	// linkelsestmtlstnode
	inode.linkElseStmtLstNode(&enode);

	// getvarnode
	CPPUNIT_ASSERT_EQUAL(&vnode, inode.getVarNode());
	CPPUNIT_ASSERT_EQUAL(&inode, (IfNode*)(vnode.getParent()));

	// getthenstmtlstnode
	CPPUNIT_ASSERT_EQUAL(&tnode, inode.getThenStmtLstNode());
	CPPUNIT_ASSERT_EQUAL(&inode, (IfNode*)(tnode.getParent()));

	// getelsestmtlstnode
	CPPUNIT_ASSERT_EQUAL(&enode, inode.getElseStmtLstNode());
	CPPUNIT_ASSERT_EQUAL(&inode, (IfNode*)(enode.getParent()));

	return;
}