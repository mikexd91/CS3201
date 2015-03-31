#pragma once

#include <cppunit/extensions/HelperMacros.h>

class ProcTableTest : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( ProcTableTest );
	CPPUNIT_TEST( testInstance );
	CPPUNIT_TEST( testAddProc );
	CPPUNIT_TEST( testFalseAddProc );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testInstance();			// method to test singleton instance getter
	void testAddProc();				// method to test adding of proc to table
	void testFalseAddProc();		// method to test false adding of proc to table
};