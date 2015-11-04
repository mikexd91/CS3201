/*
 * CFGbipBuilderTest.h
 *
 *  Created on: 21 Oct, 2015
 *      Author: Leon
 */

#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

#include "../SPA/CFGbipBuilder.h"
#include "../SPA/ParsedData.h"
#include "../SPA/PDR.h"

class CFGbipBuilderTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( CFGbipBuilderTest );

	CPPUNIT_TEST( testProc );
	CPPUNIT_TEST( testProc2 );
	CPPUNIT_TEST( testCall1Proc );
	CPPUNIT_TEST( testCallAssign );
	CPPUNIT_TEST( testCallIfThen );

	CPPUNIT_TEST( testCallIfElse );
	CPPUNIT_TEST( testThenElseCalls );
	CPPUNIT_TEST( testCallWhile );
	CPPUNIT_TEST( testIfWhile );
	CPPUNIT_TEST( testWhileIf );

	CPPUNIT_TEST( testCallMulti );
	CPPUNIT_TEST( testNestedCall );

	CPPUNIT_TEST_SUITE_END();

public:

	void setUp();
	void tearDown();

	void testProc();
	void testProc2();
	void testCall1Proc();
	void testCallAssign();
	void testCallIfThen();
	void testCallIfElse();
	void testThenElseCalls();
	void testCallWhile();
	void testIfWhile();
	void testWhileIf();
	void testCallMulti();
	void testNestedCall();

private:

	CFGbipBuilder* builder;
	CFGbip* cfg;
	PDR* pdr;
	StmtTable* stmtTable;
	ProcTable* procTable;
	ParsedData* endProg;

	void parse(ParsedData);
	bool contains(vector<GNode*>, GNode*);
	int getPosition(vector<GNode*>, GNode*);
};