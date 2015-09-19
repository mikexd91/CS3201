#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ResultTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ResultTest );
	CPPUNIT_TEST( testSingleInsertSynPresent );
	CPPUNIT_TEST( testSingleInsertSynAbsent );
	CPPUNIT_TEST( testMultiInsertBothSynsPresent );
	CPPUNIT_TEST( testMultiInsertBothSynsAbsent );
	CPPUNIT_TEST( testMultiInsertOneSynPresent );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSingleInsertSynPresent();
	void testSingleInsertSynAbsent();
	void testMultiInsertBothSynsPresent();
	void testMultiInsertBothSynsAbsent();
	void testMultiInsertOneSynPresent();

	void testLinks();
};
    