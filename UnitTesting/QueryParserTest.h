#pragma once
#include <cppunit/extensions/HelperMacros.h>

class QueryParserTest : public CPPUNIT_NS::TestFixture{

	CPPUNIT_TEST_SUITE ( QueryParserTest );
	CPPUNIT_TEST( testDeclaration );
	CPPUNIT_TEST( testSelectSingle );
	CPPUNIT_TEST( testSelectBoolean );
	CPPUNIT_TEST( testSelectTuple );
	CPPUNIT_TEST( testParseClauseUses );
	CPPUNIT_TEST( testParseParentStar );
	CPPUNIT_TEST( testWith );
	CPPUNIT_TEST ( testParser );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

	void testDeclaration();
	void testSelectSingle();
	void testSelectBoolean();
	void testSelectTuple();
	void testParseClauseUses();
	void testParseParentStar();
	void testWith();
	void testParser();
};