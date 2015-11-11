#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AffectsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AffectsClauseTest );
	CPPUNIT_TEST( testGenericGenericPass );
	CPPUNIT_TEST( testGenericSynPass );
	CPPUNIT_TEST( testSynGenericPass );
	CPPUNIT_TEST( testFixedFixedSameProc );
	CPPUNIT_TEST( testFixedFixedInWhile );
	CPPUNIT_TEST( testFixedFixedFail );
	CPPUNIT_TEST( testFixedFixedCallFail );
	CPPUNIT_TEST( testFixedFixedIfPass );
	CPPUNIT_TEST( testFixedFixedOutsideContainerPass );
	CPPUNIT_TEST( testFixedGenericPass );
	CPPUNIT_TEST( testFixedGenericFail );
	CPPUNIT_TEST( testFixedSynPass );
	CPPUNIT_TEST( testFixedSynPassInWhile );
	CPPUNIT_TEST( testSynSynPass );
	CPPUNIT_TEST( testGenericFixedPass );
	CPPUNIT_TEST( testGenericFixedFail );
	CPPUNIT_TEST( testSynFixedPass );
	CPPUNIT_TEST( testSynFixedFail );
	CPPUNIT_TEST( testSameSynSynPass );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testGenericGenericPass();
	void testGenericSynPass();
	void testSynGenericPass();
	void testFixedFixedSameProc();
	void testFixedFixedInWhile();
	void testFixedFixedFail();
	void testFixedFixedCallFail();
	void testFixedFixedIfPass();
	void testFixedFixedOutsideContainerPass();
	void testFixedGenericPass();
	void testFixedGenericFail();
	void testFixedSynPass();
	void testFixedSynPassInWhile();
	void testSynSynPass();
	void testGenericFixedPass();
	void testGenericFixedFail();
	void testSynFixedPass();
	void testSynFixedFail();
	void testSameSynSynPass();
};
    