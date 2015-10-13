#include <cppunit/config/SourcePrefix.h>
#include "NextClauseTest.h"
#include "../SPA/NextClause.h"
#include "../SPA/Statement.h"
#include "../SPA/StmtTable.h"
#include "../SPA/SuchThatClauseBuilder.h"
#include "../SPA/Utils.h"
#include "boost/foreach.hpp"

#include <iostream>
#include <string>

using namespace stringconst;
using namespace std;
using namespace boost;

void NextClauseTest::setUp() {
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
}

void NextClauseTest::tearDown() {
	StmtTable::getInstance()->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( NextClauseTest );

void NextClauseTest::testFixFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* nextBuilder = new SuchThatClauseBuilder(NEXT_);
	nextBuilder->setArg(1, "3");
	nextBuilder->setArg(2, "7");
	nextBuilder->setArgFixed(1, true);
	nextBuilder->setArgFixed(2, true);
	nextBuilder->setArgType(1, ARG_STATEMENT);
	nextBuilder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) nextBuilder->build();
	
	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* nextBuilder = new SuchThatClauseBuilder(NEXT_);
	nextBuilder->setArg(1, "2");
	nextBuilder->setArg(2, "5");
	nextBuilder->setArgFixed(1, true);
	nextBuilder->setArgFixed(2, true);
	nextBuilder->setArgType(1, ARG_STATEMENT);
	nextBuilder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) nextBuilder->build();
	
	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testGeneGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "_");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();
	
	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testGeneFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "12");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testGeneFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "1");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "5");
	builder->setArg(2, "_");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixGeneFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "8");
	builder->setArg(2, "_");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "3");
	builder->setArg(2, "s");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
	
	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"4", "7"};
	unordered_set<string> compareSet(compare, compare + 2);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testFixSynStmtFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "6");
	builder->setArg(2, "s");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "10");
	builder->setArg(2, "if");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"11"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);

}

void NextClauseTest::testFixSynIfFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "4");
	builder->setArg(2, "if");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "9");
	builder->setArg(2, "w");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"10"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testFixSynWhileFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "7");
	builder->setArg(2, "w");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "11");
	builder->setArg(2, "a");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"12", "13"};
	unordered_set<string> compareSet(compare, compare + 2);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testFixSynAssgFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "3");
	builder->setArg(2, "a");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testFixSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "3");
	builder->setArg(2, "c");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testFixSynCallFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "13");
	builder->setArg(2, "c");
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testGeneSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "s");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"2", "3", "4", "5", "6", "7", "8", "10", "11", "12", "13"};
	unordered_set<string> compareSet(compare, compare + 11);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testGeneSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"3", "11"};
	unordered_set<string> compareSet(compare, compare + 2);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testGeneSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "w");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"4", "10"};
	unordered_set<string> compareSet(compare, compare + 2);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testGeneSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "c");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testGeneSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "_");
	builder->setArg(2, "a");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_GENERIC);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"2", "5", "6", "8", "12", "13"};
	unordered_set<string> compareSet(compare, compare + 6);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynStmtFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "5");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"4"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynStmtFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "9");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynIfFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "7");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"3"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynIfFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "8");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynWhileFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "11");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"10"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynWhileFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "9");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynAssgFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "2");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"1"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynAssgFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "6");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynCallFixPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "8");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynCallFixFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "13");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynStmtGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "_");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 11);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"1", "2", "3", "4", "5", "7", "9", "10", "11", "12", "13"};
	unordered_set<string> compareSet(compare, compare + 11);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynIfGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "_");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"3", "11"};
	unordered_set<string> compareSet(compare, compare + 2);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynWhileGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "_");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"4", "10"};
	unordered_set<string> compareSet(compare, compare + 2);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynAssgGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "_");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"1", "2", "5", "9", "12", "13"};
	unordered_set<string> compareSet(compare, compare + 6);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynCallGenePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "_");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_GENERIC);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);

	list<Row> answers = result->getResultTable().rows;
	unordered_set<string> ansSet;
	BOOST_FOREACH(auto i, answers) {
		ansSet.insert(i.at(0));
	}
	string compare[] = {"7"};
	unordered_set<string> compareSet(compare, compare + 1);
	CPPUNIT_ASSERT(compareSet == ansSet);
}

void NextClauseTest::testSynStmtSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s1");
	builder->setArg(2, "s2");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 14);
}

void NextClauseTest::testSynStmtSynStmtFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s1");
	builder->setArg(2, "s1");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynStmtSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextClauseTest::testSynStmtSynIfFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynStmtSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "w");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 5);
}

void NextClauseTest::testSynStmtSynWhileFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "w");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynStmtSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "a");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);
}

void NextClauseTest::testSynStmtSynAssgFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "a");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynStmtSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "s");
	builder->setArg(2, "c");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynStmtSynCallFail() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "c");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynIfSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynIfSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "s");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 4);
}

void NextClauseTest::testSynIfSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "w");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynIfSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "a");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextClauseTest::testSynIfSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "if");
	builder->setArg(2, "c");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_IF);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynWhileSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w1");
	builder->setArg(2, "w2");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynWhileSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "s");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 3);
}

void NextClauseTest::testSynWhileSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "c");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynWhileSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "a");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 2);
}

void NextClauseTest::testSynWhileSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "w");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_WHILE);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynCallSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c1");
	builder->setArg(2, "c2");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynCallSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "w");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynCallSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "s");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynCallSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynCallSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "c");
	builder->setArg(2, "a");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_CALL);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynAssgSynAssgPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a1");
	builder->setArg(2, "a2");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_ASSIGN);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynAssgSynStmtPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "s");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_STATEMENT);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 6);
}

void NextClauseTest::testSynAssgSynCallPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "c");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_CALL);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result) == false);
	CPPUNIT_ASSERT(result->getResultTableSize() == 0);
}

void NextClauseTest::testSynAssgSynIfPass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "if");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_IF);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 1);
}

void NextClauseTest::testSynAssgSynWhilePass() {
	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXT_);
	builder->setArg(1, "a");
	builder->setArg(2, "w");
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, false);
	builder->setArgType(1, ARG_ASSIGN);
	builder->setArgType(2, ARG_WHILE);
	NextClause* clause = (NextClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	CPPUNIT_ASSERT(clause->evaluate(result));
	CPPUNIT_ASSERT(result->getResultTableSize() == 4);
}