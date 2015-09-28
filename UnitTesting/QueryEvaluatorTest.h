#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryEvaluatorTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );
	// Testing no clause in query, getting table values
	
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectAssignSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectStmtSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectWhileSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectIfSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectCallSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectVarSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectProcSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectConstSyn );

	// YET TO TEST: GETTING TABLE VALUES WHEN TABLE IS EMPTY

	// Testing Modifies clause, 1 clause in query.
	// All synonyms in select list are appearing in clause list
	CPPUNIT_TEST( testModifiesEvaluateFixedSynProcPass );
	CPPUNIT_TEST( testModifiesEvaluateSynFixedWhilePass );
	CPPUNIT_TEST( testModifiesEvaluateSynSynAssgPass );
	
	// Testing Modifies clause, 1 clause in query
	// Not all synonyms in select list are appearing in clause list
	CPPUNIT_TEST( testHalfInClauseWithModifiesSynSynStmtPass );
	
	// Testing Modifies clause, 1 clause in query
	// No synonyms in select list match synonyms in clause list
	CPPUNIT_TEST( testModifiesEvaluateSynGenericStmtPass );

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

	void testModifiesEvaluateFixedSynProcPass();
	void testModifiesEvaluateSynFixedWhilePass();
	void testModifiesEvaluateSynSynAssgPass();

	void testHalfInClauseWithModifiesSynSynStmtPass();

	void testModifiesEvaluateSynGenericStmtPass();
};

