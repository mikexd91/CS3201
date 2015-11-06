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
#include "../SPA/Parser.h"
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


bool sortFunc(const vector<int> &p1, const vector<int> &p2) {
	return p1[0] < p2[0];
}

void NextStarBipClauseTest::setUp() {

}

void NextStarBipClauseTest::tearDown() {
	PDR::resetInstanceFlag();
	AST::reset();
	CFG::reset();
	CFGbip::reset();
	CFGBuilder::resetInstanceFlag();
	CFGbipBuilder::resetInstanceFlag();
	VarTable::reset();
	
	StmtTable* stmtTable = StmtTable::getInstance();
	stmtTable->clearTable();

	ConstTable* constTable = ConstTable::getInstance();
	constTable->clearTable();

	ProcTable* procTable = ProcTable::getInstance();
	procTable->clearTable();
}

CPPUNIT_TEST_SUITE_REGISTRATION( NextStarBipClauseTest );

void NextStarBipClauseTest::testFixFix() {
	Parser parser = Parser();
	parser.parse("procedure proc1 { x = 2; call proc2; x = 3; } procedure proc2 { x = 2; y = 3; } procedure proc3 { y = 2; call proc2; y = 3; }");

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
	bool val = clause->evaluate(result);
	cout << endl;
	if(val) {
		cout << "true";
	} else {
		cout << "false";
	}
}

void NextStarBipClauseTest::testSynFix() {
	Parser parser = Parser();
	parser.parse("procedure proc1 { x = 2; call proc2; x = 3; } procedure proc2 { x = 2; y = 3; } procedure proc3 { y = 2; call proc2; y = 3; }");

	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "7");
	builder->setArg(2, "s");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, false);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	bool val = clause->evaluate(result);
	cout << endl;
	cout << "Table size: " << result->getResultTableSize();
	cout << endl;
	if(val) {
		cout << "true";
	} else {
		cout << "false";
	}
	cout << endl;

	list<Row> ans = result->getResultTable().rows;
	vector<int> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.push_back(atoi(s.at(0).c_str()));
	}
	sort(ansSet.begin(), ansSet.end());

	cout << "Set:";
	BOOST_FOREACH(auto i, ansSet) {
		cout << " " << i;
	}
	cout << endl;
}

void NextStarBipClauseTest::testFixSyn() {
	Parser parser = Parser();
	parser.parse("procedure proc1 {x = 2;call proc2;x = 2;}procedure proc2 {x = 2;if x then {y = 2;} else {y = 3;}x = 2;}procedure proc3 {y = 3;call proc2;y = 3;}");

	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "s");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, false);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	bool val = clause->evaluate(result);
	cout << endl;
	cout << "Table size: " << result->getResultTableSize();
	cout << endl;
	if(val) {
		cout << "true";
	} else {
		cout << "false";
	}
	cout << endl;

	list<Row> ans = result->getResultTable().rows;
	vector<int> ansSet;
	BOOST_FOREACH(auto s, ans) {
		ansSet.push_back(atoi(s.at(0).c_str()));
	}
	sort(ansSet.begin(), ansSet.end());

	cout << "Set:";
	BOOST_FOREACH(auto i, ansSet) {
		cout << " " << i;
	}
	cout << endl;
}

void NextStarBipClauseTest::testSynSyn() {
	Parser parser = Parser();
	parser.parse("procedure proc1 { x = 2; while x { if x then { x = 2; x = 2; } else { x = 2; } } x = 3; }");

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
	bool val = clause->evaluate(result);
	cout << endl;
	cout << "Table size: " << result->getResultTableSize();
	cout << endl;
	if(val) {
		cout << "true";
	} else {
		cout << "false";
	}
	cout << endl;

	list<Row> ans = result->getResultTable().rows;
	vector<vector<int>> ansSet;
	BOOST_FOREACH(auto s, ans) {
		vector<int> pair;
		pair.push_back(atoi(s.at(0).c_str()));
		pair.push_back(atoi(s.at(1).c_str()));
		ansSet.push_back(pair);
	}
	sort(ansSet.begin(), ansSet.end());

	cout << "Set:";
	BOOST_FOREACH(auto i, ansSet) {
		cout << " (" << i.at(0) << ", " << i.at(1) << ")";
	}
}
