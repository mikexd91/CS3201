#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class CallsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( CallsClauseTest );
	CPPUNIT_TEST( testCallsFixedFixedPass );
	CPPUNIT_TEST( testCallsFixedFixedFail );
	CPPUNIT_TEST( testCallsSynFixedPass );
	CPPUNIT_TEST( testCallsSynFixedFail );
	CPPUNIT_TEST( testCallsFixedSynPass );
	CPPUNIT_TEST( testCallsFixedSynFail );
	CPPUNIT_TEST( testCallsFirstUnderscorePass );
	CPPUNIT_TEST( testCallsSecondUnderscorePass );
	CPPUNIT_TEST( testCallsBothUnderscorePass );
	CPPUNIT_TEST( testCallsSynSynPass );
	CPPUNIT_TEST( testCallsSynSynSame );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testCallsFixedFixedPass();
	void testCallsFixedFixedFail();
	void testCallsSynFixedPass();
	void testCallsSynFixedFail();
	void testCallsFixedSynPass();
	void testCallsFixedSynFail();
	void testCallsFirstUnderscorePass();
	void testCallsSecondUnderscorePass();
	void testCallsBothUnderscorePass();
	void testCallsSynSynPass();
	void testCallsSynSynSame();
};