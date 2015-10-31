#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryEvaluatorTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );
	// Testing no clause in query, getting table values
	//CPPUNIT_TEST( testEvaluateEmptySelectBoolean );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectAssignSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectStmtSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectWhileSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectIfSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectCallSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectVarSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectProcSyn );
	//CPPUNIT_TEST( testEvalauteEmptyClauseListSelectConstSyn );

	// YET TO TEST: GETTING TABLE VALUES WHEN TABLE IS EMPTY

	// Testing Modifies, Follows clause, 1 clause in query.
	// All synonyms in select list are appearing in clause list
	//CPPUNIT_TEST( testModifiesEvaluateFixedSynProcPass );
	//CPPUNIT_TEST( testModifiesEvaluateSynFixedWhilePass );
	//CPPUNIT_TEST( testModifiesEvaluateSynSynAssgPass );
	//CPPUNIT_TEST( testFollowsEvaluateSynSynStmtPass );
	
	// Testing Modifies clause, 1 clause in query
	// Not all synonyms in select list are appearing in clause list
	//CPPUNIT_TEST( testHalfInClauseWithModifiesSynSynStmtPass );
	
	// Testing Modifies clause, 1 clause in query
	// No synonyms in select list match synonyms in clause list
	//CPPUNIT_TEST( testModifiesEvaluateSynGenericStmtPass );

	// Testing Follows clause, 1 clause in query
	// select BOOLEAN
	//CPPUNIT_TEST( testFollowsEvaluateFixedFixedPass );
	//CPPUNIT_TEST( testFollowsEvaluateFixedFixedFail );
	
	// Testing Follows, Parent, Modifies clause in query
	// select BOOLEAN
	//CPPUNIT_TEST( testUnderscoreFixedPass );
	//CPPUNIT_TEST( testUnderscoreFixedFail );
	//CPPUNIT_TEST( testFixedUnderscorePass );
	//CPPUNIT_TEST( testFixedUnderscoreFail );
	//CPPUNIT_TEST( testUnderscoreUnderscorePass );
	//CPPUNIT_TEST( testFixedFixedPass );
	//CPPUNIT_TEST( testFixedFixedFail );
	CPPUNIT_TEST( testNoResult );
	CPPUNIT_TEST( testBoolNoClause );
	CPPUNIT_TEST( testStmtNoClause );
	CPPUNIT_TEST( testCallNoClause );
	CPPUNIT_TEST( testAssNoClause );
	CPPUNIT_TEST( testIfNoClause );
	CPPUNIT_TEST( testWhileNoClause );
	CPPUNIT_TEST( testProcNoClause );
	CPPUNIT_TEST( testConstNoClause );
	CPPUNIT_TEST( testVarNoClause );
	CPPUNIT_TEST( testBoolOneClause );
	CPPUNIT_TEST( testBoolMoreClauses );
	CPPUNIT_TEST( testSynNotInClause );
	CPPUNIT_TEST( testSynInOneClause );
	CPPUNIT_TEST( testSynInMoreClauses );
	CPPUNIT_TEST( testSynInSomeClauses );
	CPPUNIT_TEST( testTupleNoClause );
	CPPUNIT_TEST( testOptEval );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	//old stuff
	// Testing category NONE_IN_CLAUSE. No clause in query, getting table values
	//void testEvaluateEmptySelectBoolean();
	//void testEvalauteEmptyClauseListSelectAssignSyn();
	//void testEvalauteEmptyClauseListSelectStmtSyn();
	//void testEvalauteEmptyClauseListSelectWhileSyn();
	//void testEvalauteEmptyClauseListSelectIfSyn();
	//void testEvalauteEmptyClauseListSelectCallSyn();
	//void testEvalauteEmptyClauseListSelectVarSyn();
	//void testEvalauteEmptyClauseListSelectProcSyn();
	//void testEvalauteEmptyClauseListSelectConstSyn();

	//void testModifiesEvaluateFixedSynProcPass();
	//void testModifiesEvaluateSynFixedWhilePass();
	//void testModifiesEvaluateSynSynAssgPass();
	//void testFollowsEvaluateSynSynStmtPass();

	//void testHalfInClauseWithModifiesSynSynStmtPass();

	//void testModifiesEvaluateSynGenericStmtPass();

	//void testFollowsEvaluateFixedFixedPass();
	//void testFollowsEvaluateFixedFixedFail();

	//void testUnderscoreFixedPass();
	//void testUnderscoreFixedFail();
	//void testFixedUnderscorePass();
	//void testFixedUnderscoreFail();
	//void testUnderscoreUnderscorePass();
	//void testFixedFixedPass();
	//void testFixedFixedFail();

	//////////////////////////////////
	//new tests
	void testNoResult();

	void testBoolNoClause();
	void testStmtNoClause();
	void testCallNoClause();
	void testAssNoClause();
	void testIfNoClause();
	void testWhileNoClause();
	void testProcNoClause();
	void testConstNoClause();
	void testVarNoClause();

	void testBoolOneClause();
	void testBoolMoreClauses();

	void testSynNotInClause();
	void testSynInOneClause();
	void testSynInMoreClauses();
	void testSynInSomeClauses();

	void testTupleNoClause();

	void testOptEval();
};

//sample source
//procedure zumba {
//		i = 1+2;	//1
//		j = 2+3+4;	//2
//		k = 3;		//3
//		w = i;		//4
//		x = w+k;	//5
//		i = i;		//6
//		while j {	//7
//			x = 4;	//8
//		}
//		if w then { //9
//			z = 2;	//10
//		} else {
//			y = 6;	//11
//		}
//	}

//vars = i j k w x y z
//const = 1 2 3 4 6
//stmts = 1-11
//assg = 1 2 3 4 5 6 8 10 11
//if = 9
//while = 7
//call = null

