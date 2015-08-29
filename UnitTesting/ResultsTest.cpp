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
	bool test = obj1.hasResults("a");
	CPPUNIT_ASSERT(obj1.hasResults("a") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 3);

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

	//SHOULD WE DO AN ASSERTION INSTEAD OF FAILING THE CLAUSE
	//BECAUSE A CLAUSE SHOULD NOT INSERT IN AN EMPTY STRING
	/**
	obj3.insertResult("sheep", "");
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == false);
	CPPUNIT_ASSERT(obj3.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj3.hasResults("sheep") == false);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 3);

	// TO ADD
	// function to reset ClausePass and clean private variable states
	**/
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
	unordered_map<string, string>* row1_1 = new unordered_map<string, string>();
	(*row1_1)["anne"] = "1";
	(*row1_1)["ben"] = "a";
	obj1.insertMultiResult(row1_1);

	unordered_map<string, string>* row1_2 = new unordered_map<string, string>();
	(*row1_2)["anne"] = "1";
	(*row1_2)["ben"] = "b";
	obj1.insertMultiResult(row1_2);

	unordered_map<string, string>* row1_3 = new unordered_map<string, string>();
	(*row1_3)["anne"] = "1";
	(*row1_3)["ben"] = "c";
	obj1.insertMultiResult(row1_3);

	unordered_map<string, string>* row1_4 = new unordered_map<string, string>();
	(*row1_4)["anne"] = "2";
	(*row1_4)["ben"] = "a";
	obj1.insertMultiResult(row1_4);

	unordered_map<string, string>* row1_5 = new unordered_map<string, string>();
	(*row1_5)["anne"] = "2";
	(*row1_5)["ben"] = "b";
	obj1.insertMultiResult(row1_5);

	unordered_map<string, string>* row1_6 = new unordered_map<string, string>();
	(*row1_6)["anne"] = "2";
	(*row1_6)["ben"] = "c";
	obj1.insertMultiResult(row1_6);
	obj1.push();

	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj1.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 6);

	//comment out empty results section
	/**
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
	**/

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
	unordered_map<string, string>* row3_1 = new unordered_map<string, string>();
	(*row3_1)["anne"] = "1";
	(*row3_1)["ben"] = "a";
	obj3.insertMultiResult(row3_1);

	unordered_map<string, string>* row3_2 = new unordered_map<string, string>();
	(*row3_2)["anne"] = "1";
	(*row3_2)["ben"] = "b";
	obj3.insertMultiResult(row3_2);

	unordered_map<string, string>* row3_3 = new unordered_map<string, string>();
	(*row3_3)["anne"] = "2";
	(*row3_3)["ben"] = "a";
	obj3.insertMultiResult(row3_3);

	unordered_map<string, string>* row3_4 = new unordered_map<string, string>();
	(*row3_4)["anne"] = "2";
	(*row3_4)["ben"] = "b";
	obj3.insertMultiResult(row3_4);
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj3.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 4);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	unordered_map<string, string>* row3_5 = new unordered_map<string, string>();
	(*row3_5)["ken"] = "8";
	(*row3_5)["dan"] = "x";
	obj3.insertMultiResult(row3_5);

	unordered_map<string, string>* row3_6 = new unordered_map<string, string>();
	(*row3_6)["ken"] = "8";
	(*row3_6)["dan"] = "y";
	obj3.insertMultiResult(row3_6);

	unordered_map<string, string>* row3_7 = new unordered_map<string, string>();
	(*row3_7)["ken"] = "9";
	(*row3_7)["dan"] = "x";
	obj3.insertMultiResult(row3_7);

	unordered_map<string, string>* row3_8 = new unordered_map<string, string>();
	(*row3_8)["ken"] = "9";
	(*row3_8)["dan"] = "y";
	obj3.insertMultiResult(row3_8);
	obj3.push();

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj3.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj3.hasResults("ken") == true);
	CPPUNIT_ASSERT(obj3.hasResults("dan") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 16);

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
	unordered_map<string, string>* row4_1 = new unordered_map<string, string>();
	(*row4_1)["anne"] = "1";
	(*row4_1)["ben"] = "a";
	obj4.insertMultiResult(row4_1);

	unordered_map<string, string>* row4_2 = new unordered_map<string, string>();
	(*row4_2)["anne"] = "1";
	(*row4_2)["ben"] = "b";
	obj4.insertMultiResult(row4_2);

	unordered_map<string, string>* row4_3 = new unordered_map<string, string>();
	(*row4_3)["anne"] = "2";
	(*row4_3)["ben"] = "a";
	obj4.insertMultiResult(row4_3);

	unordered_map<string, string>* row4_4 = new unordered_map<string, string>();
	(*row4_4)["anne"] = "2";
	(*row4_4)["ben"] = "b";
	obj4.insertMultiResult(row4_4);
	obj4.push();

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 4);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	//COMBINE WITH RESTRICTIONS NOT TEST YET
	/**
	unordered_map<string, string>* row4_5 = new unordered_map<string, string>();
	(*row4_5)["ben"] = "b";
	(*row4_5)["ken"] = "90";
	obj4.insertMultiResult(row4_5);

	unordered_map<string, string>* row4_6 = new unordered_map<string, string>();
	(*row4_6)["ben"] = "b";
	(*row4_6)["ken"] = "100";
	obj4.insertMultiResult(row4_6);

	unordered_map<string, string>* row4_7 = new unordered_map<string, string>();
	(*row4_7)["ben"] = "b";
	(*row4_7)["ken"] = "70";
	obj4.insertMultiResult(row4_7);
	obj4.push();

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	//obj4.hasResults("ben");
	//bool test = obj4.hasResults("ben");
	//CPPUNIT_ASSERT(obj4.hasResults("anne") == true);
					/**
	CPPUNIT_ASSERT(obj4.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ken") == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 6);

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
	unordered_map<string, string>* row5 = new unordered_map<string, string>();
	(*row5)["anne"] = "1";
	(*row5)["ben"] = "a";
	obj5.insertMultiResult(row5);
	// STOPPED HERE.
	**/
}

void ResultsTest::testSelectSyn() {

}

void ResultsTest::testSelectMultiSyn() {

}
