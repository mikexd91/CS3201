#include <cppunit/config/SourcePrefix.h>
#include "ParserTest.h"
#include "../../SPA/Parser.h"
#include "../../SPA/PDR.h"
#include "../../SPA/ProcTable.h"

// TEST VARIABLES

//ProcTable* procTable = ProcTable::getInstance();// test proctable instance
	Parser parser;
	PDR* pdr;

void ParserTest::setUp() {
	pdr = PDR::getInstance();
	parser = Parser();
}

void ParserTest::tearDown() {
	PDR::resetInstanceFlag();
}

CPPUNIT_TEST_SUITE_REGISTRATION( ParserTest );

// method to test adding of proc to table
void ParserTest::testAddProc() {
	parser.parse("procedure test {}");
	ProcTable* procTable = ProcTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
}

void ParserTest::testAssign() {
	
	parser.parse("procedure test {x = 2;}");
	
	StmtTable* stmtTable = StmtTable::getInstance();
	
	Statement* stmt = stmtTable->getStmtObj(1);
	
	string init[] = { "x" };
	set<string> str(init, init + 1);
	CPPUNIT_ASSERT(stmt->getModifies() == str);


}


// method to test false adding of proc to table
void ParserTest::testFalseAddProc() {
	// Create a procedure
	
	CPPUNIT_ASSERT(true);
}