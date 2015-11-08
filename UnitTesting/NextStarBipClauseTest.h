/*
 * NextStarBipClauseTest.h
 *
 *  Created on: 4 Nov, 2015
 *      Author: Leon
 */

#pragma once

#include <cppunit/extensions/HelperMacros.h>

class NextStarBipClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( NextStarBipClauseTest );

	CPPUNIT_TEST( testFixFixTrue );
	CPPUNIT_TEST( testFixFixFalse );

	CPPUNIT_TEST( testGeneGeneTrue );
	
	CPPUNIT_TEST( testGeneFixTrue );
	CPPUNIT_TEST( testGeneFixFalse );
	CPPUNIT_TEST( testFixGeneTrue );
	CPPUNIT_TEST( testFixGeneFalse );

	CPPUNIT_TEST( testFixSynStmtTrue );
	CPPUNIT_TEST( testFixSynStmtFalse );
	CPPUNIT_TEST( testFixSynAssgTrue );
	CPPUNIT_TEST( testFixSynAssgFalse );
	CPPUNIT_TEST( testFixSynCallTrue );
	CPPUNIT_TEST( testFixSynCallFalse );
	CPPUNIT_TEST( testFixSynIfTrue );
	CPPUNIT_TEST( testFixSynIfFalse );
	CPPUNIT_TEST( testFixSynWhileTrue );
	CPPUNIT_TEST( testFixSynWhileFalse );

	CPPUNIT_TEST( testGeneSynStmtTrue );
	CPPUNIT_TEST( testGeneSynAssgTrue );
	CPPUNIT_TEST( testGeneSynCallTrue );
	CPPUNIT_TEST( testGeneSynIfTrue );
	CPPUNIT_TEST( testGeneSynWhileTrue );

	CPPUNIT_TEST( testSynStmtFixTrue );
	CPPUNIT_TEST( testSynStmtFixFalse );
	CPPUNIT_TEST( testSynAssgFixTrue );
	CPPUNIT_TEST( testSynAssgFixFalse );
	CPPUNIT_TEST( testSynCallFixTrue );
	CPPUNIT_TEST( testSynCallFixFalse );
	CPPUNIT_TEST( testSynIfFixTrue );
	CPPUNIT_TEST( testSynIfFixFalse );
	CPPUNIT_TEST( testSynWhileFixTrue );
	CPPUNIT_TEST( testSynWhileFixFalse );

	CPPUNIT_TEST( testSynStmtGeneTrue );
	CPPUNIT_TEST( testSynAssgGeneTrue );
	CPPUNIT_TEST( testSynCallGeneTrue );
	CPPUNIT_TEST( testSynIfGeneTrue );
	CPPUNIT_TEST( testSynWhileGeneTrue );

	CPPUNIT_TEST( testSynStmtSynStmtTrue );
	CPPUNIT_TEST( testSynStmtSynStmtSameTrue );
	CPPUNIT_TEST( testSynStmtSynAssgTrue );
	CPPUNIT_TEST( testSynStmtSynCallTrue );
	CPPUNIT_TEST( testSynStmtSynIfTrue );
	CPPUNIT_TEST( testSynStmtSynWhileTrue );

	CPPUNIT_TEST( testSynAssgSynAssgTrue );
	CPPUNIT_TEST( testSynAssgSynAssgSameTrue );
	CPPUNIT_TEST( testSynAssgSynStmtTrue );
	CPPUNIT_TEST( testSynAssgSynCallTrue );
	CPPUNIT_TEST( testSynAssgSynIfTrue );
	CPPUNIT_TEST( testSynAssgSynWhileTrue );

	CPPUNIT_TEST( testSynCallSynCallTrue );
	CPPUNIT_TEST( testSynCallSynCallSameTrue );
	CPPUNIT_TEST( testSynCallSynStmtTrue );
	CPPUNIT_TEST( testSynCallSynAssgTrue );
	CPPUNIT_TEST( testSynCallSynIfTrue );
	CPPUNIT_TEST( testSynCallSynWhileTrue );

	CPPUNIT_TEST( testSynIfSynIfTrue );
	CPPUNIT_TEST( testSynIfSynIfSameTrue );
	CPPUNIT_TEST( testSynIfSynStmtTrue );
	CPPUNIT_TEST( testSynIfSynAssgTrue );
	CPPUNIT_TEST( testSynIfSynCallTrue );
	CPPUNIT_TEST( testSynIfSynWhileTrue );

	CPPUNIT_TEST( testSynWhileSynWhileTrue );
	CPPUNIT_TEST( testSynWhileSynWhileSameTrue );
	CPPUNIT_TEST( testSynWhileSynStmtTrue );
	CPPUNIT_TEST( testSynWhileSynAssgTrue );
	CPPUNIT_TEST( testSynWhileSynCallTrue );
	CPPUNIT_TEST( testSynWhileSynIfTrue );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testFixFixTrue();
	void testFixFixFalse();

	void testGeneGeneTrue();

	void testGeneFixTrue();
	void testGeneFixFalse();
	void testFixGeneTrue();
	void testFixGeneFalse();

	void testFixSynStmtTrue();
	void testFixSynStmtFalse();
	void testFixSynAssgTrue();
	void testFixSynAssgFalse();
	void testFixSynCallTrue();
	void testFixSynCallFalse();
	void testFixSynIfTrue();
	void testFixSynIfFalse();
	void testFixSynWhileTrue();
	void testFixSynWhileFalse();

	void testGeneSynStmtTrue();
	void testGeneSynAssgTrue();
	void testGeneSynCallTrue();
	void testGeneSynIfTrue();
	void testGeneSynWhileTrue();

	void testSynStmtFixTrue();
	void testSynStmtFixFalse();
	void testSynAssgFixTrue();
	void testSynAssgFixFalse();
	void testSynCallFixTrue();
	void testSynCallFixFalse();
	void testSynIfFixTrue();
	void testSynIfFixFalse();
	void testSynWhileFixTrue();
	void testSynWhileFixFalse();

	void testSynStmtGeneTrue();
	void testSynAssgGeneTrue();
	void testSynCallGeneTrue();
	void testSynIfGeneTrue();
	void testSynWhileGeneTrue();

	// Next*(syn, syn)
	void testSynStmtSynStmtTrue();
	void testSynStmtSynStmtSameTrue();
	void testSynStmtSynAssgTrue();
	void testSynStmtSynCallTrue();
	void testSynStmtSynIfTrue();
	void testSynStmtSynWhileTrue();

	void testSynAssgSynAssgTrue();
	void testSynAssgSynAssgSameTrue();
	void testSynAssgSynStmtTrue();
	void testSynAssgSynCallTrue();
	void testSynAssgSynIfTrue();
	void testSynAssgSynWhileTrue();

	void testSynCallSynCallTrue();
	void testSynCallSynCallSameTrue();
	void testSynCallSynStmtTrue();
	void testSynCallSynAssgTrue();
	void testSynCallSynIfTrue();
	void testSynCallSynWhileTrue();

	void testSynIfSynIfTrue();
	void testSynIfSynIfSameTrue();
	void testSynIfSynStmtTrue();
	void testSynIfSynAssgTrue();
	void testSynIfSynCallTrue();
	void testSynIfSynWhileTrue();

	void testSynWhileSynWhileTrue();
	void testSynWhileSynWhileSameTrue();
	void testSynWhileSynStmtTrue();
	void testSynWhileSynAssgTrue();
	void testSynWhileSynCallTrue();
	void testSynWhileSynIfTrue(); 
};
