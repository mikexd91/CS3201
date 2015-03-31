#include <cppunit/config/SourcePrefix.h>
#include "StmtTableTest.h"
#include "../../SPA/StmtTable.h"
#include "../../SPA/Statement.h"
#include "../../SPA/TNode.h"

// TEST VARIABLES
StmtTable* table = StmtTable::getInstance();		// test stmttable instance

void StmtTableTest::setUp() {}

void StmtTableTest::tearDown() {
	table->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( StmtTableTest );

// method to test singleton instance getter
void StmtTableTest::testInstance() {
	CPPUNIT_ASSERT(dynamic_cast<StmtTable*>(table));
}

// method to test retrieval of assignment statements
void StmtTableTest::testGetAssgStmts() {
	// Create some assignment statements
	Statement* assg1 = new Statement(); assg1->setType(ASSIGN_STMT_);
	Statement* assg2 = new Statement(); assg2->setType(ASSIGN_STMT_);
	Statement* assg3 = new Statement(); assg3->setType(ASSIGN_STMT_);
	Statement* assg4 = new Statement(); assg4->setType(ASSIGN_STMT_);

	// Add statements into table
	table->addStmt(assg1);
	table->addStmt(assg2);
	table->addStmt(assg3);
	table->addStmt(assg4);

	// Check that assignment statement set returns statements correctly
	set<Statement*> result = table->getAssgStmts();
	CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());

	set<Statement*>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); iter++) {
		CPPUNIT_ASSERT_EQUAL(ASSIGN_STMT_, (*iter)->getType());
	}
}

// method to test retrieval of call statements
void StmtTableTest::testGetCallStmts() {
	// Create some call statements
	Statement* call1 = new Statement(); call1->setType(CALL_STMT_);
	Statement* call2 = new Statement(); call2->setType(CALL_STMT_);
	Statement* call3 = new Statement(); call3->setType(CALL_STMT_);
	Statement* call4 = new Statement(); call4->setType(CALL_STMT_);

	// Add statements into table
	table->addStmt(call1);
	table->addStmt(call2);
	table->addStmt(call3);
	table->addStmt(call4);

	// Check that call statement set returns statements correctly
	set<Statement*> result = table->getCallStmts();
	CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());

	set<Statement*>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); iter++) {
		CPPUNIT_ASSERT_EQUAL(CALL_STMT_, (*iter)->getType());
	}
}

// method to test retrieval of while statements
void StmtTableTest::testGetWhileStmts() {
	// Create some while statements
	Statement* while1 = new Statement(); while1->setType(WHILE_STMT_);
	Statement* while2 = new Statement(); while2->setType(WHILE_STMT_);
	Statement* while3 = new Statement(); while3->setType(WHILE_STMT_);
	Statement* while4 = new Statement(); while4->setType(WHILE_STMT_);

	// Add statements into table
	table->addStmt(while1);
	table->addStmt(while2);
	table->addStmt(while3);
	table->addStmt(while4);

	// Check that while statement set returns statements correctly
	set<Statement*> result = table->getWhileStmts();
	set<Statement*>::iterator iter;
	CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());

	for(iter=result.begin(); iter!=result.end(); iter++) {
		CPPUNIT_ASSERT_EQUAL(WHILE_STMT_, (*iter)->getType());
	}
}

// method to test retrieval of if statements
void StmtTableTest::testGetIfStmts() {
	// Create some if statements
	Statement* if1 = new Statement(); if1->setType(IF_STMT_);
	Statement* if2 = new Statement(); if2->setType(IF_STMT_);
	Statement* if3 = new Statement(); if3->setType(IF_STMT_);
	Statement* if4 = new Statement(); if4->setType(IF_STMT_);

	// Add statements into table
	table->addStmt(if1);
	table->addStmt(if2);
	table->addStmt(if3);
	table->addStmt(if4);

	// Check that if statement set returns statements correctly
	set<Statement*> result = table->getIfStmts();
	CPPUNIT_ASSERT_EQUAL((size_t)4, result.size());

	set<Statement*>::iterator iter;
	for(iter=result.begin(); iter!=result.end(); iter++) {
		CPPUNIT_ASSERT_EQUAL(IF_STMT_, (*iter)->getType());
	}
}

// method to test false getting of statement sets
void StmtTableTest:: testFalseGetStmts() {
	// Create an assignment statement
	Statement* assgStmt = new Statement(); assgStmt->setType(ASSIGN_STMT_);

	// Add statements into table
	table->addStmt(assgStmt);
	
	table->clearTable();

	// Check that assgStmt is not in the set of while statements
	set<Statement*> result = table->getWhileStmts();
	CPPUNIT_ASSERT_EQUAL((size_t)0, result.size());
}