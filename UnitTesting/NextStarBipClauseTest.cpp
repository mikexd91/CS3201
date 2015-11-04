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
	parser.parse("procedure proc1 { x = 2; call proc2; x = 3; } procedure proc2 { x = 2; y = 3; } procedure proc3 { y = 2; call proc2; y = 3; }");

	Result* result = new Result();
	SuchThatClauseBuilder* builder = new SuchThatClauseBuilder(NEXTSTARBIP_);
	builder->setArg(1, "6");
	builder->setArg(2, "8");
	builder->setArgType(1, ARG_STATEMENT);
	builder->setArgType(2, ARG_STATEMENT);
	builder->setArgFixed(1, true);
	builder->setArgFixed(2, true);
	NextStarBipClause* clause = (NextStarBipClause*) builder->build();

	CPPUNIT_ASSERT(clause->isValid());
	bool val = clause->evaluate(result);
	cout << val;
}