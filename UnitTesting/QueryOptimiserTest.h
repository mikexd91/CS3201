#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryOptimiserTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryOptimiserTest );

	CPPUNIT_TEST ( testGettingStartingSynonym );
	CPPUNIT_TEST ( testWithCombination );
	CPPUNIT_TEST ( testPatternAssignWhileIfCombination );
	CPPUNIT_TEST ( testManySingleSyn );
	CPPUNIT_TEST ( testManyComponent );
	CPPUNIT_TEST ( testTrickyCase );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testGettingStartingSynonym();
	void testWithCombination();
	void testPatternAssignWhileIfCombination();
	void testManySingleSyn();
	void testManyComponent();
	void testTrickyCase();
	
};

