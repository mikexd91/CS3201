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
	CPPUNIT_TEST( testFollowsSynFixedPassWithWhile );
	CPPUNIT_TEST( testFollowsSynFixedPassWithAssign );
	
	CPPUNIT_TEST( testFollowsSynFixedFail );
	CPPUNIT_TEST( testFollowsSynFixedFailWithIf );
	CPPUNIT_TEST( testFollowsSynFixedFailWithAssign );

	CPPUNIT_TEST( testFollowsSynFixedFailWithCalls );
	
	CPPUNIT_TEST( testFollowsFixedSynPass );
	CPPUNIT_TEST( testFollowsStarFixedSynPassWithWhile );
	CPPUNIT_TEST( testFollowsStarFixedSynPassWithIf );
	CPPUNIT_TEST( testFollowsStarFixedSynPassWithAssign );

	CPPUNIT_TEST( testFollowsFixedSynFail );
	CPPUNIT_TEST( testFollowsFixedSynFailWithWhile );
	CPPUNIT_TEST( testFollowsFixedSynFailWithIf );
	CPPUNIT_TEST( testFollowsFixedSynFailWithAssign );

	CPPUNIT_TEST( testFollowsSynSynPass );
	CPPUNIT_TEST( testFollowsSynSynPassWithWhile );
	CPPUNIT_TEST( testFollowsSynSynPassWithIf );
	CPPUNIT_TEST( testFollowsSynSynPassWithAssign );

	CPPUNIT_TEST( testFollowsSynSynSameFail );

	CPPUNIT_TEST( testFollowsGenericSynPass );
	CPPUNIT_TEST( testFollowsGenericSynPassWithWhile );
	CPPUNIT_TEST( testFollowsGenericSynPassWithIf );
	CPPUNIT_TEST( testFollowsGenericSynPassWithAssign );

	CPPUNIT_TEST( testFollowsSynGenericPass );
	CPPUNIT_TEST( testFollowsSynGenericPassWithWhile );
	CPPUNIT_TEST( testFollowsSynGenericPassWithAssign ); 

	CPPUNIT_TEST( testFollowsGenericGenericPass );

	CPPUNIT_TEST( testFollowsStmtOverflow );
	CPPUNIT_TEST( testFollowsUnacceptedType );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFollowsFixedFixedPass();
	void testFollowsFixedFixedFailWrongOrder();
	void testFollowsFixedFixedFailNestingLevel();
	void testFollowsFixedFixedFailSameStmt();

	void testFollowsSynFixedPass();
	void testFollowsSynFixedPassWithWhile();
	void testFollowsSynFixedPassWithAssign();

	void testFollowsSynFixedFail();
	void testFollowsSynFixedFailWithIf();
	void testFollowsSynFixedFailWithAssign();
	void testFollowsSynFixedFailWithCalls();

	void testFollowsFixedSynPass();
	void testFollowsStarFixedSynPassWithWhile();
	void testFollowsStarFixedSynPassWithIf();
	void testFollowsStarFixedSynPassWithAssign();

	void testFollowsFixedSynFail();
	void testFollowsFixedSynFailWithWhile();
	void testFollowsFixedSynFailWithIf();
	void testFollowsFixedSynFailWithAssign();

	void testFollowsSynSynPass();
	void testFollowsSynSynPassWithWhile();
	void testFollowsSynSynPassWithIf();
	void testFollowsSynSynPassWithAssign();

	void testFollowsSynSynSameFail();

	void testFollowsGenericSynPass();
	void testFollowsGenericSynPassWithWhile();
	void testFollowsGenericSynPassWithIf();
	void testFollowsGenericSynPassWithAssign();

	void testFollowsSynGenericPass();
	void testFollowsSynGenericPassWithWhile();
	void testFollowsSynGenericPassWithAssign(); 

	void testFollowsGenericGenericPass();

	void testFollowsStmtOverflow();
	void testFollowsUnacceptedType();
};
    