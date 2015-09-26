#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class CallsStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( CallsStarClauseTest );
	CPPUNIT_TEST( testCallsStarFixedFixedPass );
	CPPUNIT_TEST( testCallsStarFixedFixedFail );
	CPPUNIT_TEST( testCallsStarSynFixedPass );
	CPPUNIT_TEST( testCallsStarSynFixedFail );
	CPPUNIT_TEST( testCallsStarFixedSynPass );
	CPPUNIT_TEST( testCallsStarFixedSynFail );
	CPPUNIT_TEST( testCallsStarFirstUnderscorePass );
	CPPUNIT_TEST( testCallsStarSecondUnderscorePass );
	CPPUNIT_TEST( testCallsStarBothUnderscorePass );
	CPPUNIT_TEST( testCallsStarSynSynPass );
	CPPUNIT_TEST( testCallsStarSynSynSame );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testCallsStarFixedFixedPass();
	void testCallsStarFixedFixedFail();
	void testCallsStarSynFixedPass();
	void testCallsStarSynFixedFail();
	void testCallsStarFixedSynPass();
	void testCallsStarFixedSynFail();
	void testCallsStarFirstUnderscorePass();
	void testCallsStarSecondUnderscorePass();
	void testCallsStarBothUnderscorePass();
	void testCallsStarSynSynPass();
	void testCallsStarSynSynSame();
};
