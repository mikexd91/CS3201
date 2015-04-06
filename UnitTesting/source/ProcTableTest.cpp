#include <cppunit/config/SourcePrefix.h>
#include "ProcTableTest.h"
#include "../../SPA/ProcTable.h"
#include "../../SPA/Procedure.h"

// TEST VARIABLES
ProcTable* table = ProcTable::getInstance();		// test proctable instance

void ProcTableTest::setUp() {}

void ProcTableTest::tearDown() {
	table->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( ProcTableTest );

// method to test singleton instance getter
void ProcTableTest::testInstance() {
	CPPUNIT_ASSERT(dynamic_cast<ProcTable*>(table));
}

// method to test instance copies
void ProcTableTest::testInstanceCopies() {
	// Create a procedure
	Procedure* proc = new Procedure("new");

	// Add proc to original table instance
	table->addProc(proc);

	// Create new table instance
	ProcTable* testTable = ProcTable::getInstance();

	// Check that procedure instance are the same and contain the same proc
	CPPUNIT_ASSERT_EQUAL(table, testTable);
	CPPUNIT_ASSERT(table->contains("new"));
	CPPUNIT_ASSERT(testTable->contains("new"));
}

// method to test adding of proc to table
void ProcTableTest::testAddProc() {
	// Create some procedures
	Procedure* proc1 = new Procedure("first");
	Procedure* proc2 = new Procedure("second");

	// Add procedures to table
	table->addProc(proc1);
	table->addProc(proc2);

	// Check that both procedures are in the table
	CPPUNIT_ASSERT(table->contains("first"));
	CPPUNIT_ASSERT(table->contains("second"));
}

// method to test false adding of proc to table
void ProcTableTest::testFalseAddProc() {
	// Create a procedure
	Procedure* proc = new Procedure("one");

	// Add procedure to table
	table->addProc(proc);

	// Check that the procedure is in table and another non-existent one
	// is not
	CPPUNIT_ASSERT(table->contains("one"));
	CPPUNIT_ASSERT(!(table->contains("two")));
}