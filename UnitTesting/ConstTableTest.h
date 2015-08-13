#pragma once

#include <cppunit/extensions/HelperMacros.h>

class ConstTableTest : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( ConstTableTest );
	CPPUNIT_TEST( testInstance );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testInstance();			// method to test singleton instance getter

};