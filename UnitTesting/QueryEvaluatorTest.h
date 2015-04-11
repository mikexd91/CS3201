#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryEvaluatorTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );
	CPPUNIT_TEST( testEvaluator );
	CPPUNIT_TEST( testEvaluator2 );
	CPPUNIT_TEST( testEvaluator3 );
	CPPUNIT_TEST_SUITE_END();

public:

	void testEvaluator();
	void testEvaluator2();
	void testEvaluator3();
};

