#include <cppunit/config/SourcePrefix.h>
#include "StmtTableTest.h"
#include "../../SPA/StmtTable.h"
#include "../../SPA/Statement.h"

void StmtTableTest::setUp() {
}

void StmtTableTest::tearDown() {
}

CPPUNIT_TEST_SUITE_REGISTRATION( StmtTableTest );

// method to test singleton instance geetter
void StmtTableTest::testInstance() {
	StmtTable* instance = StmtTable::getInstance();

	//CPPUNIT_ASSERT(dynamic_cast<StmtTable*>(instance));
}

// method to test retrieval of assignment statements
void StmtTableTest::testGetAssgStmts() {

}

// method to test retrieval of call statements
void StmtTableTest::testGetCallStmts() {

}

// method to test retrieval of while statements
void StmtTableTest::testGetWhileStmts() {

}

// method to test retrieval of if statements
void StmtTableTest::testGetIfStmts() {

}