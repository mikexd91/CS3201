#pragma once

#include <cppunit/extensions/HelperMacros.h>

class PDR_Integration_Test : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( PDR_Integration_Test );
	CPPUNIT_TEST( testWhile );
	CPPUNIT_TEST( testNestedWhile );
	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

	void testWhile();
	void testNestedWhile();

};