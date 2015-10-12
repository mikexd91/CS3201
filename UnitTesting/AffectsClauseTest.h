#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AffectsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AffectsClauseTest );
	CPPUNIT_TEST( testSynSynFixedSameProc );
	CPPUNIT_TEST( testSynSynFixedInWhile );
	CPPUNIT_TEST( testSynSynFixedFail );
	CPPUNIT_TEST( testSynFixedFixedIfPass );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testSynSynFixedSameProc();
	void testSynSynFixedInWhile();
	void testSynSynFixedFail();
	void testSynFixedFixedIfPass();
};
    