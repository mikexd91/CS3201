#include <cppunit/config/SourcePrefix.h>
#include "AffectsClauseTest.h"
#include "../SPA/AST.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/AssgGNode.h"
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
		x = z;
		while x {
			i = x;
			x = x + i;
		}
	}

	procedure hey {
		z = x;
		call test;
		if y then {
			z = i;
		} else {
			b = z;
			while x {
				a = b;
			}
		}
	}
	*/

	//build statement table and cfg
	//no follows before/after set since they are not needed
	StmtTable* stable = StmtTable::getInstance();

	//procedure

	//Build CFG

	ProcGNode* proc1 = new ProcGNode("test");
	AssgGNode* assg1 = new AssgGNode(1);
	proc1->addChild(assg1);
	WhileGNode* while2 = new WhileGNode(2);
	assg1->setChild(while2);
	while2->setFirstParent(assg1);
	AssgGNode* assg3 = new AssgGNode(3);
	assg3->setEndStmt(4);
	while2->setBeforeLoopChild(assg3);
	while2->setFirstParent(assg3);
	assg3->setFirstParent(while2);
	assg3->setChild(while2);
	EndGNode* end1 = new EndGNode();
	end1->setParent(while2);
	while2->setAfterLoopChild(end1);

	ProcGNode* proc2 = new ProcGNode("hey");
	AssgGNode* assg5 = new AssgGNode(5);
	proc2->setChild(assg5);
	CallGNode* call6 = new CallGNode(6);
	call6->setCallChild(proc1);
	assg5->setChild(call6);
	call6->setParent(assg5);
	IfGNode* if7 = new IfGNode(7);
	call6->setChild(if7);
	if7->setFirstParent(call6);
	call6->setChild(if7);
	AssgGNode* assg8 = new AssgGNode(8);
	if7->setThenChild(assg8);
	assg8->setFirstParent(if7);
	AssgGNode* assg9 = new AssgGNode(9);
	if7->setElseChild(assg9);
	assg9->setFirstParent(if7);
	WhileGNode* while10 = new WhileGNode(10);
	assg9->setChild(while10);
	while10->setFirstParent(assg9);
	AssgGNode* assg11 = new AssgGNode(11);
	while10->setBeforeLoopChild(assg11);
	assg11->setFirstParent(while10);
	assg11->setChild(while10);
	EndGNode* end2 = new EndGNode();
	while10->setAfterLoopChild(end2);

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
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(WHILE_STMT_);
	string modifiesArray2[] = {"i", "x"};
	unordered_set<string> mods2(modifiesArray2, modifiesArray2 + 2);
	stmt2->setModifies(mods2);
	string usesArray2[] = {"x"};
	unordered_set<string> uses2(usesArray2, usesArray2 + 1);
	stmt2->setUses(uses2);
	stmt2->setGNodeRef(while2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	string modifiesArray3[] = {"i"};
	unordered_set<string> mods3(modifiesArray3, modifiesArray3 + 1);
	stmt3->setModifies(mods3);
	string usesArray3[] = {"x"};
	unordered_set<string> uses3(usesArray3, usesArray3 + 1);
	stmt3->setUses(uses3);
	stmt3->setGNodeRef(assg3);
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	string modifiesArray4[] = {"x"};
	unordered_set<string> mods4(modifiesArray4, modifiesArray4 + 1);
	stmt4->setModifies(mods4);
	string usesArray4[] = {"x", "i"};
	unordered_set<string> uses4(usesArray4, usesArray4 + 2);
	stmt4->setUses(uses4);
	stmt4->setGNodeRef(assg3);
	stable->addStmt(stmt4);

	//next procedure
	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	string modifiesArray5[] = {"z"};
	unordered_set<string> mods5(modifiesArray5, modifiesArray5 + 1);
	stmt5->setModifies(mods5);
	string usesArray5[] = {"x"};
	unordered_set<string> uses5(usesArray5, usesArray5 + 1);
	stmt5->setUses(uses5);
	stmt5->setGNodeRef(assg5);
	stable->addStmt(stmt5);
	
	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(CALL_STMT_);
	string modifiesArray6[] = {"i", "x"};
	unordered_set<string> mods6(modifiesArray6, modifiesArray6 + 2);
	stmt5->setModifies(mods6);
	string usesArray6[] = {"x", "b", "z"};
	unordered_set<string> uses6(usesArray6, usesArray6 + 3);
	stmt6->setUses(uses6);
	stmt6->setGNodeRef(call6);
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(IF_STMT_);
	string modifiesArray7[] = {"z", "b"};
	unordered_set<string> mods7(modifiesArray7, modifiesArray7 + 2);
	stmt7->setModifies(mods7);
	string usesArray7[] = {"y", "i", "z"};
	unordered_set<string> uses7(usesArray7, usesArray7 + 3);
	stmt7->setUses(uses7);
	stmt7->setGNodeRef(if7);
	stable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(ASSIGN_STMT_);
	string modifiesArray8[] = {"z"};
	unordered_set<string> mods8(modifiesArray8, modifiesArray8 + 1);
	stmt8->setModifies(mods8);
	string usesArray8[] = {"i"};
	unordered_set<string> uses8(usesArray8, usesArray8 + 1);
	stmt8->setUses(uses8);
	stmt8->setGNodeRef(assg8);
	stable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	stmt9->setStmtNum(9);
	stmt9->setType(ASSIGN_STMT_);
	string modifiesArray9[] = {"b"};
	unordered_set<string> mods9(modifiesArray9, modifiesArray9 + 1);
	stmt9->setModifies(mods9);
	string usesArray9[] = {"z"};
	unordered_set<string> uses9(usesArray9, usesArray9 + 1);
	stmt9->setUses(uses9);
	stmt9->setGNodeRef(assg9);
	stable->addStmt(stmt9);

	Statement* stmt10 = new Statement();
	stmt10->setStmtNum(10);
	stmt10->setType(WHILE_STMT_);
	string modifiesArray10[] = {"a"};
	unordered_set<string> mods10(modifiesArray10, modifiesArray10 + 1);
	stmt10->setModifies(mods10);
	string usesArray10[] = {"x", "b"};
	unordered_set<string> uses10(usesArray10, usesArray10 + 1);
	stmt10->setUses(uses10);
	stmt10->setGNodeRef(while10);
	stable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	stmt11->setStmtNum(11);
	stmt11->setType(ASSIGN_STMT_);
	string modifiesArray11[] = {"a"};
	unordered_set<string> mods11(modifiesArray11, modifiesArray11 + 1);
	stmt11->setModifies(mods11);
	string usesArray11[] = {"b"};
	unordered_set<string> uses11(usesArray11, usesArray11 + 1);
	stmt11->setUses(uses11);
	stmt11->setGNodeRef(assg11);
	stable->addStmt(stmt11);
}

void 
AffectsClauseTest::tearDown() {
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( AffectsClauseTest );

void AffectsClauseTest::testSynSynFixedSameProc() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "3");
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

void AffectsClauseTest::testSynSynFixedInWhile() { 
	Result res = Result();
	SuchThatClauseBuilder* affectsBuilder = new SuchThatClauseBuilder(AFFECTS_);
	affectsBuilder->setArg(1, "1");
	affectsBuilder->setArgFixed(1, true);
	affectsBuilder->setArgType(1, ARG_PROGLINE);
	affectsBuilder->setArg(2, "3");
	affectsBuilder->setArgFixed(2, true);
	affectsBuilder->setArgType(2, ARG_PROGLINE);
	AffectsClause* m1 = (AffectsClause*) affectsBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}
