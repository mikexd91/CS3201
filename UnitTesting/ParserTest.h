#pragma once

// Note 1
#include <cppunit/extensions/HelperMacros.h>

class ParserTest : public CPPUNIT_NS::TestFixture {
	CPPUNIT_TEST_SUITE( ParserTest );

	CPPUNIT_TEST( testInvalidProcKeyword );
	CPPUNIT_TEST( testInvalidProcName );
	CPPUNIT_TEST( testInvalidAssignName );
	CPPUNIT_TEST( testInvalidAssignSymbol );
		
	CPPUNIT_TEST( testMissingSemicolon );
	CPPUNIT_TEST( testExtraSemicolon );
	CPPUNIT_TEST( testInvalidWhileKeyword );
	CPPUNIT_TEST( testInvalidWhileVar );
	CPPUNIT_TEST( testMissingOpenBrace );
	//CPPUNIT_TEST( testMissingCloseBrace );
	
	
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testInvalidProcKeyword();
	void testInvalidProcName();
	void testInvalidAssignName();
	void testInvalidAssignSymbol();
	void testMissingSemicolon();
	void testExtraSemicolon();
	void testInvalidWhileKeyword();
	void testInvalidWhileVar();
	void testMissingOpenBrace();
	void testMissingCloseBrace();
};
    