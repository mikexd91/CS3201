#include <cppunit/config/SourcePrefix.h>
#include "MultiSynInsertTest.h"
#include "../SPA/MultiSynInsert.h"
#include "../SPA/ResultTable.h"

#include <iostream>
#include <string>

using namespace std;

void 
MultiSynInsertTest::setUp() {
}

void MultiSynInsertTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( MultiSynInsertTest );

void MultiSynInsertTest::testSynAbsent() { 
	//test inserting into empty table
	ResultTable resultTable = ResultTable();

	MultiSynInsert insert = MultiSynInsert();
	string syn1[] = {"a", "b"};
	insert.setSyns(vector<string>(syn1, syn1+2));
	string row1[] = {"1", "2"};
	string row2[] = {"1", "3"};
	insert.insertValues(vector<string>(row1, row1+2));
	insert.insertValues(vector<string>(row2, row2+2));
	insert.execute(resultTable);

	//Check syns
	CPPUNIT_ASSERT_EQUAL(2, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("a" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("b" == resultTable.synList.at(1));
	CPPUNIT_ASSERT_EQUAL(2, (int) resultTable.rows.size());
	auto front = resultTable.rows.begin();

	//Check rows
	//check first row
	Row result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 2);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	bool has2Appeared = result1.at(1) == "2";
	bool has3Appeared = result1.at(1) == "3";
	CPPUNIT_ASSERT(has2Appeared || has3Appeared);

	//check second row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 2);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	if (has2Appeared) {
		CPPUNIT_ASSERT(result1.at(1) == "3");
	} else {
		CPPUNIT_ASSERT(result1.at(1) == "2");
	}
	return;
}