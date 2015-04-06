#include <cppunit/config/SourcePrefix.h>
#include "TestOne.h"
#include "../../SPA/Parser.h"
#include "../../SPA/PDR.h"

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
	parser.parse("procedure test {x = 2; y=x+z; z=x+y+z;}");
	ProcTable* procTable = ProcTable::getInstance();
	CPPUNIT_ASSERT(procTable->contains("test"));
	StmtTable* stmtTable = StmtTable::getInstance();
	VarTable* varTable = VarTable::getInstance();
	
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

	Variable* varX = varTable->getVariable("x");
	int initXUsedBy[] = {2, 3};
	int initXModifiedBy[] = {1};
	set<int> setXUsedBy(initXUsedBy, initXUsedBy + 2);
	set<int> setXModifiedBy(initXModifiedBy, initXModifiedBy + 1);
	CPPUNIT_ASSERT(varX->getUsedByStmts() == setXUsedBy);
	CPPUNIT_ASSERT(varX->getModifiedByStmts() == setXModifiedBy);

	Variable* varY = varTable->getVariable("y");
	int initYUsedBy[] = {3};
	int initYModifiedBy[] = {2};
	set<int> setYUsedBy(initYUsedBy, initYUsedBy + 1);
	set<int> setYModifiedBy(initYModifiedBy, initYModifiedBy + 1);
	CPPUNIT_ASSERT(varY->getUsedByStmts() == setYUsedBy);
	CPPUNIT_ASSERT(varY->getModifiedByStmts() == setYModifiedBy);

	Variable* varZ = varTable->getVariable("z");
	int initZUsedBy[] = {2, 3};
	int initZModifiedBy[] = {3};
	set<int> setZUsedBy(initZUsedBy, initZUsedBy + 2);
	set<int> setZModifiedBy(initZModifiedBy, initZModifiedBy + 1);
	CPPUNIT_ASSERT(varZ->getUsedByStmts() == setZUsedBy);
	CPPUNIT_ASSERT(varZ->getModifiedByStmts() == setZModifiedBy);
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