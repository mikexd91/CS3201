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

	//Check rows
	//check first row
	front = resultTable.rows.begin();
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

	//check third row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 4);
	CPPUNIT_ASSERT(result1.at(0) == "1");
	if (has245Appeared && has267Appeared) {
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT( has345Appeared || has367Appeared);
	} else if (has245Appeared && has345Appeared) {
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";	
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has267Appeared || has367Appeared);
	} else if (has245Appeared && has367Appeared) {
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";	
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		CPPUNIT_ASSERT(has267Appeared || has345Appeared);
	}  else if (has267Appeared && has345Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has367Appeared = result1.at(1) == "3" && result1.at(2) == "6" && result1.at(3) == "7";
		CPPUNIT_ASSERT(has245Appeared || has367Appeared);
	} else if (has267Appeared && has367Appeared) {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has345Appeared = result1.at(1) == "3" && result1.at(2) == "4" && result1.at(3) == "5";
		CPPUNIT_ASSERT(has245Appeared || has345Appeared);
	} else {
		has245Appeared = result1.at(1) == "2" && result1.at(2) == "4" && result1.at(3) == "5";
		has267Appeared = result1.at(1) == "2" && result1.at(2) == "6" && result1.at(3) == "7";	
		CPPUNIT_ASSERT(has245Appeared || has267Appeared);
	}

	//check last row
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

void MultiSynInsertTest::testBothSynsPresent() { 
	ResultTable resultTable = ResultTable();
	//initial rows: <a,b,c> = <1,2,3>, <1,3,4>
	//insert<a,b> = <1,2>
	//outcome: <a,b,c> = <1,2,3>
	string tmpSyn[] = { "a", "b", "c" };
	vector<string> synVect(tmpSyn, tmpSyn+3);
	resultTable.synList = synVect;

	string tmpRow1[] = { "1", "2", "5" };
	Row row1(tmpRow1, tmpRow1+3);
	string tmpRow2[] = { "1", "3", "4" };
	Row row2(tmpRow2, tmpRow2+3);
	resultTable.rows.push_back(row1);
	resultTable.rows.push_back(row2);
	MultiSynInsert insert = MultiSynInsert();
	string syn1[] = {"a", "b"};
	insert.setSyns(vector<string>(syn1, syn1+2));
	string tmpInsertRow[] = {"1", "2"};
	insert.insertValues(vector<string>(tmpInsertRow, tmpInsertRow+2));
	insert.execute(resultTable);

	//Check syns
	CPPUNIT_ASSERT_EQUAL(3, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("a" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("b" == resultTable.synList.at(1));
	CPPUNIT_ASSERT("c" == resultTable.synList.at(2));
	//check values
	CPPUNIT_ASSERT_EQUAL(1, (int) resultTable.rows.size());
	Row result = *(resultTable.rows.begin());
	CPPUNIT_ASSERT(result.size() == 3);
	CPPUNIT_ASSERT(result.at(0) == "1");
	CPPUNIT_ASSERT(result.at(1) == "2");
	CPPUNIT_ASSERT(result.at(2) == "5");
	return;
}

void MultiSynInsertTest::testOneSynPresent() { 
	ResultTable resultTable = ResultTable();
	//initial rows: <a,b> = <1,2>, <1,3>, <4,2>
	//insert<b,c> = <2,3> <2,5>
	//outcome: <a,b,c> = <1,2,3> <1,2,5> <4,2,3>, <4,2,5>
	string tmpSyn[] = { "a", "b"};
	vector<string> synVect(tmpSyn, tmpSyn+2);
	resultTable.synList = synVect;

	string tmpRow1[] = { "1", "2"};
	Row row1(tmpRow1, tmpRow1+2);
	string tmpRow2[] = { "1", "3"};
	Row row2(tmpRow2, tmpRow2+2);
	string tmpRow3[] = { "4", "2"};
	Row row3(tmpRow3, tmpRow3+2);
	resultTable.rows.push_back(row1);
	resultTable.rows.push_back(row2);
	resultTable.rows.push_back(row3);

	MultiSynInsert insert = MultiSynInsert();
	string syn1[] = {"b", "c"};
	insert.setSyns(vector<string>(syn1, syn1+2));
	string tmpInsertRow1[] = {"2", "3"};
	insert.insertValues(vector<string>(tmpInsertRow1, tmpInsertRow1+2));
	string tmpInsertRow2[] = {"2", "5"};
	insert.insertValues(vector<string>(tmpInsertRow2, tmpInsertRow2+2));
	insert.execute(resultTable);

	//Check syns
	CPPUNIT_ASSERT_EQUAL(3, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("a" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("b" == resultTable.synList.at(1));
	CPPUNIT_ASSERT("c" == resultTable.synList.at(2));
	//check values
	CPPUNIT_ASSERT_EQUAL(4, (int) resultTable.rows.size());

	//Check rows
	//check first row
	auto front = resultTable.rows.begin();
	vector<string> result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	bool has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
	bool has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
	bool has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
	bool has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
	CPPUNIT_ASSERT(has123Appeared || has125Appeared || has423Appeared || has425Appeared);

	//check second row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	if (has123Appeared) {
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has125Appeared || has423Appeared || has425Appeared);
	} else if (has125Appeared) {
		has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has123Appeared || has423Appeared || has425Appeared);
	} else if (has423Appeared) {
		has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has123Appeared || has125Appeared || has425Appeared);
	} else {
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has125Appeared || has423Appeared || has425Appeared);
	}

	//check third row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	if (has123Appeared && has125Appeared) {
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has423Appeared || has425Appeared);
	} else if (has123Appeared && has423Appeared) {
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has125Appeared || has425Appeared);
	} else if (has123Appeared && has425Appeared) {
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		CPPUNIT_ASSERT(has125Appeared || has423Appeared);
	} else if (has125Appeared && has423Appeared) {
		has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has123Appeared || has425Appeared);
	} else if (has125Appeared && has425Appeared) {
		has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		CPPUNIT_ASSERT(has123Appeared || has425Appeared);
	} else {
		has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has123Appeared || has125Appeared);
	}

	//check last row
	advance(front, 1);
	result1 = *front;
	CPPUNIT_ASSERT(result1.size() == 3);
	if (!has123Appeared) {
		has123Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "3";
		CPPUNIT_ASSERT(has123Appeared);
	} else if (!has125Appeared) {
		has125Appeared = result1.at(0) == "1" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has125Appeared);
	} else if (!has423Appeared) {
		has423Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "3";
		CPPUNIT_ASSERT(has423Appeared);
	} else if (!has425Appeared) {
		has425Appeared = result1.at(0) == "4" && result1.at(1) == "2" && result1.at(2) == "5";
		CPPUNIT_ASSERT(has425Appeared);
	} else {
		CPPUNIT_ASSERT(false);
	}
	return;
}