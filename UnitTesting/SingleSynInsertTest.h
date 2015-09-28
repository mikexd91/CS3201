#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class SingleSynInsertTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( SingleSynInsertTest );
	CPPUNIT_TEST( testSynPresent );
	CPPUNIT_TEST( testSynAbsent );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSynPresent();

	void testSynAbsent();
};
    