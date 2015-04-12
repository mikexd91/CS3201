#pragma once

#include "../SPA/PDR.h"

#include <cppunit/extensions/HelperMacros.h>

class PDRTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( PDRTest );
	CPPUNIT_TEST( testProcessProc );
	CPPUNIT_TEST( testProcessAssign );
	CPPUNIT_TEST_SUITE_END();

public:
	
	void setUp();
	void tearDown();

	void testProcessProc();
	void testProcessAssign();
};