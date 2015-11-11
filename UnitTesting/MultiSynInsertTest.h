#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class MultiSynInsertTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( MultiSynInsertTest );
	CPPUNIT_TEST( testSynAbsent );
	CPPUNIT_TEST( testBothSynsPresent );
	CPPUNIT_TEST( testOneSynPresent );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSynAbsent();
	void testBothSynsPresent();
	void testOneSynPresent();
};
    