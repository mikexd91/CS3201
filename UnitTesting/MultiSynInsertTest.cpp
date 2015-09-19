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
	//insert <a,b> = <1,2>, <1,3>
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
	//insert <c,d> = <4,5>, <6,7>
	MultiSynInsert insert2 = MultiSynInsert();
	string syn2[] = {"c", "d"};
	insert2.setSyns(vector<string>(syn2, syn2+2));
	string row3[] = {"4", "5"};
	string row4[] = {"6", "7"};
	insert2.insertValues(vector<string>(row3, row3+2));
	insert2.insertValues(vector<string>(row4, row4+2));
	insert2.execute(resultTable);

	/**
	outcome:
	<a,b,c,d>: <1,2,4,5>, <1,2,6,7>, <1,3,4,5>, <1,3,6,7>
	**/
	//Check syns
	CPPUNIT_ASSERT_EQUAL(4, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("a" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("b" == resultTable.synList.at(1));
	CPPUNIT_ASSERT("c" == resultTable.synList.at(2));
	CPPUNIT_ASSERT("d" == resultTable.synList.at(3));
	CPPUNIT_ASSERT_EQUAL(4, (int) resultTable.rows.size());
	front = resultTable.rows.begin();

	//Check rows
	//check first row
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 4);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	bool has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
	bool has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
	bool has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
	bool has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
	CPPUNIT_ASSERT(has245Appeared || has267Appeared || has345Appeared || has367Appeared);

	//check second row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 4);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	if (has245Appeared) {
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has267Appeared || has345Appeared || has367Appeared);
	} else if (has267Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has245Appeared || has345Appeared || has367Appeared);
	} else if (has345Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has245Appeared || has267Appeared || has367Appeared);
	} else {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		CPPUNIT_ASSERT(has245Appeared || has267Appeared || has345Appeared);
	}

		//check second row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 4);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	if (has245Appeared) {
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has267Appeared || has345Appeared || has367Appeared);
	} else if (has267Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has245Appeared || has345Appeared || has367Appeared);
	} else if (has345Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has245Appeared || has267Appeared || has367Appeared);
	} else {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		CPPUNIT_ASSERT(has245Appeared || has267Appeared || has345Appeared);
	}

	advance(front, 1);

	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 4);
	CPPUNIT_ASSERT(result1.at(0) == "1");

	if (!has245Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		CPPUNIT_ASSERT(has245Appeared);
	} else if (!has267Appeared) {
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has267Appeared);
	} else if (!has345Appeared) {
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		CPPUNIT_ASSERT(has267Appeared);
	} else if (!has367Appeared) {
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has367Appeared);
	} else {
		CPPUNIT_ASSERT(false);
	}
	return;
}