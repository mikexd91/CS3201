#pragma once

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

class FrontEndTest : public CPPUNIT_NS::TestFixture { 
	CPPUNIT_TEST_SUITE( FrontEndTest );

	//CPPUNIT_TEST( testParent );
	//CPPUNIT_TEST( testAddProc );
	//CPPUNIT_TEST( testAssign );
	//CPPUNIT_TEST( testWhileAST );
	//CPPUNIT_TEST( testNestedWhileAST );
	//CPPUNIT_TEST( testSiblingsAST );
	//CPPUNIT_TEST( testMultipleProcAST );
	//CPPUNIT_TEST( testFollows );
	//CPPUNIT_TEST( testWhileUses );
	//CPPUNIT_TEST( testWhileModifies );
	//CPPUNIT_TEST( testStmtTableAllWhile );
	//CPPUNIT_TEST( testConstTable );
	//CPPUNIT_TEST( testCallsAST );
	//CPPUNIT_TEST( testCallsPKB );
	//CPPUNIT_TEST( testProcedureUses );
	//CPPUNIT_TEST( testProcedureModifies );
	//CPPUNIT_TEST( testNestedProceduresUses );

	//CPPUNIT_TEST( testNestedProceduresModifies );
	//CPPUNIT_TEST( testIfStatement );
	//
	//CPPUNIT_TEST( testNestedIfStatement );
	//CPPUNIT_TEST( testUsingProc );						
	//CPPUNIT_TEST( testModifyingProc );
	//CPPUNIT_TEST( testUsingMultipleProc );
	//CPPUNIT_TEST( testModifyingMultipleProc );
	//CPPUNIT_TEST( testModifyingMultipleProcCall );
	//CPPUNIT_TEST( testUsingMultipleProcCall );

	//CPPUNIT_TEST( testCFGSingleAssg );
	//CPPUNIT_TEST( testCFGMultAssg );
	//CPPUNIT_TEST( testCFGMultProc );
	//CPPUNIT_TEST( testCFGIfStmt );
	//CPPUNIT_TEST( testCFGIfNested );
	//CPPUNIT_TEST( testCFGWhileStmt );
	//CPPUNIT_TEST( testCFGWhileNested );
	//CPPUNIT_TEST( testCFGMixedNested );

	//CPPUNIT_TEST( testCyclicCalls );
	CPPUNIT_TEST( testMultiProcUses );

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
	void testWhileUses();
	void testWhileModifies();
	void testParent();

	void testStmtTableAllWhile();
	void testConstTable();

	void testCallsAST();
	void testCallsPKB();

	void testProcedureUses();
	void testProcedureModifies();
	void testNestedProceduresUses();
	void testNestedProceduresModifies();

	void testIfStatement();
	void testNestedIfStatement();

	void testUsingProc();
	void testModifyingProc();
	void testUsingMultipleProc();
	void testModifyingMultipleProc();
	void testModifyingMultipleProcCall();
	void testUsingMultipleProcCall();

	void testCFGSingleAssg();
	void testCFGMultAssg();
	void testCFGMultProc();
	void testCFGIfStmt();
	void testCFGIfNested();
	void testCFGWhileStmt();
	void testCFGWhileNested();
	void testCFGMixedNested();

	void testCyclicCalls();
	void testMultiProcUses();
	void testMultiProcModifies();
};
