#include <cppunit/config/SourcePrefix.h>
#include "AffectsClauseTest.h"
#include "../SPA/AST.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/AssgGNode.h"
#include "../SPA/DummyGNode.h"
#include "../SPA/WhileGNode.h"
#include "../SPA/EndGNode.h"
#include "../SPA/CallGNode.h"
#include "../SPA/IfGNode.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/Utils.h"
#include "../SPA/AffectsClause.h"

#include <iostream>
#include <string>

using namespace stringconst;
using namespace std;

void 
AffectsClauseTest::setUp() {
	/*
	procedure test {
1		x = z;
2		c = d;
3		while x {
4			i = x;
5			x = x;
		}
	}

	procedure hey {
6		z = x;
7		c = x;
8		call test;
9		if y then {
10			z = i;
		} else {
11			b = z;
12			while x {
13				a = b;
			}
		}
14		y=z;
15		a = c;
	}
	*/

	//build statement table and cfg
	//no follows before/after set since they are not needed
	StmtTable* stable = StmtTable::getInstance();
	CFG* cfg = CFG::getInstance();

	//procedure

	//Build CFG

	ProcGNode* proc1 = new ProcGNode("test");
	AssgGNode* assg1 = new AssgGNode(1);
	assg1->setEndStmt(2);
	proc1->addChild(assg1);
	WhileGNode* while3 = new WhileGNode(3);
	while3->setStartStmt(4);
	while3->setEndStmt(5);
	assg1->setChild(while3);
	while3->setFirstParent(assg1);
	AssgGNode* assg4 = new AssgGNode(4);
	assg4->setEndStmt(5);
	while3->setBeforeLoopChild(assg4);
	while3->setFirstParent(assg1);
	assg4->setFirstParent(while3);
	assg4->setChild(while3);
	EndGNode* end1 = new EndGNode();
	end1->setParent(while3);
	while3->setAfterLoopChild(end1);

	ProcGNode* proc2 = new ProcGNode("hey");
	AssgGNode* assg6 = new AssgGNode(6);
	assg6->setEndStmt(7);
	proc2->setChild(assg6);
	assg6->setFirstParent(proc2);
	CallGNode* call8 = new CallGNode(8);
	call8->setCallChild(proc1);
	assg6->setChild(call8);
	call8->setParent(assg6);
	IfGNode* if9 = new IfGNode(9);
	call8->setChild(if9);
	if9->setFirstParent(call8);
	call8->setChild(if9);
	AssgGNode* assg10 = new AssgGNode(10);
	if9->setThenChild(assg10);
	assg10->setFirstParent(if9);
	AssgGNode* assg11 = new AssgGNode(11);
	if9->setElseChild(assg11);
	assg11->setFirstParent(if9);
	WhileGNode* while12 = new WhileGNode(12);
	assg11->setChild(while12);
	while12->setFirstParent(assg11);
	AssgGNode* assg13 = new AssgGNode(13);
	assg13->setEndStmt(14);
	while12->setBeforeLoopChild(assg13);
	while12->setStartStmt(11);
	while12->setEndStmt(11);
	assg13->setFirstParent(while12);
	assg13->setChild(while12);
	DummyGNode* dummy1 = new DummyGNode();
	if9->setExit(dummy1);
	while12->setAfterLoopChild(dummy1);
	assg10->setChild(dummy1);
	dummy1->setElseParentStmt(10);
	dummy1->setIfParentStmt(8);
	dummy1->setEntrance(if9);
	AssgGNode* assg14 = new AssgGNode(14);
	assg14->setEndStmt(15);
	assg14->setFirstParent(dummy1);
	dummy1->setFirstChild(assg14);
	EndGNode* end2 = new EndGNode();
	end2->setParent(assg14);
	assg14->setFirstChild(end2);
	cfg->addProcedure(proc1);
	cfg->addProcedure(proc2);


	Procedure* procedure1 = new Procedure();
	procedure1->setProcName("test");
	Procedure* procedure2 = new Procedure();
	procedure1->setProcName("hey");
	unordered_set<Procedure*> proc1CalledBy = unordered_set<Procedure*>();
	proc1CalledBy.insert(procedure2);
	procedure1->setCalledBy(proc1CalledBy);
	unordered_set<Procedure*> proc2Calls = unordered_set<Procedure*>();
	proc2Calls.insert(procedure1);
	procedure2->setCalls(proc2Calls);

	//Set statement table
	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	string modifiesArray1[] = {"x"};
	unordered_set<string> mods1(modifiesArray1, modifiesArray1 + 1);
	stmt1->setModifies(mods1);
	string usesArray1[] = {"z"};
	unordered_set<string> uses1(usesArray1, usesArray1 + 1);
	stmt1->setUses(uses1);
	stmt1->setGNodeRef(assg1);
	stmt1->setProcedure(procedure1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	string modifiesArray2[] = {"c"};
	unordered_set<string> mods2(modifiesArray2, modifiesArray2 + 1);
	stmt2->setModifies(mods2);
	string usesArray2[] = {"d"};
	unordered_set<string> uses2(usesArray2, usesArray2 + 1);
	stmt2->setUses(uses2);
	stmt2->setGNodeRef(assg1);
	stmt2->setProcedure(procedure1);
	stable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(WHILE_STMT_);
	string modifiesArray3[] = {"x", "i"};
	unordered_set<string> mods3(modifiesArray3, modifiesArray3 + 2);
	stmt3->setModifies(mods3);
	string usesArray3[] = {"x"};
	unordered_set<string> uses3(usesArray3, usesArray3 + 1);
	stmt3->setUses(uses3);
	stmt3->setGNodeRef(while3);
	stmt3->setProcedure(procedure1);
	stable->addStmt(stmt3);
	
	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	string modifiesArray4[] = {"i"};
	unordered_set<string> mods4(modifiesArray4, modifiesArray4 + 1);
	stmt4->setModifies(mods4);
	string usesArray4[] = {"x"};
	unordered_set<string> uses4(usesArray4, usesArray4 + 1);
	stmt4->setUses(uses4);
	stmt4->setGNodeRef(assg4);
	stmt4->setProcedure(procedure1);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	string modifiesArray5[] = {"x"};
	unordered_set<string> mods5(modifiesArray5, modifiesArray5 + 1);
	stmt5->setModifies(mods5);
	string usesArray5[] = {"x"};
	unordered_set<string> uses5(usesArray5, usesArray5 + 1);
	stmt5->setUses(uses5);
	stmt5->setGNodeRef(assg4);
	stmt5->setProcedure(procedure1);
	stable->addStmt(stmt5);

	//next procedure
	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	//string modifiesArray5[] = {"z"};
	unordered_set<string> mods6;//(modifiesArray5, modifiesArray5 + 1);
	mods6.insert("z");
	stmt6->setModifies(mods6);
	string usesArray6[] = {"x"};
	unordered_set<string> uses6(usesArray6, usesArray6 + 1);
	stmt6->setUses(uses6);
	stmt6->setGNodeRef(assg6);
	stmt6->setProcedure(procedure2);
	stable->addStmt(stmt6);
	
	//to add stmt7
	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	string modifiesArray7[] = {"c"};
	unordered_set<string> mods7(modifiesArray7, modifiesArray7 + 1);
	stmt7->setModifies(mods7);
	string usesArray7[] = {"x"};
	unordered_set<string> uses7(usesArray7, usesArray7 + 1);
	stmt7->setUses(uses7);
	stmt7->setGNodeRef(assg6);
	stmt7->setProcedure(procedure2);
	stable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(CALL_STMT_);
	string modifiesArray8[] = {"i", "x", "c"};
	unordered_set<string> mods8(modifiesArray8, modifiesArray8 + 3);
	stmt8->setModifies(mods8);
	string usesArray8[] = {"x", "b", "z"};
	unordered_set<string> uses8(usesArray8, usesArray8 + 3);
	stmt8->setUses(uses8);
	stmt8->setGNodeRef(call8);
	stmt8->setProcedure(procedure2);
	stable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	stmt9->setStmtNum(9);
	stmt9->setType(IF_STMT_);
	string modifiesArray9[] = {"z", "b", "a"};
	unordered_set<string> mods9(modifiesArray9, modifiesArray9 + 3);
	stmt9->setModifies(mods9);
	string usesArray9[] = {"y", "i", "z", "b"};
	unordered_set<string> uses9(usesArray9, usesArray9 + 4);
	stmt9->setUses(uses9);
	stmt9->setGNodeRef(if9);
	stmt9->setProcedure(procedure2);
	stable->addStmt(stmt9);

	Statement* stmt10 = new Statement();
	stmt10->setStmtNum(10);
	stmt10->setType(ASSIGN_STMT_);
	string modifiesArray10[] = {"z"};
	unordered_set<string> mods10(modifiesArray10, modifiesArray10 + 1);
	stmt10->setModifies(mods10);
	string usesArray10[] = {"i"};
	unordered_set<string> uses10(usesArray10, usesArray10 + 1);
	stmt10->setUses(uses10);
	stmt10->setGNodeRef(assg10);
	stmt10->setProcedure(procedure2);
	stable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	stmt11->setStmtNum(11);
	stmt11->setType(ASSIGN_STMT_);
	string modifiesArray11[] = {"b"};
	unordered_set<string> mods11(modifiesArray11, modifiesArray11 + 1);
	stmt11->setModifies(mods11);
	string usesArray11[] = {"z"};
	unordered_set<string> uses11(usesArray11, usesArray11 + 1);
	stmt11->setUses(uses11);
	stmt11->setGNodeRef(assg11);
	stmt11->setProcedure(procedure2);
	stable->addStmt(stmt11);

	Statement* stmt12 = new Statement();
	stmt12->setStmtNum(12);
	stmt12->setType(WHILE_STMT_);
	string modifiesArray12[] = {"a"};
	unordered_set<string> mods12(modifiesArray12, modifiesArray12 + 1);
	stmt12->setModifies(mods12);
	string usesArray12[] = {"x", "b"};
	unordered_set<string> uses12(usesArray12, usesArray12 + 2);
	stmt12->setUses(uses12);
	stmt12->setGNodeRef(while12);
	stmt12->setProcedure(procedure2);
	stable->addStmt(stmt12);

	Statement* stmt13 = new Statement();
	stmt13->setStmtNum(13);
	stmt13->setType(ASSIGN_STMT_);
	string modifiesArray13[] = {"a"};
	unordered_set<string> mods13(modifiesArray13, modifiesArray13 + 1);
	stmt13->setModifies(mods13);
	string usesArray13[] = {"b"};
	unordered_set<string> uses13(usesArray13, usesArray13 + 1);
	stmt13->setUses(uses13);
	stmt13->setGNodeRef(assg13);
	stmt13->setProcedure(procedure2);
	stable->addStmt(stmt13);

	Statement* stmt14 = new Statement();
	stmt14->setStmtNum(14);
	stmt14->setType(ASSIGN_STMT_);
	string modifiesArray14[] = {"y"};
	unordered_set<string> mods14(modifiesArray14, modifiesArray14 + 1);
	stmt14->setModifies(mods14);
	string usesArray14[] = {"z"};
	unordered_set<string> uses14(usesArray14, usesArray14 + 1);
	stmt14->setUses(uses14);
	stmt14->setGNodeRef(assg14);
	stmt14->setProcedure(procedure2);
	stable->addStmt(stmt14);

	Statement* stmt15 = new Statement();
	stmt15->setStmtNum(15);
	stmt15->setType(ASSIGN_STMT_);
	string modifiesArray15[] = {"a"};
	unordered_set<string> mods15(modifiesArray15, modifiesArray15 + 1);
	stmt15->setModifies(mods15);
	string usesArray15[] = {"c"};
	unordered_set<string> uses15(usesArray15, usesArray15 + 1);
	stmt15->setUses(uses15);
	stmt15->setGNodeRef(assg14);
	stmt15->setProcedure(procedure2);
	stable->addStmt(stmt15);
}

void 
AffectsClauseTest::tearDown() {
	StmtTable::getInstance()->clearTable();
	CFG::getInstance()->reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( AffectsClauseTest );

void AffectsClauseTest::testFixedFixedSameProc() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "1");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "4");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsClauseTest::testFixedFixedInWhile() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "5");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "5");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsClauseTest::testFixedFixedFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "1");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "2");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
}

