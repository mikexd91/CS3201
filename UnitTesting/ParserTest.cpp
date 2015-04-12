#include <cppunit/config/SourcePrefix.h>
#include "ParserTest.h"

#include "../SPA/Parser.h"
#include "../SPA/InvalidCodeException.h"

#include <iostream>
#include <string>

using namespace std;


Parser parser;
void 
ParserTest::setUp() {
	parser = Parser();
}

void 
ParserTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest );

//proc keyword must start with lower case
void ParserTest::testInvalidProcKeyword() { 
	string code = "Procedure test {}";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

//Proc name must start with letter
void ParserTest::testInvalidProcName() { 
	string code = "procedure 1test {}";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testInvalidAssignName() { 
	string code = "procedure test { hi+ = 3; }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testInvalidAssignSymbol() { 
	string code = "procedure test { hi + 3; }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testMissingSemicolon() { 
	string code = "procedure test { x=2 }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testExtraSemicolon() { 
	string code = "procedure test { x= 12;; }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testInvalidWhileKeyword() { 
	string code = "procedure test { x= 12 +   2 *a; While a {   } }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testInvalidWhileVar() { 
	string code = "procedure test { x= 12 +   2 *a; while 1 {   } }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testMissingOpenBrace() { 
	string code = "procedure test { x= 12 +   2 *a; while a  }   }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
	code = "procedure test  x= 12 +   2 *a; while a  { }   }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}

void ParserTest::testMissingCloseBrace() { 
	string code = "procedure test { x= 12 +   2 *a; while a {    }";
	CPPUNIT_ASSERT_THROW(parser.parse(code), InvalidCodeException);
}