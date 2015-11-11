#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParentStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ParentStarClauseTest );
	
	CPPUNIT_TEST( testParentStarFixedFixedPass );
	CPPUNIT_TEST( testParentStarFixedFixedFail );
	CPPUNIT_TEST( testParentStarFixedFixedFailSameStmt );
	CPPUNIT_TEST( testParentStarGenericGenericPass );
	CPPUNIT_TEST( testParentStarFixedGenericPass );
	CPPUNIT_TEST( testParentStarGenericFixedPass );
	CPPUNIT_TEST( testParentStarGenericStatementPass );
	CPPUNIT_TEST( testParentStarGenericWhilePass );
	CPPUNIT_TEST( testParentStarWhileGenericPass );
	CPPUNIT_TEST( testParentStarSynFixedPass );
	CPPUNIT_TEST( testParentStarSynFixedFail );
	CPPUNIT_TEST( testParentStarFixedSynPass );
	CPPUNIT_TEST( testParentStarFixedSynPassWithIf );
	CPPUNIT_TEST( testParentStarFixedSynFail );
	CPPUNIT_TEST( testParentStarSynSynPass );
	CPPUNIT_TEST( testParentStarSynSynPassWithIf );
	CPPUNIT_TEST( testParentStarInvalid );
	CPPUNIT_TEST( testParentStarStmtOverflow );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testParentStarFixedFixedPass();
	void testParentStarFixedFixedFail();
	void testParentStarFixedFixedFailSameStmt();
	void testParentStarGenericGenericPass();
	void testParentStarFixedGenericPass();
	void testParentStarGenericFixedPass();
	void testParentStarGenericStatementPass();
	void testParentStarGenericWhilePass();
	void testParentStarWhileGenericPass();
	void testParentStarSynFixedPass();
	void testParentStarSynFixedFail();
	void testParentStarFixedSynPass();
	void testParentStarFixedSynPassWithIf();
	void testParentStarFixedSynFail();
	void testParentStarSynSynPass();
	void testParentStarSynSynPassWithIf();
	void testParentStarInvalid();
	void testParentStarStmtOverflow();
};    