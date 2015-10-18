/*
 * NextStarClauseTest.h
 *
 *  Created on: 15 Oct, 2015
 *      Author: Leon
 */

#pragma once

#include <cppunit/extensions/HelperMacros.h>

class NextStarClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( NextStarClauseTest );

	CPPUNIT_TEST( testFixFixPass );
	CPPUNIT_TEST( testFixFixFail );

	CPPUNIT_TEST( testGeneGenePass );

	CPPUNIT_TEST( testGeneFixPass );
	CPPUNIT_TEST( testGeneFixFail );
	CPPUNIT_TEST( testFixGenePass );
	CPPUNIT_TEST( testFixGeneFail );

	CPPUNIT_TEST( testFixSynStmtPass );
	CPPUNIT_TEST( testFixSynStmtFail );
	CPPUNIT_TEST( testFixSynAssgPass );
	CPPUNIT_TEST( testFixSynAssgFail );
	CPPUNIT_TEST( testFixSynCallPass );
	CPPUNIT_TEST( testFixSynCallFail );
	CPPUNIT_TEST( testFixSynIfPass );
	CPPUNIT_TEST( testFixSynIfFail );
	CPPUNIT_TEST( testFixSynWhilePass );
	CPPUNIT_TEST( testFixSynWhileFail );

	CPPUNIT_TEST( testGeneSynStmtPass );
	CPPUNIT_TEST( testGeneSynAssgPass );
	CPPUNIT_TEST( testGeneSynCallPass );
	CPPUNIT_TEST( testGeneSynIfPass );
	CPPUNIT_TEST( testGeneSynWhilePass );

	CPPUNIT_TEST( testSynStmtFixPass );
	CPPUNIT_TEST( testSynStmtFixFail );
	CPPUNIT_TEST( testSynAssgFixPass );
	CPPUNIT_TEST( testSynAssgFixFail );
	CPPUNIT_TEST( testSynCallFixPass );
	CPPUNIT_TEST( testSynCallFixFail );
	CPPUNIT_TEST( testSynIfFixPass );
	CPPUNIT_TEST( testSynIfFixFail );
	CPPUNIT_TEST( testSynWhileFixPass );
	CPPUNIT_TEST( testSynWhileFixFail );
	
	CPPUNIT_TEST( testSynStmtGenePass );
	CPPUNIT_TEST( testSynAssgGenePass );
	CPPUNIT_TEST( testSynCallGenePass );
	CPPUNIT_TEST( testSynIfGenePass );
	CPPUNIT_TEST( testSynWhileGenePass );

	CPPUNIT_TEST( testSynStmtSynStmtPass );
	CPPUNIT_TEST( testSynStmtSynStmtSamePass );
	CPPUNIT_TEST( testSynStmtSynAssgPass );
	CPPUNIT_TEST( testSynStmtSynCallPass );
	CPPUNIT_TEST( testSynStmtSynIfPass );
	CPPUNIT_TEST( testSynStmtSynWhilePass );
	
	CPPUNIT_TEST( testSynAssgSynAssgPass );
	CPPUNIT_TEST( testSynAssgSynAssgSamePass );
	CPPUNIT_TEST( testSynAssgSynStmtPass );
	CPPUNIT_TEST( testSynAssgSynCallPass );
	CPPUNIT_TEST( testSynAssgSynIfPass );
	CPPUNIT_TEST( testSynAssgSynWhilePass );

	CPPUNIT_TEST( testSynCallSynCallPass );
	CPPUNIT_TEST( testSynCallSynCallSamePass );
	CPPUNIT_TEST( testSynCallSynStmtPass );
	CPPUNIT_TEST( testSynCallSynAssgPass );
	CPPUNIT_TEST( testSynCallSynIfPass );
	CPPUNIT_TEST( testSynCallSynWhilePass );

	CPPUNIT_TEST( testSynIfSynIfPass );
	CPPUNIT_TEST( testSynIfSynIfSamePass );
	CPPUNIT_TEST( testSynIfSynStmtPass );
	CPPUNIT_TEST( testSynIfSynAssgPass );
	CPPUNIT_TEST( testSynIfSynCallPass );
	CPPUNIT_TEST( testSynIfSynWhilePass );

	CPPUNIT_TEST( testSynWhileSynWhilePass );
	CPPUNIT_TEST( testSynWhileSynWhileSamePass );
	CPPUNIT_TEST( testSynWhileSynStmtPass );
	CPPUNIT_TEST( testSynWhileSynAssgPass );
	CPPUNIT_TEST( testSynWhileSynCallPass );
	CPPUNIT_TEST( testSynWhileSynIfPass );
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFixFixPass();
	void testFixFixFail();

	void testGeneGenePass();

	void testGeneFixPass();
	void testGeneFixFail();
	void testFixGenePass();
	void testFixGeneFail();

	// Next*(fixed, syn)
	void testFixSynStmtPass();
	void testFixSynStmtFail();
	void testFixSynAssgPass();
	void testFixSynAssgFail();
	void testFixSynCallPass();
	void testFixSynCallFail();
	void testFixSynIfPass();
	void testFixSynIfFail();
	void testFixSynWhilePass();
	void testFixSynWhileFail();

	// Next*(_, syn)
	void testGeneSynStmtPass();
	void testGeneSynAssgPass();
	void testGeneSynCallPass();
	void testGeneSynIfPass();
	void testGeneSynWhilePass();

	// Next*(syn, fixed)
	void testSynStmtFixPass();
	void testSynStmtFixFail();
	void testSynAssgFixPass();
	void testSynAssgFixFail();
	void testSynCallFixPass();
	void testSynCallFixFail();
	void testSynIfFixPass();
	void testSynIfFixFail();
	void testSynWhileFixPass();
	void testSynWhileFixFail();

	// Next*(syn, _)
	void testSynStmtGenePass();
	void testSynAssgGenePass();
	void testSynCallGenePass();
	void testSynIfGenePass();
	void testSynWhileGenePass();

	// Next*(syn, syn)
	void testSynStmtSynStmtPass();
	void testSynStmtSynStmtSamePass();
	void testSynStmtSynAssgPass();
	void testSynStmtSynCallPass();
	void testSynStmtSynIfPass();
	void testSynStmtSynWhilePass();

	void testSynAssgSynAssgPass();
	void testSynAssgSynAssgSamePass();
	void testSynAssgSynStmtPass();
	void testSynAssgSynCallPass();
	void testSynAssgSynIfPass();
	void testSynAssgSynWhilePass();

	void testSynCallSynCallPass();
	void testSynCallSynCallSamePass();
	void testSynCallSynStmtPass();
	void testSynCallSynAssgPass();
	void testSynCallSynIfPass();
	void testSynCallSynWhilePass();

	void testSynIfSynIfPass();
	void testSynIfSynIfSamePass();
	void testSynIfSynStmtPass();
	void testSynIfSynAssgPass();
	void testSynIfSynCallPass();
	void testSynIfSynWhilePass();

	void testSynWhileSynWhilePass();
	void testSynWhileSynWhileSamePass();
	void testSynWhileSynStmtPass();
	void testSynWhileSynAssgPass();
	void testSynWhileSynCallPass();
	void testSynWhileSynIfPass();
};