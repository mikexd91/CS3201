#include <cppunit/config/SourcePrefix.h>
#include "ResultTest.h"
#include "../SPA/SingleSynInsert.h"
#include "../SPA/Result.h"

#include <iostream>
#include <string>

using namespace std;

void 
ResultTest::setUp() {
}

void ResultTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ResultTest );

void ResultTest::testSingleInsertSynPresent() { 

	//table has <a,b> = <1,2> <1,3>, insert b = 3
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

	Result result = Result();
	result.setResultTable(resultTable);
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn("b");
	insert.insertValue("3");
	result.push(insert);


	resultTable = result.getResultTable();
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

void ResultTest::testSingleInsertSynAbsent() { 

	//result table is empty, insert b = 3, 4 
	Result result = Result();
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn("b");
	insert.insertValue("3");
	insert.insertValue("4");
	result.push(insert);

	ResultTable resultTable = result.getResultTable();
	//Check syn
	CPPUNIT_ASSERT_EQUAL(1, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("b" == resultTable.synList.at(0));
	//Check number of rows
	CPPUNIT_ASSERT_EQUAL(2, (int) resultTable.rows.size());
	//check first row
	auto rowIter = resultTable.rows.begin();
	CPPUNIT_ASSERT_EQUAL(1, (int) (*rowIter).size());
	bool isThreePresent = (*rowIter).at(0) == "3";
	bool isFourPresent = (*rowIter).at(0) == "4";
	CPPUNIT_ASSERT(isThreePresent || isFourPresent);
	//check second row
	advance(rowIter, 1);
	CPPUNIT_ASSERT_EQUAL(1, (int) (*rowIter).size());
	if (isThreePresent) {
		CPPUNIT_ASSERT((*rowIter).at(0) == "4");
	} else {
		CPPUNIT_ASSERT((*rowIter).at(0) == "3");
	}

	//Result table has b = 3, 4, insert a = 1, 2
	SingleSynInsert insert2 = SingleSynInsert();
	insert2.setSyn("a");
	insert2.insertValue("1");
	insert2.insertValue("2");
	result.push(insert2);

	resultTable = result.getResultTable();
	//Check syn
	CPPUNIT_ASSERT_EQUAL(2, (int) resultTable.synList.size());
	CPPUNIT_ASSERT("b" == resultTable.synList.at(0));
	CPPUNIT_ASSERT("a" == resultTable.synList.at(1));
	//Check number of rows
	CPPUNIT_ASSERT_EQUAL(4, (int) resultTable.rows.size());
	
	//check first row
	rowIter = resultTable.rows.begin();
	CPPUNIT_ASSERT_EQUAL(2, (int) (*rowIter).size());
	bool isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
	bool isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
	bool isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
	bool isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
	CPPUNIT_ASSERT(isThreeAndOnePresent || isThreeAndTwoPresent || isFourAndOnePresent || isFourAndTwoPresent);

	//check second row
	advance(rowIter, 1);
	CPPUNIT_ASSERT_EQUAL(2, (int) (*rowIter).size());
	if (isThreeAndOnePresent) {
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndTwoPresent || isFourAndOnePresent || isFourAndTwoPresent);
	} else if (isThreeAndTwoPresent) {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndOnePresent || isFourAndOnePresent || isFourAndTwoPresent);
	} else if (isFourAndOnePresent) {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndOnePresent || isThreeAndTwoPresent || isFourAndTwoPresent);
	} else {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		CPPUNIT_ASSERT(isThreeAndOnePresent || isThreeAndTwoPresent || isFourAndOnePresent);
	}

	//check third row
	advance(rowIter, 1);
	CPPUNIT_ASSERT_EQUAL(2, (int) (*rowIter).size());
	if (isThreeAndOnePresent && isThreeAndTwoPresent) {
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isFourAndOnePresent || isFourAndTwoPresent);
	} else if (isThreeAndOnePresent && isFourAndOnePresent) {
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndTwoPresent || isFourAndTwoPresent);
	} else if (isThreeAndOnePresent && isFourAndTwoPresent) {
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		CPPUNIT_ASSERT(isThreeAndTwoPresent || isFourAndOnePresent);
	} else if (isThreeAndTwoPresent && isFourAndOnePresent) {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndOnePresent || isFourAndTwoPresent);
	} else if (isThreeAndTwoPresent && isFourAndTwoPresent) {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		CPPUNIT_ASSERT(isThreeAndOnePresent || isFourAndOnePresent);
	} else {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndOnePresent || isThreeAndTwoPresent);
	}
	//check last row
	advance(rowIter, 1);
	CPPUNIT_ASSERT_EQUAL(2, (int) (*rowIter).size());
	if (!isFourAndOnePresent) {
		isFourAndOnePresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "1";
		CPPUNIT_ASSERT(isFourAndOnePresent);
	} else if (!isFourAndTwoPresent) {
		isFourAndTwoPresent = (*rowIter).at(0) == "4" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isFourAndTwoPresent);
	} else if (!isThreeAndTwoPresent) {
		isThreeAndTwoPresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "2";
		CPPUNIT_ASSERT(isThreeAndTwoPresent);
	} else if (!isThreeAndOnePresent) {
		isThreeAndOnePresent = (*rowIter).at(0) == "3" && (*rowIter).at(1) == "1";
		CPPUNIT_ASSERT(isThreeAndOnePresent);
	} else {
		CPPUNIT_ASSERT(false);
	}
	return;
}