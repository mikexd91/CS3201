#pragma once

#include <cppunit/extensions/HelperMacros.h>

class TestOne : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( TestOne );
	CPPUNIT_TEST( testAddProc );
	CPPUNIT_TEST( testAssign );
	CPPUNIT_TEST( testFalseAddProc );
	CPPUNIT_TEST( testWhileAST );
	CPPUNIT_TEST( testNestedWhileAST );
	CPPUNIT_TEST( testSiblingsAST );
	CPPUNIT_TEST( testMultipleProcAST );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testAddProc();			
	void testAssign();				
	void testFalseAddProc();

	void testWhileAST();
	void testNestedWhileAST();
	void testSiblingsAST();
	void testMultipleProcAST();

	void testFollows();
};