#include <cppunit/config/SourcePrefix.h>
#include "AffectsStarClauseTest.h"
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
#include "../SPA/AffectsStarClause.h"
#include <boost/foreach.hpp>

#include <iostream>
#include <string>

using namespace stringconst;
using namespace std;
using namespace boost;

void AffectsStarClauseTest::setUp() {
	/*
	procedure test {
1		x = x;
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
16 		while d {
17			a = b;
18			b = c;
19			c = d;
20			d = a;
		}
	}
	*/

	//build statement table and cfg
	//no follows before/after set since they are not needed
	StmtTable* stable = StmtTable::getInstance();
	ProcTable* ptable = ProcTable::getInstance();
	CFG* cfg = CFG::getInstance();

	//Build CFG

	ProcGNode* proc1 = new ProcGNode("test");
	AssgGNode* assg1 = new AssgGNode(1);
	assg1->setEndStmt(2);
	assg1->setFirstParent(proc1);
	proc1->addChild(assg1);
	WhileGNode* while3 = new WhileGNode(3);
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
	while3->setSecondParent(assg4);

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
	assg13->setEndStmt(13);
	while12->setBeforeLoopChild(assg13);
	assg13->setFirstParent(while12);
	assg13->setChild(while12);
	while12->setSecondParent(assg13);
	DummyGNode* dummy1 = new DummyGNode();
	if9->setExit(dummy1);
	while12->setAfterLoopChild(dummy1);
	assg10->setChild(dummy1);
	dummy1->setElseParentStmt(10);
	dummy1->setIfParentStmt(12);
	dummy1->setEntrance(if9);
	dummy1->setFirstParent(assg10);
	dummy1->setSecondParent(while12);
	AssgGNode* assg14 = new AssgGNode(14);
	assg14->setEndStmt(15);
	assg14->setFirstParent(dummy1);
	dummy1->setFirstChild(assg14);
	WhileGNode* while16 = new WhileGNode(16);
	while16->setFirstParent(assg14);
	assg14->setFirstChild(while16);
	AssgGNode* assg17 = new AssgGNode(17); 
	assg17->setEndStmt(20);
	assg17->setFirstChild(while16);
	while16->setBeforeLoopChild(assg17);
	assg17->setFirstParent(while16);
	EndGNode* end2 = new EndGNode();
	end2->setParent(while16);
	while16->setAfterLoopChild(end2);
	while16->setSecondParent(assg17);
	cfg->addProcedure(proc1);
	cfg->addProcedure(proc2);

	
	//procedure
	Procedure* procedure1 = new Procedure();
	procedure1->setProcName("test");
	Procedure* procedure2 = new Procedure();
	procedure1->setProcName("hey");

	unordered_set<Procedure*> proc1CalledBy = unordered_set<Procedure*>();
	proc1CalledBy.insert(procedure2);
	procedure1->setCalledBy(proc1CalledBy);
	int proc1ContainingStmtsArr[] = {1, 2, 3, 4, 5};
	procedure1->setContainStmts(Procedure::ContainsStmtSet(proc1ContainingStmtsArr, proc1ContainingStmtsArr+ sizeof(proc1ContainingStmtsArr)/sizeof(*proc1ContainingStmtsArr)));

	unordered_set<Procedure*> proc2Calls = unordered_set<Procedure*>();
	proc2Calls.insert(procedure1);
	procedure2->setCalls(proc2Calls);
	int proc2ContainingStmtsArr[] = {6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
	procedure2->setContainStmts(Procedure::ContainsStmtSet(proc2ContainingStmtsArr, proc2ContainingStmtsArr+ sizeof(proc2ContainingStmtsArr)/sizeof(*proc2ContainingStmtsArr)));
	
	ptable->addProc(procedure1);
	ptable->addProc(procedure2);


	//only parent and children star of container stmts are set
	//Set statement table
	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	string modifiesArray1[] = {"x"};
	unordered_set<string> mods1(modifiesArray1, modifiesArray1 + 1);
	stmt1->setModifies(mods1);
	string usesArray1[] = {"x"};
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
	int childrenStarArray3[] = {4, 5};
	unordered_set<int> childrenStar3(childrenStarArray3, childrenStarArray3 + 2);
	stmt3->setChildrenStar(childrenStar3);
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
	int parentStarArray4[] = {3};
	unordered_set<int> parentStar4(parentStarArray4, parentStarArray4 + 1);
	stmt4->setParentStar(parentStar4);
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
	stmt5->setParentStar(parentStar4);
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
	int childrenStarArray9[] = {10, 11, 12, 13};
	unordered_set<int> childrenStar9(childrenStarArray9, childrenStarArray9 + 2);
	stmt9->setChildrenStar(childrenStar9);
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
	int parentStarArray12[] = {9};
	unordered_set<int> parentStar12(parentStarArray12, parentStarArray12 + 1);
	stmt12->setParentStar(parentStar12);
	int childrenStarArray12[] = {13};
	unordered_set<int> childrenStar12(childrenStarArray12, childrenStarArray12 + 1);
	stmt12->setChildrenStar(childrenStar12);
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

	Statement* stmt16 = new Statement();
	stmt16->setStmtNum(16);
	stmt16->setType(WHILE_STMT_);
	string modifiesArray16[] = {"a", "b", "c", "d"};
	unordered_set<string> mods16(modifiesArray16, modifiesArray16 + 4);
	stmt16->setModifies(mods16);
	string usesArray16[] = {"a", "b","c", "d"};
	unordered_set<string> uses16(usesArray16, usesArray16 + 4);
	stmt16->setUses(uses16);
	stmt16->setGNodeRef(while16);
	stmt16->setProcedure(procedure2);
	stable->addStmt(stmt16);

	Statement* stmt17 = new Statement();
	stmt17->setStmtNum(17);
	stmt17->setType(ASSIGN_STMT_);
	string modifiesArray17[] = {"a"};
	unordered_set<string> mods17(modifiesArray17, modifiesArray17 + 1);
	stmt17->setModifies(mods17);
	string usesArray17[] = {"b"};
	unordered_set<string> uses17(usesArray17, usesArray17 + 1);
	stmt17->setUses(uses17);
	stmt17->setGNodeRef(assg17);
	stmt17->setProcedure(procedure2);
	stable->addStmt(stmt17);

	Statement* stmt18 = new Statement();
	stmt18->setStmtNum(18);
	stmt18->setType(ASSIGN_STMT_);
	string modifiesArray18[] = {"b"};
	unordered_set<string> mods18(modifiesArray18, modifiesArray18 + 1);
	stmt18->setModifies(mods18);
	string usesArray18[] = {"c"};
	unordered_set<string> uses18(usesArray18, usesArray18 + 1);
	stmt18->setUses(uses18);
	stmt18->setGNodeRef(assg17);
	stmt18->setProcedure(procedure2);
	stable->addStmt(stmt18);

	Statement* stmt19 = new Statement();
	stmt19->setStmtNum(19);
	stmt19->setType(ASSIGN_STMT_);
	string modifiesArray19[] = {"c"};
	unordered_set<string> mods19(modifiesArray19, modifiesArray19 + 1);
	stmt19->setModifies(mods19);
	string usesArray19[] = {"d"};
	unordered_set<string> uses19(usesArray19, usesArray19 + 1);
	stmt19->setUses(uses19);
	stmt19->setGNodeRef(assg17);
	stmt19->setProcedure(procedure2);
	stable->addStmt(stmt19);

	Statement* stmt20 = new Statement();
	stmt20->setStmtNum(20);
	stmt20->setType(ASSIGN_STMT_);
	string modifiesArray20[] = {"d"};
	unordered_set<string> mods20(modifiesArray20, modifiesArray20 + 1);
	stmt20->setModifies(mods20);
	string usesArray20[] = {"a"};
	unordered_set<string> uses20(usesArray20, usesArray20 + 1);
	stmt20->setUses(uses20);
	stmt20->setGNodeRef(assg17);
	stmt20->setProcedure(procedure2);
	stable->addStmt(stmt20);
}

void AffectsStarClauseTest::tearDown() {
	StmtTable::getInstance()->clearTable();
	CFG::getInstance()->reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( AffectsStarClauseTest );

void AffectsStarClauseTest::testFixedFixedEasyPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "1");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "4");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testFixedFixedPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "6");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "13");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testFixedFixedPassInWhile() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "20");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "17");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testFixedFixedFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "7");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "15");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testFixedGenericPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "10");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testFixedGenericFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "7");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testGenericGenericPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "_");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_GENERIC);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testFixedSynPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "6");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "s");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	CPPUNIT_ASSERT(res.getResultTableSize() == 7);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 7);
	CPPUNIT_ASSERT(s.find("11") != s.end());
	CPPUNIT_ASSERT(s.find("13") != s.end());
	CPPUNIT_ASSERT(s.find("14") != s.end());
	CPPUNIT_ASSERT(s.find("17") != s.end());
	CPPUNIT_ASSERT(s.find("18") != s.end());
	CPPUNIT_ASSERT(s.find("19") != s.end());
	CPPUNIT_ASSERT(s.find("20") != s.end());
}

