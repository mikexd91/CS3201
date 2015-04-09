#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ModifiesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ModifiesClauseTest );
	CPPUNIT_TEST( testModifiesFixedFixed );
	CPPUNIT_TEST( testModifiesSynFixed );
	CPPUNIT_TEST( testModifiesFixedSyn );
	CPPUNIT_TEST( testModifiesSynSyn );
	//CPPUNIT_TEST( evaulateVarWildExpr );
	//CPPUNIT_TEST( evaluateVarFixedExprWild );
	//CPPUNIT_TEST( evaluateVarFixedExpr );
	//CPPUNIT_TEST( evaluateVarExprWild );
	//CPPUNIT_TEST( evaluateVarExpr );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testModifiesFixedFixed();
	void testModifiesSynFixed();
	void testModifiesFixedSyn();
	void testModifiesSynSyn();

	/*void evaluateVarWildExprWild();
	void evaulateVarWildExpr();
	void evaluateVarFixedExprWild();
	void evaluateVarFixedExpr();
	void evaluateVarExprWild();
	void evaluateVarExpr();*/

};
    