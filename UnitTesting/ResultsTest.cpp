#include <cppunit/config/SourcePrefix.h>
#include "ResultsTest.h"
#include "../SPA/Results.h"
#include <iostream>

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ResultsTest );

void ResultsTest::testHasResults() {
	Results obj = Results();
	CPPUNIT_ASSERT(obj.test() == true);
	//CPPUNIT_ASSERT(obj.test2() == true);
	CPPUNIT_ASSERT(obj.getResultsTableSize() == 1);
	CPPUNIT_ASSERT(obj.hasResults("a") == true);
}

/*
void ResultsTest::testCluasePass() {
	Results obj = Results();
	// isClausePass is false by default
	CPPUNIT_ASSERT(obj.isClausePass() == false);
	
	// isCluasePass is set to true
	obj.setClausePass();
	CPPUNIT_ASSERT(obj.isClausePass() == true);

	// isClausePass is set reset back to default
	obj.resetClausePass();
	CPPUNIT_ASSERT(obj.isClausePass() == false);
}

void ResultsTest::testHasResults() {
	Results obj = Results();
	// results obj has no results immediately after creation
	CPPUNIT_ASSERT(obj.hasResults("a") == false);
	CPPUNIT_ASSERT(obj.hasResults("1") == false);

	
}
*/
/*
testInsertMultiResult will be used by clauses with more than 1 synonym,
however, the clauses used will be fixed. 
For instance, if the 3 clauses are a, b, c. Future inserts within a
clause will be limited to synonym a, b, c.
e.g. Clause(a, b) or Clause a(b, c)
*/
/*
void ResultsTest::testInsertMultiResult() {
	// test result obj as it is created
	
	Results obj1 = Results();
	// test 1 time insertion, 3 synonyms at once
	set<unordered_map<string, string>> resultSet1;
	unordered_map<string, string> resultsList1;
	resultsList1["cee"] = "1";
	resultsList1["jay"] = "2";
	resultsList1["doubleu"] = "3";

	resultSet1.insert(resultsList1);

	CPPUNIT_ASSERT(obj1.insertMultiResult(resultSet1) == true);
	CPPUNIT_ASSERT(obj1.hasResults("cee") == false);
	CPPUNIT_ASSERT(obj1.pushChanges() == true);
	CPPUNIT_ASSERT(obj1.hasResults("cee") == true);
	CPPUNIT_ASSERT(obj1.hasResults("jay") == true);
	CPPUNIT_ASSERT(obj1.hasResults("doubleu") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 1);

	Results obj2 = Results();
	// test 1 time insertion, 2 synonyms at once
	set<unordered_map<string, string>> resultSet2;
	unordered_map<string, string> resultsList2;
	resultsList2["manga"] = "com";
	resultsList2["fox"] = "me";

	resultSet2.insert(resultsList2);

	CPPUNIT_ASSERT(obj2.insertMultiResult(resultSet2) == true);
	CPPUNIT_ASSERT(obj2.pushChanges() == true);
	CPPUNIT_ASSERT(obj2.getResultsTableSize() == 1);
	CPPUNIT_ASSERT(obj2.hasResults("manga") == true);
	CPPUNIT_ASSERT(obj2.hasResults("fox") == true);
	
	Results obj3 = Results();
	// test repeated insertions, 3 synonyms at once
	set<unordered_map<string, string>> resultSet3;
	unordered_map<string, string> resultsList3;
	resultsList3["lsm1301"] = "C";
	resultsList3["cs3219"] = "C";
	resultsList3["cs3202"] = "C";
	
	resultSet3.insert(resultsList3);

	resultsList3["lsm1301"] = "B";
	resultsList3["cs3219"] = "B";
	resultsList3["cs3202"] = "B";

	resultSet3.insert(resultsList3);

	resultsList3["lsm1301"] = "A";
	resultsList3["cs3219"] = "A";
	resultsList3["cs3202"] = "A";

	resultSet3.insert(resultsList3);
	
	CPPUNIT_ASSERT(obj3.insertMultiResult(resultSet3) == true);
	CPPUNIT_ASSERT(obj3.pushChanges() == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 3);
	CPPUNIT_ASSERT(obj3.hasResults("lsm1301") == true);
	CPPUNIT_ASSERT(obj3.hasResults("cs3219") == true);
	CPPUNIT_ASSERT(obj3.hasResults("cs3202") == true);

	Results obj4 = Results();
	// test repeated values
	set<unordered_map<string, string>> resultSet4;
	unordered_map<string, string> resultsList4;
									// (a, b)
	resultsList4["a"] = "1";		
	resultsList4["b"] = "x";		// (1, x)

	resultSet4.insert(resultsList4);

	resultsList4["a"] = "1";		
	resultsList4["b"] = "y";		// (1, y)

	resultSet4.insert(resultsList4);

	resultsList4["a"] = "1";		
	resultsList4["b"] = "z";		// (1, z)

	resultSet4.insert(resultsList4);

	resultsList4["a"] = "2";		
	resultsList4["b"] = "x";		// (2, x)

	resultSet4.insert(resultsList4);

	resultsList4["a"] = "2";		
	resultsList4["b"] = "y";		// (2, y)

	resultSet4.insert(resultsList4);

	resultsList4["a"] = "2";		
	resultsList4["b"] = "z";		// (2, z)

	resultSet4.insert(resultsList4);

	CPPUNIT_ASSERT(obj4.insertMultiResult(resultSet4) == true);
	CPPUNIT_ASSERT(obj4.pushChanges() == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 6);
	*/
	/* facilitate results obj being passed to each clause.
		test result obj if it retains previous values.
	*/