void AffectsStarClauseTest::testFixedSynFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "15");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "s");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(!m1->evaluate(&res));
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testSynSynPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "s1");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_STATEMENT);
	affectsBuilder->setArg(2, "s2");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s1"));
	CPPUNIT_ASSERT(res.isSynPresent("s2"));
	vector<string> syns;
	syns.push_back("s1");
	syns.push_back("s2");
	unordered_set<vector<string>> pairResults = res.getMultiSyn(syns);
	CPPUNIT_ASSERT(pairResults.size() == 33);
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
	string pair8String[] = {"6", "13"};
	vector<string> pair8(pair8String, pair8String+2);
	string pair9String[] = {"11", "17"};
	vector<string> pair9(pair9String, pair9String+2);
	string pair10String[] = {"17", "17"};
	vector<string> pair10(pair10String, pair10String+2);
	string pair11String[] = {"17", "18"};
	vector<string> pair11(pair11String, pair11String+2);
	string pair12String[] = {"17", "19"};
	vector<string> pair12(pair12String, pair12String+2);
	string pair13String[] = {"17", "20"};
	vector<string> pair13(pair13String, pair13String+2);
	string pair14String[] = {"18", "17"};
	vector<string> pair14(pair14String, pair14String+2);
	string pair15String[] = {"18", "18"};
	vector<string> pair15(pair15String, pair15String+2);
	string pair16String[] = {"18", "19"};
	vector<string> pair16(pair16String, pair16String+2);
	string pair17String[] = {"18", "20"};
	vector<string> pair17(pair17String, pair17String+2);
	string pair18String[] = {"19", "17"};
	vector<string> pair18(pair18String, pair18String+2);
	string pair19String[] = {"19", "18"};
	vector<string> pair19(pair19String, pair19String+2);
	string pair20String[] = {"19", "19"};
	vector<string> pair20(pair20String, pair20String+2);
	string pair21String[] = {"19", "20"};
	vector<string> pair21(pair21String, pair21String+2);
	string pair22String[] = {"20", "17"};
	vector<string> pair22(pair22String, pair22String+2);
	string pair23String[] = {"20", "18"};
	vector<string> pair23(pair23String, pair23String+2);
	string pair24String[] = {"20", "19"};
	vector<string> pair24(pair24String, pair24String+2);
	string pair25String[] = {"20", "20"};
	vector<string> pair25(pair25String, pair25String+2);
	string pair26String[] = {"6", "19"};
	vector<string> pair26(pair26String, pair26String+2);
	string pair27String[] = {"11", "19"};
	vector<string> pair27(pair27String, pair27String+2);
	string pair28String[] = {"6", "18"};
	vector<string> pair28(pair28String, pair28String+2);
	string pair29String[] = {"11", "18"};
	vector<string> pair29(pair29String, pair29String+2);
	string pair30String[] = {"6", "20"};
	vector<string> pair30(pair30String, pair30String+2);
	string pair31String[] = {"11", "20"};
	vector<string> pair31(pair31String, pair31String+2);
	string pair32String[] = {"6", "20"};
	vector<string> pair32(pair32String, pair32String+2);
	string pair33String[] = {"6", "17"};
	vector<string> pair33(pair33String, pair33String+2);
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair0) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair1) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair2) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair3) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair4) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair5) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair6) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair7) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair8) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair9) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair10) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair11) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair12) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair13) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair14) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair15) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair16) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair17) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair18) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair19) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair20) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair21) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair22) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair23) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair24) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair25) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair26) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair27) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair28) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair29) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair30) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair31) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair32) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair33) != pairResults.end());
}

