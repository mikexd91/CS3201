#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AssgNodeTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AssgNodeTest );
	CPPUNIT_TEST( testProperties );
	CPPUNIT_TEST( testLinks );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testProperties();

	void testLinks();
};
    