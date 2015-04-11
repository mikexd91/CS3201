#include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/Results.h"
#include "../SPA/QueryEvaluator.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

// Test getSameClause of QueryEvaluator
void QueryEvaluatorTest::testEvaluator() {
	// Test when both results contain 0 unfixed arguments
	QueryEvaluator e = *new QueryEvaluator();

	Results r1 = *new Results();
	Results r2 = *new Results();

	r1.setNumOfSyn(0);
	r2.setNumOfSyn(0);

	int num = e.getSameClause(r1, r2);
	
	CPPUNIT_ASSERT(e.getSameClause(r1, r2) == 0);
	
	// Test when both results contains 1 unfixed argument
	// results contain 1 same clause

	Results r3 = *new Results();
	Results r4 = *new Results();

	r3.setNumOfSyn(1);
	r4.setNumOfSyn(1);

	r3.setFirstClauseSyn("a");
	r4.setFirstClauseSyn("a");

	int num2 = e.getSameClause(r3, r4);

	CPPUNIT_ASSERT(e.getSameClause(r3, r4) == 1);

	// Test when both results contains unfixed argument

	Results r5 = *new Results();
	Results r6 = *new Results();

	r5.setNumOfSyn(2);
	r6.setNumOfSyn(2);

	r5.setFirstClauseSyn("a");
	r6.setFirstClauseSyn("a");
}

void QueryEvaluatorTest::testEvaluator2() {

}

void QueryEvaluatorTest::testEvaluator3() {

}
