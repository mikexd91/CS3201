#pragma once

#include <cppunit/extensions/HelperMacros.h>

class StmtTableTest : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( StmtTableTest );
	CPPUNIT_TEST( testInstance );
	CPPUNIT_TEST( testInstanceCopies );
	CPPUNIT_TEST( testGetStmtByNum );
	CPPUNIT_TEST( testGetAssgStmts );
	CPPUNIT_TEST( testGetCallStmts );
	CPPUNIT_TEST( testGetWhileStmts );
	CPPUNIT_TEST( testGetIfStmts );
	CPPUNIT_TEST( testFalseGetStmts );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testInstance();			// method to test singleton instance getter
	void testInstanceCopies();		// method to test instance copies
	void testGetStmtByNum();		// method to test retrieval of statement by statement number
	void testGetAssgStmts();		// method to test retrieval of assignment statements
	void testGetCallStmts();		// method to test retrieval of call statements
	void testGetWhileStmts();		// method to test retrieval of while statements
	void testGetIfStmts();			// method to test retrieval of if statements
	void testFalseGetStmts();		// method to test false getting of statement sets

};