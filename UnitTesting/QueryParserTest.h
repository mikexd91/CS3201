#pragma once
#include <cppunit/extensions/HelperMacros.h>

class QueryParserTest : public CPPUNIT_NS::TestFixture{

	CPPUNIT_TEST_SUITE ( QueryParserTest );
	CPPUNIT_TEST( testTokeniser );
	CPPUNIT_TEST( testQueueBuilder );
	CPPUNIT_TEST( testDeclaration );
	CPPUNIT_TEST( testSelect );
	//CPPUNIT_TEST( testClause );
	//CPPUNIT_TEST( testPatternAssign );
	//CPPUNIT_TEST( testPatternIf );
	//CPPUNIT_TEST( testPatternWhile );
	//CPPUNIT_TEST ( testParser );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void);
	void tearDown(void);

	void testTokeniser();
	void testQueueBuilder();
	void testDeclaration();
	void testSelect();
	/*void testClause();
	void testPatternAssign();
	void testPatternIf();
	void testPatternWhile();
	void testParser();*/
};