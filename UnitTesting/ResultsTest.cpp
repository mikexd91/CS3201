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

	bool test = obj4.hasResults("anne");
	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("anne") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ben") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ken") == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 6);

	// Test 3 - eliminating existing results
	// Purpose: check if results are eliminated correctly by the 
	// number of rows
	// Inserting results for 2 synonyms, anne,ben
	// first clause:
	// (jane, jon): (1, a), (1, b), (1, c), (2, a), (2, b), (2, c) 
	// second clause:
	// (jane, jon): (1, a), (2, b)

	Results obj5 = Results();
	unordered_map<string, string>* row5_1 = new unordered_map<string, string>();
	(*row5_1)["jane"] = "1";
	(*row5_1)["jon"] = "a";
	obj5.insertMultiResult(row5_1);
	
	unordered_map<string, string>* row5_2 = new unordered_map<string, string>();
	(*row5_2)["jane"] = "1";
	(*row5_2)["jon"] = "b";
	obj5.insertMultiResult(row5_2);

	unordered_map<string, string>* row5_3 = new unordered_map<string, string>();
	(*row5_3)["jane"] = "1";
	(*row5_3)["jon"] = "c";
	obj5.insertMultiResult(row5_3);

	unordered_map<string, string>* row5_4 = new unordered_map<string, string>();
	(*row5_4)["jane"] = "2";
	(*row5_4)["jon"] = "a";
	obj5.insertMultiResult(row5_4);

	unordered_map<string, string>* row5_6 = new unordered_map<string, string>();
	(*row5_6)["jane"] = "2";
	(*row5_6)["jon"] = "b";
	obj5.insertMultiResult(row5_6);

	unordered_map<string, string>* row5_7 = new unordered_map<string, string>();
	(*row5_7)["jane"] = "2";
	(*row5_7)["jon"] = "c";
	obj5.insertMultiResult(row5_7);
	obj5.push();

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("jane") == true);
	CPPUNIT_ASSERT(obj5.hasResults("jon") == true);
	CPPUNIT_ASSERT(obj5.getResultsTableSize() == 6);

	// TO ADD
	// function to reset ClausePass and clean private variable states
	unordered_map<string, string>* row5_8 = new unordered_map<string, string>();
	(*row5_8)["jane"] = "1";
	(*row5_8)["jon"] = "a";
	obj5.insertMultiResult(row5_8);

	unordered_map<string, string>* row5_9 = new unordered_map<string, string>();
	(*row5_9)["jane"] = "2";
	(*row5_9)["jon"] = "b";
	obj5.insertMultiResult(row5_9);
	obj5.push();

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("jane") == true);
	CPPUNIT_ASSERT(obj5.hasResults("jon") == true);
	CPPUNIT_ASSERT(obj5.getResultsTableSize() == 2);
}

