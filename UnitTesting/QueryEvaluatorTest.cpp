#include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/Results.h"
#include "../SPA/QueryEvaluator.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

// Test getSameClause of QueryEvaluator
void QueryEvaluatorTest::testEvaluator() {
	QueryEvaluator e = *new QueryEvaluator();
	Results r1 = *new Results();
	Results r2 = *new Results();

	r1.setNumOfSyn(0);
	r2.setNumOfSyn(0);

	int num = e.getSameClause(r1, r2);
	
	cout << "JIAWEI";
	cout << num;
}

void QueryEvaluatorTest::testEvaluator2() {

}

void QueryEvaluatorTest::testEvaluator3() {

}
