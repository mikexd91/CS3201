#include <cppunit/config/SourcePrefix.h>
#include "SingleSynInsertTest.h"
#include "../SPA/SingleSynInsert.h"
#include "../SPA/ResultTable.h"

#include <iostream>
#include <string>

using namespace std;

void 
SingleSynInsertTest::setUp() {
}

void SingleSynInsertTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( SingleSynInsertTest );

void SingleSynInsertTest::testSynPresent() { 
	ResultTable resultTable = ResultTable();
	string tmpSyn[] = { "a", "b" };
	vector<string> synVect(tmpSyn, tmpSyn+2);
	resultTable.synList = synVect;

	string tmpRow1[] = { "1", "2" };
	Row row1(tmpRow1, tmpRow1+2);
	string tmpRow2[] = { "1", "3" };
	Row row2(tmpRow2, tmpRow2+2);
	resultTable.rows.push_back(row1);
	resultTable.rows.push_back(row2);
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn("b");
	insert.insertValue("3");
	insert.execute(resultTable);

	//Check syn
	CPPUNIT_ASSERT_EQUAL(2, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("a" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("b" == resultTable.synList.at(1));

	//Check rows
	CPPUNIT_ASSERT_EQUAL(1, (int) resultTable.rows.size());
	CPPUNIT_ASSERT_EQUAL(row2.at(0), (*resultTable.rows.begin()).at(0));
	CPPUNIT_ASSERT_EQUAL(row2.at(1), (*resultTable.rows.begin()).at(1));
	return;
}

void SingleSynInsertTest::testSynAbsent() { 
	ResultTable resultTable = ResultTable();
	string tmpSyn[] = { "a", "b" };
	vector<string> synVect(tmpSyn, tmpSyn+2);
	resultTable.synList = synVect;

	string tmpRow1[] = { "1", "2" };
	Row row1(tmpRow1, tmpRow1+2);
	string tmpRow2[] = { "1", "3" };
	Row row2(tmpRow2, tmpRow2+2);
	resultTable.rows.push_back(row1);
	resultTable.rows.push_back(row2);
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn("c");
	insert.insertValue("4");
	insert.insertValue("5");
	insert.execute(resultTable);

	//Check syns
	CPPUNIT_ASSERT_EQUAL(3, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("a" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("b" == resultTable.synList.at(1));
	CPPUNIT_ASSERT("c" == resultTable.synList.at(2));
	CPPUNIT_ASSERT_EQUAL(4, (int) resultTable.rows.size());
	auto front = resultTable.rows.begin();

	//Check rows
	//check first row
	Row result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	CPPUNIT_ASSERT(result1.at(1) == "2");
	bool has5Appeared = result1.at(2) == "5";
	bool has4Appeared = result1.at(2) == "4";
	CPPUNIT_ASSERT(has5Appeared || has4Appeared);
	advance(front, 1);

	//check second row
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	CPPUNIT_ASSERT(result1.at(1) == "2");
	if (has5Appeared) {
		CPPUNIT_ASSERT(result1.at(2) == "4");
		has5Appeared = false;
	} else {
		CPPUNIT_ASSERT(result1.at(2) == "5");
		has4Appeared = false;
	}
	advance(front, 1);

	//check third row
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	CPPUNIT_ASSERT(result1.at(1) == "3");
	has5Appeared = result1.at(2) == "5";
	has4Appeared = result1.at(2) == "4";
	CPPUNIT_ASSERT(has5Appeared || has4Appeared);
	advance(front, 1);

	//check fourth row
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	CPPUNIT_ASSERT(result1.at(1) == "3");
		if (has5Appeared) {
		CPPUNIT_ASSERT(result1.at(2) == "4");
		has5Appeared = false;
	} else {
		CPPUNIT_ASSERT(result1.at(2) == "5");
		has4Appeared = false;
	}
	return;
}