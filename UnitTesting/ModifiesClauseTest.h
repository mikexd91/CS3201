#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ModifiesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ModifiesClauseTest );
	//CPPUNIT_TEST( testFixedFixedStmtPass );
	CPPUNIT_TEST( testFixedFixedStmtFail );
	//CPPUNIT_TEST( testFixedFixedProcPass );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testFixedFixedStmtPass();
	void testFixedFixedStmtFail();
	void testFixedFixedProcPass();
	void testFixedFixedProcFail();

	//void testModifiesFixedFixed();
	//void testModifiesSynFixed();
	//void testModifiesFixedSyn();
	//void testModifiesSynSyn();
	//void testModifiesGenericFixed();
	//void testModifiesFixedGeneric();
	//void testModifiesGenericGeneric();

	/*void evaluateVarWildExprWild();
	void evaulateVarWildExpr();
	void evaluateVarFixedExprWild();
	void evaluateVarFixedExpr();
	void evaluateVarExprWild();
	void evaluateVarExpr();*/

};
    