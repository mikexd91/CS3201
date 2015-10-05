#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class FollowsClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( FollowsClauseTest );
	CPPUNIT_TEST( testFollowsFixedFixedPass );
	CPPUNIT_TEST( testFollowsFixedFixedFailWrongOrder );
	CPPUNIT_TEST( testFollowsFixedFixedFailNestingLevel );
	CPPUNIT_TEST( testFollowsFixedFixedFailSameStmt );
	CPPUNIT_TEST( testFollowsSynFixedPass );
	CPPUNIT_TEST( testFollowsSynFixedFail );
	CPPUNIT_TEST( testFollowsFixedSynPass );
	CPPUNIT_TEST( testFollowsFixedSynFail );
	CPPUNIT_TEST( testFollowsSynSynPass );
	CPPUNIT_TEST( testFollowsSynSynSameFail );
	CPPUNIT_TEST( testFollowsFirstUnderscoreFixedPass );
	CPPUNIT_TEST( testFollowsFirstUnderscoreSynPass );
	CPPUNIT_TEST( testFollowsFixedSecondUnderscorePass );
	CPPUNIT_TEST( testFollowsSynSecondUnderscorePass );
	CPPUNIT_TEST( testFollowsBothUnderscorePass );
	CPPUNIT_TEST( testFollowsStmtOverflow );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFollowsFixedFixedPass();
	void testFollowsFixedFixedFailWrongOrder();
	void testFollowsFixedFixedFailNestingLevel();
	void testFollowsFixedFixedFailSameStmt();
	void testFollowsSynFixedPass();
	void testFollowsSynFixedFail();
	void testFollowsFixedSynPass();
	void testFollowsFixedSynFail();
	void testFollowsSynSynPass();
	void testFollowsSynSynSameFail();
	void testFollowsFirstUnderscoreFixedPass();
	void testFollowsFirstUnderscoreSynPass();
	void testFollowsFixedSecondUnderscorePass();
	void testFollowsSynSecondUnderscorePass();
	void testFollowsBothUnderscorePass();
	void testFollowsStmtOverflow();
};
    