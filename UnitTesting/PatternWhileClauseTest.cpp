#include <cppunit/config/SourcePrefix.h>
#include "PatternWhileClauseTest.h"
#include "../SPA/PatternWhileClause.h"
#include "../SPA/AST.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/WhileNode.h"
#include "boost/foreach.hpp"

#include <iostream>
#include <string>
#include "../SPA/PatternClauseBuilder.h"

using namespace std;

void PatternWhileClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		while x {	//1
			while y {	//2
			}
		}
	}
	pattern w("x", "_") == 1;
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("zumba");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	WhileNode* while1 = new WhileNode(1);
	VarNode* x1 = new VarNode("x");
	while1->linkVarNode(x1);
	StmtLstNode* stmtList = new StmtLstNode();
	while1->linkStmtLstNode(stmtList);

	WhileNode* while2 = new WhileNode(2);
	VarNode* y1 = new VarNode("y");
	while2->linkVarNode(y1);
	StmtLstNode* stmtList2 = new StmtLstNode();
	while2->linkStmtLstNode(stmtList2);

	procsl->linkStmtNode(while1);
	stmtList->linkStmtNode(while2);
	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(WHILE_STMT_);
	stmt1->setTNodeRef(while1);
	string xvar = "x";
	unordered_set<string> uses1 = unordered_set<string>();
	uses1.emplace(xvar);
	stmt1->setUses(uses1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(WHILE_STMT_);
	stmt2->setTNodeRef(while2);
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
	vy->addUsingStmt(2);
	vy->addTNode(y1);
	vtable->addVariable(vy);
}

void PatternWhileClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternWhileClauseTest );

void PatternWhileClauseTest::evaluateVarWild() {
	//cout << "varwild";
	string syn1 = "w";
	/*PatternWhileClause* p1 = new PatternWhileClause(syn1, "_", "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_GENERIC);*/
	PatternClauseBuilder* whileBuilder = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder->setSynonym(syn1);
	whileBuilder->setVar("_");
	whileBuilder->setVarType(stringconst::ARG_GENERIC);
	whileBuilder->setVarFixed(false);
	whileBuilder->setExpr(1, "_");
	PatternWhileClause* p1 = (PatternWhileClause*) whileBuilder->build();
	
	CPPUNIT_ASSERT(p1->isValid());
	Result *res = new Result();
	CPPUNIT_ASSERT(p1->evaluate(res));

	CPPUNIT_ASSERT(res->isSynPresent(syn1));
	CPPUNIT_ASSERT(res->getSyn(syn1).size() == 2);

	return;
}

void PatternWhileClauseTest::evaluateVarFixed() {
	//cout << "varfixed";
	// pass pattern w("x", "_");
	string syn1 = "w";
	string var1 = "x";
	/*PatternWhileClause* p1 = new PatternWhileClause(syn1, "x", "_");
	p1->setVarFixed(true);
	p1->setVarType(stringconst::ARG_VARIABLE);*/
	PatternClauseBuilder* whileBuilder = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder->setSynonym(syn1);
	whileBuilder->setVar(var1);
	whileBuilder->setVarType(stringconst::ARG_VARIABLE);
	whileBuilder->setVarFixed(true);
	whileBuilder->setExpr(1, "_");
	PatternWhileClause* p1 = (PatternWhileClause*) whileBuilder->build();

	CPPUNIT_ASSERT(p1->isValid());
	Result *r1 = new Result();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->isSynPresent(syn1));
	CPPUNIT_ASSERT(r1->getSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(r1->getSyn(syn1).count("1") == 1);

	// pass pattern w("y", "_");
	string syn2 = "w";
	string var2 = "y";
	/*PatternWhileClause* p1 = new PatternWhileClause(syn2, "y", "_");
	p1->setVarFixed(true);
	p1->setVarType(stringconst::ARG_VARIABLE);*/
	PatternClauseBuilder* whileBuilder2 = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder2->setSynonym(syn2);
	whileBuilder2->setVar(var2);
	whileBuilder2->setVarType(stringconst::ARG_VARIABLE);
	whileBuilder2->setVarFixed(true);
	whileBuilder2->setExpr(1, "_");
	PatternWhileClause* p2 = (PatternWhileClause*) whileBuilder2->build();

	CPPUNIT_ASSERT(p2->isValid());
	Result *r2 = new Result();
	CPPUNIT_ASSERT(p2->evaluate(r2));

	CPPUNIT_ASSERT(r2->isSynPresent(syn2));
	CPPUNIT_ASSERT(r2->getSyn(syn2).size() == 1);
	CPPUNIT_ASSERT(r2->getSyn(syn2).count("2") == 1);

	// var fail, not the control var
	/*PatternWhileClause* p2 = new PatternWhileClause(syn1, "z", "_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(true);*/
	PatternClauseBuilder* whileBuilder3 = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder3->setSynonym(syn1);
	whileBuilder3->setVar("z");
	whileBuilder3->setVarType(stringconst::ARG_VARIABLE);
	whileBuilder3->setVarFixed(true);
	whileBuilder3->setExpr(1, "_");
	PatternWhileClause* p3 = (PatternWhileClause*) whileBuilder3->build();
	CPPUNIT_ASSERT(p3->isValid());

	Result* resFail = new Result();
	CPPUNIT_ASSERT(!p3->evaluate(resFail));
	
	return;
}

void PatternWhileClauseTest::evaluateVarSyn() {
	//cout << "varsyn";
	// pass pattern if(x, "_", "_");
	string syn1 = "if";
	string syn2 = "x";
	/*PatternWhileClause* p1 = new PatternWhileClause(syn1, syn2, "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_VARIABLE);*/
	PatternClauseBuilder* whileBuilder = new PatternClauseBuilder(PATTERNWHILE_);
	whileBuilder->setSynonym(syn1);
	whileBuilder->setVar(syn2);
	whileBuilder->setVarType(stringconst::ARG_VARIABLE);
	whileBuilder->setVarFixed(false);
	whileBuilder->setExpr(1, "_");
	PatternWhileClause* p1 = (PatternWhileClause*) whileBuilder->build();

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
	/*BOOST_FOREACH(auto a, multiSynResults) {
		cout << a.at(0) << "," << a.at(1) << endl;
	}*/
	
	return;
}