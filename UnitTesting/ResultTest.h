#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ResultTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ResultTest );
	CPPUNIT_TEST( testFail );
	CPPUNIT_TEST( testSingleInsertSynPresent );
	CPPUNIT_TEST( testSingleInsertSynAbsent );
	CPPUNIT_TEST( testMultiInsertBothSynsPresent );
	CPPUNIT_TEST( testMultiInsertBothSynsAbsent );
	CPPUNIT_TEST( testMultiInsertOneSynPresent );
	CPPUNIT_TEST( testGetMultiSynPass );
	CPPUNIT_TEST( testGetMultiSynFail );
	CPPUNIT_TEST( testGetSynPass );
	CPPUNIT_TEST( testGetSynFail );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testFail();
	void testSingleInsertSynPresent();
	void testSingleInsertSynAbsent();
	void testMultiInsertBothSynsPresent();
	void testMultiInsertBothSynsAbsent();
	void testMultiInsertOneSynPresent();
	void testGetMultiSynPass();
	void testGetMultiSynFail();
	void testGetSynPass();
	void testGetSynFail();

	void testLinks();
};
    