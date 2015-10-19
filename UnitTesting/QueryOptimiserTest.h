#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryOptimiserTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryOptimiserTest );
	CPPUNIT_TEST( testSortQueryDifferentBuilder );
	CPPUNIT_TEST( testSortQuerySuchThatBuilder );
	CPPUNIT_TEST( testSoryQueryPatternBuilder );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSortQueryDifferentBuilder();
	void testSortQuerySuchThatBuilder();
	void testSoryQueryPatternBuilder();
};

