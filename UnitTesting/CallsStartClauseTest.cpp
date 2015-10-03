#include <cppunit/config/SourcePrefix.h>
#include "CallsStartClauseTest.h"
#include "../SPA/ProcTable.h"
#include "../SPA/Utils.h"
#include "../SPA/CallsStarClause.h"
#include "../SPA/SuchThatClauseBuilder.h"

using namespace stringconst;
using namespace std;

void CallsStarClauseTest::setUp() {
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
	proc3CallSet.insert(proc5);
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

void CallsStarClauseTest::tearDown() {
	ProcTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( CallsStarClauseTest );

void CallsStarClauseTest::testCallsStarFixedFixedPass() {
	Result* result = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("main");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("private2");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "main");
	callsStarBuilder->setArgFixed(1, true);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "private2");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool evalResult = c1->evaluate(result);
	CPPUNIT_ASSERT(evalResult);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void CallsStarClauseTest::testCallsStarFixedFixedFail() {
	Result* result = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("private1");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("main");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "private1");
	callsStarBuilder->setArgFixed(1, true);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "main");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool evalResult = c1->evaluate(result);
	CPPUNIT_ASSERT(!evalResult);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void CallsStarClauseTest::testCallsStarSynFixedPass() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("a");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("private2");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "a");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "private2");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res->isSynPresent("a"));
	CPPUNIT_ASSERT(res->getResultTableSize() == 4);
}

void CallsStarClauseTest::testCallsStarSynFixedFail() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("a");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("main");
	c1->setSecondArgFixed(true);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "a");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "main");
	callsStarBuilder->setArgFixed(2, true);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->getResultTableSize() == 0);
	CPPUNIT_ASSERT(res->isSynPresent("a") == false);
}

void CallsStarClauseTest::testCallsStarFixedSynPass() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("childProc2");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("b");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "childProc2");
	callsStarBuilder->setArgFixed(1, true);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "b");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->isSynPresent("b") == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 3);
}

void CallsStarClauseTest::testCallsStarFixedSynFail() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("private2");
	c1->setFirstArgFixed(true);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("b");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "private2");
	callsStarBuilder->setArgFixed(1, true);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "b");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->getResultTableSize() == 0);
	CPPUNIT_ASSERT(res->isSynPresent("b") == false);
}

void CallsStarClauseTest::testCallsStarFirstUnderscorePass() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("_");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_GENERIC);
	c1->setSecondArg("p");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "_");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_GENERIC);
	callsStarBuilder->setArg(2, "p");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 7);
	CPPUNIT_ASSERT(res->isSynPresent("p") == true);
}

void CallsStarClauseTest::testCallsStarSecondUnderscorePass() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("_");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_GENERIC);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "p");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "_");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_GENERIC);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 5);
	CPPUNIT_ASSERT(res->isSynPresent("p") == true);
}

void CallsStarClauseTest::testCallsStarBothUnderscorePass() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("_");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_GENERIC);
	c1->setSecondArg("_");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_GENERIC);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "_");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_GENERIC);
	callsStarBuilder->setArg(2, "_");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_GENERIC);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
}

void CallsStarClauseTest::testCallsStarSynSynPass() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("q");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "p");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "q");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == true);
	CPPUNIT_ASSERT(res->isSynPresent("p") == true);
	CPPUNIT_ASSERT(res->isSynPresent("q") == true);
	CPPUNIT_ASSERT(res->getResultTableSize() == 14);
}

void CallsStarClauseTest::testCallsStarSynSynSame() {
	Result* res = new Result();
	/*CallsStarClause* c1 = new CallsStarClause();
	c1->setFirstArg("p");
	c1->setFirstArgFixed(false);
	c1->setFirstArgType(ARG_PROCEDURE);
	c1->setSecondArg("p");
	c1->setSecondArgFixed(false);
	c1->setSecondArgType(ARG_PROCEDURE);*/
	SuchThatClauseBuilder* callsStarBuilder = new SuchThatClauseBuilder(CALLSSTAR_);
	callsStarBuilder->setArg(1, "p");
	callsStarBuilder->setArgFixed(1, false);
	callsStarBuilder->setArgType(1, ARG_PROCEDURE);
	callsStarBuilder->setArg(2, "p");
	callsStarBuilder->setArgFixed(2, false);
	callsStarBuilder->setArgType(2, ARG_PROCEDURE);
	CallsStarClause* c1 = (CallsStarClause*) callsStarBuilder->build();
	CPPUNIT_ASSERT(c1->isValid());

	bool result = c1->evaluate(res);
	CPPUNIT_ASSERT(result == false);
	CPPUNIT_ASSERT(res->isSynPresent("p") == false);
	CPPUNIT_ASSERT(res->getResultTableSize() == 0);
}