/*
	Results obj = Results();
	// test repeated insertions, 2 synonyms at once
	set<unordered_map<string, string>> setA;
	unordered_map<string, string> mapA;
	
	mapA["budapest"] = "1";
	mapA["gg"] = "2";

	setA.insert(mapA);

	mapA["budapest"] = "11";
	mapA["gg"] = "22";

	setA.insert(mapA);

	mapA["budapest"] = "111";
	mapA["gg"] = "222";

	setA.insert(mapA);

	CPPUNIT_ASSERT(obj.insertMultiResult(setA) == true);
	CPPUNIT_ASSERT(obj.pushChanges() == true);
	CPPUNIT_ASSERT(obj.getResultsTableSize() == 3);
	CPPUNIT_ASSERT(obj.hasResults("budapest") == true);
	CPPUNIT_ASSERT(obj.hasResults("gg") == true);

	// test repeated insertions, another 2, but different synonyms at once
	set<unordered_map<string, string>> setB;
	unordered_map<string, string> mapB;

	mapB["penang"] = "georgeTown";
	mapB["KL"] = "popiah";
	mapB["johor"] = "seafood";

	setB.insert(mapB);

	mapB["penang"] = "seaside";
	mapB["KL"] = "town";
	mapB["johor"] = "town";

	setB.insert(mapB);

	mapB["penang"] = "wallArt";
	mapB["KL"] = "streetArt";
	mapB["johor"] = "noArt";

	setB.insert(mapB);

	CPPUNIT_ASSERT(obj.insertMultiResult(setB) == true);
	CPPUNIT_ASSERT(obj.pushChanges() == true);
	CPPUNIT_ASSERT(obj.getResultsTableSize() == 9);
	CPPUNIT_ASSERT(obj.hasResults("budapest") == true);
	CPPUNIT_ASSERT(obj.hasResults("gg") == true);
	CPPUNIT_ASSERT(obj.hasResults("KL") == true);
	CPPUNIT_ASSERT(obj.hasResults("penang") == true);

}
*/
/*
insertResult will be used by clauses with 1 synonym,
e.g. Clause(1, a) or Clause (_, b)
Hence, within a clause, only 1 synonym will be added at a time.
*/
/*
void ResultsTest::testInsertResult() {
	// test results obj as it is created
	
	Results obj1 = Results();
	// test insertion 1 synonym and 1 value
	CPPUNIT_ASSERT(obj1.insertResult("a", "99") == true);
	CPPUNIT_ASSERT(obj1.hasResults("a") == false);
	CPPUNIT_ASSERT(obj1.pushChanges() == true);
	CPPUNIT_ASSERT(obj1.hasResults("a") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 1);
	
	Results obj2 = Results();
	// test insertion of 1 synonym and 3 values
	CPPUNIT_ASSERT(obj2.insertResult("var","pn") == true);
	CPPUNIT_ASSERT(obj2.insertResult("var", "leon") == true);
	CPPUNIT_ASSERT(obj2.insertResult("var", "jon") == true);

	CPPUNIT_ASSERT(obj2.hasResults("var") == false);
	CPPUNIT_ASSERT(obj2.pushChanges() == true);
	CPPUNIT_ASSERT(obj2.hasResults("var") == true);
	CPPUNIT_ASSERT(obj2.getResultsTableSize() == 3);
	
	Results obj3 = Results();
	// test insertion of 3 synonyms and 1 value
	CPPUNIT_ASSERT(obj3.insertResult("GG","711") == true);
	CPPUNIT_ASSERT(obj3.pushChanges() == true);
	CPPUNIT_ASSERT(obj3.insertResult("Sistar","711") == true);
	CPPUNIT_ASSERT(obj3.pushChanges() == true);
	CPPUNIT_ASSERT(obj3.insertResult("2NE1","711") == true);
	CPPUNIT_ASSERT(obj3.pushChanges() == true);

	CPPUNIT_ASSERT(obj3.hasResults("GG") == true);
	CPPUNIT_ASSERT(obj3.hasResults("Sistar") == true);
	CPPUNIT_ASSERT(obj3.hasResults("2NE1") == true);
	CPPUNIT_ASSERT(obj3.getResultsTableSize() == 1);

	Results obj4 = Results();
	// test for duplicates with repeated insertions
	CPPUNIT_ASSERT(obj4.insertResult("grade", "Aplus") == true);
	CPPUNIT_ASSERT(obj4.insertResult("grade", "Bplus") == true);
	CPPUNIT_ASSERT(obj4.insertResult("grade", "Aplus") == true);
	CPPUNIT_ASSERT(obj4.pushChanges() == true);

	CPPUNIT_ASSERT(obj4.hasResults("grade") == true);
	CPPUNIT_ASSERT(obj4.getResultsTableSize() == 2);

	Results obj5 = Results();
	// test empty insert
	CPPUNIT_ASSERT(obj5.insertResult("", "") == false);
	CPPUNIT_ASSERT(obj5.insertResult("a", "") == false);
	CPPUNIT_ASSERT(obj5.insertResult("", "a") == false);
	CPPUNIT_ASSERT(obj5.pushChanges() == false);
	CPPUNIT_ASSERT(obj5.getResultsTableSize() == 0);
	*/
	/* facilitate results obj being passed to each clause.
		test result obj if it retains previous values.
	*/
