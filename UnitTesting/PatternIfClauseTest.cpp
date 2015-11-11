#include <cppunit/config/SourcePrefix.h>
#include "PatternIfClauseTest.h"
#include "../SPA/PatternIfClause.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/IfNode.h"
#include "boost/foreach.hpp"

#include <iostream>
#include <string>
#include "../SPA/PatternClauseBuilder.h"

using namespace std;
using namespace stringconst;

void PatternIfClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		if x then {		//1
			if y then {	//2
			} else {
			}
		} else {
		}
	}
	pattern if("x", "_", "_") == 1;
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("zumba");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	IfNode* if1 = new IfNode(1);
	VarNode* x1 = new VarNode("x");
	if1->linkVarNode(x1);
	StmtLstNode* thensl = new StmtLstNode();
	if1->linkThenStmtLstNode(thensl);
	StmtLstNode* elsesl = new StmtLstNode();
	if1->linkElseStmtLstNode(elsesl);

	IfNode* if2 = new IfNode(2);
	VarNode* y1 = new VarNode("y");
	if2->linkVarNode(y1);
	StmtLstNode* thensl2 = new StmtLstNode();
	if2->linkThenStmtLstNode(thensl2);
	StmtLstNode* elsesl2 = new StmtLstNode();
	if2->linkElseStmtLstNode(elsesl2);

	procsl->linkStmtNode(if1);
	thensl->linkStmtNode(if2);
	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(IF_STMT_);
	stmt1->setTNodeRef(if1);
	string xvar = "x";
	unordered_set<string> uses1 = unordered_set<string>();
	uses1.emplace(xvar);
	stmt1->setUses(uses1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(IF_STMT_);
	stmt2->setTNodeRef(if2);
	string yvar = "y";
	unordered_set<string> uses2 = unordered_set<string>();
	uses2.emplace(yvar);
	stmt2->setUses(uses2);
	stable->addStmt(stmt2);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vx = new Variable("x");
	vx->addUsingStmt(1);
	vx->addTNode(x1);
	vtable->addVariable(vx);

	Variable* vy = new Variable("y");
	vy->addUsingStmt(1);
	vy->addUsingStmt(2);
	vy->addTNode(y1);
	vtable->addVariable(vy);

}

void PatternIfClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternIfClauseTest );

void PatternIfClauseTest::evaluateVarWild() {
	//cout << "varwild";
	string syn1 = "if";
	/*PatternIfClause* p1 = new PatternIfClause(syn1, "_", "_", "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_GENERIC);*/
	PatternClauseBuilder* ifBuilder = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder->setSynonym(syn1);
	ifBuilder->setVar("_");
	ifBuilder->setVarType(ARG_GENERIC);
	ifBuilder->setVarFixed(false);
	ifBuilder->setExpr(1, "_");
	ifBuilder->setExpr(2, "_");
	PatternIfClause* p1 = (PatternIfClause*) ifBuilder->build();
	
	CPPUNIT_ASSERT(p1->isValid());
	Result *r1 = new Result();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->isSynPresent(syn1));
	CPPUNIT_ASSERT(r1->getSyn(syn1).size() == 2);

	return;
}

void PatternIfClauseTest::evaluateVarFixed() {
	//cout << "varfixed";
	// pass pattern if("x", "_", "_");
	string syn1 = "if";
	/*PatternIfClause* p1 = new PatternIfClause(syn1, "x", "_", "_");
	p1->setVarFixed(true);
	p1->setVarType(stringconst::ARG_VARIABLE);*/
	PatternClauseBuilder* ifBuilder = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder->setSynonym(syn1);
	ifBuilder->setVar("x");
	ifBuilder->setVarType(ARG_VARIABLE);
	ifBuilder->setVarFixed(true);
	ifBuilder->setExpr(1, "_");
	ifBuilder->setExpr(2, "_");
	PatternIfClause* p1 = (PatternIfClause*) ifBuilder->build();

	CPPUNIT_ASSERT(p1->isValid());
	Result *r1 = new Result();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->isSynPresent(syn1));
	CPPUNIT_ASSERT(r1->getSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(r1->getSyn(syn1).count("1") == 1);

	// pass pattern if("y", "_", "_");
	/*PatternIfClause* p1 = new PatternIfClause(syn1, "y", "_", "_");
	p1->setVarFixed(true);
	p1->setVarType(stringconst::ARG_VARIABLE);*/
	PatternClauseBuilder* ifBuilder2 = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder2->setSynonym(syn1);
	ifBuilder2->setVar("y");
	ifBuilder2->setVarType(ARG_VARIABLE);
	ifBuilder2->setVarFixed(true);
	ifBuilder2->setExpr(1, "_");
	ifBuilder2->setExpr(2, "_");
	PatternIfClause* p2 = (PatternIfClause*) ifBuilder2->build();

	CPPUNIT_ASSERT(p2->isValid());
	Result *r2 = new Result();
	CPPUNIT_ASSERT(p2->evaluate(r2));

	CPPUNIT_ASSERT(r2->isSynPresent(syn1));
	CPPUNIT_ASSERT(r2->getSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(r2->getSyn(syn1).count("2") == 1);

	// var fail, not the control var
	/*PatternIfClause* p2 = new PatternIfClause(syn1, "z", "_", "_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(true);*/
	PatternClauseBuilder* ifBuilder3 = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder3->setSynonym(syn1);
	ifBuilder3->setVar("z");
	ifBuilder3->setVarType(ARG_VARIABLE);
	ifBuilder3->setVarFixed(true);
	ifBuilder3->setExpr(1, "_");
	ifBuilder3->setExpr(2, "_");
	PatternIfClause* p3 = (PatternIfClause*) ifBuilder3->build();
	CPPUNIT_ASSERT(p3->isValid());

	Result* resFail = new Result();
	CPPUNIT_ASSERT(!p3->evaluate(resFail));
	
	return;
}

void PatternIfClauseTest::evaluateVarSyn() {
	//cout << "varsyn";
	// pass pattern if(x, "_", "_");
	string syn1 = "if";
	string syn2 = "x";
	/*PatternIfClause* p1 = new PatternIfClause(syn1, syn2, "_", "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_VARIABLE);*/
	PatternClauseBuilder* ifBuilder = new PatternClauseBuilder(PATTERNIF_);
	ifBuilder->setSynonym(syn1);
	ifBuilder->setVar(syn2);
	ifBuilder->setVarType(ARG_VARIABLE);
	ifBuilder->setVarFixed(false);
	ifBuilder->setExpr(1, "_");
	ifBuilder->setExpr(2, "_");
	PatternIfClause* p1 = (PatternIfClause*) ifBuilder->build();

	CPPUNIT_ASSERT(p1->isValid());
	Result *r1 = new Result();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->isSynPresent(syn1));
	CPPUNIT_ASSERT(r1->isSynPresent(syn2));
	CPPUNIT_ASSERT(r1->getSyn(syn1).size() == 2);
	CPPUNIT_ASSERT(r1->getSyn(syn1).count("1") == 1);
	CPPUNIT_ASSERT(r1->getSyn(syn1).count("2") == 1);
	// HOW TO CHECK THE PAIR RESULTS
	// 1. make unordered set of the syns you want to check
	// 2. select them as resultstable and see size
	vector<string> synList = vector<string>();
	synList.push_back(syn1);
	synList.push_back(syn2);
	unordered_set<vector<string>> multiSynResults = r1->getMultiSyn(synList);
	CPPUNIT_ASSERT(multiSynResults.size() == 2);
	
	return;
}