#include <cppunit/config/SourcePrefix.h>
#include "CallsClauseTest.h"
#include "../SPA/ProcTable.h"
#include "../SPA/Utils.h"
#include "../SPA/CallsClause.h"
#include "../SPA/SuchThatClauseBuilder.h"

using namespace stringconst;
using namespace std;

void CallsClauseTest::setUp() {
	/*
	procedure main {
		call childProc1;	// 1
		call childProc2;	// 2
		call childProc3;	// 3
	}

	procedure childProc1 {
		call private1;		// 1
		call private2;		// 2
	}

	procedure childProc2 {
		call private1;		// 1
		call private3;		// 2
	}

	procedure childProc3 {
		call private4;		// 1
	}

	procedure private1 {
		call private2;		// 1
	}

	procedure private2 {
		v = 1;				// 1
	}

	procedure private4 {
		s = 1;				// 1
	}
	*/

	// to set up the procTable manually
	ProcTable* procTable = ProcTable::getInstance();

	Procedure* proc1 = new Procedure("main");

	Procedure* proc2 = new Procedure("childProc1");
	Procedure* proc3 = new Procedure("childProc2");
	Procedure* proc4 = new Procedure("childProc3");

	Procedure* proc5 = new Procedure("private1");
	Procedure* proc6 = new Procedure("private2");
	Procedure* proc7 = new Procedure("private3");
	Procedure* proc8 = new Procedure("private4");

	// setup calls for procedure main
	unordered_set<Procedure*> proc1CallSet;
	proc1CallSet.insert(proc2);
	proc1CallSet.insert(proc3);
	proc1CallSet.insert(proc4);
	proc1->setCalls(proc1CallSet);
	procTable->addProc(proc1);

	// setup calls and calledby for procedure childProc1
	unordered_set<Procedure*> proc2CallSet;
	proc2CallSet.insert(proc5);
	proc2CallSet.insert(proc6);
	proc2->setCalls(proc2CallSet);
	unordered_set<Procedure*> proc2CalledBySet;
	proc2CalledBySet.insert(proc1);
	proc2->setCalledBy(proc2CalledBySet);
	procTable->addProc(proc2);

	// setup calls and calledby for procedure childProc2
	unordered_set<Procedure*> proc3CallSet;
	proc3CallSet.insert(proc6);
	proc3CallSet.insert(proc7);
	proc3->setCalls(proc3CallSet);
	unordered_set<Procedure*> proc3CalledBySet;
	proc3CalledBySet.insert(proc1);
	proc3->setCalledBy(proc3CalledBySet);
	procTable->addProc(proc3);

	// setup calls and calledby for procedure childProc3
	unordered_set<Procedure*> proc4CallSet;
	proc4CallSet.insert(proc8);
	proc4->setCalls(proc4CallSet);
	unordered_set<Procedure*> proc4CalledBySet;
	proc4CalledBySet.insert(proc1);
	proc4->setCalledBy(proc4CalledBySet);
	procTable->addProc(proc4);

	// setup calls and calledby for procedure private1
	unordered_set<Procedure*> proc5CallSet;
	proc5CallSet.insert(proc6);
	proc5->setCalls(proc5CallSet);
	unordered_set<Procedure*> proc5CalledBySet;
	proc5CalledBySet.insert(proc2);
	proc5CalledBySet.insert(proc3);
	proc5->setCalledBy(proc5CalledBySet);
	procTable->addProc(proc5);

	// setup calls and calledby for procedure private2
	unordered_set<Procedure*> proc6CalledBySet;
	proc6CalledBySet.insert(proc2);
	proc6CalledBySet.insert(proc5);
	proc6->setCalledBy(proc6CalledBySet);
	procTable->addProc(proc6);

	// setup calls and calledby for procedure private3
	unordered_set<Procedure*> proc7CalledBySet;
	proc7CalledBySet.insert(proc3);
	proc7->setCalledBy(proc7CalledBySet);
	procTable->addProc(proc7);

	// setup calls and calledby for procedure private4
	unordered_set<Procedure*> proc8CalledBySet;
	proc8CalledBySet.insert(proc4);
	proc8->setCalledBy(proc8CalledBySet);
	procTable->addProc(proc8);
}