/*
	Results obj = Results();
	// test insertion 1 synonym and 1 value
	CPPUNIT_ASSERT(obj.insertResult("a", "99") == true);
	CPPUNIT_ASSERT(obj.hasResults("a") == false);
	CPPUNIT_ASSERT(obj.pushChanges() == true);
	CPPUNIT_ASSERT(obj.hasResults("a") == true);
	
	// test insertion of 1 synonym and 3 values
	
	CPPUNIT_ASSERT(obj.insertResult("var","pn") == true);
	CPPUNIT_ASSERT(obj.insertResult("var", "leon") == true);
	CPPUNIT_ASSERT(obj.insertResult("var", "jon") == true);
	
	CPPUNIT_ASSERT(obj.hasResults("var") == false);
	CPPUNIT_ASSERT(obj.pushChanges() == true);
	CPPUNIT_ASSERT(obj.hasResults("var") == true);
	CPPUNIT_ASSERT(obj.hasResults("a") == true);
	
	// test inserton of 3 synonym and 1 value
	CPPUNIT_ASSERT(obj.insertResult("GG","711") == true);
	CPPUNIT_ASSERT(obj.pushChanges() == true);
	CPPUNIT_ASSERT(obj.insertResult("Sistar","711") == true);
	CPPUNIT_ASSERT(obj.pushChanges() == true);
	CPPUNIT_ASSERT(obj.insertResult("2NE1","711") == true);
	CPPUNIT_ASSERT(obj.pushChanges() == true);

	CPPUNIT_ASSERT(obj.hasResults("GG") == true);
	CPPUNIT_ASSERT(obj.hasResults("Sistar") == true);
	CPPUNIT_ASSERT(obj.hasResults("2NE1") == true);
	CPPUNIT_ASSERT(obj.getResultsTableSize() == 3);
}
*/

