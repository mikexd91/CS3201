/*
 * NextStarClauseTest.cpp
 *
 *  Created on: 15 Oct, 2015
 *      Author: Leon
 */

#include <cppunit/config/SourcePrefix.h>
#include "NextStarClauseTest.h"
#include "../SPA/AssgGNode.h"
#include "../SPA/CallGNode.h"
#include "../SPA/CFG.h"
#include "../SPA/DummyGNode.h"
#include "../SPA/EndGNode.h"
#include "../SPA/IfGNode.h"
#include "../SPA/GNode.h"
#include "../SPA/NextStarClause.h"
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

void NextStarClauseTest::setUp() {
	/*
	procedure proc {
		i = 1;
		z = i;

		if i then {
			while z {
				y = 4;
			}
			a = 4;
		} else {
			call choc;
			x = 3;
		}
	}

	procedure choc {
		x = 3;
		while x {
			if y then {
				x = 4;
			} else {
				y = 3;
			}
		}
	}
	*/
	
	/* ---------- StmtTable setup -----------*/
	StmtTable* stmtTable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	int stmt1Next[] = {2};
	unordered_set<int> stmt1NextSet(stmt1Next, stmt1Next + 1);
	unordered_set<int> stmt1PrevSet;
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setNext(stmt1NextSet);
	stmt1->setPrev(stmt1PrevSet);
	stmtTable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	int stmt2Next[] = {3};
	int stmt2Prev[] = {1};
	unordered_set<int> stmt2NextSet(stmt2Next, stmt2Next + 1);
	unordered_set<int> stmt2PrevSet(stmt2Prev, stmt2Prev + 1);
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setNext(stmt2NextSet);
	stmt2->setPrev(stmt2PrevSet);
	stmtTable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	int stmt3Next[] = {4, 7};
	int stmt3Prev[] = {2};
	unordered_set<int> stmt3NextSet(stmt3Next, stmt3Next + 2);
	unordered_set<int> stmt3PrevSet(stmt3Prev, stmt3Prev + 1);
	stmt3->setStmtNum(3);
	stmt3->setType(IF_STMT_);
	stmt3->setNext(stmt3NextSet);
	stmt3->setPrev(stmt3PrevSet);
	stmtTable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	int stmt4Next[] = {5, 6};
	int stmt4Prev[] = {3, 5};
	unordered_set<int> stmt4NextSet(stmt4Next, stmt4Next + 2);
	unordered_set<int> stmt4PrevSet(stmt4Prev, stmt4Prev + 2);
	stmt4->setStmtNum(4);
	stmt4->setType(WHILE_STMT_);
	stmt4->setNext(stmt4NextSet);
	stmt4->setPrev(stmt4PrevSet);
	stmtTable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	int stmt5arr[] = {4};
	unordered_set<int> stmt5NextSet(stmt5arr, stmt5arr + 1);
	unordered_set<int> stmt5PrevSet(stmt5arr, stmt5arr + 1);
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setNext(stmt5NextSet);
	stmt5->setPrev(stmt5PrevSet);
	stmtTable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	int stmt6Prev[] = {4};
	unordered_set<int> stmt6PrevSet(stmt6Prev, stmt6Prev + 1);
	unordered_set<int> stmt6NextSet;
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setNext(stmt6NextSet);
	stmt6->setPrev(stmt6PrevSet);
	stmtTable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	int stmt7Next[] = {8};
	int stmt7Prev[] = {3};
	unordered_set<int> stmt7NextSet(stmt7Next, stmt7Next + 1);
	unordered_set<int> stmt7PrevSet(stmt7Prev, stmt7Prev + 1);
	stmt7->setStmtNum(7);
	stmt7->setType(CALL_STMT_);
	stmt7->setNext(stmt7NextSet);
	stmt7->setPrev(stmt7PrevSet);
	stmtTable->addStmt(stmt7);

	Statement* stmt8 = new Statement();
	int stmt8Prev[] = {7};
	unordered_set<int> stmt8NextSet;
	unordered_set<int> stmt8PrevSet(stmt8Prev, stmt8Prev + 1);
	stmt8->setStmtNum(8);
	stmt8->setType(ASSIGN_STMT_);
	stmt8->setNext(stmt8NextSet);
	stmt8->setPrev(stmt8PrevSet);
	stmtTable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	int stmt9Next[] = {10};
	unordered_set<int> stmt9NextSet(stmt9Next, stmt9Next + 1);
	unordered_set<int> stmt9PrevSet;
	stmt9->setStmtNum(9);
	stmt9->setType(ASSIGN_STMT_);
	stmt9->setNext(stmt9NextSet);
	stmt9->setPrev(stmt9PrevSet);
	stmtTable->addStmt(stmt9);

	Statement* stmt10 = new Statement();
	int stmt10Next[] = {11};
	int stmt10Prev[] = {9, 12, 13};
	unordered_set<int> stmt10NextSet(stmt10Next, stmt10Next + 1);
	unordered_set<int> stmt10PrevSet(stmt10Prev, stmt10Prev + 3);
	stmt10->setStmtNum(10);
	stmt10->setType(WHILE_STMT_);
	stmt10->setNext(stmt10NextSet);
	stmt10->setPrev(stmt10PrevSet);
	stmtTable->addStmt(stmt10);

	Statement* stmt11 = new Statement();
	int stmt11Next[] = {12, 13};
	int stmt11Prev[] = {10};
	unordered_set<int> stmt11NextSet(stmt11Next, stmt11Next + 2);
	unordered_set<int> stmt11PrevSet(stmt11Prev, stmt11Prev + 1);
	stmt11->setStmtNum(11);
	stmt11->setType(IF_STMT_);
	stmt11->setNext(stmt11NextSet);
	stmt11->setPrev(stmt11PrevSet);
	stmtTable->addStmt(stmt11);

	Statement* stmt12 = new Statement();
	Statement* stmt13 = new Statement();
	int stmtNext[] = {10};
	int stmtPrev[] = {11};
	unordered_set<int> stmtNextSet(stmtNext, stmtNext + 1);
	unordered_set<int> stmtPrevSet(stmtPrev, stmtPrev + 1);
	stmt12->setStmtNum(12);
	stmt12->setType(ASSIGN_STMT_);
	stmt12->setNext(stmtNextSet);
	stmt12->setPrev(stmtPrevSet);
	stmtTable->addStmt(stmt12);
	stmt13->setStmtNum(13);
	stmt13->setType(ASSIGN_STMT_);
	stmt13->setNext(stmtNextSet);
	stmt13->setPrev(stmtPrevSet);
	stmtTable->addStmt(stmt13);

	
	/* --------- ProcTable setup --------*/
	ProcTable* procTable = ProcTable::getInstance();
	
	Procedure* proc1 = new Procedure();
	int p1Contains[] = {1, 2, 3, 4, 5, 6, 7, 8};
	unordered_set<int> p1Set(p1Contains, p1Contains + 8);
	proc1->setProcName("proc");
	proc1->setContainStmts(p1Set);
	procTable->addProc(proc1);

	Procedure* proc2 = new Procedure();
	int p2Contains[] = {9, 10, 11, 12, 13};
	unordered_set<int> p2Set(p2Contains, p2Contains + 5);
	proc2->setProcName("choc");
	proc2->setContainStmts(p2Set);
	procTable->addProc(proc2);


	/* ----------- CFG Setup ------------*/
	CFG* cfg = CFG::getInstance();
	
	ProcGNode* proc1G = new ProcGNode("proc");
	ProcGNode* proc2G = new ProcGNode("choc");
	cfg->addProcedure(proc1G);
	cfg->addProcedure(proc2G);

	AssgGNode* s1 = new AssgGNode(1);
	s1->setEndStmt(2);
	proc1G->setFirstChild(s1);
	s1->setFirstParent(proc1G);

	AssgGNode* s9 = new AssgGNode(9);
	proc2G->setFirstChild(s9);
	s9->setFirstParent(proc2G);
}

