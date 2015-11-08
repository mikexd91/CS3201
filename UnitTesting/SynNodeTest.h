#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class SynNodeTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( SynNodeTest );
	CPPUNIT_TEST( testProperties );
	CPPUNIT_TEST( testLinearLinks );
	CPPUNIT_TEST( testCircularLinks );
	CPPUNIT_TEST( testBranchedLinks );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testProperties();
	void testLinearLinks();
	void testCircularLinks();
	void testBranchedLinks();
};

