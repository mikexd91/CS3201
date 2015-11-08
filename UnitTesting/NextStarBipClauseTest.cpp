/*
 * NextStarBipClauseTest.cpp
 *
 *  Created on: 4 Nov, 2015
 *      Author: Leon
 */

#include <cppunit/config/SourcePrefix.h>

#include "NextStarBipClauseTest.h"
#include "../SPA/AssgGNode.h"
#include "../SPA/CallGNode.h"
#include "../SPA/CFGbip.h"
#include "../SPA/DummyGNode.h"
#include "../SPA/EndGNode.h"
#include "../SPA/IfGNode.h"
#include "../SPA/GNode.h"
#include "../SPA/NextStarBipClause.h"
#include "../SPA/ProcGNode.h"
#include "../SPA/ProcTable.h"
#include "../SPA/Statement.h"
#include "../SPA/StmtTable.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/Utils.h"
#include "../SPA/WhileGNode.h"
#include "boost/foreach.hpp"

#include <iostream>
#include <string>

using namespace stringconst;
using namespace std;
using namespace boost;

void NextStarBipClauseTest::setUp() {
	/*
	procedure proc1 {
	1	call proc2;
	2	x = 2;
	3	call proc2;
	}

	procedure proc2 {
	4	x = 2;
	5	if x then {
	6		x = 2;
	7		x = 2;
		} else {
	8		x = 2;
		}
	9	x = 2;
	10	x = 2;
	}

	procedure proc3 {
	11	while x {
	12		x = 3;
	13		call proc2;
		}
	}

	procedure proc4 {
	14	if x then {
	15		call proc3;
		} else {
	16		call proc2;
		}
	17	x = 2;
	}
	*/

	/* ------------- Procedure Setup ------------- */
	Procedure* proc1 = new Procedure("proc1");
	Procedure* proc2 = new Procedure("proc2");
	Procedure* proc3 = new Procedure("proc3");
	Procedure* proc4 = new Procedure("proc4");

	/* ------------- StmtTable setup ------------- */
	StmtTable* stmtTable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	int stmt1Next[] = {4};
	unordered_set<int> stmt1NextSet(stmt1Next, stmt1Next + 1);
	unordered_set<int> stmt1PrevSet;
	stmt1->setStmtNum(1);
	stmt1->setType(CALL_STMT_);
	stmt1->setNextBip(stmt1NextSet);
	stmt1->setPrevBip(stmt1PrevSet);
	stmt1->setProcedure(proc1);
	stmtTable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	int stmt2Next[] = {3};
	int stmt2Prev[] = {10};
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 1);
	unordered_set<int> stmt2PrevSet(stmt2Prev, stmt2Prev + 1);
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setNextBip(stmt2NextSet);
	stmt2->setPrevBip(stmt2PrevSet);
	stmt2->setProcedure(proc1);
	stmtTable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	int stmt3Next[] = {4};
	int stmt3Prev[] = {2};
	unordered_set<int> stmt3NextSet(stmt3Next, stmt3Next + 1);
	unordered_set<int> stmt3PrevSet(stmt3Prev, stmt3Prev + 1);
	stmt3->setStmtNum(3);
	stmt3->setType(CALL_STMT_);
	stmt3->setNextBip(stmt3NextSet);
	stmt3->setPrevBip(stmt3PrevSet);
	stmt3->setProcedure(proc1);
	stmtTable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	int stmt4Next[] = {5};
	int stmt4Prev[] = {1, 3, 13, 16};
	unordered_set<int> stmt4NextSet(stmt4Next, stmt4Next + 1);
	unordered_set<int> stmt4PrevSet(stmt4Prev, stmt4Prev + 4);
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	stmt4->setNextBip(stmt4NextSet);
	stmt4->setPrevBip(stmt4PrevSet);
	stmt4->setProcedure(proc2);
	stmtTable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	int stmt5Next[] = {6, 8};
	int stmt5Prev[] = {4};
	unordered_set<int> stmt5NextSet(stmt5Next, stmt5Next + 2);
	unordered_set<int> stmt5PrevSet(stmt5Prev, stmt5Prev + 1);
	stmt5->setStmtNum(5);
	stmt5->setType(IF_STMT_);
	stmt5->setNextBip(stmt5NextSet);
	stmt5->setPrevBip(stmt5PrevSet);
	stmt5->setProcedure(proc2);
	stmtTable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	int stmt6Next[] = {7};
	int stmt6Prev[] = {5};
	unordered_set<int> stmt6NextSet(stmt6Next, stmt6Next + 1);
	unordered_set<int> stmt6PrevSet(stmt6Prev, stmt6Prev + 1);
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setNextBip(stmt6NextSet);
	stmt6->setPrevBip(stmt6PrevSet);
	stmt6->setProcedure(proc2);
	stmtTable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	int stmt7Next[] = {9};
	int stmt7Prev[] = {6};
	unordered_set<int> stmt7NextSet(stmt7Next, stmt7Next + 1);
	unordered_set<int> stmt7PrevSet(stmt7Prev, stmt7Prev + 1);
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	stmt7->setNextBip(stmt7NextSet);
	stmt7->setPrevBip(stmt7PrevSet);
	stmt7->setProcedure(proc2);
	stmtTable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	int stmt8Next[] = {9};
	int stmt8Prev[] = {5};
	unordered_set<int> stmt8NextSet(stmt8Next, stmt8Next + 1);
	unordered_set<int> stmt8PrevSet(stmt8Prev, stmt8Prev + 1);
	stmt8->setStmtNum(8);
	stmt8->setType(ASSIGN_STMT_);
	stmt8->setNextBip(stmt8NextSet);
	stmt8->setPrevBip(stmt8PrevSet);
	stmt8->setProcedure(proc2);
	stmtTable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	int stmt9Next[] = {10};
	int stmt9Prev[] = {7, 8};
	unordered_set<int> stmt9NextSet(stmt9Next, stmt9Next + 1);
	unordered_set<int> stmt9PrevSet(stmt9Prev, stmt9Prev + 2);
	stmt9->setStmtNum(9);
	stmt9->setType(ASSIGN_STMT_);
	stmt9->setNextBip(stmt9NextSet);
	stmt9->setPrevBip(stmt9PrevSet);
	stmt9->setProcedure(proc2);
	stmtTable->addStmt(stmt9);

	Statement* stmt10 = new Statement();
	int stmt10Next[] = {2, 11, 17};
	int stmt10Prev[] = {9};
	unordered_set<int> stmt10NextSet(stmt10Next, stmt10Next + 3);
	unordered_set<int> stmt10PrevSet(stmt10Prev, stmt10Prev + 1);
	stmt10->setStmtNum(10);
	stmt10->setType(ASSIGN_STMT_);
	stmt10->setNextBip(stmt10NextSet);
	stmt10->setPrevBip(stmt10PrevSet);
	stmt10->setProcedure(proc2);
	stmtTable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	int stmt11Next[] = {12, 17};
	int stmt11Prev[] = {10, 15};
	unordered_set<int> stmt11NextSet(stmt11Next, stmt11Next + 2);
	unordered_set<int> stmt11PrevSet(stmt11Prev, stmt11Prev + 2);
	stmt11->setStmtNum(11);
	stmt11->setType(WHILE_STMT_);
	stmt11->setNextBip(stmt11NextSet);
	stmt11->setPrevBip(stmt11PrevSet);
	stmt11->setProcedure(proc3);
	stmtTable->addStmt(stmt11);

	Statement* stmt12 = new Statement();
	int stmt12Next[] = {13};
	int stmt12Prev[] = {11};
	unordered_set<int> stmt12NextSet(stmt12Next, stmt12Next + 1);
	unordered_set<int> stmt12PrevSet(stmt12Prev, stmt12Prev + 1);
	stmt12->setStmtNum(12);
	stmt12->setType(ASSIGN_STMT_);
	stmt12->setNextBip(stmt12NextSet);
	stmt12->setPrevBip(stmt12PrevSet);
	stmt12->setProcedure(proc3);
	stmtTable->addStmt(stmt12);

	Statement* stmt13 = new Statement();
	int stmt13Next[] = {4};
	int stmt13Prev[] = {12};
	unordered_set<int> stmt13NextSet(stmt13Next, stmt13Next + 1);
	unordered_set<int> stmt13PrevSet(stmt13Prev, stmt13Prev + 1);
	stmt13->setStmtNum(13);
	stmt13->setType(CALL_STMT_);
	stmt13->setNextBip(stmt13NextSet);
	stmt13->setPrevBip(stmt13PrevSet);
	stmt13->setProcedure(proc3);
	stmtTable->addStmt(stmt13);

	Statement* stmt14 = new Statement();
	int stmt14Next[] = {15, 16};
	unordered_set<int> stmt14NextSet(stmt14Next, stmt14Next + 2);
	unordered_set<int> stmt14PrevSet;
	stmt14->setStmtNum(14);
	stmt14->setType(IF_STMT_);
	stmt14->setNextBip(stmt14NextSet);
	stmt14->setPrevBip(stmt14PrevSet);
	stmt14->setProcedure(proc4);
	stmtTable->addStmt(stmt14);

	Statement* stmt15 = new Statement();
	int stmt15Next[] = {11};
	int stmt15Prev[] = {14};
	unordered_set<int> stmt15NextSet(stmt15Next, stmt15Next + 1);
	unordered_set<int> stmt15PrevSet(stmt15Prev, stmt15Prev + 1);
	stmt15->setStmtNum(15);
	stmt15->setType(CALL_STMT_);
	stmt15->setNextBip(stmt15NextSet);
	stmt15->setPrevBip(stmt15PrevSet);
	stmt15->setProcedure(proc4);
	stmtTable->addStmt(stmt15);

	Statement* stmt16 = new Statement();
	int stmt16Next[] = {4};
	int stmt16Prev[] = {14};
	unordered_set<int> stmt16NextSet(stmt16Next, stmt16Next + 1);
	unordered_set<int> stmt16PrevSet(stmt16Prev, stmt16Prev + 1);
	stmt16->setStmtNum(16);
	stmt16->setType(CALL_STMT_);
	stmt16->setNextBip(stmt16NextSet);
	stmt16->setPrevBip(stmt16PrevSet);
	stmt16->setProcedure(proc4);
	stmtTable->addStmt(stmt16);

	Statement* stmt17 = new Statement();
	int stmt17Prev[] = {10, 11};
	unordered_set<int> stmt17NextSet;
	unordered_set<int> stmt17PrevSet(stmt17Prev, stmt17Prev + 2);
	stmt17->setStmtNum(17);
	stmt17->setType(ASSIGN_STMT_);
	stmt17->setNextBip(stmt17NextSet);
	stmt17->setPrevBip(stmt17PrevSet);
	stmt17->setProcedure(proc4);
	stmtTable->addStmt(stmt17);


	/* ------------ CFGBip Setup ------------- */
	CFGbip* cfg = CFGbip::getInstance();

	ProcGNode* p1Node = new ProcGNode("proc1");
	ProcGNode* p2Node = new ProcGNode("proc2");
	ProcGNode* p3Node = new ProcGNode("proc3");
	ProcGNode* p4Node = new ProcGNode("proc4");

	cfg->addProcedure(p1Node);
	cfg->addProcedure(p2Node);
	cfg->addProcedure(p3Node);
	cfg->addProcedure(p4Node);

	CallGNode* node1 = new CallGNode(1);
	CallGNode* node3 = new CallGNode(3);
	CallGNode* node13 = new CallGNode(13);
	CallGNode* node15 = new CallGNode(15);
	CallGNode* node16 = new CallGNode(16);

	AssgGNode* node2 = new AssgGNode(2);
	AssgGNode* node4 = new AssgGNode(4);
	AssgGNode* node6 = new AssgGNode(6);
	node6->setEndStmt(7);
	AssgGNode* node8 = new AssgGNode(8);
	AssgGNode* node9 = new AssgGNode(9);
	node9->setEndStmt(10);
	AssgGNode* node12 = new AssgGNode(12);
	AssgGNode* node17 = new AssgGNode(17);

	WhileGNode* node11 = new WhileGNode(11);

	IfGNode* node5 = new IfGNode(5);
	IfGNode* node14 = new IfGNode(14);

	EndGNode* p1End = new EndGNode();
	EndGNode* p2End = new EndGNode();
	EndGNode* p3End = new EndGNode();
	EndGNode* p4End = new EndGNode();

	DummyGNode* dum1 = new DummyGNode();
	DummyGNode* dum2 = new DummyGNode();

	p1Node->setChild(node1);

	node1->setParent(p1Node);
	node1->setChild(p2Node);

	vector<GNode*> node2Parents;
	node2Parents.push_back(p2End);
	node2->setChild(node3);
	node2->setParents(node2Parents);
	
	node3->setParent(node2);
	node3->setChild(p2Node);

	p1End->setParent(p2End);

	p2Node->setChild(node4);

	vector<GNode*> node4Parents;
	node4Parents.push_back(p2Node);
	node4->setParents(node4Parents);
	node4->setChild(node5);

	vector<GNode*> node5Parents;
	node5Parents.push_back(node4);
	node5->setParents(node5Parents);
	node5->setThenChild(node6);
	node5->setElseChild(node8);

	vector<GNode*> node6Parents;
	node6Parents.push_back(node5);
	node6->setParents(node6Parents);
	node6->setChild(dum1);

	vector<GNode*> node8Parents;
	node8Parents.push_back(node5);
	node8->setParents(node8Parents);
	node8->setChild(dum1);

	vector<GNode*> dum1Children;
	dum1Children.push_back(node9);
	dum1->setIfParent(node6);
	dum1->setElseParent(node8);
	dum1->setChildren(dum1Children);

	vector<GNode*> node9Parents;
	node9Parents.push_back(dum1);
	node9->setParents(node9Parents);
	node9->setChild(p2End);

	p2End->setParent(node9);

	p3Node->setChild(node11);

	node11->setFirstParent(p3Node);
	node11->setSecondParent(p2End);
	node11->setFirstChild(node12);
	node11->setSecondChild(p3End);

	node12->setFirstParent(node11);
	node12->setChild(node13);

	node13->setFirstParent(node12);
	node13->setChild(p2Node);

	p3End->setFirstParent(node11);

	p4Node->setFirstChild(node14);

	node14->setFirstParent(p4Node);
	node14->setFirstChild(node15);
	node14->setSecondChild(node16);

	node15->setFirstParent(node14);
	node15->setChild(p3Node);

	node16->setFirstParent(node14);
	node16->setChild(p2Node);

	dum2->setFirstParent(p3End);
	dum2->setSecondParent(p2End);
	dum2->setFirstChild(node17);

	node17->setFirstParent(dum2);
	node17->setFirstChild(p4End);

	p4End->setFirstParent(node17);

	vector<GNode*> p2Parents;
	p2Parents.push_back(node1);
	p2Parents.push_back(node3);
	p2Parents.push_back(node13);
	p2Parents.push_back(node16);
	vector<GNode*> p2Children;
	p2Children.push_back(node2);
	p2Children.push_back(p1End);
	p2Children.push_back(node11);
	p2Children.push_back(dum2);
	p2Node->setParents(p2Parents);
	p2End->setChildren(p2Children);

	vector<GNode*> p3Parents;
	p3Parents.push_back(node15);
	vector<GNode*> p3Children;
	p3Children.push_back(dum2);
	p3Node->setParents(p3Parents);
	p3End->setChildren(p3Children);

	
	/* --------- link stmt ref ------------ */
	stmt1->setGBipNodeRef(node1);
	stmt2->setGBipNodeRef(node2);
	stmt3->setGBipNodeRef(node3);
	stmt4->setGBipNodeRef(node4);
	stmt5->setGBipNodeRef(node5);
	stmt6->setGBipNodeRef(node6);
	stmt7->setGBipNodeRef(node6);
	stmt8->setGBipNodeRef(node8);
	stmt9->setGBipNodeRef(node9);
	stmt10->setGBipNodeRef(node9);
	stmt11->setGBipNodeRef(node11);
	stmt12->setGBipNodeRef(node12);
	stmt13->setGBipNodeRef(node13);
	stmt14->setGBipNodeRef(node14);
	stmt15->setGBipNodeRef(node15);
	stmt16->setGBipNodeRef(node16);
	stmt17->setGBipNodeRef(node17);
}

