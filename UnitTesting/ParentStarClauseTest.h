#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParentStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ParentStarClauseTest );
	
	CPPUNIT_TEST( testParentStarFixedFixedPass );
	CPPUNIT_TEST( testParentStarFixedFixedFail );
	CPPUNIT_TEST( testParentStarFixedFixedFailSameStmt );
	CPPUNIT_TEST( testParentStarSynFixedPass );
	CPPUNIT_TEST( testParentStarSynFixedPassWithGeneric );
	CPPUNIT_TEST( testParentStarSynFixedFail );
	CPPUNIT_TEST( testParentStarFixedSynPass );
	CPPUNIT_TEST( testParentStarFixedSynPassWithWhile );
	CPPUNIT_TEST( testParentStarFixedSynPassWithGeneric );
	CPPUNIT_TEST( testParentStarFixedSynFail );
	CPPUNIT_TEST( testParentStarFixedSynFailWithWhile );
	CPPUNIT_TEST( testParentStarSynSynPass );
	CPPUNIT_TEST( testParentStarSynSynPassWithWhile );
	CPPUNIT_TEST( testParentStarSynSynPassWithGeneric );
	CPPUNIT_TEST( testParentStarInvalid );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testParentStarFixedFixedPass();
	void testParentStarFixedFixedFail();
	void testParentStarFixedFixedFailSameStmt();
	void testParentStarSynFixedPass();
	void testParentStarSynFixedPassWithGeneric();
	void testParentStarSynFixedFail();
	void testParentStarFixedSynPass();
	void testParentStarFixedSynPassWithWhile();
	void testParentStarFixedSynPassWithGeneric();
	void testParentStarFixedSynFail();
	void testParentStarFixedSynFailWithWhile();
	void testParentStarSynSynPass();
	void testParentStarSynSynPassWithWhile();
	void testParentStarSynSynPassWithGeneric();
	void testParentStarInvalid();
};
    