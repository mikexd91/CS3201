#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ResultsTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ResultsTest );
	CPPUNIT_TEST( testHasResults );
	CPPUNIT_TEST( testIsClausePass );
	CPPUNIT_TEST( testInsertResult );
	CPPUNIT_TEST( testInsertMultiResult );
	CPPUNIT_TEST( testSelectSyn );
	CPPUNIT_TEST( testSelectMultiSyn );
	CPPUNIT_TEST_SUITE_END();

public:
	void testHasResults();
	void testIsClausePass();
	void testInsertResult();
	void testInsertMultiResult();
	void testSelectSyn();
	void testSelectMultiSyn();
	
};