void ResultsTest::testSelectSyn() {
	// test result obj as it is created

	// Inserting paired results for 2 synonyms, java and jedi
	// (java,jedi): (GG, rank1), (AoA, rank4), (sistar, rank2), (2ne1, rank3)
	// getting java
	// getting jedi
	Results obj1 = Results();
	unordered_map<string, string>* row1_1 = new unordered_map<string, string>();
	(*row1_1)["java"] = "GG";
	(*row1_1)["jedi"] = "rank1";
	obj1.insertMultiResult(row1_1);

	unordered_map<string, string>* row1_2 = new unordered_map<string, string>();
	(*row1_2)["java"] = "AoA";
	(*row1_2)["jedi"] = "rank4";
	obj1.insertMultiResult(row1_2);

	unordered_map<string, string>* row1_3 = new unordered_map<string, string>();
	(*row1_3)["java"] = "sistar";
	(*row1_3)["jedi"] = "rank2";
	obj1.insertMultiResult(row1_3);

	unordered_map<string, string>* row1_4 = new unordered_map<string, string>();
	(*row1_4)["java"] = "2ne1";
	(*row1_4)["jedi"] = "rank3";
	obj1.insertMultiResult(row1_4);
	obj1.push();

	unordered_set<string> results1 = obj1.selectSyn("java");
	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("java") == true);
	CPPUNIT_ASSERT(obj1.hasResults("jedi") == true);
	CPPUNIT_ASSERT(results1.size() == 4);

	// test results obj that has been passed around

	// Test 1 - adding of 2 separate synonyms
	// Purpose: check if results obtained contains the
	// right elements and do not contain duplicates
	// Inserting results for 2 synonyms, juicyCulture 
	// and chanel
	// first clause:
	// juicyCulture: ah, bah, wah, duh, lah, huh
	// second clause:
	// chanel: 4, 4, 2, 5

	Results obj3 = Results();
	obj3.insertResult("juicyCulture", "ah");
	obj3.insertResult("juicyCulture", "bah");
	obj3.insertResult("juicyCulture", "wah");
	obj3.insertResult("juicyCulture", "duh");
	obj3.insertResult("juicyCulture", "lah");
	obj3.insertResult("juicyCulture", "huh");
	obj3.push();

	// TO ADD
	// function to reset ClausePass and clean private variable states

	obj3.insertResult("chanel", "4");
	obj3.insertResult("chanel", "4");
	obj3.insertResult("chanel", "2");
	obj3.insertResult("chanel", "5");
	obj3.push();

	unordered_set<string> results3a = obj3.selectSyn("juicyCulture");
	unordered_set<string> results3b = obj3.selectSyn("chanel");

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("juicyCulture") == true);
	CPPUNIT_ASSERT(obj3.hasResults("chanel") == true);
	CPPUNIT_ASSERT(results3a.size() == 6);
	CPPUNIT_ASSERT(results3b.size() == 3);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 18);

	// Test 2 - adding of 2 paired results synonyms
	// Purpose: check if results obtained contains the
	// right elements and do not contain duplicates
	// Inserting results for 4 synonyms, pair juicyCulture 
	// and chanel, pair girlsGeneration and juicyCulture
	// first clause:
	// (juicyCulture, chanel): (9, x), (9, y), (10, a), (10, b),
	// (10, x), (10, y)
	// second clause:
	// (chanel, girlsGeneration): (x, mrmr), (x, rundevilrun)

	Results obj4 = Results();
	unordered_map<string, string>* row4_1 = new unordered_map<string, string>();
	(*row4_1)["juicyCulture"] = "9";
	(*row4_1)["chanel"] = "x";
	obj4.insertMultiResult(row4_1);

	unordered_map<string, string>* row4_2 = new unordered_map<string, string>();
	(*row4_2)["juicyCulture"] = "9";
	(*row4_2)["chanel"] = "y";
	obj4.insertMultiResult(row4_2);

	unordered_map<string, string>* row4_3 = new unordered_map<string, string>();
	(*row4_3)["juicyCulture"] = "10";
	(*row4_3)["chanel"] = "a";
	obj4.insertMultiResult(row4_3);

	unordered_map<string, string>* row4_4 = new unordered_map<string, string>();
	(*row4_4)["juicyCulture"] = "10";
	(*row4_4)["chanel"] = "b";
	obj4.insertMultiResult(row4_4);

	unordered_map<string, string>* row4_5 = new unordered_map<string, string>();
	(*row4_5)["juicyCulture"] = "10";
	(*row4_5)["chanel"] = "x";
	obj4.insertMultiResult(row4_5);

	unordered_map<string, string>* row4_6 = new unordered_map<string, string>();
	(*row4_6)["juicyCulture"] = "10";
	(*row4_6)["chanel"] = "y";
	obj4.insertMultiResult(row4_6);
	obj4.push();

	// TO ADD
	// function to reset ClausePass and clean private variable states
	unordered_map<string, string>* row4_7 = new unordered_map<string, string>();
	(*row4_7)["chanel"] = "x";
	(*row4_7)["girlsGeneration"] = "mrmr";
	obj4.insertMultiResult(row4_7);

	unordered_map<string, string>* row4_8 = new unordered_map<string, string>();
	(*row4_8)["chanel"] = "x";
	(*row4_8)["girlsGeneration"] = "rundevilrun";
	obj4.insertMultiResult(row4_8);
	obj4.push();

	unordered_set<string> results4a = obj4.selectSyn("juicyCulture");
	unordered_set<string> results4b = obj4.selectSyn("chanel");
	unordered_set<string> results4c = obj4.selectSyn("girlsGeneration");

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("juicyCulture") == true);
	CPPUNIT_ASSERT(obj4.hasResults("chanel") == true);
	CPPUNIT_ASSERT(obj4.hasResults("girlsGeneration") == true);
	CPPUNIT_ASSERT(results4a.size() == 2);
	CPPUNIT_ASSERT(results4b.size() == 1);
	CPPUNIT_ASSERT(results4c.size() == 2);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 4);
}

