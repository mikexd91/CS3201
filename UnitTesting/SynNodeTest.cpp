#include <cppunit/config/SourcePrefix.h>
#include "SynNodeTest.h"
#include "../SPA/SynNode.h"

void SynNodeTest::setUp() {
}

void SynNodeTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SynNodeTest );

void SynNodeTest::testProperties() { 

	// constructor
	SynNode* syn1 = new SynNode("a");
	SynNode* syn2 = new SynNode("b");
	syn1->setWeight(100);
	syn2->setWeight(1);
	syn1->setEdge(10, syn2);

	CPPUNIT_ASSERT(100 == syn1->getWeight());
	CPPUNIT_ASSERT(1 == syn2->getWeight());
	CPPUNIT_ASSERT(1 == syn1->getEdges().size());
}

void SynNodeTest::testLinearLinks() {
	SynNode* syn1 = new SynNode("a");
	SynNode* syn2 = new SynNode("b");
	SynNode* syn3 = new SynNode("c");
	SynNode* syn4 = new SynNode("d");
	SynNode* syn5 = new SynNode("e");
	SynNode* syn6 = new SynNode("f");
	SynNode* syn7 = new SynNode("g");

	syn1->setEdge(1, syn2);
	syn2->setEdge(1, syn3);
	syn3->setEdge(1, syn4);
	syn4->setEdge(1, syn5);
	syn5->setEdge(1, syn6);
	syn6->setEdge(1, syn7);

	CPPUNIT_ASSERT(1 == syn1->getEdges().size());
	CPPUNIT_ASSERT(2 == syn2->getEdges().size());
	CPPUNIT_ASSERT(2 == syn3->getEdges().size());
	CPPUNIT_ASSERT(2 == syn4->getEdges().size());
	CPPUNIT_ASSERT(2 == syn5->getEdges().size());
	CPPUNIT_ASSERT(2 == syn6->getEdges().size());
	CPPUNIT_ASSERT(1 == syn7->getEdges().size());
}

void SynNodeTest::testCircularLinks() {
	SynNode* syn1 = new SynNode("a");
	SynNode* syn2 = new SynNode("b");
	SynNode* syn3 = new SynNode("c");
	SynNode* syn4 = new SynNode("d");
	SynNode* syn5 = new SynNode("e");
	SynNode* syn6 = new SynNode("f");
	SynNode* syn7 = new SynNode("g");

	syn1->setEdge(1, syn2);
	syn2->setEdge(1, syn3);
	syn3->setEdge(1, syn4);
	syn4->setEdge(1, syn5);
	syn5->setEdge(1, syn6);
	syn6->setEdge(1, syn7);
	syn7->setEdge(1, syn1);

	CPPUNIT_ASSERT(2 == syn1->getEdges().size());
	CPPUNIT_ASSERT(2 == syn2->getEdges().size());
	CPPUNIT_ASSERT(2 == syn3->getEdges().size());
	CPPUNIT_ASSERT(2 == syn4->getEdges().size());
	CPPUNIT_ASSERT(2 == syn5->getEdges().size());
	CPPUNIT_ASSERT(2 == syn6->getEdges().size());
	CPPUNIT_ASSERT(2 == syn7->getEdges().size());
}

void SynNodeTest::testBranchedLinks() {
	SynNode* syn1 = new SynNode("a");
	SynNode* syn2 = new SynNode("b");
	SynNode* syn3 = new SynNode("c");
	SynNode* syn4 = new SynNode("d");
	SynNode* syn5 = new SynNode("e");
	SynNode* syn6 = new SynNode("f");
	SynNode* syn7 = new SynNode("g");

	syn1->setEdge(1, syn2);
	syn1->setEdge(1, syn3);
	syn2->setEdge(1, syn4);
	syn2->setEdge(1, syn5);
	syn3->setEdge(1, syn6);
	syn3->setEdge(1, syn7);

	CPPUNIT_ASSERT(2 == syn1->getEdges().size());
	CPPUNIT_ASSERT(3 == syn2->getEdges().size());
	CPPUNIT_ASSERT(3 == syn3->getEdges().size());
	CPPUNIT_ASSERT(1 == syn4->getEdges().size());
	CPPUNIT_ASSERT(1 == syn5->getEdges().size());
	CPPUNIT_ASSERT(1 == syn6->getEdges().size());
	CPPUNIT_ASSERT(1 == syn7->getEdges().size());
}