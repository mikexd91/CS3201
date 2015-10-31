#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryOptimiserTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryOptimiserTest );
	CPPUNIT_TEST ( testMutiSingleSyn1Component );
	CPPUNIT_TEST ( testMultiComponent );
	CPPUNIT_TEST ( testTrickyCase );
	
	//CPPUNIT_TEST( testSortingPatternAndSuchThatClauses );
	//CPPUNIT_TEST( testSortingWhileAndSuchThatClauses );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testMutiSingleSyn1Component();
	void testMultiComponent();
	void testTrickyCase();
	
	//void testSortingPatternAndSuchThatClauses();
	//void testSortingWhileAndSuchThatClauses();
};

