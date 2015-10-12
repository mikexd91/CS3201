#pragma once

#include <cppunit/extensions/HelperMacros.h>

class PQLIntegration : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( PQLIntegration );
	CPPUNIT_TEST( testSelectAssign );
	CPPUNIT_TEST( testSelectProgLine );
	CPPUNIT_TEST( testSelectBoolean );
	CPPUNIT_TEST( testSelectProcName );

	CPPUNIT_TEST( testSelectStmtModifies );
	CPPUNIT_TEST( testSelectProcUses );
	CPPUNIT_TEST( testSelectPatternAssg );
	CPPUNIT_TEST( testSelectPatternWhile );
	CPPUNIT_TEST( testSelectParentStar );
	CPPUNIT_TEST( testSelectNextStar );
	CPPUNIT_TEST( testSelectAffectsStar );
	CPPUNIT_TEST( testSelectCValueWith );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// SELECT WITHOUT CLAUSES
	void testSelectAssign();
	void testSelectProgLine();
	void testSelectBoolean();
	void testSelectProcName();

	// SELECT WITH ONE CLAUSE
	void testSelectStmtModifies();
	void testSelectProcUses();
	void testSelectPatternAssg();
	void testSelectPatternWhile();
	void testSelectParentStar();
	void testSelectNextStar();
	void testSelectAffectsStar();
	void testSelectCValueWith();
};