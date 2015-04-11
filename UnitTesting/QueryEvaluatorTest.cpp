#include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/Results.h"

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

void QueryEvaluatorTest::testEvaluator() {
	Results* resObj1 = new Results();
	Results* resObj2 = new Results();

	resObj1->setClausePassed(true);
}

void QueryEvaluatorTest::testEvaluator2() {

}

void QueryEvaluatorTest::testEvaluator3() {

}