void NextStarClauseTest::tearDown() {
	StmtTable::getInstance()->clearTable();
	ProcTable::getInstance()->clearTable();
	CFG::reset();
}

CPPUNIT_TEST_SUITE_REGISTRATION( NextStarClauseTest );

void NextStarClauseTest::testFixFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "13");
	builder->setArg(2, "10");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}
void NextStarClauseTest::testFixFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "4");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testGeneGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testGeneFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "3");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testGeneFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "17");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "13");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixGeneFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "100");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "11");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 4);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"10", "11", "12", "13"};
	unordered_set<string> compSet(compare, compare + 4);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testFixSynStmtFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "6");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "12");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"12", "13"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testFixSynAssgFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "8");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "2");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testFixSynCallFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "4");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "1");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"3"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testFixSynIfFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "5");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testFixSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "1");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"4"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testFixSynWhileFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "7");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testGeneSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"2", "3", "4", "5", "6", "7", "8", "10", "11", "12", "13"};
	unordered_set<string> compSet(compare, compare + 11);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testGeneSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"2", "5", "6", "8", "12", "13"};
	unordered_set<string> compSet(compare, compare + 6);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testGeneSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testGeneSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"3", "11"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testGeneSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "_");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"4", "10"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testSynStmtFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "5");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
	
	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"1", "2", "3", "4", "5"};
	unordered_set<string> compSet(compare, compare + 5);
	CPPUNIT_ASSERT(ansSet == compSet);
}

