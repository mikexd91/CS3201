#include <cppunit/config/SourcePrefix.h>
#include "TestOne.h"
#include "../../SPA/Parser.h"
#include "../../SPA/PDR.h"
#include "../../SPA/ProcTable.h"

// TEST VARIABLES

//ProcTable* procTable = ProcTable::getInstance();// test proctable instance
	Parser parser;
	PDR* pdr;

void TestOne::setUp() {
	pdr = PDR::getInstance();
	parser = Parser();
}

void TestOne::tearDown() {
	PDR::resetInstanceFlag();
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestOne );

// method to test adding of proc to table
void TestOne::testAddProc() {
	parser.parse("procedure test {x = 2; y=3;}");
	ProcTable* procTable = ProcTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
	StmtTable* stmtTable = StmtTable::getInstance();
	
	Statement* stmt = stmtTable->getStmtObj(1);
	
	string init[] = { "x" };
	set<string> str(init, init + 1);
	CPPUNIT_ASSERT(stmt->getModifies() == str);
}

void TestOne::testAssign() {
	
	parser.parse("procedure test {x = 2; y=x;}");
	
	StmtTable* stmtTable = StmtTable::getInstance();
	
	Statement* stmt = stmtTable->getStmtObj(2);
	
	string init[] = { "y" };
	set<string> str(init, init + 1);
	CPPUNIT_ASSERT(stmt->getModifies() == str);

}

// method to test false adding of proc to table
void TestOne::testFalseAddProc() {
	// Create a procedure
	
	CPPUNIT_ASSERT(true);
}