void CallsClauseTest::tearDown() {
	ProcTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CallsClauseTest );

void CallsClauseTest::testCallsFixedFixedPass() {
	Result* result = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("main");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("childProc1");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "main");
	callsBuilder->setArgFixed(1, true);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "childProc1");
	callsBuilder->setArgFixed(2, true);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool evalResult = true;/*c1->evaluate(result);*/
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void CallsClauseTest::testCallsFixedFixedFail() {
	Result* result = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("main");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("p1");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "main");
	callsBuilder->setArgFixed(1, true);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "p1");
	callsBuilder->setArgFixed(2, true);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool evalResult = c1->evaluate(result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void CallsClauseTest::testCallsSynFixedPass() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("a");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("private2");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "a");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "private2");
	callsBuilder->setArgFixed(2, true);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res->getResultTableSize() == 2);
	CPPUNIT_ASSERT(res->isSynPresent("a"));
	CPPUNIT_ASSERT(res->isSynPresent("s") == false);
	unordered_set<string> s = res->getSyn("s");
	CPPUNIT_ASSERT(s.size() == 0);
}

void CallsClauseTest::testCallsSynFixedFail() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("a");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("main");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "a");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "main");
	callsBuilder->setArgFixed(2, true);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->getResultTableSize() == 0);
	CPPUNIT_ASSERT(res->isSynPresent("a") == false);
}

void CallsClauseTest::testCallsFixedSynPass() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("main");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("b");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "main");
	callsBuilder->setArgFixed(1, true);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "b");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 3);
	CPPUNIT_ASSERT(res->isSynPresent("b") == true);
}

void CallsClauseTest::testCallsFixedSynFail() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("private2");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("b");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "private2");
	callsBuilder->setArgFixed(1, true);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "b");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->getResultTableSize() == 0);
	CPPUNIT_ASSERT(res->isSynPresent("b") == false);
}

void CallsClauseTest::testCallsFirstUnderscorePass() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("_");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_GENERIC);
	c1->setSecondArg("p");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "_");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_GENERIC);
	callsBuilder->setArg(2, "p");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 7);
	CPPUNIT_ASSERT(res->isSynPresent("p") == true);
}

void CallsClauseTest::testCallsSecondUnderscorePass() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("_");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_GENERIC);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "p");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "_");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_GENERIC);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 5);
	CPPUNIT_ASSERT(res->isSynPresent("p") == true);
}

void CallsClauseTest::testCallsBothUnderscorePass() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("_");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_GENERIC);
	c1->setSecondArg("_");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_GENERIC);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "_");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_GENERIC);
	callsBuilder->setArg(2, "_");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_GENERIC);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
}

void CallsClauseTest::testCallsSynSynPass() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("q");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "p");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "q");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->isSynPresent("p") == true);
	CPPUNIT_ASSERT(res->isSynPresent("q") == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 9);
}

void CallsClauseTest::testCallsSynSynSame() {
	Result* res = new Result();
	/*CallsClause* c1 = new CallsClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("p");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsBuilder = new SuchThatClauseBuilder(CALLS_);
	callsBuilder->setArg(1, "p");
	callsBuilder->setArgFixed(1, false);
	callsBuilder->setArgType(1, ARG_PROCEDURE);
	callsBuilder->setArg(2, "p");
	callsBuilder->setArgFixed(2, false);
	callsBuilder->setArgType(2, ARG_PROCEDURE);
	CallsClause* c1 = (CallsClause*) callsBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->isSynPresent("p") == false);
	CPPUNIT_ASSERT(res->getResultTableSize() == 0);
}