void NextStarClauseTest::testSynStmtFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "1");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynAssgFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "11");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 3);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"9", "12", "13"};
	unordered_set<string> compSet(compare, compare + 3);
	CPPUNIT_ASSERT(ansSet == compSet);
}

void NextStarClauseTest::testSynAssgFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "9");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynCallFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(ansSet == compSet);
}

void NextStarClauseTest::testSynCallFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "4");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynIfFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "11");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

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
	CPPUNIT_ASSERT(ansSet == compSet);
}

void NextStarClauseTest::testSynIfFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "3");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynWhileFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "6");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"4"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(ansSet == compSet);
}

void NextStarClauseTest::testSynWhileFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynStmtGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"1", "2", "3", "4", "5", "7", "9", "10", "11", "12", "13"};
	unordered_set<string> compSet(compare, compare + 11);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testSynAssgGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"1", "2", "5", "9", "12", "13"};
	unordered_set<string> compSet(compare, compare + 6);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testSynCallGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compSet(compare, compare + 1);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testSynIfGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"3", "11"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testSynWhileGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "_");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_GENERIC);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> ans = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.insert(s.at(0));
	}
	string compare[] = {"4", "10"};
	unordered_set<string> compSet(compare, compare + 2);
	CPPUNIT_ASSERT(compSet == ansSet);
}

void NextStarClauseTest::testSynStmtSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s1");
	builder->setArg(2, "s2");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 45);
}

void NextStarClauseTest::testSynStmtSynStmtSamePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);
}

void NextStarClauseTest::testSynStmtSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 25);
}

void NextStarClauseTest::testSynStmtSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 3);
}

void NextStarClauseTest::testSynStmtSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);
}

void NextStarClauseTest::testSynStmtSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "s");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 10);
}

void NextStarClauseTest::testSynAssgSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a1");
	builder->setArg(2, "a2");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 15);
}

void NextStarClauseTest::testSynAssgSynAssgSamePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 3);
}

void NextStarClauseTest::testSynAssgSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 28);
}

void NextStarClauseTest::testSynAssgSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarClauseTest::testSynAssgSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
}

void NextStarClauseTest::testSynAssgSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "a");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);
}

void NextStarClauseTest::testSynCallSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c1");
	builder->setArg(2, "c2");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynCallSynCallSamePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynCallSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarClauseTest::testSynCallSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarClauseTest::testSynCallSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynCallSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "c");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynIfSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if1");
	builder->setArg(2, "if2");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarClauseTest::testSynIfSynIfSamePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarClauseTest::testSynIfSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 9);
}

void NextStarClauseTest::testSynIfSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
}

void NextStarClauseTest::testSynIfSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextStarClauseTest::testSynIfSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "if");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarClauseTest::testSynWhileSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w1");
	builder->setArg(2, "w2");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarClauseTest::testSynWhileSynWhileSamePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "w");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_WHILE);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextStarClauseTest::testSynWhileSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 7);
}

void NextStarClauseTest::testSynWhileSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "a");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_ASSIGN);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 4);
}

void NextStarClauseTest::testSynWhileSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "c");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_CALL);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextStarClauseTest::testSynWhileSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTAR_);
	builder->setArg(1, "w");
	builder->setArg(2, "if");
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_IF);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	NextStarClause* clause = (NextStarClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}