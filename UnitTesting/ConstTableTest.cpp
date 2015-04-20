#include <cppunit/config/SourcePrefix.h>
#include "ConstTableTest.h"
#include "../../SPA/ConstTable.h"
#include "../../SPA/Constant.h"
#include "../../SPA/TNode.h"

// TEST VARIABLES
ConstTable* table = ConstTable::getInstance();		// test consttable instance

void ConstTableTest::setUp() {}

void ConstTableTest::tearDown() {
	table->clearTable();
}

