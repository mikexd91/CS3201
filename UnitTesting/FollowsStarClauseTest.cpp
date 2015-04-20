#include <cppunit/config/SourcePrefix.h>
#include "FollowsStarClauseTest.h"
#include "../SPA/FollowsStarClause.h"
#include "../SPA/AST.h"
#include "../SPA/WhileNode.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"

#include <iostream>
#include <string>
#include <boost/foreach.hpp>

using namespace stringconst;
using namespace std;

void FollowsStarClauseTest::setUp() {
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
	stmt3->setFollowsAfter(4);
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

void FollowsStarClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( FollowsStarClauseTest );

//stmt 1-6 all follow each other

void FollowsStarClauseTest::testFollowsStarFixedFixed() {
	// pass, 1, 2
	FollowsStarClause* f1 = new FollowsStarClause();
	f1->setFirstArg("1");
	f1->setFirstArgFixed(true);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("2");
	f1->setSecondArgFixed(true);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid());

	Results r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 0);

	// pass, 1, 3
	f1 = new FollowsStarClause();
	f1->setFirstArg("1");
	f1->setFirstArgFixed(true);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("3");
	f1->setSecondArgFixed(true);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid());

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 0);

	// fail, targeting same stmt
	FollowsStarClause* f2 = new FollowsStarClause();
	f2->setFirstArg("1");
	f2->setFirstArgFixed(true);
	f2->setFirstArgType(ARG_STATEMENT);
	f2->setSecondArg("1");
	f2->setSecondArgFixed(true);
	f2->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f2->isValid());

	Results r2 = f2->evaluate();
	CPPUNIT_ASSERT(!r2.isClausePassed());

	// fail, targeting stmt num exceed
	FollowsStarClause* f3 = new FollowsStarClause();
	f3->setFirstArg("16");
	f3->setFirstArgFixed(true);
	f3->setFirstArgType(ARG_STATEMENT);
	f3->setSecondArg("77");
	f3->setSecondArgFixed(true);
	f3->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f3->isValid());

	Results r3 = f3->evaluate();
	CPPUNIT_ASSERT(!r3.isClausePassed());
}

void FollowsStarClauseTest::testFollowsStarFixedSyn() {
	// pass, 1, s
	FollowsStarClause* f1 = new FollowsStarClause();
	f1->setFirstArg("1");
	f1->setFirstArgFixed(true);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("s");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid());

	Results r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);

	// pass, 1, a
	f1 = new FollowsStarClause();
	f1->setFirstArg("1");
	f1->setFirstArgFixed(true);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("a");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_ASSIGN);
	CPPUNIT_ASSERT(f1->isValid());

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);

	// pass, 1, _
	f1 = new FollowsStarClause();
	f1->setFirstArg("1");
	f1->setFirstArgFixed(true);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("_");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(f1->isValid());

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 0);

	// fail, targeting arg type not exist
	FollowsStarClause* f2 = new FollowsStarClause();
	f2->setFirstArg("1");
	f2->setFirstArgFixed(true);
	f2->setFirstArgType(ARG_STATEMENT);
	f2->setSecondArg("w");
	f2->setSecondArgFixed(false);
	f2->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(f2->isValid());

	Results r2 = f2->evaluate();
	CPPUNIT_ASSERT(!r2.isClausePassed());

	// fail, targeting stmt num overshot
	FollowsStarClause* f3 = new FollowsStarClause();
	f3->setFirstArg("66");
	f3->setFirstArgFixed(true);
	f3->setFirstArgType(ARG_WHILE);
	f3->setSecondArg("w");
	f3->setSecondArgFixed(false);
	f3->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f3->isValid());

	Results r3 = f3->evaluate();
	CPPUNIT_ASSERT(!r3.isClausePassed());
}

