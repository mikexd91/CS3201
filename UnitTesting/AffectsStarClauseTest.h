#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AffectsStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AffectsStarClauseTest );
	CPPUNIT_TEST( testFixedFixedEasyPass );
	CPPUNIT_TEST( testFixedFixedPass );
	CPPUNIT_TEST( testFixedFixedPassInWhile );
	CPPUNIT_TEST( testFixedFixedFail );
	CPPUNIT_TEST( testFixedGenericPass );
	CPPUNIT_TEST( testFixedGenericFail );
	CPPUNIT_TEST( testGenericGenericPass );
	CPPUNIT_TEST( testFixedSynPass );
	CPPUNIT_TEST( testFixedSynFail );
	CPPUNIT_TEST( testSynSynPass );
	CPPUNIT_TEST( testSameSynSynPass );
	CPPUNIT_TEST( testSynGenericPass );
	CPPUNIT_TEST( testGenericSynPass );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFixedFixedEasyPass();
	void testFixedFixedPass();
	void testFixedFixedPassInWhile();
	void testFixedFixedFail();
	void testFixedGenericPass();
	void testFixedGenericFail();
	void testGenericGenericPass();
	void testFixedSynPass();
	void testFixedSynFail();
	void testSynSynPass();
	void testSameSynSynPass();
	void testSynGenericPass();
	void testGenericSynPass();
};
    