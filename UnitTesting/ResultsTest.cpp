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
	// (anne, ben): (1,a), (1,b), (2,a), (2,b)
	// next clause:
	// (ken, dan): (8,x), (8,y), (9,x), (9,y)
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
	// (anne, ben): (1,a), (1,b), (2,a), (2,b)
	// next clause:
	// (ben, ken): (b, 90), (b, 100), (b, 70)

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
	// Inserting results for 2 synonyms, anne,ben
	// first clause:
	// (jane, jon): (1, a), (1, b), (1, c), (2, a), (2, b), (2, c) 
	// second clause:
	// (jane, jon): (1, a), (2, b)

	Results obj5 = Results();
	unordered_map<string, string>* row5 = new unordered_map<string, string>();
	(*row5)["jane"] = "1";
	(*row5)["jon"] = "a";
	obj5.insertMultiResult(row5);
	
	(*row5)["jane"] = "1";
	(*row5)["jon"] = "b";
	obj5.insertMultiResult(row5);

	(*row5)["jane"] = "1";
	(*row5)["jon"] = "c";
	obj5.insertMultiResult(row5);

	(*row5)["jane"] = "2";
	(*row5)["jon"] = "a";
	obj5.insertMultiResult(row5);

	(*row5)["jane"] = "2";
	(*row5)["jon"] = "b";
	obj5.insertMultiResult(row5);

	(*row5)["jane"] = "2";
	(*row5)["jon"] = "c";
	obj5.insertMultiResult(row5);
	obj5.push();

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("jane") == true);
	CPPUNIT_ASSERT(obj5.hasResults("jon") == true);
	CPPUNIT_ASSERT(obj5.getResultsTableSize() == 6);

	// TO ADD
	// function to reset ClausePass and clean private variable states

	(*row5)["jane"] = "1";
	(*row5)["jon"] = "a";
	obj5.insertMultiResult(row5);

	(*row5)["jane"] = "2";
	(*row5)["jon"] = "b";
	obj5.insertMultiResult(row5);
	obj5.push();

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("jane") == true);
	CPPUNIT_ASSERT(obj5.hasResults("jon") == true);
	CPPUNIT_ASSERT(obj5.getResultsTableSize() == 2);
	obj5.~Results();
}

void ResultsTest::testSelectSyn() {
	// test result obj as it is created

	// Inserting paired results for 2 synonyms, java and jedi
	// (java,jedi): (GG, rank1), (AoA, rank4), (sistar, rank2), (2ne1, rank3)
	// getting java
	// getting jedi
	Results obj1 = Results();
	unordered_map<string, string>* row1 = new unordered_map<string, string>();
	(*row1)["java"] = "GG";
	(*row1)["jedi"] = "rank1";
	obj1.insertMultiResult(row1);

	(*row1)["java"] = "AoA";
	(*row1)["jedi"] = "rank4";
	obj1.insertMultiResult(row1);

	(*row1)["java"] = "sistar";
	(*row1)["jedi"] = "rank2";
	obj1.insertMultiResult(row1);

	(*row1)["java"] = "2ne1";
	(*row1)["jedi"] = "rank3";
	obj1.insertMultiResult(row1);
	obj1.push();

	unordered_set<string> results1 = obj1.selectSyn("java");
	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("java") == true);
	CPPUNIT_ASSERT(obj1.hasResults("jedi") == true);
	CPPUNIT_ASSERT(results1.size() == 4);
	
	obj1.~Results();

	// test getting non-existence synonym
	Results obj2 = Results();
	unordered_set<string> results2 = obj2.selectSyn("juicyCulture");
	
	CPPUNIT_ASSERT(obj2.isClausePass() == false);
	CPPUNIT_ASSERT(obj2.hasResults("juicyCulture") == false);
	CPPUNIT_ASSERT(results2.size() == 0);
	CPPUNIT_ASSERT(results2.size() == 0);

	obj2.~Results();

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

	obj3.~Results();

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
	unordered_map<string, string>* row4 = new unordered_map<string, string>();
	(*row4)["juicyCulture"] = "9";
	(*row4)["chanel"] = "x";
	obj4.insertMultiResult(row4);

	(*row4)["juicyCulture"] = "9";
	(*row4)["chanel"] = "y";
	obj4.insertMultiResult(row4);

	(*row4)["juicyCulture"] = "10";
	(*row4)["chanel"] = "a";
	obj4.insertMultiResult(row4);

	(*row4)["juicyCulture"] = "10";
	(*row4)["chanel"] = "b";
	obj4.insertMultiResult(row4);

	(*row4)["juicyCulture"] = "10";
	(*row4)["chanel"] = "x";
	obj4.insertMultiResult(row4);

	(*row4)["juicyCulture"] = "10";
	(*row4)["chanel"] = "y";
	obj4.insertMultiResult(row4);
	obj4.push();

	// TO ADD
	// function to reset ClausePass and clean private variable states

	(*row4)["chanel"] = "x";
	(*row4)["girlsGeneration"] = "mrmr";
	obj4.insertMultiResult(row4);

	(*row4)["chanel"] = "x";
	(*row4)["girlsGeneration"] = "rundevilrun";
	obj4.insertMultiResult(row4);
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
	obj4.~Results();
}

