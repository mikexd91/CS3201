#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ModifiesClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ModifiesClauseTest );
	CPPUNIT_TEST( testModifiesFixedFixed );
	CPPUNIT_TEST( testModifiesSynFixed );
	CPPUNIT_TEST( testModifiesFixedSyn );
	CPPUNIT_TEST( testModifiesSynSyn );
	CPPUNIT_TEST( testModifiesGenericFixed );
	CPPUNIT_TEST( testModifiesFixedGeneric );
	CPPUNIT_TEST( testModifiesGenericGeneric );

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
	void testModifiesGenericFixed();
	void testModifiesFixedGeneric();
	void testModifiesGenericGeneric();

	/*void evaluateVarWildExprWild();
	void evaulateVarWildExpr();
	void evaluateVarFixedExprWild();
	void evaluateVarFixedExpr();
	void evaluateVarExprWild();
	void evaluateVarExpr();*/

};
    