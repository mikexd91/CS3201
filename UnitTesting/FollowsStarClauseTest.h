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
	CPPUNIT_TEST( testFollowsStarSynFixedPassWithWhile );
	CPPUNIT_TEST( testFollowsStarSynFixedPassWithAssign );

	CPPUNIT_TEST( testFollowsStarSynFixedFail );
	CPPUNIT_TEST( testFollowsStarSynFixedFailWithIf );
	CPPUNIT_TEST( testFollowsStarSynFixedFailWithWhile );
	CPPUNIT_TEST( testFollowsStarSynFixedFailWithAssign );
	CPPUNIT_TEST( testFollowsStarSynFixedFailWithCalls );

	CPPUNIT_TEST( testFollowsStarFixedSynPass );
	CPPUNIT_TEST( testFollowsStarFixedSynPassWithWhile );
	CPPUNIT_TEST( testFollowsStarFixedSynPassWithIf );
	CPPUNIT_TEST( testFollowsStarFixedSynPassWithAssign );

	CPPUNIT_TEST( testFollowsStarFixedSynFail );
	
	CPPUNIT_TEST( testFollowsStarSynSynPass );
	
	CPPUNIT_TEST( testFollowsStarSynSynSameFail );

	CPPUNIT_TEST( testFollowsStarGenericSynPass  );
	CPPUNIT_TEST( testFollowsStarGenericSynPassWithWhile );
	CPPUNIT_TEST( testFollowsStarGenericSynPassWithIf );
	CPPUNIT_TEST( testFollowsStarGenericSynPassWithAssign );

	CPPUNIT_TEST( testFollowsStarGenericSynFailWithCalls );
	
	CPPUNIT_TEST( testFollowsStarSynGenericPass );
	CPPUNIT_TEST( testFollowsStarSynGenericPassWithWhile );
	CPPUNIT_TEST( testFollowsStarSynGenericPassWithAssign ); 

	CPPUNIT_TEST( testFollowsStarGenericGenericPass );

	CPPUNIT_TEST( testFollowsStarSynSynPass );
	CPPUNIT_TEST( testFollowsStarSynSynPassWithWhile );
	CPPUNIT_TEST( testFollowsStarSynSynPassWithIf );
	CPPUNIT_TEST( testFollowsStarSynSynPassWithAssign );

	CPPUNIT_TEST( testFollowsStarSynSynFailWithCalls );
	CPPUNIT_TEST( testFollowsStarSynSynSameFail );
	CPPUNIT_TEST( testFollowsStarStmtOverflow );
	CPPUNIT_TEST( testFollowsStarUnacceptedType );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testFollowsStarFixedFixedPass();

	void testFollowsStarFixedFixedFailWrongOrder();
	void testFollowsStarFixedFixedFailNestingLevel();
	void testFollowsStarFixedFixedFailSameStmt();

	void testFollowsStarSynFixedPass();
	void testFollowsStarSynFixedPassWithWhile();
	void testFollowsStarSynFixedPassWithAssign();

	void testFollowsStarSynFixedFail();
	void testFollowsStarSynFixedFailWithIf();
	void testFollowsStarSynFixedFailWithWhile();
	void testFollowsStarSynFixedFailWithAssign();
	void testFollowsStarSynFixedFailWithCalls();

	void testFollowsStarFixedSynPass();
	void testFollowsStarFixedSynPassWithWhile();
	void testFollowsStarFixedSynPassWithIf();
	void testFollowsStarFixedSynPassWithAssign();

	void testFollowsStarFixedSynFail();

	void testFollowsStarGenericSynPass();
	void testFollowsStarGenericSynPassWithWhile();
	void testFollowsStarGenericSynPassWithIf();
	void testFollowsStarGenericSynPassWithAssign();

	void testFollowsStarGenericSynFailWithCalls();
	
	void testFollowsStarSynGenericPass();
	void testFollowsStarSynGenericPassWithWhile();
	void testFollowsStarSynGenericPassWithAssign(); 

	void testFollowsStarGenericGenericPass();

	void testFollowsStarSynSynPass();
	void testFollowsStarSynSynPassWithWhile();
	void testFollowsStarSynSynPassWithIf();
	void testFollowsStarSynSynPassWithAssign();

	void testFollowsStarSynSynFailWithCalls();
	void testFollowsStarSynSynSameFail();
	void testFollowsStarStmtOverflow();
	void testFollowsStarUnacceptedType();
};
    