void ResultsTest::testSelectMultiSyn() {
	// test result obj as it is created

	// Inserting paired results for 2 synonym, animals and money
	// (animals, money): (tiger, 1), (rabbit, 2), (mouse, 5), (horse, 10),
	// (mouse, 1), (tiger, 2), (rabbit, 5)
	
	Results obj1 = Results();
	unordered_map<string, string>* row1 = new unordered_map<string, string>();
	(*row1)["animals"] = "tiger";
	(*row1)["money"] = "1";
	obj1.insertMultiResult(row1);

	(*row1)["animals"] = "rabbit";
	(*row1)["money"] = "2";
	obj1.insertMultiResult(row1);

	(*row1)["animals"] = "mouse";
	(*row1)["money"] = "5";
	obj1.insertMultiResult(row1);

	(*row1)["animals"] = "horse";
	(*row1)["money"] = "10";
	obj1.insertMultiResult(row1);

	(*row1)["animals"] = "mouse";
	(*row1)["money"] = "1";
	obj1.insertMultiResult(row1);

	(*row1)["animals"] = "tiger";
	(*row1)["money"] = "2";
	obj1.insertMultiResult(row1);

	(*row1)["animals"] = "rabbit";
	(*row1)["money"] = "5";
	obj1.insertMultiResult(row1);
	obj1.push();

	unordered_set<string> synList1 = unordered_set<string>();
	synList1.insert("animals");
	synList1.insert("money");
	unordered_set<unordered_map<string, string>*> resTable1 = obj1.selectMultiSyn(synList1);
	
	CPPUNIT_ASSERT(obj1.isClausePass() == true);
	CPPUNIT_ASSERT(obj1.hasResults("animals") == true);
	CPPUNIT_ASSERT(obj1.hasResults("money") == true);
	CPPUNIT_ASSERT(resTable1.size() == 7);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 7);

	obj1.~Results();
	
	// test getting non-existance data
	Results obj2 = Results();
	unordered_set<string> synList2 = unordered_set<string>();
	synList2.insert("animals");
	synList2.insert("money");
	unordered_set<unordered_map<string, string>*> resTable2 = obj2.selectMultiSyn(synList2);

	CPPUNIT_ASSERT(obj2.isClausePass() == true);
	CPPUNIT_ASSERT(obj2.hasResults("animals") == false);
	CPPUNIT_ASSERT(obj2.hasResults("money") == false);
	CPPUNIT_ASSERT(resTable2.size() == 0);

	obj2.~Results();

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
	unordered_map<string, string>* row3 = new unordered_map<string, string>();
	(*row3)["animals"] = "tiger";
	(*row3)["money"] = "1";
	obj3.insertMultiResult(row3);

	(*row3)["animals"] = "rabbit";
	(*row3)["money"] = "2";
	obj3.insertMultiResult(row3);

	(*row3)["animals"] = "mouse";
	(*row3)["money"] = "5";
	obj3.insertMultiResult(row3);

	(*row3)["animals"] = "horse";
	(*row3)["money"] = "10";
	obj3.insertMultiResult(row3);

	(*row3)["animals"] = "mouse";
	(*row3)["money"] = "1";
	obj3.insertMultiResult(row3);

	(*row3)["animals"] = "tiger";
	(*row3)["money"] = "2";
	obj3.insertMultiResult(row3);

	(*row3)["animals"] = "rabbit";
	(*row3)["money"] = "5";
	obj3.insertMultiResult(row3);
	obj3.push();

	(*row3)["feeling"] = "happy";
	(*row3)["exercise"] = "9";
	obj3.insertMultiResult(row3);
	
	(*row3)["feeling"] = "happy";
	(*row3)["exercise"] = "10";
	obj3.insertMultiResult(row3);

	(*row3)["feeling"] = "excited";
	(*row3)["exercise"] = "10";
	obj3.insertMultiResult(row3);

	(*row3)["feeling"] = "excited";
	(*row3)["exercise"] = "10";
	obj3.insertMultiResult(row3);

	(*row3)["feeling"] = "loving";
	(*row3)["exercise"] = "10";
	obj3.insertMultiResult(row3);

	(*row3)["feeling"] = "angry";
	(*row3)["exercise"] = "1";
	obj3.insertMultiResult(row3);

	(*row3)["feeling"] = "bored";
	(*row3)["exercise"] = "2";
	obj3.insertMultiResult(row3);
	obj3.push();

	unordered_set<string> synList3 = unordered_set<string>();
	synList3.insert("feeling");
	synList3.insert("money");
	unordered_set<unordered_map<string, string>*> resTable3 = obj3.selectMultiSyn(synList3);

	CPPUNIT_ASSERT(obj3.isClausePass() == true);
	CPPUNIT_ASSERT(obj3.hasResults("feeling") == true);
	CPPUNIT_ASSERT(obj3.hasResults("money") == true);
	CPPUNIT_ASSERT(resTable3.size() == 20);

	obj3.~Results();

	// Test 2 - getting the common synonym whos
	// results was reduced
	// Purpose: check if results obtained contains the
	// right elements and do not contain duplicates
	// Inserting paired results for 3 synonym, 
	// ocbc and dbs, dbs and kpmg
	// (ocbc, dbs): (lao,1), (tao,1), (bao,1), (cao,5)
	// (bao,5), (tao,5), (lao,9), (cao,10), (bao,11)
	// (dbs, kpmg): (1,north), (1,south), (1,east), (1,west),
	// (1,gold), (1,silver), (1,bronze), (1,noprize), (1,center)

	Results obj4 = Results();
	unordered_map<string, string>* row4 = new unordered_map<string, string>();
	(*row4)["ocbc"] = "lao";
	(*row4)["dbs"] = "1";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "tao";
	(*row4)["dbs"] = "1";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "bao";
	(*row4)["dbs"] = "1";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "cao";
	(*row4)["dbs"] = "5";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "bao";
	(*row4)["dbs"] = "5";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "tao";
	(*row4)["dbs"] = "5";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "lao";
	(*row4)["dbs"] = "9";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "cao";
	(*row4)["dbs"] = "10";
	obj4.insertMultiResult(row4);

	(*row4)["ocbc"] = "bao";
	(*row4)["dbs"] = "11";
	obj4.insertMultiResult(row4);
	obj4.push();

	(*row4)["dbs"] = "1";
	(*row4)["kpmg"] = "north";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "1";
	(*row4)["kpmg"] = "south";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "1";
	(*row4)["kpmg"] = "east";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "1";
	(*row4)["kpmg"] = "west";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "1";
	(*row4)["kpmg"] = "center";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "10";
	(*row4)["kpmg"] = "gold";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "10";
	(*row4)["kpmg"] = "silver";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "10";
	(*row4)["kpmg"] = "bronze";
	obj4.insertMultiResult(row4);

	(*row4)["dbs"] = "10";
	(*row4)["kpmg"] = "noprize";
	obj4.insertMultiResult(row4);
	obj4.push();

	unordered_set<string> synList4 = unordered_set<string>();
	synList4.insert("dbs");
	synList4.insert("ocbc");
	unordered_set<unordered_map<string, string>*> resTable4 = obj4.selectMultiSyn(synList4);

	CPPUNIT_ASSERT(obj4.isClausePass() == true);
	CPPUNIT_ASSERT(obj4.hasResults("dbs") == true);
	CPPUNIT_ASSERT(obj4.hasResults("ocbc") == true);
	CPPUNIT_ASSERT(resTable4.size() == 4);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 24);
	obj4.~Results();

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
	unordered_map<string, string>* row5 = new unordered_map<string, string>();
	(*row5)["uk"] = "clothes";
	(*row5)["british"] = "9";
	obj5.insertMultiResult(row5);

	(*row5)["uk"] = "clothes";
	(*row5)["british"] = "8";
	obj5.insertMultiResult(row5);

	(*row5)["uk"] = "clothes";
	(*row5)["british"] = "7";
	obj5.insertMultiResult(row5);

	(*row5)["uk"] = "zalora";
	(*row5)["british"] = "1";
	obj5.insertMultiResult(row5);

	(*row5)["uk"] = "uniquo";
	(*row5)["british"] = "1";
	obj5.insertMultiResult(row5);

	(*row5)["uk"] = "forever21";
	(*row5)["british"] = "1";
	obj5.insertMultiResult(row5);
	
	obj5.push();

	(*row5)["clock"] = "5";
	(*row5)["tower"] = "london";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "5";
	(*row5)["tower"] = "tokyo";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "5";
	(*row5)["tower"] = "us";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "5";
	(*row5)["tower"] = "spore";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "1";
	(*row5)["tower"] = "spore";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "2";
	(*row5)["tower"] = "spore";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "3";
	(*row5)["tower"] = "spore";
	obj5.insertMultiResult(row5);

	(*row5)["clock"] = "4";
	(*row5)["tower"] = "spore";
	obj5.insertMultiResult(row5);
	obj5.push();

	// intermediate check. Check number of results for uk and clock 
	unordered_set<string> synList5a = unordered_set<string>();
	synList5a.insert("uk");
	synList5a.insert("clock");
	unordered_set<unordered_map<string, string>*> resTable5a = obj5.selectMultiSyn(synList5a);

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj5.hasResults("clock") == true);
	CPPUNIT_ASSERT(resTable5a.size() == 24);

	(*row5)["uk"] = "clothes";
	(*row5)["clock"] = "5";
	obj5.insertMultiResult(row5);
	obj5.push();
	
	unordered_set<string> synList5b = unordered_set<string>();
	synList5b.insert("uk");
	synList5b.insert("clock");
	unordered_set<unordered_map<string, string>*> resTable5b = obj5.selectMultiSyn(synList5b);

	CPPUNIT_ASSERT(obj5.isClausePass() == true);
	CPPUNIT_ASSERT(obj5.hasResults("uk") == true);
	CPPUNIT_ASSERT(obj5.hasResults("clock") == true);
	CPPUNIT_ASSERT(resTable5b.size() == 9);

	obj5.~Results();
}
