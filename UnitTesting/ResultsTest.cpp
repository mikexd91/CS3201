#include <cppunit/config/SourcePrefix.h>
#include "ResultsTest.h"
#include "../SPA/Results.h"
#include <iostream>

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ResultsTest );

void ResultsTest::testHasResults() {
	Results obj = Results();
	// results obj has no results immediately after creation
	CPPUNIT_ASSERT(obj.hasResults("a") == false);
	CPPUNIT_ASSERT(obj.hasResults("1") == false);
}

void ResultsTest::testIsClausePass() {
	Results obj = Results();
	// isClausePass is false by default
	CPPUNIT_ASSERT(obj.isClausePass() == false);

	// isClausePass will be set to true during insert
	// hence it will be tested again in insert
}

void ResultsTest::testInsertResult() {
	// test result obj as it is created

	Results obj1 = Results();
	// Inserting results for 1 synonym a
	obj1.insertResult("a", "1");
	obj1.insertResult("a", "2");
	obj1.insertResult("a", "3");
	obj1.push();

	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("a") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 3);

	obj1.~Results();

	// test results obj that has been passed around

	// Test 1 - adding of 2 separate synonyms
	// Purpose: check if results are combined correctly by the 
	// number of rows
	// Insert Synonym ah with results 1,1,2,3,4,5 (check for duplicates)
	// Insert Synonym bah with results darkChocolate,milkChocolate,whiteChocolate
	
	Results obj2 = Results();
	obj2.insertResult("ah", "1");
	obj2.insertResult("ah", "2");
	obj2.insertResult("ah", "1");
	obj2.insertResult("ah", "3");
	obj2.insertResult("ah", "4");
	obj2.insertResult("ah", "5");
	obj2.push();

	CPPUNIT_ASSERT(obj2.isClausePass() == true);
	CPPUNIT_ASSERT(obj2.hasResults("ah") == true);
	CPPUNIT_ASSERT(obj2.getResultsTableSize() == 5);
	
	// TO ADD
	// function to reset ClausePass and clean private variable states

	obj2.insertResult("bah", "darkChocolate");
	obj2.insertResult("bah", "milkChocolate");
	obj2.insertResult("bah", "whiteChocolate");
	obj2.push();

	CPPUNIT_ASSERT(obj2.isClausePass() == true);
	CPPUNIT_ASSERT(obj2.hasResults("ah") == true);
	CPPUNIT_ASSERT(obj2.hasResults("bah") == true);
	CPPUNIT_ASSERT(obj2.getResultsTableSize() == 15);

	obj2.~Results();
	
	// Test 2 - eliminating synonyms results
	// Purpose: check if results are eliminated correctly by the 
	// number of rows and synonyms remaining
	// Insert synonym uk with results england, scotland, finland, 
	// soland, sweden, arkland, borderland, wasteland
	// Insert synonym uk with results england, scotland, sweden
	// Insert nothing
	// Insert england

	Results obj3 = Results();
	obj3.insertResult("uk", "england");
	obj3.insertResult("uk", "scotland");
	obj3.insertResult("uk", "finland");
	obj3.insertResult("uk", "soland");
	obj3.insertResult("uk", "sweden");
	obj3.insertResult("uk", "arkland");
	obj3.insertResult("uk", "borderland");
	obj3.insertResult("uk", "wasteland");
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 8);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	obj3.insertResult("uk", "england");
	obj3.insertResult("uk", "scotland");
	obj3.insertResult("uk", "sweden");
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 3);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	obj3.insertResult("sheep", "");
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == false);
	CPPUNIT_ASSERT(obj3.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj3.hasResults("sheep") == false);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 3);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	obj3.insertResult("uk", "england");
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 1);
}

