#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class FollowsStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( FollowsStarClauseTest );
	CPPUNIT_TEST( testFollowsStarFixedFixedPass );
	CPPUNIT_TEST( testFollowsStarFixedFixedFailWrongOrder );
	CPPUNIT_TEST( testFollowsStarFixedFixedFailNestingLevel );
	CPPUNIT_TEST( testFollowsStarFixedFixedFailSameStmt );
	CPPUNIT_TEST( testFollowsStarSynFixedPass );
	CPPUNIT_TEST( testFollowsStarSynFixedFail );
	CPPUNIT_TEST( testFollowsStarFixedSynPass );
	CPPUNIT_TEST( testFollowsStarFixedSynFail );
	CPPUNIT_TEST( testFollowsStarSynSynPass );
	CPPUNIT_TEST( testFollowsStarFirstUnderscoreFixedPass );
	CPPUNIT_TEST( testFollowsStarFirstUnderscoreSynPass );
	CPPUNIT_TEST( testFollowsStarFixedSecondUnderscorePass );
	CPPUNIT_TEST( testFollowsStarSynSecondUnderscorePass );
	CPPUNIT_TEST( testFollowsStarBothUnderscorePass );
	CPPUNIT_TEST( testFollowsStarStmtOverflow );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testFollowsStarFixedFixedPass();
	void testFollowsStarFixedFixedFailWrongOrder();
	void testFollowsStarFixedFixedFailNestingLevel();
	void testFollowsStarFixedFixedFailSameStmt();
	void testFollowsStarSynFixedPass();
	void testFollowsStarSynFixedFail();
	void testFollowsStarFixedSynPass();
	void testFollowsStarFixedSynFail();
	void testFollowsStarSynSynPass();
	void testFollowsStarFirstUnderscoreFixedPass();
	void testFollowsStarFirstUnderscoreSynPass();
	void testFollowsStarFixedSecondUnderscorePass();
	void testFollowsStarSynSecondUnderscorePass();
	void testFollowsStarBothUnderscorePass();
	void testFollowsStarStmtOverflow();

};
    