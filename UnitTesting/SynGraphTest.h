#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

// SynGraphTest will test the construction of the graphTable
// and component formation.
// The rearranging of clauses, sorting and all will be done in 
// QueryOptimiser
class SynGraphTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( SynGraphTest );
	CPPUNIT_TEST ( testPopulateGraphTableSelectBoolNoClause );
	//CPPUNIT_TEST ( testPopulateGraphTableSelectBoolWithFixedClauses );
	//CPPUNIT_TEST ( testPopulateGraphTableSelectBoolWithUnfixedClause );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testPopulateGraphTableSelectBoolNoClause();
};