void AffectsStarClauseTest::testSameSynSynPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "s");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_STATEMENT);
	affectsBuilder->setArg(2, "s");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	vector<string> syns;
	syns.push_back("s");
	syns.push_back("s");
	unordered_set<vector<string>> pairResults = res.getMultiSyn(syns);
	CPPUNIT_ASSERT(pairResults.size() == 5);
	string pair0String[] = {"5", "5"};
	vector<string> pair0(pair0String, pair0String+2);
	string pair1String[] = {"17", "17"};
	vector<string> pair1(pair1String, pair1String+2);
	string pair2String[] = {"18", "18"};
	vector<string> pair2(pair2String, pair2String+2);
	string pair3String[] = {"19", "19"};
	vector<string> pair3(pair3String, pair3String+2);
	string pair4String[] = {"20", "20"};
	vector<string> pair4(pair4String, pair4String+2);
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair0) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair1) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair2) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair3) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair4) != pairResults.end());
}

void AffectsStarClauseTest::testSynGenericPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "s");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_STATEMENT);
	affectsBuilder->setArg(2, "_");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_GENERIC);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	CPPUNIT_ASSERT(res.getResultTableSize() == 9);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 9);
	CPPUNIT_ASSERT(s.find("1") != s.end());
	CPPUNIT_ASSERT(s.find("5") != s.end());
	CPPUNIT_ASSERT(s.find("6") != s.end());
	CPPUNIT_ASSERT(s.find("10") != s.end());
	CPPUNIT_ASSERT(s.find("11") != s.end());
	CPPUNIT_ASSERT(s.find("17") != s.end());
	CPPUNIT_ASSERT(s.find("18") != s.end());
	CPPUNIT_ASSERT(s.find("19") != s.end());
	CPPUNIT_ASSERT(s.find("20") != s.end());
}

