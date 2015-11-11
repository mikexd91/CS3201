#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class WithClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( WithClauseTest );
	CPPUNIT_TEST( testIntEqualsInt );
	CPPUNIT_TEST( testStringEqualsString );
	CPPUNIT_TEST( testStmtEqualsInt );
	CPPUNIT_TEST( testVNameEqualsString );
	CPPUNIT_TEST( testPNameEqualsString );
	CPPUNIT_TEST( testCallEqualsString );
	CPPUNIT_TEST( testPNameEqualsPName );
	CPPUNIT_TEST( testVNameEqualsVName );
	CPPUNIT_TEST( testSynEqualsInt );
	CPPUNIT_TEST( testConstEqualsInt );
	CPPUNIT_TEST( testConstEqualsConst );
	CPPUNIT_TEST( testConstEqualsStmt );
	CPPUNIT_TEST( testSynEqualsSyn );
	CPPUNIT_TEST( testStmtEqualsStmt );
	CPPUNIT_TEST( testSynEqualsValue );
	CPPUNIT_TEST( testSynEqualsStmt );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testIntEqualsInt();
	void testStringEqualsString();
	void testStmtEqualsInt();
	void testVNameEqualsString();
	void testPNameEqualsString();
	void testCallEqualsString();
	void testPNameEqualsPName();
	void testVNameEqualsVName();
	void testSynEqualsInt();
	void testConstEqualsInt();
	void testConstEqualsConst();
	void testConstEqualsStmt();
	void testSynEqualsSyn();
	void testStmtEqualsStmt();
	void testSynEqualsValue();
	void testSynEqualsStmt();
};

/* sample source
	procedure zumba {
		i = 1+2;	//1
		j = 2+3+4;	//2
		k = 3;		//3
		w = i;		//4
		x = w+k;	//5
		i = i;		//6
		while j {	//7
			x = 4;	//8
		}
		if w then { //9
			z = 2;	//10
		} else {
			y = 6;	//11
		}

		call proc2; //12
	}

	procedure proc2 {
		a = 2;		//13
		b = a;		//14
	}
	*/