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
	parser.parse("procedure proc1 { a = (b + c) * d * 5; b = a * d + 3 * c; call proc2; while c { d = c * c + c; if b then { e = f + g - h; while f { g = h * (a - d); } } else { e = f - g - h; while f { g = h * (b - a); } call proc3; } while a { b = (c + d) * 2; c = (d - e) * 69; d = e * f; f = (a + e) * 55; if a then { b = c * a * (d + e); while b { c = d * e; } } else { b = (c - a) * (h + g); } } } } procedure proc2 { what = (how + why) * 9001; how = (why - what) * 69; why = how * what; while how { if why then { call proc3; } else { how = (how + how) * (why + why) * (what + what); } } } procedure proc3 { mouse = (keyboard - button) * speakers; ram = cpu - gpu; if mouse then { if keyboard then { if speakers then { while ram { cpu = money * (parts + gpu); } } else { keyboard = parts - (money - gpu); } } else { mouse = (money - cpu) * gpu; } } else { iranoutofnames = mouse * (keyboard + gpu) * cpu; } }");

	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "7");
	builder->setArg(2, "24");
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