void AffectsStarClauseTest::testGenericSynPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "_");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_GENERIC);
	affectsBuilder->setArg(2, "s");
	affectsBuilder->setArgFixed(2, false);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	CPPUNIT_ASSERT(res.getResultTableSize() == 9);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 9);
	CPPUNIT_ASSERT(s.find("4") != s.end());
	CPPUNIT_ASSERT(s.find("5") != s.end());
	CPPUNIT_ASSERT(s.find("11") != s.end());
	CPPUNIT_ASSERT(s.find("13") != s.end());
	CPPUNIT_ASSERT(s.find("14") != s.end());
	CPPUNIT_ASSERT(s.find("17") != s.end());
	CPPUNIT_ASSERT(s.find("18") != s.end());
	CPPUNIT_ASSERT(s.find("19") != s.end());
	CPPUNIT_ASSERT(s.find("20") != s.end());
}

void AffectsStarClauseTest::testGenericFixedPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "_");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_GENERIC);
	affectsBuilder->setArg(2, "13");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void AffectsStarClauseTest::testGenericFixedFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "_");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_GENERIC);
	affectsBuilder->setArg(2, "1");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);

	Result res2 = Result();
	SuchThatClauseBuilder* affectsBuilder2 = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder2->setArg(1, "_");
	affectsBuilder2->setArgFixed(1, false);
	affectsBuilder2->setArgType(1, ARG_GENERIC);
	affectsBuilder2->setArg(2, "15");
	affectsBuilder2->setArgFixed(2, true);
	affectsBuilder2->setArgType(2, ARG_PROGLINE);
	AffectsStarClause* m2 = (AffectsStarClause*) affectsBuilder2->build();
	CPPUNIT_ASSERT(m2->isValid());

	bool result2 = m2->evaluate(&res2);
	CPPUNIT_ASSERT(!result2);
}

