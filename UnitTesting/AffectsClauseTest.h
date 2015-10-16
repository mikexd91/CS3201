#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AffectsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AffectsClauseTest );
	CPPUNIT_TEST( testFixedFixedSameProc );
	CPPUNIT_TEST( testFixedFixedInWhile );
	CPPUNIT_TEST( testFixedFixedFail );
	CPPUNIT_TEST( testFixedFixedIfPass );
	CPPUNIT_TEST( testFixedFixedOutsideContainerPass );
	CPPUNIT_TEST( testFixedGenericPass );
	CPPUNIT_TEST( testFixedGenericFail );
	CPPUNIT_TEST( testFixedSynPass );
	CPPUNIT_TEST( testFixedSynPassInWhile );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFixedFixedSameProc();
	void testFixedFixedInWhile();
	void testFixedFixedFail();
	void testFixedFixedIfPass();
	void testFixedFixedOutsideContainerPass();
	void testFixedGenericPass();
	void testFixedGenericFail();
	void testFixedSynPass();
	void testFixedSynPassInWhile();
};
    