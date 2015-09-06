#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryEvaluatorTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );
	// Testing category NONE_IN_CLAUSE. No clause in query, getting table values
	
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectAssignSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectStmtSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectWhileSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectIfSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectCallSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectVarSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectProcSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectConstSyn );

	// Testing Modifies clause, category ALL_IN_CLAUSE, 1 clause in query
	CPPUNIT_TEST( testModifiesEvaluateFixedSynProcPass );
	CPPUNIT_TEST( testModifiesEvaluateSynFixedWhilePass );
	CPPUNIT_TEST( testModifiesEvaluateSynGenericStmtPass );
	CPPUNIT_TEST( testModifiesEvaluateSynSynAssgPass );
	
	// Testing Modifies clause, category HALF_IN_CLAUSE, 1 cluase in query
	//CPPUNIT_TEST( testHalfInClauseWithModifiesSynSynStmtPass );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// Testing category NONE_IN_CLAUSE. No clause in query, getting table values
	
	void testEvalauteEmptyClauseListSelectAssignSyn();
	void testEvalauteEmptyClauseListSelectStmtSyn();
	void testEvalauteEmptyClauseListSelectWhileSyn();
	void testEvalauteEmptyClauseListSelectIfSyn();
	void testEvalauteEmptyClauseListSelectCallSyn();
	void testEvalauteEmptyClauseListSelectVarSyn();
	void testEvalauteEmptyClauseListSelectProcSyn();
	void testEvalauteEmptyClauseListSelectConstSyn();

	// Testing Modifies clause, category ALL_IN_CLAUSE, 1 clause in query
	void testModifiesEvaluateFixedSynProcPass();
	void testModifiesEvaluateSynFixedWhilePass();
	void testModifiesEvaluateSynGenericStmtPass();
	void testModifiesEvaluateSynSynAssgPass();

	// Testing Modifies clause, category HALF_IN_CLAUSE, 1 cluase in query
	//void testHalfInClauseWithModifiesSynSynStmtPass();
};