void NextStarBipClauseTest::tearDown() {
	CFGbip::reset();
	StmtTable::getInstance()->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( NextStarBipClauseTest );

void NextStarBipClauseTest::testFixFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "7");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "1");
	builder->setArg(2, "11");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testGeneGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testGeneFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testGeneFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "14");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "3");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixGeneFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "17");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "11");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"12", "13", "4", "5", "6", "7", "8", "9", "10", "11", "17"};
	unordered_set<string> compSet(compare, compare + 11);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testFixSynStmtFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "17");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "2");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"4", "6", "7", "8", "9", "10"};
	unordered_set<string> compSet(compare, compare + 6);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testFixSynAssgFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "17");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "12");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"13"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testFixSynCallFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "3");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "4");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"5"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testFixSynIfFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "17");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testFixSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "8");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"11"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testFixSynWhileFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "16");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testGeneSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 15);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "15", "16", "17"};
	unordered_set<string> compSet(compare, compare + 15);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testGeneSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 9);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"2", "4", "6", "7", "8", "9", "10", "12", "17"};
	unordered_set<string> compSet(compare, compare + 9);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testGeneSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 4);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"3", "13", "15", "16",};
	unordered_set<string> compSet(compare, compare + 4);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testGeneSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"5"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testGeneSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "_");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"11"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynStmtFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "7");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 16);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
	unordered_set<string> compSet(compare, compare + 16);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynStmtFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "1");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testSynAssgFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "13");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"4", "6", "7", "8", "9", "10", "12"};
	unordered_set<string> compSet(compare, compare + 7);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynAssgFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "15");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testSynCallFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "17");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 3);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"13", "15", "16"};
	unordered_set<string> compSet(compare, compare + 3);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynCallFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "15");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testSynIfFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "17");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"5", "14"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynIfFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "14");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testSynWhileFixTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "5");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"11"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynWhileFixFalse() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "3");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarBipClauseTest::testSynStmtGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 16);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16"};
	unordered_set<string> compSet(compare, compare + 16);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynAssgGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 8);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"2", "4", "6", "7", "8", "9", "10", "12"};
	unordered_set<string> compSet(compare, compare + 8);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynCallGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"1", "3", "13", "15", "16"};
	unordered_set<string> compSet(compare, compare + 5);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynIfGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"5", "14"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynWhileGeneTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"11"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarBipClauseTest::testSynStmtSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s1");
	builder->setArg(2, "s2");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 180);
}

