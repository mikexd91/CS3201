#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ResultsTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ResultsTest );
	CPPUNIT_TEST( testSingleGetIntercept );
	CPPUNIT_TEST( testSinglePairGetIntercept );
	CPPUNIT_TEST( testPairGetIntercept );
	CPPUNIT_TEST_SUITE_END();

public:
	// Testing of intercept will be done
	// It will be split into the following methods:

	void testSingleGetIntercept();
	void testSinglePairGetIntercept();
	void testPairGetIntercept();
};

