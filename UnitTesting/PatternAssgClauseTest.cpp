#include <cppunit/config/SourcePrefix.h>
#include "PatternAssgClauseTest.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "boost/foreach.hpp"

#include <iostream>
#include <string>

using namespace std;

void PatternAssgClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		i = 1+2;	//1
		j = 2+3+4;	//2
		k = 3;	//3
	}
	Follows(1, 2) == true;
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("zumba");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	AssgNode* assg1 = new AssgNode(1);
	VarNode* i1 = new VarNode("i");
	assg1->linkVarNode(i1);
	OpNode* plus1 = new OpNode("+");
	ConstNode* one1 = new ConstNode("1");
	ConstNode* two1 = new ConstNode("2");
	plus1->linkLeftNode(one1);
	plus1->linkRightNode(two1);
	assg1->linkExprNode(plus1);
	procsl->linkStmtNode(assg1);

	AssgNode* assg2 = new AssgNode(2);
	VarNode* j2 = new VarNode("j");
	OpNode* plus2_1 = new OpNode("+");
	ConstNode* four2 = new ConstNode("4");
	OpNode* plus2_2 = new OpNode("+");
	ConstNode* three2 = new ConstNode("3");
	ConstNode* two2 = new ConstNode("2");
	plus2_2->linkRightNode(three2);
	plus2_2->linkLeftNode(two2);
	plus2_1->linkRightNode(four2);
	plus2_1->linkLeftNode(plus2_2);
	assg2->linkVarNode(j2);
	assg2->linkExprNode(plus2_1);
	procsl->linkStmtNode(assg2);

	AssgNode* assg3 = new AssgNode(3);
	VarNode* k3 = new VarNode("k");
	ConstNode* three3 = new ConstNode("3");
	assg3->linkVarNode(k3);
	assg3->linkExprNode(three3);
	procsl->linkStmtNode(assg3);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	string ivar = "i";
	unordered_set<string> uses1 = unordered_set<string>();
	uses1.emplace(ivar);
	stmt1->setModifies(uses1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsBefore(1);
	stmt2->setFollowsAfter(3);
	string jvar = "j";
	unordered_set<string> uses2 = unordered_set<string>();
	uses2.emplace(jvar);
	stmt2->setModifies(uses2);
	stmt2->setTNodeRef(assg2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2);
	string kvar = "k";
	unordered_set<string> uses3 = unordered_set<string>();
	uses3.emplace(kvar);
	stmt3->setModifies(uses3);
	stmt3->setTNodeRef(assg3);
	stable->addStmt(stmt3);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(1);
	vi->addTNode(assg1);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(2);
	vj->addTNode(assg2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(2);
	vk->addTNode(assg3);
	vtable->addVariable(vk);
}

void PatternAssgClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternAssgClauseTest );

void PatternAssgClauseTest::evaluateVarWildExprWild() {
	//cout << "varwildexprwild";
	PatternAssgClause* p1 = new PatternAssgClause("a", "_", "_");
	p1->setSecondArgFixed(false);
	p1->setSecondArgType(stringconst::ARG_GENERIC);

	CPPUNIT_ASSERT(p1->isValid());
	Results *r1 = new Results();
	CPPUNIT_ASSERT(p1->evaluate(r1));
	string syn1 = "a";

	CPPUNIT_ASSERT(r1->hasResults(syn1));
	CPPUNIT_ASSERT(r1->selectSyn(syn1).size() == 3);
	
	unordered_set<string> v = r1->selectSyn(syn1);
	//BOOST_FOREACH(auto i, v) {
	//	cout << i;
	//}

	return;
}

