#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>
#include "../SPA/TNode.h"
#include <string>

using namespace std;

class TNodeTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( TNodeTest ); // Note 3 
	CPPUNIT_TEST( testTNodeConstructor );
	CPPUNIT_TEST( testLinkChild );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// method to test the constructor
	void testTNodeConstructor();

	// method to test the assigning and retrieval of grades
	void testLinkChild();
};
    