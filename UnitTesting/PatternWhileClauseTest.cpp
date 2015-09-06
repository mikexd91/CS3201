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

using namespace std;

void PatternWhileClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		while x {	//1
		}			//2
	}
	pattern if("x", "_", "_") == 1;
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

	procsl->linkStmtNode(while1);
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

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vx = new Variable("x");
	vx->addUsingStmt(1);
	vx->addTNode(x1);
	vtable->addVariable(vx);

}

void PatternWhileClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternWhileClauseTest );

void PatternWhileClauseTest::evaluateVarWild() {
	cout << "varwild";
	string syn1 = "if";
	PatternWhileClause* p1 = new PatternWhileClause(syn1, "_", "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_GENERIC);
	
	CPPUNIT_ASSERT(p1->isValid());
	Results *r1 = new Results();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->hasResults(syn1));
	CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 1);

	return;
}

void PatternWhileClauseTest::evaluateVarFixed() {
	cout << "varfixed";
	// pass pattern if("x", "_", "_");
	string syn1 = "if";
	PatternWhileClause* p1 = new PatternWhileClause(syn1, "x", "_");
	p1->setVarFixed(true);
	p1->setVarType(stringconst::ARG_VARIABLE);

	CPPUNIT_ASSERT(p1->isValid());
	Results *r1 = new Results();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->hasResults(syn1));
	CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(r1->selectSyn(syn1).count("1") == 1);


	// var fail, not the control var
	PatternWhileClause* p2 = new PatternWhileClause(syn1, "y", "_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(true);
	CPPUNIT_ASSERT(p2->isValid());

	Results* resFail = new Results();
	CPPUNIT_ASSERT(!p2->evaluate(resFail));
	
	return;
}

void PatternWhileClauseTest::evaluateVarSyn() {
	cout << "varsyn";
	// pass pattern if(x, "_", "_");
	string syn1 = "if";
	string syn2 = "x";
	PatternWhileClause* p1 = new PatternWhileClause(syn1, syn2, "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_VARIABLE);

	CPPUNIT_ASSERT(p1->isValid());
	Results *r1 = new Results();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->hasResults(syn1));
	CPPUNIT_ASSERT(r1->hasResults(syn2));
	CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(r1->selectSyn(syn1).count("1") == 1);
	// HOW TO CHECK THE PAIR RESULTS
	// 1. make unordered set of the syns you want to check
	// 2. select them as resultstable and see size
	unordered_set<string> synList = unordered_set<string>();
	synList.insert(syn1);
	synList.insert(syn2);
	Results::ResultsTable multiSynResults = r1->selectMultiSyn(synList);
	CPPUNIT_ASSERT(multiSynResults.size() == 1);
	
	return;
}