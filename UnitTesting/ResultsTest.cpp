#include <cppunit/config/SourcePrefix.h>
#include "ResultsTest.h"
#include "../SPA/Results.h"
#include <iostream>

using namespace std;

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ResultsTest );

void ResultsTest::testSingleGetIntercept() {
	// Select a such that Uses(a,"v") pattern a("v", _)
	Results r1 = *new Results();
	r1.setClausePassed(true);
	r1.setNumOfSyn(1);
	r1.setFirstClauseSyn("a");
	r1.addSingleResult("1");
	r1.addSingleResult("3");
	r1.addSingleResult("4");

	Results r2 = *new Results();
	r2.setClausePassed(true);
	r2.setNumOfSyn(1);
	r2.setFirstClauseSyn("a");
	r2.addSingleResult("1");
	r2.addSingleResult("2");
	r2.addSingleResult("4");

	r1.getIntersect(r2);
	
	CPPUNIT_ASSERT(r1.getSinglesResults() == r2.getSinglesResults());

	// Select a such that Uses(a, "v") pattern a1("v", _);
	// The results for this obj should remain the same
	Results r3 = *new Results();
	r3.setClausePassed(true);
	r3.setNumOfSyn(1);
	r3.setFirstClauseSyn("a");
	r3.addSingleResult("1");
	r3.addSingleResult("3");
	r3.addSingleResult("4");

	Results r4 = *new Results();
	r4.setClausePassed(true);
	r4.setNumOfSyn(1);
	r4.setFirstClauseSyn("a1");
	r4.addSingleResult("1");
	r4.addSingleResult("2");
	r4.addSingleResult("4");

	r4.getIntersect(r3);

	CPPUNIT_ASSERT(r3.getSinglesResults() != r4.getSinglesResults());
	CPPUNIT_ASSERT(!r3.getSinglesResults().empty());

	// Select a such that Uses(a, "v") pattern a("v",_)

	Results r5 = *new Results();
	r5.setClausePassed(true);
	r5.setNumOfSyn(1);
	r5.setFirstClauseSyn("a");
	r5.addSingleResult("1");
	r5.addSingleResult("3");
	r5.addSingleResult("4");

	Results r6 = *new Results();
	r6.setClausePassed(true);
	r6.setNumOfSyn(1);
	r6.setFirstClauseSyn("a");
	r6.addSingleResult("2");
	r6.addSingleResult("5");
	r6.addSingleResult("6");

	r6.getIntersect(r5);

	CPPUNIT_ASSERT(r5.getSinglesResults() == r6.getSinglesResults());
	CPPUNIT_ASSERT(r6.getSinglesResults().empty());
}

void ResultsTest::testSinglePairGetIntercept() {
	// Select a such that Uses(a, "v") pattern a(v,_)
	Results r1 = *new Results();
	r1.setClausePassed(true);
	r1.setNumOfSyn(2);
	r1.setFirstClauseSyn("a");
	r1.setSecondClauseSyn("v");
	r1.addPairResult("1", "v");
	r1.addPairResult("1", "y");
	r1.addPairResult("1", "z");
	r1.addPairResult("2", "v");
	r1.addPairResult("2", "z");
	r1.addPairResult("3", "z");

	Results r2 = *new Results();
	r2.setClausePassed(true);
	r2.setNumOfSyn(1);
	r2.setFirstClauseSyn("a");
	r2.addSingleResult("1");
	r2.addSingleResult("2");
	r2.addSingleResult("4");
	
	r1.getIntersect(r2);

	CPPUNIT_ASSERT(r1.getPairResults().size() == 5);
	CPPUNIT_ASSERT(r2.getSinglesResults().size() == 2);

	// Select a such that Uses(1, v) pattern a(v,_)
	Results r3 = *new Results();
	r3.setClausePassed(true);
	r3.setNumOfSyn(2);
	r3.setFirstClauseSyn("a");
	r3.setSecondClauseSyn("v");
	r3.addPairResult("1", "v");
	r3.addPairResult("1", "y");
	r3.addPairResult("1", "z");
	r3.addPairResult("2", "v");
	r3.addPairResult("2", "z");
	r3.addPairResult("3", "z");

	Results r4 = *new Results();
	r4.setClausePassed(true);
	r4.setNumOfSyn(1);
	r4.setFirstClauseSyn("v");
	r4.addSingleResult("v");
	r4.addSingleResult("x");
	
	r3.getIntersect(r4);

	CPPUNIT_ASSERT(r3.getPairResults().size() == 2);
	CPPUNIT_ASSERT(r4.getSinglesResults().size() == 1);

	// Select a such that Uses(1, v) pattern a(v,_)
	Results r5 = *new Results();
	r5.setClausePassed(true);
	r5.setNumOfSyn(2);
	r5.setFirstClauseSyn("a");
	r5.setSecondClauseSyn("v1");
	r5.addPairResult("1", "v");
	r5.addPairResult("1", "y");
	r5.addPairResult("1", "z");
	r5.addPairResult("2", "v");
	r5.addPairResult("2", "z");
	r5.addPairResult("3", "z");

	Results r6 = *new Results();
	r6.setClausePassed(true);
	r6.setNumOfSyn(1);
	r6.setFirstClauseSyn("v");
	r6.addSingleResult("v");
	r6.addSingleResult("x");
	
	r5.getIntersect(r6);

	CPPUNIT_ASSERT(r5.getPairResults().size() == 6);
	CPPUNIT_ASSERT(r6.getSinglesResults().size() == 2);

}

