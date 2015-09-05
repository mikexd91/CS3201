#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryEvaluatorTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectAssignSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectStmtSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectWhileSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectIfSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectCallSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectVarSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectProcSyn );
	CPPUNIT_TEST( testEvalauteEmptyClauseListSelectConstSyn );

	CPPUNIT_TEST( testEvaluateParentFixedFixedPass );
	CPPUNIT_TEST( testEvaluateParentFixedFixedFail );
	CPPUNIT_TEST( testEvaluateParentSynFixedPass );
	CPPUNIT_TEST( testEvaluateParentSynFixedFail );
	CPPUNIT_TEST( testEvaluateParentFixedSynPass );
	CPPUNIT_TEST( testEvaluateParentFixedSynPassWithWhile );
	CPPUNIT_TEST( testEvaluateParentFixedSynFail );
	CPPUNIT_TEST( testEvaluateParentFixedSynFailWithWhile );
	CPPUNIT_TEST( testEvaluateParentSynSynPass );
	CPPUNIT_TEST( testEvaluateParentSynSynPassWithWhile );
	CPPUNIT_TEST( testEvaluateParentSynSynPassWithIf );
	CPPUNIT_TEST( testEvaluateParentFirstUnderscorePass );
	CPPUNIT_TEST( testEvaluateParentSecondUnderscorePass );
	CPPUNIT_TEST( testEvaluateParentBothUnderscorePass );
	CPPUNIT_TEST( testEvaluateParentCallPass );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	// test getting all values from tables
	void testEvalauteEmptyClauseListSelectAssignSyn();
	void testEvalauteEmptyClauseListSelectStmtSyn();
	void testEvalauteEmptyClauseListSelectWhileSyn();
	void testEvalauteEmptyClauseListSelectIfSyn();
	void testEvalauteEmptyClauseListSelectCallSyn();
	void testEvalauteEmptyClauseListSelectVarSyn();
	void testEvalauteEmptyClauseListSelectProcSyn();
	void testEvalauteEmptyClauseListSelectConstSyn();

	// test parent clause
	void testEvaluateParentFixedFixedPass();
	void testEvaluateParentFixedFixedFail();
	void testEvaluateParentSynFixedPass();
	void testEvaluateParentSynFixedFail();
	void testEvaluateParentFixedSynPass();
	void testEvaluateParentFixedSynPassWithWhile();
	void testEvaluateParentFixedSynFail();
	void testEvaluateParentFixedSynFailWithWhile();
	void testEvaluateParentSynSynPass();
	void testEvaluateParentSynSynPassWithWhile();
	void testEvaluateParentSynSynPassWithIf();
	void testEvaluateParentFirstUnderscorePass();
	void testEvaluateParentSecondUnderscorePass();
	void testEvaluateParentBothUnderscorePass();
	void testEvaluateParentCallPass();
};

