#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

using namespace std;

class QueryEvaluatorTest: public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( QueryEvaluatorTest );

	CPPUNIT_TEST( testNoResult );
	CPPUNIT_TEST( testBoolNoResult );
	CPPUNIT_TEST( testBoolNoClause );
	CPPUNIT_TEST( testStmtNoClause );
	CPPUNIT_TEST( testCallNoClause );
	CPPUNIT_TEST( testAssNoClause );
	CPPUNIT_TEST( testIfNoClause );
	CPPUNIT_TEST( testWhileNoClause );
	CPPUNIT_TEST( testProcNoClause );
	CPPUNIT_TEST( testConstNoClause );
	CPPUNIT_TEST( testVarNoClause );
	CPPUNIT_TEST( testBoolOneClause );
	CPPUNIT_TEST( testBoolMoreClauses );
	CPPUNIT_TEST( testSynNotInClause );
	CPPUNIT_TEST( testSynInOneClause );
	CPPUNIT_TEST( testSynInMoreClauses );
	CPPUNIT_TEST( testSynInSomeClauses );
	CPPUNIT_TEST( testTupleNoClause );
	CPPUNIT_TEST( testOptEval );
	CPPUNIT_TEST( testResultMergerSingle );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testNoResult();
	void testBoolNoResult();

	void testBoolNoClause();
	void testStmtNoClause();
	void testCallNoClause();
	void testAssNoClause();
	void testIfNoClause();
	void testWhileNoClause();
	void testProcNoClause();
	void testConstNoClause();
	void testVarNoClause();

	void testBoolOneClause();
	void testBoolMoreClauses();

	void testSynNotInClause();
	void testSynInOneClause();
	void testSynInMoreClauses();
	void testSynInSomeClauses();

	void testTupleNoClause();

	void testOptEval();
	void testResultMergerSingle();
};

//sample source
//procedure zumba {
//		i = 1+2;	//1
//		j = 2+3+4;	//2
//		k = 3;		//3
//		w = i;		//4
//		x = w+k;	//5
//		i = i;		//6
//		while j {	//7
//			x = 4;	//8
//		}
//		if w then { //9
//			z = 2;	//10
//		} else {
//			y = 6;	//11
//		}
//	}

//vars = i j k w x y z
//const = 1 2 3 4 6
//stmts = 1-11
//assg = 1 2 3 4 5 6 8 10 11
//if = 9
//while = 7
//call = null