void PatternAssgClauseTest::evaulateVarWildExpr() {
	//cout << "varwildexpr";
	PatternAssgClause* p1 = new PatternAssgClause("a", "_", "_\"1 2 +\"_");
	p1->setVarType(stringconst::ARG_GENERIC);
	p1->setVarFixed(false);
	CPPUNIT_ASSERT(p1->isValid());
	Results* res = new Results();
	CPPUNIT_ASSERT(p1->evaluate(res));

	string syn1 = "a";
	long long num = 1;
	CPPUNIT_ASSERT(res->hasResults(syn1));
	CPPUNIT_ASSERT(res->selectSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(res->selectSyn(syn1).count("1") == 1);

	// expr fail
	PatternAssgClause* p2 = new PatternAssgClause("a", "_", "_\"3 4 +\"_");
	p2->setVarType(stringconst::ARG_GENERIC);
	p2->setVarFixed(false);
	CPPUNIT_ASSERT(p2->isValid());
	
	Results* resFail = new Results();
	CPPUNIT_ASSERT(!p2->evaluate(resFail));
	
	return;
}

void PatternAssgClauseTest::evaluateVarFixedExprWild() {
	//cout << "varfixedexprwild";
	// pass
	PatternAssgClause* p1 = new PatternAssgClause("a", "i", "_");
	p1->setVarType(stringconst::ARG_VARIABLE);
	p1->setVarFixed(true);
	CPPUNIT_ASSERT(p1->isValid());

	Results* res = new Results();
	CPPUNIT_ASSERT(p1->evaluate(res));
	
	string syn1 = "a";
	long long num = 1;

	CPPUNIT_ASSERT(res->hasResults(syn1));
	CPPUNIT_ASSERT(res->selectSyn(syn1).size() == 1);
	CPPUNIT_ASSERT(res->selectSyn(syn1).count("1") == 1);

	//cout << r1.getFirstClauseSyn() << endl;
	//CPPUNIT_ASSERT(r1.isClausePassed());
	//CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	//CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	//CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == to_string(num));

	// var fail
	PatternAssgClause* p2 = new PatternAssgClause("a", "x", "_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(true);
	CPPUNIT_ASSERT(p2->isValid());

	Results* resFail = new Results();
	CPPUNIT_ASSERT(!p2->evaluate(resFail));
	
	return;
}

void PatternAssgClauseTest::evaluateVarFixedExpr() {
	//cout << "varfixedexpr";
	// pass targeting j = 2+3+4
	PatternAssgClause* p1 = new PatternAssgClause("a", "j", "_\"2 3 +\"_");
	p1->setVarType(stringconst::ARG_VARIABLE);
	p1->setVarFixed(true);
	CPPUNIT_ASSERT(p1->isValid());

	Results* res = new Results();
	CPPUNIT_ASSERT(p1->evaluate(res));
	
	string expectedSyn1 = "a";
	string expectedNum = "2";

	CPPUNIT_ASSERT(res->hasResults(expectedSyn1));
	CPPUNIT_ASSERT(res->selectSyn(expectedSyn1).size() == 1);
	CPPUNIT_ASSERT(res->selectSyn(expectedSyn1).count(expectedNum) == 1);

	// expr fail targeting j = 2+3+4
	PatternAssgClause* p2 = new PatternAssgClause("a", "j", "_\"3 4 +\"_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(true);
	CPPUNIT_ASSERT(p2->isValid());

	Results* resFail = new Results();
	CPPUNIT_ASSERT(!p2->evaluate(resFail));

	// var fail
	PatternAssgClause* p3 = new PatternAssgClause("a", "x", "_\"1 2 +\"_");
	p3->setVarType(stringconst::ARG_VARIABLE);
	p3->setVarFixed(true);
	CPPUNIT_ASSERT(p3->isValid());

	Results* resFail2 = new Results();
	CPPUNIT_ASSERT(!p3->evaluate(resFail2));

	return;
}

void PatternAssgClauseTest::evaluateVarExprWild() {
	// pass, pattern a(v, "_");
	PatternAssgClause* p1 = new PatternAssgClause("a", "v", "_");
	p1->setVarType(stringconst::ARG_VARIABLE);
	p1->setVarFixed(false);
	p1->setExpression("_");
	CPPUNIT_ASSERT(p1->isValid());

	Results* res = new Results();
	CPPUNIT_ASSERT(p1->evaluate(res));
	
	string expectedSyn1 = "a";
	string expectedSyn2 = "v";
	int expectedSize = 3;

	CPPUNIT_ASSERT(res->hasResults(expectedSyn1));
	CPPUNIT_ASSERT(res->hasResults(expectedSyn2));
	// HOW TO CHECK THE PAIR RESULTS
	// 1. make unordered set of the syns you want to check
	// 2. select them as resultstable and see size
	unordered_set<string> synList = unordered_set<string>();
	synList.insert(expectedSyn1);
	synList.insert(expectedSyn2);
	Results::ResultsTable multiSynResults = res->selectMultiSyn(synList);
	CPPUNIT_ASSERT(multiSynResults.size() == expectedSize);

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

void PatternAssgClauseTest::evaluateVarExpr() {
	// pass targeting j = 2+3+4
	// pattern a(v, "_2+3+4_");
	PatternAssgClause* p1 = new PatternAssgClause("a", "v", "_\"2 3 +\"_");
	p1->setVarType(stringconst::ARG_VARIABLE);
	p1->setVarFixed(false);
	CPPUNIT_ASSERT(p1->isValid());

	Results* res = new Results();
	CPPUNIT_ASSERT(p1->evaluate(res));
	
	string expectedSyn1 = "a";
	string expectedSyn2 = "v";
	int expectedSize = 1;

	CPPUNIT_ASSERT(res->hasResults(expectedSyn1));
	CPPUNIT_ASSERT(res->hasResults(expectedSyn2));
	// HOW TO CHECK THE PAIR RESULTS
	// 1. make unordered set of the syns you want to check
	// 2. select them as resultstable and see size
	unordered_set<string> synList = unordered_set<string>();
	synList.insert(expectedSyn1);
	synList.insert(expectedSyn2);
	Results::ResultsTable multiSynResults = res->selectMultiSyn(synList);
	CPPUNIT_ASSERT(multiSynResults.size() == expectedSize);

	//cout << r1.getFirstClauseSyn() << endl;
	//cout << r1.getSecondClauseSyn() << endl;
	//CPPUNIT_ASSERT(r1.isClausePassed());
	//CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	//CPPUNIT_ASSERT(r1.getSecondClauseSyn() == syn2);
	//CPPUNIT_ASSERT(r1.getPairResults().size() == 1);
	//CPPUNIT_ASSERT(r1.getPairResults().at(0).first == to_string(num));
	//CPPUNIT_ASSERT(r1.getPairResults().at(0).second == "j");
	
	// expr fail
	PatternAssgClause* p2 = new PatternAssgClause("a", "j", "_\"3 4 +\"_");
	p2->setVarType(stringconst::ARG_VARIABLE);
	p2->setVarFixed(false);
	CPPUNIT_ASSERT(p2->isValid());

	Results* resFail = new Results();
	CPPUNIT_ASSERT(!p2->evaluate(resFail));

	return;
}