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
	parser.parse("procedure test {x = 2; y=x+z; z=x+y;}");
	ProcTable* procTable = ProcTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
	StmtTable* stmtTable = StmtTable::getInstance();
	
	Statement* stmt1 = stmtTable->getStmtObj(1);
	string initUses1[] = { "x" };
	set<string> str1(initUses1, initUses1 + 1);
	CPPUNIT_ASSERT(stmt1->getModifies() == str1);
	
	Statement* stmt2 = stmtTable->getStmtObj(2);
	string initModifies2[] = { "y" };
	set<string> strModifies2(initModifies2, initModifies2 + 1);
	CPPUNIT_ASSERT(stmt2->getModifies() == strModifies2);
	string initUses2[] = { "x", "z" };
	set<string> strUses2(initUses2, initUses2 + 2);
	CPPUNIT_ASSERT(stmt2->getUses() == strUses2);

	Statement* stmt3 = stmtTable->getStmtObj(3);
	string initModifies3[] = { "z" };
	set<string> strModifies3(initModifies3, initModifies3 + 1);
	CPPUNIT_ASSERT(stmt3->getModifies() == strModifies3);
	string initUses3[] = { "x", "y" };
	set<string> strUses3(initUses3, initUses3 + 2);
	CPPUNIT_ASSERT(stmt3->getUses() == strUses3);
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