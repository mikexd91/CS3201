#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ProcNodeTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ProcNodeTest );
	CPPUNIT_TEST( testProperties );
	CPPUNIT_TEST( testLinks );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testProperties();

	void testLinks();
};
    