void FollowsStarClauseTest::testFollowsStarSynFixed() {
	// pass, s, 6
	FollowsStarClause* f1 = new FollowsStarClause();
	f1->setFirstArg("s");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("6");
	f1->setSecondArgFixed(true);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid());

	Results r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);

	// pass, a, 6
	f1 = new FollowsStarClause();
	f1->setFirstArg("a");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_ASSIGN);
	f1->setSecondArg("6");
	f1->setSecondArgFixed(true);
	f1->setSecondArgType(ARG_ASSIGN);
	CPPUNIT_ASSERT(f1->isValid());

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);

	// pass, _, 6
	f1 = new FollowsStarClause();
	f1->setFirstArg("_");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_GENERIC);
	f1->setSecondArg("6");
	f1->setSecondArgFixed(true);
	f1->setSecondArgType(ARG_ASSIGN);
	CPPUNIT_ASSERT(f1->isValid());

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 0);

	// fail, targeting arg type not exist
	FollowsStarClause* f2 = new FollowsStarClause();
	f2->setFirstArg("w");
	f2->setFirstArgFixed(false);
	f2->setFirstArgType(ARG_WHILE);
	f2->setSecondArg("6");
	f2->setSecondArgFixed(true);
	f2->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(f2->isValid());

	Results r2 = f2->evaluate();
	CPPUNIT_ASSERT(!r2.isClausePassed());

	// fail, targeting stmt num overshot
	FollowsStarClause* f3 = new FollowsStarClause();
	f3->setFirstArg("w");
	f3->setFirstArgFixed(false);
	f3->setFirstArgType(ARG_WHILE);
	f3->setSecondArg("66");
	f3->setSecondArgFixed(true);
	f3->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f3->isValid());

	Results r3 = f3->evaluate();
	CPPUNIT_ASSERT(!r3.isClausePassed());
}

void FollowsStarClauseTest::testFollowsStarSynSyn() {
	// pass, s, s1
	FollowsStarClause* f1 = new FollowsStarClause();
	f1->setFirstArg("s");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("s1");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid()); 

	Results r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 2);
	CPPUNIT_ASSERT(r1.getPairResults().size() == 15);

	// pass, _, s1
	f1 = new FollowsStarClause();
	f1->setFirstArg("_");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_GENERIC);
	f1->setSecondArg("s1");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f1->isValid()); 

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 1);
	//cout << r1.getSinglesResults().size() << endl;
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);

	/*BOOST_FOREACH(auto p, r1.getSinglesResults()) {
		cout << p << " ";
	}*/

	// pass, s, _
	f1 = new FollowsStarClause();
	f1->setFirstArg("s");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_STATEMENT);
	f1->setSecondArg("s1");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(f1->isValid()); 

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 1);
	//cout << r1.getSinglesResults().size() << endl;
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);

	// pass _,_
	f1 = new FollowsStarClause();
	f1->setFirstArg("s");
	f1->setFirstArgFixed(false);
	f1->setFirstArgType(ARG_GENERIC);
	f1->setSecondArg("s1");
	f1->setSecondArgFixed(false);
	f1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(f1->isValid()); 

	r1 = f1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 0);

	// fail, targeting arg type not exist
	FollowsStarClause* f2 = new FollowsStarClause();
	f2->setFirstArg("w");
	f2->setFirstArgFixed(false);
	f2->setFirstArgType(ARG_WHILE);
	f2->setSecondArg("a"); 
	f2->setSecondArgFixed(false);
	f2->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(f2->isValid());

	Results r2 = f2->evaluate();
	CPPUNIT_ASSERT(!r2.isClausePassed());

	// fail, s, s
	FollowsStarClause* f3 = new FollowsStarClause();
	f3->setFirstArg("s");
	f3->setFirstArgFixed(false);
	f3->setFirstArgType(ARG_STATEMENT);
	f3->setSecondArg("s");
	f3->setSecondArgFixed(false);
	f3->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(f3->isValid()); 

	Results r3 = f3->evaluate();
	//cout << "evaled" << endl;
	CPPUNIT_ASSERT(!r3.isClausePassed());
}
