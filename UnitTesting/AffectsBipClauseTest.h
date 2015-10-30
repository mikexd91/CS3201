#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class AffectsBipClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( AffectsBipClauseTest );
	CPPUNIT_TEST( testFixedFixedSameProc );
	CPPUNIT_TEST( testFixedFixedInWhile );
	CPPUNIT_TEST( testFixedFixedPass );
	CPPUNIT_TEST( testFixedFixedCallPass );
	CPPUNIT_TEST( testFixedFixedCallFail );
	CPPUNIT_TEST( testFixedSynPass );
	CPPUNIT_TEST( testFixedSynPassWithoutCall );
	CPPUNIT_TEST( testSynSynPass );
	CPPUNIT_TEST( testGenericGenericPass );
	CPPUNIT_TEST( testGenericSynPass );
	CPPUNIT_TEST( testSynGenericPass );
	CPPUNIT_TEST( testFixedGenericPass );
	CPPUNIT_TEST( testFixedSynPassInWhile );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testFixedFixedSameProc();
	void testFixedFixedInWhile();
	void testFixedFixedPass();
	void testFixedFixedCallPass();
	void testFixedFixedCallFail();
	void testFixedSynPass();
	void testFixedSynPassWithoutCall();
	void testSynSynPass();
	void testGenericGenericPass();
	void testGenericSynPass();
	void testSynGenericPass();
	void testFixedGenericPass();
	void testFixedSynPassInWhile();
};
    