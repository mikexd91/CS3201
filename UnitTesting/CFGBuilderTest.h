#pragma once

#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/config/SourcePrefix.h>

#include "../SPA/CFGBuilder.h"
#include "../SPA/ParsedData.h"
#include "../SPA/PDR.h"

class CFGBuilderTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( CFGBuilderTest );

	CPPUNIT_TEST( testProc );
	CPPUNIT_TEST( testWithIf );
	CPPUNIT_TEST( testIfAssg );
	CPPUNIT_TEST( testMultAssg );
	CPPUNIT_TEST( testMultProc );
	CPPUNIT_TEST( testMultAssgCall );
	CPPUNIT_TEST( testWithWhile );
	CPPUNIT_TEST( testIfNested );
	CPPUNIT_TEST( testWhileNested );
	CPPUNIT_TEST( testIfWhile );

	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testProc();
	void testWithIf();
	void testIfAssg();
	void testMultAssg();
	void testMultProc();
	void testMultAssgCall();
	void testWithWhile();
	void testIfNested();
	void testWhileNested();
	void testIfWhile();

private:
	void parse(ParsedData);
};