void ResultsTest::testPairGetIntercept() {
	// Select a such that Uses(a, v) pattern a(v,_)
	Results r1 = *new Results();
	r1.setClausePassed(true);
	r1.setNumOfSyn(2);
	r1.setFirstClauseSyn("a");
	r1.setSecondClauseSyn("v");
	r1.addPairResult("1", "v");
	r1.addPairResult("1", "y");
	r1.addPairResult("1", "z");
	r1.addPairResult("2", "v");
	r1.addPairResult("2", "z");
	r1.addPairResult("3", "z");

	Results r2 = *new Results();
	r2.setClausePassed(true);
	r2.setNumOfSyn(2);
	r2.setFirstClauseSyn("a");
	r2.setSecondClauseSyn("v");
	r2.addPairResult("1", "v");
	r2.addPairResult("1", "x");
	r2.addPairResult("2", "z");
	r2.addPairResult("2", "v");
	r2.addPairResult("3", "z");
	
	r2.getIntersect(r1);

	CPPUNIT_ASSERT(r1.getPairResults().size() == 4);
	CPPUNIT_ASSERT(r2.getPairResults().size() == 4);

	// Select a such that Uses(a, v1) pattern a(v,_)
	Results r3 = *new Results();
	r3.setClausePassed(true);
	r3.setNumOfSyn(2);
	r3.setFirstClauseSyn("a");
	r3.setSecondClauseSyn("v1");
	r3.addPairResult("1", "v");
	r3.addPairResult("1", "y");
	r3.addPairResult("1", "z");
	r3.addPairResult("2", "v");
	r3.addPairResult("2", "z");
	r3.addPairResult("3", "z");

	Results r4 = *new Results();
	r4.setClausePassed(true);
	r4.setNumOfSyn(2);
	r4.setFirstClauseSyn("a");
	r4.setSecondClauseSyn("v");
	r4.addPairResult("1", "v");
	r4.addPairResult("1", "x");
	r4.addPairResult("3", "z");
	r4.addPairResult("4", "v");
	r4.addPairResult("5", "z");
	
	r4.getIntersect(r3);

	CPPUNIT_ASSERT(r3.getPairResults().size() == 4);
	CPPUNIT_ASSERT(r4.getPairResults().size() == 3);

	// Select a such that Follows(a, a1) pattern a1(v,_)
	Results r5 = *new Results();
	r5.setClausePassed(true);
	r5.setNumOfSyn(2);
	r5.setFirstClauseSyn("a");
	r5.setSecondClauseSyn("a1");
	r5.addPairResult("1", "2");
	r5.addPairResult("2", "3");
	r5.addPairResult("1", "5");
	r5.addPairResult("2", "4");
	r5.addPairResult("9", "1");
	r5.addPairResult("2", "1");

	Results r6 = *new Results();
	r6.setClausePassed(true);
	r6.setNumOfSyn(2);
	r6.setFirstClauseSyn("a1");
	r6.setSecondClauseSyn("v");
	r6.addPairResult("2", "v");
	r6.addPairResult("1", "x");
	r6.addPairResult("8", "z");
	r6.addPairResult("4", "v");
	r6.addPairResult("9", "z");
	
	r5.getIntersect(r6);

	CPPUNIT_ASSERT(r5.getPairResults().size() == 4);
	CPPUNIT_ASSERT(r6.getPairResults().size() == 3);

	// Select a such that Uses(a, v1) pattern a1(v1,_)
	Results r7 = *new Results();
	r7.setClausePassed(true);
	r7.setNumOfSyn(2);
	r7.setFirstClauseSyn("a");
	r7.setSecondClauseSyn("v1");
	r7.addPairResult("1", "v");
	r7.addPairResult("2", "x");
	r7.addPairResult("1", "5");
	r7.addPairResult("2", "4");
	r7.addPairResult("9", "c");
	r7.addPairResult("2", "z");

	Results r8 = *new Results();
	r8.setClausePassed(true);
	r8.setNumOfSyn(2);
	r8.setFirstClauseSyn("a1");
	r8.setSecondClauseSyn("v1");
	r8.addPairResult("2", "v");
	r8.addPairResult("1", "x");
	r8.addPairResult("8", "w");
	r8.addPairResult("4", "v");
	r8.addPairResult("9", "z");
	
	r7.getIntersect(r8);

	CPPUNIT_ASSERT(r7.getPairResults().size() == 3);
	CPPUNIT_ASSERT(r8.getPairResults().size() == 4);

	// Select a such that Uses(a, v1) pattern a1(v,_)
	Results r9 = *new Results();
	r9.setClausePassed(true);
	r9.setNumOfSyn(2);
	r9.setFirstClauseSyn("a");
	r9.setSecondClauseSyn("v1");
	r9.addPairResult("1", "v");
	r9.addPairResult("2", "x");
	r9.addPairResult("1", "5");
	r9.addPairResult("2", "4");
	r9.addPairResult("9", "c");
	r9.addPairResult("2", "z");

	Results r10 = *new Results();
	r10.setClausePassed(true);
	r10.setNumOfSyn(2);
	r10.setFirstClauseSyn("a1");
	r10.setSecondClauseSyn("v");
	r10.addPairResult("2", "v");
	r10.addPairResult("1", "x");
	r10.addPairResult("8", "w");
	r10.addPairResult("4", "v");
	r10.addPairResult("9", "z");
	
	r9.getIntersect(r10);

	CPPUNIT_ASSERT(r9.getPairResults().size() == 6);
	CPPUNIT_ASSERT(r10.getPairResults().size() == 5);
}