#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>


//The formal testing will be done in QueryOptimiserTest

class SynGraphTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( SynGraphTest );
	// testing private methods
	
	CPPUNIT_TEST ( testMultiClauseContainsDesiredNeighbourClause );
	CPPUNIT_TEST ( testMultiClauseOnlyUndesiredNeighbourClause );
	CPPUNIT_TEST ( testMultiClauseOnlyUndesiredSingleSynClause );

	CPPUNIT_TEST ( testSynSelectNotUsedByMultiArgUnfixedClause );

	CPPUNIT_TEST ( testSynSelectUsedByMultiArgUnfixedClauseSecondArg );
	CPPUNIT_TEST ( testSynSelectUsedByMultiArgUnfixedClauseFirstArg );
	CPPUNIT_TEST ( testSynSelectUsedBySingleArgUnfixedClause );

	CPPUNIT_TEST ( testPopulateGraphTableManySynSelect1UnFixedClauseCircle );
	CPPUNIT_TEST ( testPopulateGraphTableManySynSelectManyUnFixedClauseCircle );
	CPPUNIT_TEST ( testPopulateGraphTableManySynSelectManyUnFixedClauseTree );

	CPPUNIT_TEST ( testPopulateGraphTableManySynSelect1FixedClause );
	CPPUNIT_TEST ( testPopulateGraphTableManySynSelectManyFixedClause );
	CPPUNIT_TEST ( testPopulateGraphTable1SynSelect1FixedClause );
	CPPUNIT_TEST ( testPopulateGraphTable1SynSelectManyFixedClause );
	CPPUNIT_TEST ( testPopulateGraphTableBoolSelect1FixedClause );
	CPPUNIT_TEST ( testPopulateGraphTableBoolSelectManyFixedClause );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testMultiClauseContainsDesiredNeighbourClause();
	void testMultiClauseOnlyUndesiredNeighbourClause();
	void testMultiClauseOnlyUndesiredSingleSynClause();

	void testSynSelectNotUsedByMultiArgUnfixedClause();

	void testSynSelectUsedByMultiArgUnfixedClauseSecondArg();
	void testSynSelectUsedByMultiArgUnfixedClauseFirstArg();
	void testSynSelectUsedBySingleArgUnfixedClause();

	void testPopulateGraphTableManySynSelect1UnFixedClauseCircle();
	void testPopulateGraphTableManySynSelectManyUnFixedClauseCircle();
	void testPopulateGraphTableManySynSelectManyUnFixedClauseTree();

	void testPopulateGraphTableManySynSelect1FixedClause();
	void testPopulateGraphTableManySynSelectManyFixedClause();
	void testPopulateGraphTable1SynSelect1FixedClause();
	void testPopulateGraphTable1SynSelectManyFixedClause();
	void testPopulateGraphTableBoolSelect1FixedClause();
	void testPopulateGraphTableBoolSelectManyFixedClause();

};