void AffectsClauseTest::testFixedFixedCallFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "7");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "15");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
}

void AffectsClauseTest::testFixedFixedIfPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "6");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "11");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
}

void AffectsClauseTest::testFixedFixedOutsideContainerPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "6");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "14");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
}

void AffectsClauseTest::testFixedGenericPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "6");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
}

void AffectsClauseTest::testFixedGenericFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "4");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
}

void AffectsClauseTest::testFixedSynPassInWhile() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "5");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "s");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	CPPUNIT_ASSERT(res.getResultTableSize() == 2);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 2);
	CPPUNIT_ASSERT(s.find("4") != s.end());
	CPPUNIT_ASSERT(s.find("5") != s.end());
}

void AffectsClauseTest::testFixedSynPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "6");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "s");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	CPPUNIT_ASSERT(res.getResultTableSize() == 2);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 2);
	CPPUNIT_ASSERT(s.find("11") != s.end());
	CPPUNIT_ASSERT(s.find("14") != s.end());
}

void AffectsClauseTest::testSynSynPass() { 
	//need to wait for pointer from if to dummy node
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "s1");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_STATEMENT);
	affectsBuilder->setArg(2, "s2");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s1"));
	CPPUNIT_ASSERT(res.isSynPresent("s2"));
	vector<string> syns;
	syns.push_back("s1");
	syns.push_back("s2");
	unordered_set<vector<string>> pairResults = res.getMultiSyn(syns);
	CPPUNIT_ASSERT(pairResults.size() == 8);
	string pair0String[] = {"1", "4"};
	vector<string> pair0(pair0String, pair0String+2);
	string pair1String[] = {"5", "4"};
	vector<string> pair1(pair1String, pair1String+2);
	string pair2String[] = {"5", "5"};
	vector<string> pair2(pair2String, pair2String+2);
	string pair3String[] = {"6", "11"};
	vector<string> pair3(pair3String, pair3String+2);
	string pair4String[] = {"11", "13"};
	vector<string> pair4(pair4String, pair4String+2);
	string pair5String[] = {"6", "14"};
	vector<string> pair5(pair5String, pair5String+2);
	string pair6String[] = {"10", "14"};
	vector<string> pair6(pair6String, pair6String+2);
	string pair7String[] = {"1", "5"};
	vector<string> pair7(pair7String, pair7String+2);
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair0) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair1) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair2) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair3) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair4) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair5) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair6) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair7) != pairResults.end());
}