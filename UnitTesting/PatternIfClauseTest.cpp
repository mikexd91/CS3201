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

using namespace std;

void PatternIfClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		if x then {	//1
		}			//2
		else {		//3
		}			//4
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

	procsl->linkStmtNode(if1);
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

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vx = new Variable("x");
	vx->addUsingStmt(1);
	vx->addTNode(x1);
	vtable->addVariable(vx);

}

void PatternIfClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternIfClauseTest );

void PatternIfClauseTest::evaluateVarWild() {
	cout << "varwild";
	string syn1 = "if";
	PatternIfClause* p1 = new PatternIfClause(syn1, "_", "_", "_");
	p1->setVarFixed(false);
	p1->setVarType(stringconst::ARG_GENERIC);
	
	CPPUNIT_ASSERT(p1->isValid());
	Results *r1 = new Results();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->hasResults(syn1));
	CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 1);

	//PatternIfClause* p1 = new PatternIfClause("a", "_", "_");
	//p1->setSecondArgFixed(false);
	//p1->setSecondArgType(stringconst::ARG_GENERIC);

	//CPPUNIT_ASSERT(p1->isValid());
	//Results *r1 = new Results();
	//CPPUNIT_ASSERT(p1->evaluate(r1));
	//string syn1 = "a";

	//CPPUNIT_ASSERT(r1->hasResults(syn1));
	//CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 3);
	//
	//unordered_set<string> v = r1->selectSyn(syn1);
	////BOOST_FOREACH(auto i, v) {
	////	cout << i;
	////}

	return;
}

void PatternIfClauseTest::evaluateVarFixed() {
	cout << "varfixed";
	// pass pattern if("x", "_", "_");
	string syn1 = "if";
	PatternIfClause* p1 = new PatternIfClause(syn1, "x", "_", "_");
	p1->setVarFixed(true);
	p1->setVarType(stringconst::ARG_VARIABLE);

	CPPUNIT_ASSERT(p1->isValid());
	Results *r1 = new Results();
	CPPUNIT_ASSERT(p1->evaluate(r1));

	CPPUNIT_ASSERT(r1->hasResults(syn1));
	CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(r1->selectSyn(syn1).count("1") == 1);


	// var fail, not the control var
	PatternIfClause* p2 = new PatternIfClause(syn1, "y", "_", "_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(true);
	CPPUNIT_ASSERT(p2->isValid());

	Results* resFail = new Results();
	CPPUNIT_ASSERT(!p2->evaluate(resFail));

	//PatternIfClause* p1 = new PatternIfClause("a", "i", "_");
	//p1->setVarType(stringconst::ARG_VARIABLE);
	//p1->setVarFixed(true);
	//CPPUNIT_ASSERT(p1->isValid());

	//Results* res = new Results();
	//CPPUNIT_ASSERT(p1->evaluate(res));
	//
	//string syn1 = "a";
	//long long num = 1;

	//CPPUNIT_ASSERT(res->hasResults(syn1));
	//CPPUNIT_ASSERT(res->selectSyn(syn1).size() == 1);
	//CPPUNIT_ASSERT(res->selectSyn(syn1).count("1") == 1);

	////cout << r1.getFirstClauseSyn() << endl;
	////CPPUNIT_ASSERT(r1.isClausePassed());
	////CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	////CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	////CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == to_string(num));

	//// var fail
	//PatternIfClause* p2 = new PatternIfClause("a", "x", "_");
	//p2->setVarType(stringconst::ARG_VARIABLE);
	//p2->setVarFixed(true);
	//CPPUNIT_ASSERT(p2->isValid());

	//Results* resFail = new Results();
	//CPPUNIT_ASSERT(!p2->evaluate(resFail));
	
	return;
}

void PatternIfClauseTest::evaluateVarSyn() {
	cout << "varsyn";
	// pass pattern if(x, "_", "_");
	string syn1 = "if";
	string syn2 = "x";
	PatternIfClause* p1 = new PatternIfClause(syn1, syn2, "_", "_");
	p1->setVarFixed(true);
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

	// pass, pattern a(v, "_");
	//PatternIfClause* p1 = new PatternIfClause("a", "v", "_");
	//p1->setVarType(stringconst::ARG_VARIABLE);
	//p1->setVarFixed(false);
	//p1->setExpression("_");
	//CPPUNIT_ASSERT(p1->isValid());

	//Results* res = new Results();
	//CPPUNIT_ASSERT(p1->evaluate(res));
	//
	//string expectedSyn1 = "a";
	//string expectedSyn2 = "v";
	//int expectedSize = 3;

	//CPPUNIT_ASSERT(res->hasResults(expectedSyn1));
	//CPPUNIT_ASSERT(res->hasResults(expectedSyn2));
	//// HOW TO CHECK THE PAIR RESULTS
	//// 1. make unordered set of the syns you want to check
	//// 2. select them as resultstable and see size
	//unordered_set<string> synList = unordered_set<string>();
	//synList.insert(expectedSyn1);
	//synList.insert(expectedSyn2);
	//Results::ResultsTable multiSynResults = res->selectMultiSyn(synList);
	//CPPUNIT_ASSERT(multiSynResults.size() == expectedSize);

	//cout << "print the thing" << endl;
	//for (int i = 0; i < r1.getPairResults().size(); i++) {
	//	//cout << r1.getPairResults().at(i).first << " " 
	//		//<< r1.getPairResults().at(i).second << endl;
	//	if (r1.getPairResults().at(i).first == "1") {
	//		CPPUNIT_ASSERT(r1.getPairResults().at(i).second == "i");
	//	}
	//	if (r1.getPairResults().at(i).first == "2") {
	//		CPPUNIT_ASSERT(r1.getPairResults().at(i).second == "j");
	//	}
	//	if (r1.getPairResults().at(i).first == "3") {
	//		CPPUNIT_ASSERT(r1.getPairResults().at(i).second == "k");
	//	}
	//}
	
	return;
}