void AffectsStarClauseTest::testSynFixedPass() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "s");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_STATEMENT);
	affectsBuilder->setArg(2, "13");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	CPPUNIT_ASSERT(m1->evaluate(&res));
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	CPPUNIT_ASSERT(res.getResultTableSize() == 2);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 2);
	CPPUNIT_ASSERT(s.find("6") != s.end());
	CPPUNIT_ASSERT(s.find("11") != s.end());
	
	// affects*(s, 20) -> s = {6, 11, 17, 18, 19, 20}
	Result res2 = Result();
	SuchThatClauseBuilder* affectsBuilder2 = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder2->setArg(1, "s");
	affectsBuilder2->setArgFixed(1, false);
	affectsBuilder2->setArgType(1, ARG_STATEMENT);
	affectsBuilder2->setArg(2, "20");
	affectsBuilder2->setArgFixed(2, true);
	affectsBuilder2->setArgType(2, ARG_PROGLINE);
	SuchThatClause* m2 = affectsBuilder2->build();
	CPPUNIT_ASSERT(m2->isValid());

	CPPUNIT_ASSERT(m2->evaluate(&res2));
	CPPUNIT_ASSERT(res2.isSynPresent("s"));
	CPPUNIT_ASSERT(res2.getResultTableSize() == 6);
	unordered_set<string> s2 = res2.getSyn("s");
	CPPUNIT_ASSERT(s2.size() == 6);
	CPPUNIT_ASSERT(s2.find("6") != s2.end());
	CPPUNIT_ASSERT(s2.find("11") != s2.end());
	CPPUNIT_ASSERT(s2.find("17") != s2.end());
	CPPUNIT_ASSERT(s2.find("18") != s2.end());
	CPPUNIT_ASSERT(s2.find("19") != s2.end());
	CPPUNIT_ASSERT(s2.find("20") != s2.end());

	// affects(s, 4) -> s = {1, 5}
	Result res3 = Result();
	SuchThatClauseBuilder* affectsBuilder3 = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder3->setArg(1, "s");
	affectsBuilder3->setArgFixed(1, false);
	affectsBuilder3->setArgType(1, ARG_STATEMENT);
	affectsBuilder3->setArg(2, "4");
	affectsBuilder3->setArgFixed(2, true);
	affectsBuilder3->setArgType(2, ARG_PROGLINE);
	AffectsStarClause* m3 = (AffectsStarClause*) affectsBuilder3->build();
	CPPUNIT_ASSERT(m3->isValid());

	CPPUNIT_ASSERT(m3->evaluate(&res3));
	CPPUNIT_ASSERT(res3.isSynPresent("s"));
	CPPUNIT_ASSERT(res3.getResultTableSize() == 2);
	unordered_set<string> s3 = res3.getSyn("s");
	CPPUNIT_ASSERT(s3.size() == 2);
	CPPUNIT_ASSERT(s3.find("1") != s3.end());
	CPPUNIT_ASSERT(s3.find("5") != s3.end());
}

void AffectsStarClauseTest::testSynFixedFail() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder->setArg(1, "s");
	affectsBuilder->setArgFixed(1, false);
	affectsBuilder->setArgType(1, ARG_STATEMENT);
	affectsBuilder->setArg(2, "1");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_STATEMENT);
	AffectsStarClause* m1 = (AffectsStarClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);

	Result res2 = Result();
	SuchThatClauseBuilder* affectsBuilder2 = new SuchThatClauseBuilder(AFFECTSSTAR_);
	affectsBuilder2->setArg(1, "s");
	affectsBuilder2->setArgFixed(1, false);
	affectsBuilder2->setArgType(1, ARG_GENERIC);
	affectsBuilder2->setArg(2, "15");
	affectsBuilder2->setArgFixed(2, true);
	affectsBuilder2->setArgType(2, ARG_PROGLINE);
	SuchThatClause* m2 = affectsBuilder2->build();
	CPPUNIT_ASSERT(m2->isValid());

	bool result2 = m2->evaluate(&res2);
	CPPUNIT_ASSERT(!result2);
}