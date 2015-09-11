#pragma once

#include <cppunit/extensions/HelperMacros.h>

class UsesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( UsesClauseTest );

	CPPUNIT_TEST( testFixedFixedStmtPass );
	CPPUNIT_TEST( testFixedFixedStmtFail );
	CPPUNIT_TEST( testFixedFixedProcPass );
	CPPUNIT_TEST( testFixedFixedProcFail );

	CPPUNIT_TEST( testGenericGenericPass );
	CPPUNIT_TEST( testGenericFixedPass );
	CPPUNIT_TEST( testGenericFixedFail );

	CPPUNIT_TEST( testFixedGenericStmtPass );
	CPPUNIT_TEST( testFixedGenericStmtFail );
	CPPUNIT_TEST( testFixedGenericProcPass );
	CPPUNIT_TEST( testFixedGenericProcFail );

	CPPUNIT_TEST( testFixedSynStmtPass );
	CPPUNIT_TEST( testFixedSynStmtFail );
	CPPUNIT_TEST( testFixedSynProcPass );
	CPPUNIT_TEST( testFixedSynProcFail );

	CPPUNIT_TEST ( testGenericSynPass );

	CPPUNIT_TEST( testSynFixedStmtPass );
	CPPUNIT_TEST( testSynFixedStmtFail );
	CPPUNIT_TEST( testSynFixedProcPass );
	CPPUNIT_TEST( testSynFixedProcFail );
	CPPUNIT_TEST( testSynFixedWhilePass );
	CPPUNIT_TEST( testSynFixedWhileFail );
	CPPUNIT_TEST( testSynFixedIfPass );
	CPPUNIT_TEST( testSynFixedIfFail );
	CPPUNIT_TEST( testSynFixedAssgPass );
	CPPUNIT_TEST( testSynFixedAssgFail );

	CPPUNIT_TEST( testSynGenericProcPass );
	CPPUNIT_TEST( testSynGenericIfPass );
	CPPUNIT_TEST( testSynGenericWhilePass );
	CPPUNIT_TEST( testSynGenericStmtPass );
	CPPUNIT_TEST( testSynGenericAssgPass );	

	CPPUNIT_TEST( testSynSynProcPass );
	CPPUNIT_TEST( testSynSynIfPass );
	CPPUNIT_TEST( testSynSynWhilePass );
	CPPUNIT_TEST( testSynSynStmtPass );
	CPPUNIT_TEST( testSynSynAssgPass );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testFixedFixedStmtPass();
	void testFixedFixedStmtFail();
	void testFixedFixedProcPass();
	void testFixedFixedProcFail();

	void testGenericGenericPass();

	void testGenericFixedPass();
	void testGenericFixedFail();

	void testFixedGenericStmtPass();
	void testFixedGenericStmtFail();
	void testFixedGenericProcPass();
	void testFixedGenericProcFail();

	void testFixedSynStmtPass();
	void testFixedSynStmtFail();
	void testFixedSynProcPass();
	void testFixedSynProcFail();
	void testGenericSynPass();

	void testSynFixedStmtPass();
	void testSynFixedStmtFail();
	void testSynFixedProcPass();
	void testSynFixedProcFail();
	void testSynFixedWhilePass();
	void testSynFixedWhileFail();
	void testSynFixedIfPass();
	void testSynFixedIfFail();
	void testSynFixedAssgPass();
	void testSynFixedAssgFail();

	void testSynGenericProcPass();
	void testSynGenericIfPass();
	void testSynGenericWhilePass();
	void testSynGenericStmtPass();
	void testSynGenericAssgPass();

	void testSynSynProcPass();
	void testSynSynIfPass();
	void testSynSynWhilePass();
	void testSynSynStmtPass();
	void testSynSynAssgPass();
};