#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ModifiesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ModifiesClauseTest );
	CPPUNIT_TEST( testModifiesNumNum );
	CPPUNIT_TEST( testModifiesSynNum );
	CPPUNIT_TEST( testModifiesNumSyn );
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
	
	void testModifiesNumNum();
	void testModifiesSynNum();
	void testModifiesNumSyn();
	void testModifiesSynSyn();

	/*void evaluateVarWildExprWild();
	void evaulateVarWildExpr();
	void evaluateVarFixedExprWild();
	void evaluateVarFixedExpr();
	void evaluateVarExprWild();
	void evaluateVarExpr();*/

};
    