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

	CPPUNIT_TEST( testSelectVVarNameWithPatternAssg );
	CPPUNIT_TEST( testSelectPatternIfParent );
	CPPUNIT_TEST( testSelectCallsStarUses );
	CPPUNIT_TEST( testSelectFollowsStarModifies );

	CPPUNIT_TEST( testSelectPProcNameWithPatternAssgCalls );

	CPPUNIT_TEST( testFailSelectIStmtNumWith );
	CPPUNIT_TEST( testFailSelectBooleanFollowsStar );
	CPPUNIT_TEST( testFailSelectPatternAssgPatternAssg );
	CPPUNIT_TEST( testFailSelectModifiesModifies );
	
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

	// SELECT WITH TWO CLAUSES
	void testSelectVVarNameWithPatternAssg();
	void testSelectPatternIfParent();
	void testSelectCallsStarUses();
	void testSelectFollowsStarModifies();

	// SELECT WITH THREE CLAUSES
	void testSelectPProcNameWithPatternAssgCalls();

	// SELECT WITH NO RESULTS
	void testFailSelectIStmtNumWith();
	void testFailSelectBooleanFollowsStar();
	void testFailSelectPatternAssgPatternAssg();
	void testFailSelectModifiesModifies();
};