void ResultsTest::testInsertMultiResult() {
	// test result obj as it is created

	// Inserting results for 2 synonyms, anne and ben
	// anne has results: 1, 2
	// ben has results: a, b, c
	Results obj1 = Results();
	unordered_map<string, string>* row1 = new unordered_map<string, string>();
	(*row1)["anne"] = "1";
	(*row1)["ben"] = "a";
	obj1.insertMultiResult(row1);

	(*row1)["anne"] = "1";
	(*row1)["ben"] = "b";
	obj1.insertMultiResult(row1);

	(*row1)["anne"] = "1";
	(*row1)["ben"] = "c";
	obj1.insertMultiResult(row1);

	(*row1)["anne"] = "2";
	(*row1)["ben"] = "a";
	obj1.insertMultiResult(row1);

	(*row1)["anne"] = "2";
	(*row1)["ben"] = "b";
	obj1.insertMultiResult(row1);

	(*row1)["anne"] = "2";
	(*row1)["ben"] = "c";
	obj1.insertMultiResult(row1);
	obj1.push();

	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj1.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 6);

	obj1.~Results();

	// test empty results
	Results obj2 = Results();
	unordered_map<string, string>* row2 = new unordered_map<string, string>();
	(*row2)["anne"] = "";
	(*row2)["ben"] = "";
	obj2.insertMultiResult(row2);
	obj2.push();

	CPPUNIT_ASSERT(obj2.isClausePass() == false);
	CPPUNIT_ASSERT(obj2.hasResults("anne") == false);
	CPPUNIT_ASSERT(obj2.hasResults("ben") == false);
	CPPUNIT_ASSERT(obj2.getResultsTableSize() == 0);

	obj2.~Results();

	// test results obj that has been passed around

	// Test 1 - adding of 4 separate synonyms
	// Purpose: check if results are combined correctly by the 
	// number of rows
	// Inserting results for 4 synonyms, anne,ben,ken,dan
	// first clause:
	// anne has results: 1, 2
	// ben has results: a, b
	// next clause:
	// ken has results: 8, 9
	// dan has results: x, y
	Results obj3 = Results();
	unordered_map<string, string>* row3 = new unordered_map<string, string>();
	(*row3)["anne"] = "1";
	(*row3)["ben"] = "a";
	obj3.insertMultiResult(row3);

	(*row3)["anne"] = "1";
	(*row3)["ben"] = "b";
	obj3.insertMultiResult(row3);

	(*row3)["anne"] = "2";
	(*row3)["ben"] = "a";
	obj3.insertMultiResult(row3);

	(*row3)["anne"] = "2";
	(*row3)["ben"] = "b";
	obj3.insertMultiResult(row3);
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj3.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 4);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	(*row3)["ken"] = "8";
	(*row3)["dan"] = "x";
	obj3.insertMultiResult(row3);

	(*row3)["ken"] = "8";
	(*row3)["dan"] = "y";
	obj3.insertMultiResult(row3);

	(*row3)["ken"] = "9";
	(*row3)["dan"] = "x";
	obj3.insertMultiResult(row3);

	(*row3)["ken"] = "9";
	(*row3)["dan"] = "y";
	obj3.insertMultiResult(row3);
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj3.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj3.hasResults("ken") == true);
	CPPUNIT_ASSERT(obj3.hasResults("dan") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 16);

	obj3.~Results();

	// Test 2 - adding of 1 existing synonym
	// Purpose: check if results are eliminated correctly by the 
	// number of rows
	// Inserting results for 3 synonyms, anne,ben,ken
	// first clause:
	// anne has results: 1, 2
	// ben has results: a, b
	// next clause:
	// ben has results: b
	// ken has results: 90, 100, 70

	Results obj4 = Results();
	unordered_map<string, string>* row4 = new unordered_map<string, string>();
	(*row4)["anne"] = "1";
	(*row4)["ben"] = "a";
	obj4.insertMultiResult(row4);

	(*row4)["anne"] = "1";
	(*row4)["ben"] = "b";
	obj4.insertMultiResult(row4);

	(*row4)["anne"] = "2";
	(*row4)["ben"] = "a";
	obj4.insertMultiResult(row4);

	(*row4)["anne"] = "2";
	(*row4)["ben"] = "b";
	obj4.insertMultiResult(row4);
	obj4.push();

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 4);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	(*row4)["ben"] = "b";
	(*row4)["ken"] = "90";
	obj4.insertMultiResult(row4);

	(*row4)["ben"] = "b";
	(*row4)["ken"] = "100";
	obj4.insertMultiResult(row4);

	(*row4)["ben"] = "b";
	(*row4)["ken"] = "70";
	obj4.insertMultiResult(row4);
	obj4.push();

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ken") == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 6);

	obj4.~Results();

	// Test 3 - eliminating existing results
	// Purpose: check if results are eliminated correctly by the 
	// number of rows
	// Inserting results for 3 synonyms, anne,ben,ken
	// first clause:
	// anne has results: 1, 2
	// ben has results: a, b
	// next clause:
	// ben has results: b
	// ken has results: 90, 100, 70

	Results obj5 = Results();
	unordered_map<string, string>* row4 = new unordered_map<string, string>();
	(*row4)["anne"] = "1";
	(*row4)["ben"] = "a";
	obj4.insertMultiResult(row4);
	// STOPPED HERE.
}

void ResultsTest::testSelectSyn() {

}

void ResultsTest::testSelectMultiSyn() {

}