//void ResultsTest::testInsertResultWhere() {
	/* facilitate results obj being passed to each clause.
		test result obj if it retains previous values.
	*/
	
	//Results obj1 = Results();
	/* test insertion of synonym c in the case of 1 common
	synonym. Eg. clause(a,b) and clause(b,c) where resultsTable
	already contain synonym a and b.

	clause(a, b)	clause(b, c)
		(1,2)			(2, 1)
		(1,3)			(2, 2)
		(1,4)			(3, 1)
						(3, 2)
	*/

	// Setup of resultsTable
/*
	set<unordered_map<string, string>> resultSet1;
	unordered_map<string, string> resultsList1;
									// (a, b)
	resultsList1["a"] = "1";		
	resultsList1["b"] = "2";		// (1, 2)

	resultSet1.insert(resultsList1);

	resultsList1["a"] = "1";		
	resultsList1["b"] = "3";		// (1, 3)

	resultSet1.insert(resultsList1);

	resultsList1["a"] = "1";		
	resultsList1["b"] = "4";		// (1, 4)

	resultSet1.insert(resultsList1);
	CPPUNIT_ASSERT(obj1.insertMultiResult(resultSet1) == true);
	CPPUNIT_ASSERT(obj1.pushChanges() == true);
	CPPUNIT_ASSERT(obj1.hasResults("a") == true);
	CPPUNIT_ASSERT(obj1.hasResults("b") == true);
	// End of setup

	// Setup of synonym c results
	resultSet1.clear();
	resultsList1.clear();

	resultsList1["c"] = "1";		
	resultSet1.insert(resultsList1);

	resultsList1["c"] = "2";		
	resultSet1.insert(resultsList1);

	CPPUNIT_ASSERT(obj1.insertResultWhere("b", "2", resultSet1) == true);

	resultsList1["c"] = "1";		
	resultSet1.insert(resultsList1);

	resultsList1["c"] = "2";		
	resultSet1.insert(resultsList1);

	CPPUNIT_ASSERT(obj1.insertResultWhere("b", "3", resultSet1) == true);
	CPPUNIT_ASSERT(obj1.pushChanges() == true);
	CPPUNIT_ASSERT(obj1.hasResults("a") == true);
	CPPUNIT_ASSERT(obj1.hasResults("b") == true);
	CPPUNIT_ASSERT(obj1.hasResults("c") == true);
	CPPUNIT_ASSERT(obj1.getResultsTableSize() == 4);
	// End of setup

}
*/
/*
void ResultsTest::testSelectMultiSyn() {
	//CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
}

void ResultsTest::testSelectSyn() {
	//CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
}

void ResultsTest::testSelectSynWhere() {
	//CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
}

void ResultsTest::testPushChanges() {
	//CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
}

void ResultsTest::testDelMultiResult() {
	//CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
}

void ResultsTest::testDelResult() {
	//CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
}
*/