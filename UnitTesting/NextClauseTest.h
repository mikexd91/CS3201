#pragma once

#include <cppunit/extensions/HelperMacros.h>

class NextClauseTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( NextClauseTest );

	CPPUNIT_TEST( testFixFixPass );
	CPPUNIT_TEST( testFixFixFail );
	CPPUNIT_TEST( testGeneGenePass );
	CPPUNIT_TEST( testGeneFixPass );
	CPPUNIT_TEST( testGeneFixFail );
	CPPUNIT_TEST( testFixGenePass );
	CPPUNIT_TEST( testFixGeneFail );

	CPPUNIT_TEST( testFixSynStmtPass );
	CPPUNIT_TEST( testFixSynStmtFail );
	CPPUNIT_TEST( testFixSynIfPass );
	CPPUNIT_TEST( testFixSynIfFail );
	CPPUNIT_TEST( testFixSynWhilePass );
	CPPUNIT_TEST( testFixSynWhileFail );
	CPPUNIT_TEST( testFixSynAssgPass );
	CPPUNIT_TEST( testFixSynAssgFail );
	CPPUNIT_TEST( testFixSynCallPass );
	CPPUNIT_TEST( testFixSynCallFail );

	CPPUNIT_TEST( testGeneSynStmtPass );
	CPPUNIT_TEST( testGeneSynIfPass );
	CPPUNIT_TEST( testGeneSynWhilePass );
	CPPUNIT_TEST( testGeneSynCallPass );
	CPPUNIT_TEST( testGeneSynAssgPass );

	CPPUNIT_TEST( testSynStmtFixPass );
	CPPUNIT_TEST( testSynStmtFixFail );
	CPPUNIT_TEST( testSynIfFixPass );
	CPPUNIT_TEST( testSynIfFixFail );
	CPPUNIT_TEST( testSynWhileFixPass );
	CPPUNIT_TEST( testSynWhileFixFail );
	CPPUNIT_TEST( testSynAssgFixPass );
	CPPUNIT_TEST( testSynAssgFixFail );
	CPPUNIT_TEST( testSynCallFixPass );
	CPPUNIT_TEST( testSynCallFixFail );

	CPPUNIT_TEST( testSynStmtGenePass );
	CPPUNIT_TEST( testSynIfGenePass );
	CPPUNIT_TEST( testSynWhileGenePass );
	CPPUNIT_TEST( testSynAssgGenePass );
	CPPUNIT_TEST( testSynCallGenePass );

	CPPUNIT_TEST( testSynStmtSynStmtPass );
	CPPUNIT_TEST( testSynStmtSynStmtFail );
	CPPUNIT_TEST( testSynStmtSynIfPass );
	CPPUNIT_TEST( testSynStmtSynIfFail );
	CPPUNIT_TEST( testSynStmtSynWhilePass );
	CPPUNIT_TEST( testSynStmtSynWhileFail );
	CPPUNIT_TEST( testSynStmtSynAssgPass );
	CPPUNIT_TEST( testSynStmtSynAssgFail );
	CPPUNIT_TEST( testSynStmtSynCallPass );
	CPPUNIT_TEST( testSynStmtSynCallFail );

	CPPUNIT_TEST( testSynIfSynIfPass );
	CPPUNIT_TEST( testSynIfSynStmtPass );
	CPPUNIT_TEST( testSynIfSynWhilePass );
	CPPUNIT_TEST( testSynIfSynCallPass );
	CPPUNIT_TEST( testSynIfSynAssgPass );

	CPPUNIT_TEST( testSynWhileSynWhilePass );
	CPPUNIT_TEST( testSynWhileSynStmtPass );
	CPPUNIT_TEST( testSynWhileSynCallPass );
	CPPUNIT_TEST( testSynWhileSynAssgPass );
	CPPUNIT_TEST( testSynWhileSynIfPass );

	CPPUNIT_TEST( testSynCallSynCallPass );
	CPPUNIT_TEST( testSynCallSynWhilePass );
	CPPUNIT_TEST( testSynCallSynStmtPass );
	CPPUNIT_TEST( testSynCallSynIfPass );
	CPPUNIT_TEST( testSynCallSynAssgPass );

	CPPUNIT_TEST( testSynAssgSynAssgPass );
	CPPUNIT_TEST( testSynAssgSynStmtPass );
	CPPUNIT_TEST( testSynAssgSynCallPass );
	CPPUNIT_TEST( testSynAssgSynIfPass );
	CPPUNIT_TEST( testSynAssgSynWhilePass );

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
	
	void testFixSynStmtPass();
	void testFixSynStmtFail();
	void testFixSynIfPass();
	void testFixSynIfFail();
	void testFixSynWhilePass();
	void testFixSynWhileFail();
	void testFixSynAssgPass();
	void testFixSynAssgFail();
	void testFixSynCallPass();
	void testFixSynCallFail();

	void testGeneSynStmtPass();
	void testGeneSynIfPass();
	void testGeneSynWhilePass();
	void testGeneSynCallPass();
	void testGeneSynAssgPass();

	void testSynStmtFixPass();
	void testSynStmtFixFail();
	void testSynIfFixPass();
	void testSynIfFixFail();
	void testSynWhileFixPass();
	void testSynWhileFixFail();
	void testSynAssgFixPass();
	void testSynAssgFixFail();
	void testSynCallFixPass();
	void testSynCallFixFail();

	void testSynStmtGenePass();
	void testSynIfGenePass();
	void testSynWhileGenePass();
	void testSynAssgGenePass();
	void testSynCallGenePass();

	void testSynStmtSynStmtPass();
	void testSynStmtSynStmtFail();
	void testSynStmtSynIfPass();
	void testSynStmtSynIfFail();
	void testSynStmtSynWhilePass();
	void testSynStmtSynWhileFail();
	void testSynStmtSynAssgPass();
	void testSynStmtSynAssgFail();
	void testSynStmtSynCallPass();
	void testSynStmtSynCallFail();

	void testSynIfSynIfPass();
	void testSynIfSynStmtPass();
	void testSynIfSynWhilePass();
	void testSynIfSynCallPass();
	void testSynIfSynAssgPass();

	void testSynWhileSynWhilePass();
	void testSynWhileSynStmtPass();
	void testSynWhileSynCallPass();
	void testSynWhileSynAssgPass();
	void testSynWhileSynIfPass();

	void testSynCallSynCallPass();
	void testSynCallSynWhilePass();
	void testSynCallSynStmtPass();
	void testSynCallSynIfPass();
	void testSynCallSynAssgPass();

	void testSynAssgSynAssgPass();
	void testSynAssgSynStmtPass();
	void testSynAssgSynCallPass();
	void testSynAssgSynIfPass();
	void testSynAssgSynWhilePass();
};