void NextStarBipClauseTest::testSynStmtSynStmtSameTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 10);
}

void NextStarBipClauseTest::testSynStmtSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 129);
}

void NextStarBipClauseTest::testSynStmtSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 23);
}

void NextStarBipClauseTest::testSynStmtSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 16);
}

void NextStarBipClauseTest::testSynStmtSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 12);
}

void NextStarBipClauseTest::testSynAssgSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a1");
	builder->setArg(2, "a2");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 68);
}

void NextStarBipClauseTest::testSynAssgSynAssgSameTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);
}

void NextStarBipClauseTest::testSynAssgSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 97);
}

void NextStarBipClauseTest::testSynAssgSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 14);
}

void NextStarBipClauseTest::testSynAssgSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 8);
}

void NextStarBipClauseTest::testSynAssgSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "a");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);
}

void NextStarBipClauseTest::testSynCallSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c1");
	builder->setArg(2, "c2");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 3);
}

void NextStarBipClauseTest::testSynCallSynCallSameTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarBipClauseTest::testSynCallSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 46);
}

void NextStarBipClauseTest::testSynCallSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 36);
}

void NextStarBipClauseTest::testSynCallSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
}

void NextStarBipClauseTest::testSynCallSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "c");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarBipClauseTest::testSynIfSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if1");
	builder->setArg(2, "if2");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarBipClauseTest::testSynIfSynIfSameTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarBipClauseTest::testSynIfSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 26);
}

void NextStarBipClauseTest::testSynIfSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 17);
}

void NextStarBipClauseTest::testSynIfSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
}

void NextStarBipClauseTest::testSynIfSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "if");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarBipClauseTest::testSynWhileSynWhileTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w1");
	builder->setArg(2, "w2");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarBipClauseTest::testSynWhileSynWhileSameTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarBipClauseTest::testSynWhileSynStmtTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);
}

void NextStarBipClauseTest::testSynWhileSynAssgTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 8);
}

void NextStarBipClauseTest::testSynWhileSynCallTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarBipClauseTest::testSynWhileSynIfTrue() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "w");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}
