#include <cppunit/config/SourcePrefix.h>
#include "TNodeTest.h"
#include "../SPA/TNode.h"

#include <iostream>
#include <string>

void 
TNodeTest::setUp()
{
}

void 
TNodeTest::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TNodeTest ); // Note 4 

// method to test the constructor
void TNodeTest::testTNodeConstructor() {  // Note 5
	// create a student object
	TNode tnode(NodeType::PROGRAM_, "Alice");

	// check that the object is constructed correctly - Note 6
	std::string name = tnode.getName();
	CPPUNIT_ASSERT(name == "Alice");

	NodeType nodeType = tnode.getNodeType();
	CPPUNIT_ASSERT(nodeType == NodeType::PROGRAM_);

	return;
}

// method to test the assigning and retrieval of grades
void TNodeTest::testLinkChild()
{
	// create a student
	TNode anode(NodeType::PROGRAM_, "Alice");
	TNode bnode(NodeType::PROCEDURE_, "Alice");

	// link bnode as child of anode
	anode.linkChild(&bnode);

	// verify that the assignment is correct - Note 7
	CPPUNIT_ASSERT_EQUAL(&bnode, anode.getChildren()[0]);
	CPPUNIT_ASSERT_EQUAL(&anode, bnode.getParent());

	return;
}