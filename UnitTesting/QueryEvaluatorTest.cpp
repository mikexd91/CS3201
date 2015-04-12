#include <cppunit/config/SourcePrefix.h>
#include "QueryEvaluatorTest.h"
#include "../SPA/Results.h"
#include "../SPA/QueryEvaluator.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"

using namespace stringconst;
using namespace std;

void QueryEvaluatorTest::setUp() {
	/* testing this source
	procedure zumba {
		i = 1+2;	//1
		j = 2+3+4;	//2
		k = 3;		//3
		w = i;		//4
		x = w+k;	//5
		i = i;		//6
	}
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

	AssgNode* assg4 = new AssgNode(4);
	VarNode* w4 = new VarNode("w");
	assg4->linkVarNode(w4);
	VarNode* i4 = new VarNode("i");
	assg4->linkExprNode(i4);
	procsl->linkStmtNode(assg4);

	AssgNode* assg5 = new AssgNode(5);
	VarNode* x5 = new VarNode("x");
	assg5->linkVarNode(x5);
	OpNode* plus5 = new OpNode("+");
	VarNode* w5 = new VarNode("w");
	VarNode* k5 = new VarNode("k");
	plus5->linkLeftNode(w5);
	plus5->linkRightNode(k5);
	assg5->linkExprNode(plus5);
	procsl->linkStmtNode(assg5);

	AssgNode* assg6 = new AssgNode(6);
	VarNode* i6 = new VarNode("i");
	assg6->linkVarNode(i6);
	VarNode* i6_2 = new VarNode("i");
	assg6->linkExprNode(i6_2);
	procsl->linkStmtNode(assg6);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	string ivar = "i";
	set<string> mods1 = set<string>();
	mods1.emplace(ivar);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsBefore(1);
	stmt2->setFollowsAfter(3);
	string jvar = "j";
	set<string> mods2 = set<string>();
	mods2.emplace(jvar);
	stmt2->setModifies(mods2);
	stmt2->setTNodeRef(assg2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2);
	string kvar = "k";
	set<string> mods3 = set<string>();
	mods3.emplace(kvar);
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg3);
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	stmt4->setFollowsBefore(3);
	stmt4->setFollowsAfter(5);
	string wvar = "w";
	set<string> mods4 = set<string>();
	mods4.emplace(wvar);
	stmt4->setModifies(mods4);
	set<string> uses4 = set<string>();
	uses4.emplace(ivar);
	stmt4->setUses(uses4);
	stmt4->setTNodeRef(assg4);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsBefore(4);
	stmt5->setFollowsAfter(6);
	string xvar = "x";
	set<string> mods5 = set<string>();
	mods5.emplace(xvar);
	stmt5->setModifies(mods5);
	set<string> uses5 = set<string>();
	uses5.emplace(wvar);
	uses5.emplace(kvar);
	stmt5->setUses(uses5);
	stmt5->setTNodeRef(assg5);
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	set<string> mods6 = set<string>();
	mods6.emplace(ivar);
	stmt6->setModifies(mods6);
	set<string> uses6 = set<string>();
	uses6.emplace(ivar);
	stmt6->setUses(uses6);
	stmt6->setTNodeRef(assg6);
	stable->addStmt(stmt6);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(1);
	vi->addModifyingStmt(6);
	vi->addUsingStmt(4);
	vi->addUsingStmt(6);
	vi->addTNode(i1);
	vi->addTNode(i4);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(2);
	vj->addTNode(j2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(3);
	vk->addUsingStmt(5);
	vk->addTNode(k3);
	vtable->addVariable(vk);

	Variable* vw = new Variable("w");
	vw->addModifyingStmt(4);
	vw->addUsingStmt(5);
	vw->addTNode(w4);
	vtable->addVariable(vw);

	Variable* vx = new Variable("x");
	vx->addModifyingStmt(5);
	vx->addTNode(x5);
	vtable->addVariable(vx);
}

void QueryEvaluatorTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( QueryEvaluatorTest );

// Test getSameClause of QueryEvaluator
void QueryEvaluatorTest::testEvaluator() {
	// Test when both results contain 0 unfixed arguments
	QueryEvaluator e = *new QueryEvaluator();

	Results r1 = *new Results();
	Results r2 = *new Results();

	r1.setNumOfSyn(0);
	r2.setNumOfSyn(0);

	//int num = e.getSameClause(r1, r2);
	
	CPPUNIT_ASSERT(e.getSameClause(r1, r2) == 0);
	
	// Test when both results contains 1 unfixed argument
	// results contain 1 same clause, <a> and <a>

	Results r3 = *new Results();
	Results r4 = *new Results();

	r3.setNumOfSyn(1);
	r4.setNumOfSyn(1);

	r3.setFirstClauseSyn("a");
	r4.setFirstClauseSyn("a");

	//int num2 = e.getSameClause(r3, r4);

	CPPUNIT_ASSERT(e.getSameClause(r3, r4) == 1);

	// Test when both results contain 1 unfixed argument
	// results contain no same clause, <a> and <v>

	r3.setFirstClauseSyn("a");
	r4.setFirstClauseSyn("v");

	CPPUNIT_ASSERT(e.getSameClause(r3, r4) == 0);

	// Test when both results contains unfixed argument
	// results contain 2 same clause, <a,v> and <a,v>

	Results r5 = *new Results();
	Results r6 = *new Results();

	r5.setNumOfSyn(2);
	r6.setNumOfSyn(2);

	r5.setFirstClauseSyn("a");
	r5.setSecondClauseSyn("v");

	r6.setFirstClauseSyn("a");
	r6.setSecondClauseSyn("v");
	
	//int num3 = e.getSameClause(r5, r6);

	CPPUNIT_ASSERT(e.getSameClause(r5, r6) == 2);

	// Test when both results contains unfixed argument
	// results contain 1 same clause, <a1,v> and <a2,v>

	r5.setFirstClauseSyn("a1");
	r5.setSecondClauseSyn("v");

	r6.setFirstClauseSyn("a2");
	r6.setSecondClauseSyn("v");

	CPPUNIT_ASSERT(e.getSameClause(r5, r6) == 1);
	
	// Test when both result contains unfixed argument
	// results contain no same clause <a1, v1> <a2, v2>

	r5.setFirstClauseSyn("a1");
	r5.setSecondClauseSyn("v1");

	r6.setFirstClauseSyn("a2");
	r6.setSecondClauseSyn("v2");

	CPPUNIT_ASSERT(e.getSameClause(r5, r6) == 0);

	// Test when a result contains an unfixed argument
	// results contain 1 same clause <a,v> <a>

	Results r7 = *new Results();
	Results r8 = *new Results();

	r7.setNumOfSyn(2);
	r8.setNumOfSyn(1);

	r7.setFirstClauseSyn("a");
	r7.setSecondClauseSyn("v");

	r8.setFirstClauseSyn("a");
	
	//int num4 = e.getSameClause(r7, r8);
	
	CPPUNIT_ASSERT(e.getSameClause(r7, r8) == 1);
}

// Test getAllSynValues
void QueryEvaluatorTest::testEvaluator2() {
	QueryEvaluator e = *new QueryEvaluator();

	vector<StringPair> selectList = *new vector<StringPair>();
	
	StringPair pair1 = *new StringPair();
	pair1.setFirst("a");
	pair1.setSecond(stringconst::ARG_ASSIGN);

	StringPair pair2 = *new StringPair();
	pair2.setFirst("s");
	pair2.setSecond(stringconst::ARG_STATEMENT);

	StringPair pair3 = *new StringPair();
	pair3.setFirst("w");
	pair3.setSecond(stringconst::ARG_WHILE);

	StringPair pair4 = *new StringPair();
	pair4.setFirst("v");
	pair4.setSecond(stringconst::ARG_VARIABLE);

	selectList.push_back(pair1);

	Results r1 = *new Results();
	r1.setClausePassed(true);
	r1.setNumOfSyn(2);
	r1.setFirstClauseSyn("a1");
	r1.setSecondClauseSyn("a2");
	r1.addPairResult("1","2");
	r1.addPairResult("2","3");

	Results r2 = *new Results();
	r2.setClausePassed(true);
	r2.setNumOfSyn(2);
	r1.setFirstClauseSyn("a");
	r1.setSecondClauseSyn("v");
	r1.addPairResult("1","2");
	r1.addPairResult("2","3");
	
	vector<Results> resultsList = *new vector<Results>();

	resultsList.push_back(r1);
	resultsList.push_back(r2);

	set<string> res1 = e.evaluateManyClause(resultsList, selectList);


	/*
	for (set<string>::iterator iter = res1.begin() ; iter != res1.end(); iter++) {
		cout << "result: " << *iter << "! ";
	}
	*/
}

void QueryEvaluatorTest::testEvaluator3() {

}
