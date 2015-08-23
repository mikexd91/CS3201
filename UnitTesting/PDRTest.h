#pragma once

#include "../SPA/PDR.h"

#include <cppunit/extensions/HelperMacros.h>

class PDRTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( PDRTest );
	CPPUNIT_TEST( testProcessProc );
	CPPUNIT_TEST( testProcessAssign );
	CPPUNIT_TEST( testProcessWhile );
	CPPUNIT_TEST( testProcessCallStmts1 );
	CPPUNIT_TEST( testMultipleProcs1 );
	CPPUNIT_TEST( testMultipleProcs2 );
	CPPUNIT_TEST( testNestingLevel );
	CPPUNIT_TEST_SUITE_END();

public:
	
	void setUp();
	void tearDown();

	void testProcessProc();
	void testProcessAssign();
	void testProcessWhile();
	void testProcessCallStmts1();
	void testMultipleProcs1();
	void testMultipleProcs2();
	void testNestingLevel();
};