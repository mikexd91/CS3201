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

	
};