void ResultsTest::testSelectMultiSyn() {
	// test result obj as it is created

	// Inserting paired results for 2 synonym, animals and money
	// (animals, money): (tiger, 1), (rabbit, 2), (mouse, 5), (horse, 10),
	// (mouse, 1), (tiger, 2), (rabbit, 5)
	
	Results obj1 = Results();
	unordered_map<string, string>* row1_1 = new unordered_map<string, string>();
	(*row1_1)["animals"] = "tiger";
	(*row1_1)["money"] = "1";
	obj1.insertMultiResult(row1_1);

	unordered_map<string, string>* row1_2 = new unordered_map<string, string>();
	(*row1_2)["animals"] = "rabbit";
	(*row1_2)["money"] = "2";
	obj1.insertMultiResult(row1_2);

	unordered_map<string, string>* row1_3 = new unordered_map<string, string>();
	(*row1_3)["animals"] = "mouse";
	(*row1_3)["money"] = "5";
	obj1.insertMultiResult(row1_3);

	unordered_map<string, string>* row1_4 = new unordered_map<string, string>();
	(*row1_4)["animals"] = "horse";
	(*row1_4)["money"] = "10";
	obj1.insertMultiResult(row1_4);

	unordered_map<string, string>* row1_5 = new unordered_map<string, string>();
	(*row1_5)["animals"] = "mouse";
	(*row1_5)["money"] = "1";
	obj1.insertMultiResult(row1_5);

	unordered_map<string, string>* row1_6 = new unordered_map<string, string>();
	(*row1_6)["animals"] = "tiger";
	(*row1_6)["money"] = "2";
	obj1.insertMultiResult(row1_6);

	unordered_map<string, string>* row1_7 = new unordered_map<string, string>();
	(*row1_7)["animals"] = "rabbit";
	(*row1_7)["money"] = "5";
	obj1.insertMultiResult(row1_7);
	obj1.push();

	unordered_set<string> synList1 = unordered_set<string>();
	synList1.insert("animals");
	synList1.insert("money");
	Results::ResultsTable resTable1 = obj1.selectMultiSyn(synList1);
	
	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("animals") == true);
	CPPUNIT_ASSERT(obj1.hasResults("money") == true);
	CPPUNIT_ASSERT(resTable1.size() == 7);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 7);

	// test results obj that has been passed around

	// Test 1 - getting paired synonyms that were
	// not added together
	// Purpose: check if results obtained contains the
	// right elements and do not contain duplicates
	// Inserting paired results for 2 synonym, animals and money, 
	// feeling and excercise
	// (animals, money): (tiger, 1), (rabbit, 2), (mouse, 5), (horse, 10),
	// (mouse, 1), (tiger, 2), (rabbit, 5)
	// (feeling, exercise): (happy, 9), (happy, 10), (exceited, 10), (loving, 10),
	// (loving, 10), (angry, 1), (bored, 2)

	Results obj3 = Results();
	unordered_map<string, string>* row3_1 = new unordered_map<string, string>();
	(*row3_1)["animals"] = "tiger";
	(*row3_1)["money"] = "1";
	obj3.insertMultiResult(row3_1);

	unordered_map<string, string>* row3_2 = new unordered_map<string, string>();
	(*row3_2)["animals"] = "rabbit";
	(*row3_2)["money"] = "2";
	obj3.insertMultiResult(row3_2);

	unordered_map<string, string>* row3_3 = new unordered_map<string, string>();
	(*row3_3)["animals"] = "mouse";
	(*row3_3)["money"] = "5";
	obj3.insertMultiResult(row3_3);

	unordered_map<string, string>* row3_4 = new unordered_map<string, string>();
	(*row3_4)["animals"] = "horse";
	(*row3_4)["money"] = "10";
	obj3.insertMultiResult(row3_4);

	unordered_map<string, string>* row3_5 = new unordered_map<string, string>();
	(*row3_5)["animals"] = "mouse";
	(*row3_5)["money"] = "1";
	obj3.insertMultiResult(row3_5);

	unordered_map<string, string>* row3_6 = new unordered_map<string, string>();
	(*row3_6)["animals"] = "tiger";
	(*row3_6)["money"] = "2";
	obj3.insertMultiResult(row3_6);

	unordered_map<string, string>* row3_7 = new unordered_map<string, string>();
	(*row3_7)["animals"] = "rabbit";
	(*row3_7)["money"] = "5";
	obj3.insertMultiResult(row3_7);
	obj3.push();

	unordered_map<string, string>* row3_8 = new unordered_map<string, string>();
	(*row3_8)["feeling"] = "happy";
	(*row3_8)["exercise"] = "9";
	obj3.insertMultiResult(row3_8);
	
	unordered_map<string, string>* row3_9 = new unordered_map<string, string>();
	(*row3_9)["feeling"] = "happy";
	(*row3_9)["exercise"] = "10";
	obj3.insertMultiResult(row3_9);

	unordered_map<string, string>* row3_10 = new unordered_map<string, string>();
	(*row3_10)["feeling"] = "excited";
	(*row3_10)["exercise"] = "10";
	obj3.insertMultiResult(row3_10);

	unordered_map<string, string>* row3_11 = new unordered_map<string, string>();
	(*row3_11)["feeling"] = "excited";
	(*row3_11)["exercise"] = "10";
	obj3.insertMultiResult(row3_11);

	unordered_map<string, string>* row3_12 = new unordered_map<string, string>();
	(*row3_12)["feeling"] = "loving";
	(*row3_12)["exercise"] = "10";
	obj3.insertMultiResult(row3_12);

	unordered_map<string, string>* row3_13 = new unordered_map<string, string>();
	(*row3_13)["feeling"] = "angry";
	(*row3_13)["exercise"] = "1";
	obj3.insertMultiResult(row3_13);

	unordered_map<string, string>* row3_14 = new unordered_map<string, string>();
	(*row3_14)["feeling"] = "bored";
	(*row3_14)["exercise"] = "2";
	obj3.insertMultiResult(row3_14);

	obj3.push();

	unordered_set<string> synList3 = unordered_set<string>();
	synList3.insert("feeling");
	synList3.insert("money");
	Results::ResultsTable resTable3 = obj3.selectMultiSyn(synList3);

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("feeling") == true);
	CPPUNIT_ASSERT(obj3.hasResults("money") == true);

	CPPUNIT_ASSERT(resTable3.size() == 20);

	// Test 2 - getting the common synonym whos
	// results was reduced
	// Purpose: check if results obtained contains the
	// right elements and do not contain duplicates
	// Inserting paired results for 3 synonym, 
	// ocbc and dbs, dbs and kpmg
	// (ocbc, dbs): (lao,1), (tao,1), (bao,1), (cao,5)
	// (bao,5), (tao,5), (lao,9), (cao,10), (bao,11)
	// (dbs, kpmg): (1,north), (1,south), (1,east), (1,west), (1, center)
	//  (10, gold), (10, silver), (10, bronze), (10, noprize)
	
	Results obj4 = Results();
	unordered_map<string, string>* row4_1 = new unordered_map<string, string>();
	(*row4_1)["ocbc"] = "lao";
	(*row4_1)["dbs"] = "1";
	obj4.insertMultiResult(row4_1);

	unordered_map<string, string>* row4_2 = new unordered_map<string, string>();
	(*row4_2)["ocbc"] = "tao";
	(*row4_2)["dbs"] = "1";
	obj4.insertMultiResult(row4_2);

	unordered_map<string, string>* row4_3 = new unordered_map<string, string>();
	(*row4_3)["ocbc"] = "bao";
	(*row4_3)["dbs"] = "1";
	obj4.insertMultiResult(row4_3);

	unordered_map<string, string>* row4_4 = new unordered_map<string, string>();
	(*row4_4)["ocbc"] = "cao";
	(*row4_4)["dbs"] = "5";
	obj4.insertMultiResult(row4_4);

	unordered_map<string, string>* row4_5 = new unordered_map<string, string>();
	(*row4_5)["ocbc"] = "bao";
	(*row4_5)["dbs"] = "5";
	obj4.insertMultiResult(row4_5);

	unordered_map<string, string>* row4_6 = new unordered_map<string, string>();
	(*row4_6)["ocbc"] = "tao";
	(*row4_6)["dbs"] = "5";
	obj4.insertMultiResult(row4_6);

	unordered_map<string, string>* row4_7 = new unordered_map<string, string>();
	(*row4_7)["ocbc"] = "lao";
	(*row4_7)["dbs"] = "9";
	obj4.insertMultiResult(row4_7);

	unordered_map<string, string>* row4_8 = new unordered_map<string, string>();
	(*row4_8)["ocbc"] = "cao";
	(*row4_8)["dbs"] = "10";
	obj4.insertMultiResult(row4_8);

	unordered_map<string, string>* row4_9 = new unordered_map<string, string>();
	(*row4_9)["ocbc"] = "bao";
	(*row4_9)["dbs"] = "11";
	obj4.insertMultiResult(row4_9);
	obj4.push();

	unordered_map<string, string>* row4_10 = new unordered_map<string, string>();
	(*row4_10)["dbs"] = "1";
	(*row4_10)["kpmg"] = "north";
	obj4.insertMultiResult(row4_10);

	unordered_map<string, string>* row4_11 = new unordered_map<string, string>();
	(*row4_11)["dbs"] = "1";
	(*row4_11)["kpmg"] = "south";
	obj4.insertMultiResult(row4_11);
	
	unordered_map<string, string>* row4_12 = new unordered_map<string, string>();
	(*row4_12)["dbs"] = "1";
	(*row4_12)["kpmg"] = "east";
	obj4.insertMultiResult(row4_12);

	unordered_map<string, string>* row4_13 = new unordered_map<string, string>();
	(*row4_13)["dbs"] = "1";
	(*row4_13)["kpmg"] = "west";
	obj4.insertMultiResult(row4_13);

	unordered_map<string, string>* row4_14 = new unordered_map<string, string>();
	(*row4_14)["dbs"] = "1";
	(*row4_14)["kpmg"] = "center";
	obj4.insertMultiResult(row4_14);

	unordered_map<string, string>* row4_15 = new unordered_map<string, string>();
	(*row4_15)["dbs"] = "10";
	(*row4_15)["kpmg"] = "gold";
	obj4.insertMultiResult(row4_15);

	unordered_map<string, string>* row4_16 = new unordered_map<string, string>();
	(*row4_16)["dbs"] = "10";
	(*row4_16)["kpmg"] = "silver";
	obj4.insertMultiResult(row4_16);

	unordered_map<string, string>* row4_17 = new unordered_map<string, string>();
	(*row4_17)["dbs"] = "10";
	(*row4_17)["kpmg"] = "bronze";
	obj4.insertMultiResult(row4_17);

	unordered_map<string, string>* row4_18 = new unordered_map<string, string>();
	(*row4_18)["dbs"] = "10";
	(*row4_18)["kpmg"] = "noprize";
	obj4.insertMultiResult(row4_18);
	obj4.push();

	unordered_set<string> synList4 = unordered_set<string>();
	synList4.insert("dbs");
	synList4.insert("ocbc");
	Results::ResultsTable resTable4 = obj4.selectMultiSyn(synList4);

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("dbs") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ocbc") == true);

	int size = resTable4.size();
	CPPUNIT_ASSERT(resTable4.size() == 4);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 19);


	// Test 3 - getting the synonyms whos
	// results were affected by the combining of additional results
	// Purpose: check if results obtained contains the
	// right elements and do not contain duplicates
	// Inserting paired results for 4 synonym, 
	// ocbc and dbs, dbs and kpmg
	// (uk, british): (clothes,9), (clothes,8), (clothes,7), (zalora,1)
	// (uniquo,1), (forever21,1)
	// (clock, tower): (5,london), (5,toyok), (5,us), (5,singapore),
	// (1,singapore), (2,singapore), (3,singapore), (4,singapore)

	Results obj5 = Results();
	unordered_map<string, string>* row5_1 = new unordered_map<string, string>();
	(*row5_1)["uk"] = "clothes";
	(*row5_1)["british"] = "9";
	obj5.insertMultiResult(row5_1);

	unordered_map<string, string>* row5_2 = new unordered_map<string, string>();
	(*row5_2)["uk"] = "clothes";
	(*row5_2)["british"] = "8";
	obj5.insertMultiResult(row5_2);

	unordered_map<string, string>* row5_3 = new unordered_map<string, string>();
	(*row5_3)["uk"] = "clothes";
	(*row5_3)["british"] = "7";
	obj5.insertMultiResult(row5_3);

	unordered_map<string, string>* row5_4 = new unordered_map<string, string>();
	(*row5_4)["uk"] = "zalora";
	(*row5_4)["british"] = "1";
	obj5.insertMultiResult(row5_4);

	unordered_map<string, string>* row5_5 = new unordered_map<string, string>();
	(*row5_5)["uk"] = "uniquo";
	(*row5_5)["british"] = "1";
	obj5.insertMultiResult(row5_5);

	unordered_map<string, string>* row5_6 = new unordered_map<string, string>();
	(*row5_6)["uk"] = "forever21";
	(*row5_6)["british"] = "1";
	obj5.insertMultiResult(row5_6);
	obj5.push();

	unordered_map<string, string>* row5_7 = new unordered_map<string, string>();
	(*row5_7)["clock"] = "5";
	(*row5_7)["tower"] = "london";
	obj5.insertMultiResult(row5_7);

	unordered_map<string, string>* row5_8 = new unordered_map<string, string>();
	(*row5_8)["clock"] = "5";
	(*row5_8)["tower"] = "tokyo";
	obj5.insertMultiResult(row5_8);

	unordered_map<string, string>* row5_9 = new unordered_map<string, string>();
	(*row5_9)["clock"] = "5";
	(*row5_9)["tower"] = "us";
	obj5.insertMultiResult(row5_9);

	unordered_map<string, string>* row5_10 = new unordered_map<string, string>();
	(*row5_10)["clock"] = "5";
	(*row5_10)["tower"] = "spore";
	obj5.insertMultiResult(row5_10);

	unordered_map<string, string>* row5_11 = new unordered_map<string, string>();
	(*row5_11)["clock"] = "1";
	(*row5_11)["tower"] = "spore";
	obj5.insertMultiResult(row5_11);

	unordered_map<string, string>* row5_12 = new unordered_map<string, string>();
	(*row5_12)["clock"] = "2";
	(*row5_12)["tower"] = "spore";
	obj5.insertMultiResult(row5_12);

	unordered_map<string, string>* row5_13 = new unordered_map<string, string>();
	(*row5_13)["clock"] = "3";
	(*row5_13)["tower"] = "spore";
	obj5.insertMultiResult(row5_13);

	unordered_map<string, string>* row5_14 = new unordered_map<string, string>();
	(*row5_14)["clock"] = "4";
	(*row5_14)["tower"] = "spore";
	obj5.insertMultiResult(row5_14);
	obj5.push();

	// intermediate check. Check number of results for uk and clock 
	unordered_set<string> synList5a = unordered_set<string>();
	synList5a.insert("uk");
	synList5a.insert("clock");
	Results::ResultsTable resTable5a = obj5.selectMultiSyn(synList5a);

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj5.hasResults("clock") == true);
	//4 unique uk multiply by 5 unique clock
	CPPUNIT_ASSERT(resTable5a.size() == 20);

	unordered_map<string, string>* row5_15 = new unordered_map<string, string>();
	(*row5_15)["uk"] = "clothes";
	(*row5_15)["clock"] = "5";
	obj5.insertMultiResult(row5_15);
	obj5.push();
	
	unordered_set<string> synList5b = unordered_set<string>();
	synList5b.insert("uk");
	synList5b.insert("clock");
	Results::ResultsTable resTable5b = obj5.selectMultiSyn(synList5b);

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj5.hasResults("clock") == true);
	//cos only uk: clothes and clock: 5 can exist
	CPPUNIT_ASSERT(resTable5b.size() == 1);
}
