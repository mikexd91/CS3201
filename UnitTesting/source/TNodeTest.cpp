#include <cppunit/config/SourcePrefix.h>
#include "TNodeTest.h"
#include "../SPA/TNode.h"

#include <iostream>
#include <string>

using namespace std;

void 
TNodeTest::setUp() {
}

void 
TNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TNodeTest );

void TNodeTest::testProperties() { 

	// constructor
	TNode tnode(PROGRAM_, "Alice");

	// getnodetype
	// ok
	CPPUNIT_ASSERT(tnode.getNodeType() == PROGRAM_);
	// nok
	CPPUNIT_ASSERT(tnode.getNodeType() != CALL_STMT_);

	// getname
	// ok
	CPPUNIT_ASSERT(tnode.getName() == "Alice");
	// nok
	CPPUNIT_ASSERT(tnode.getName() != "alice");

	// constructor
	TNode snode(ASSIGN_STMT_);

	// setname
	snode.setName("Bob");
	CPPUNIT_ASSERT(snode.getName() == "Bob");

	// isnodetype
	// ok
	CPPUNIT_ASSERT(snode.isNodeType(ASSIGN_STMT_));
	// nok
	CPPUNIT_ASSERT(!snode.isNodeType(CONSTANT_));

	return;
}

void TNodeTest::testLinks() {

	// a, b
	TNode anode(PROGRAM_, "Alice");
	TNode bnode(PROCEDURE_, "Alice");
	
	// linkchild
	anode.linkChild(&bnode);

	// getchildren
	// ok
	CPPUNIT_ASSERT_EQUAL(&bnode, anode.getChildren()[0]);
	
	// getparent
	// ok
	CPPUNIT_ASSERT_EQUAL(&anode, bnode.getParent());
	// nok
	CPPUNIT_ASSERT(&bnode != anode.getParent());

	// hasparent
	// ok
	CPPUNIT_ASSERT(bnode.hasParent());
	// nok
	CPPUNIT_ASSERT(!anode.hasParent());

	// haschildren
	// ok
	CPPUNIT_ASSERT(anode.hasChildren());
	// nok
	CPPUNIT_ASSERT(!bnode.hasChildren());

	// c, d
	TNode cnode(PROGRAM_, "Alice");
	TNode dnode(PROCEDURE_, "Alice");

	// linkparent
	dnode.linkParent(&cnode);

	CPPUNIT_ASSERT_EQUAL(&dnode, cnode.getChildren()[0]);
	
	CPPUNIT_ASSERT_EQUAL(&cnode, dnode.getParent());

	// e, f
	TNode enode(PROGRAM_, "Alice");
	TNode fnode(PROCEDURE_, "Alice");

	// linkrightsibling
	enode.linkRightSibling(&fnode);

	// getrightsibling
	// ok
	CPPUNIT_ASSERT_EQUAL(&fnode, enode.getRightSibling());
	// nok
	CPPUNIT_ASSERT(&enode != fnode.getRightSibling());
	
	// getleftsibling
	// ok
	CPPUNIT_ASSERT_EQUAL(&enode, fnode.getLeftSibling());
	// nok
	CPPUNIT_ASSERT(&fnode != enode.getLeftSibling());

	// hasrightsibling
	// ok
	CPPUNIT_ASSERT(enode.hasRightSibling());
	// nok
	CPPUNIT_ASSERT(!fnode.hasRightSibling());

	// hasleftsibling
	// ok
	CPPUNIT_ASSERT(fnode.hasLeftSibling());
	// nok
	CPPUNIT_ASSERT(!enode.hasLeftSibling());

	// g, h
	TNode gnode(PROGRAM_, "Alice");
	TNode hnode(PROCEDURE_, "Alice");

	// linkleftsibling
	hnode.linkLeftSibling(&gnode);

	CPPUNIT_ASSERT_EQUAL(&hnode, gnode.getRightSibling());
	
	CPPUNIT_ASSERT_EQUAL(&gnode, hnode.getLeftSibling());

	return;
}