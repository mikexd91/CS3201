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
}