#pragma once

#include <cppunit/extensions/HelperMacros.h>

class UsesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( UsesClauseTest );
	CPPUNIT_TEST( testUsesAssignFixedFixedPass );
	CPPUNIT_TEST( testUsesAssignFixedFixedFail );
	CPPUNIT_TEST( testUsesAssignSynFixedPass );
	CPPUNIT_TEST( testUsesGenericSynFixedPass );
	CPPUNIT_TEST( testUsesAssignSynFixedFail );
	CPPUNIT_TEST( testUsesAssignFixedSynPass );
	CPPUNIT_TEST( testUsesGenericFixedSynPass );
	CPPUNIT_TEST( testUsesAssignFixedSynFail );
	CPPUNIT_TEST( testUsesAssignSynSynPass );
	CPPUNIT_TEST( testUsesWhileSynSynPass );
	CPPUNIT_TEST( testUsesGenericSynSynPass );
	CPPUNIT_TEST( testUsesStmtOverflow );
	CPPUNIT_TEST( testUsesFirstGenericPass );
	CPPUNIT_TEST( testUsesSecondGenericPass );
	CPPUNIT_TEST( testUsesGenericFixedPass );
	CPPUNIT_TEST( testUsesFixedGenericFail );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testUsesAssignFixedFixedPass();
	void testUsesAssignFixedFixedFail();
	void testUsesAssignSynFixedPass();
	void testUsesGenericSynFixedPass();
	void testUsesAssignSynFixedFail();
	void testUsesAssignFixedSynPass();
	void testUsesGenericFixedSynPass();
	void testUsesAssignFixedSynFail();
	void testUsesAssignSynSynPass();
	void testUsesWhileSynSynPass();
	void testUsesGenericSynSynPass();
	void testUsesStmtOverflow();
	void testUsesFirstGenericPass();
	void testUsesSecondGenericPass();
	void testUsesGenericFixedPass();
	void testUsesFixedGenericFail();
};