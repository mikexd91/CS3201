#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class ResultsTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ResultsTest );
	CPPUNIT_TEST( testHasResults );
	CPPUNIT_TEST_SUITE_END();

public:
	void testHasResults();
	
};

