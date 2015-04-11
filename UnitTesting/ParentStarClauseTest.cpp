#include <cppunit/config/SourcePrefix.h>
#include "ParentStarClauseTest.h"
#include "../SPA/ParentStarClause.h"
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

using namespace stringconst;
using namespace std;

void ParentStarClauseTest::setUp() {
	/* testing this source

	procedure chocs {
		a=4;
		while i {
			k = 3;
			while j {
				i=1;
				j=2;
			}
			b=5;
		}	
	}
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("chocs");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	AssgNode* assg1 = new AssgNode(1);
	VarNode* a1 = new VarNode("a");
	assg1->linkVarNode(a1);
	assg1->linkExprNode(new ConstNode("4"));
	procsl->linkStmtNode(assg1);

	WhileNode* while1 = new WhileNode(2);
	VarNode* i1 = new VarNode("i");
	while1->linkVarNode(i1);
	StmtLstNode* whilesl1 = new StmtLstNode();
	while1->linkStmtLstNode(whilesl1);
	procsl->linkStmtNode(while1);

	AssgNode* assg2 = new AssgNode(3);
	VarNode* k1 = new VarNode("k");
	assg2->linkVarNode(k1);
	assg2->linkExprNode(new ConstNode("3"));
	whilesl1->linkStmtNode(assg2);

	WhileNode* while2 = new WhileNode(4);
	VarNode* j1 = new VarNode("j");
	while2->linkVarNode(j1);
	StmtLstNode* whilesl2 = new StmtLstNode();
	while2->linkStmtLstNode(whilesl2);
	whilesl1->linkStmtNode(while2);

	AssgNode* assg3 = new AssgNode(5);
	VarNode* i2 = new VarNode("i");
	assg3->linkVarNode(i2);
	assg3->linkExprNode(new ConstNode("1"));
	whilesl2->linkStmtNode(assg3);

	AssgNode* assg4 = new AssgNode(6);
	VarNode* j2 = new VarNode("j");
	assg4->linkVarNode(j2);
	assg4->linkExprNode(new ConstNode("4"));
	whilesl2->linkStmtNode(assg4);

	AssgNode* assg5 = new AssgNode(7);
	VarNode* b1 = new VarNode("b");
	assg5->linkVarNode(b1);
	assg5->linkExprNode(new ConstNode("5"));
	whilesl1->linkStmtNode(assg5);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	string modifiesArray1[] = {"a"};
	set<string> mods1(modifiesArray1, modifiesArray1 + 1);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(WHILE_STMT_);
	stmt2->setFollowsBefore(1);
	string modifiesArray2[] = {"k", "i", "j", "b"};
	set<string> mods2(modifiesArray2, modifiesArray2 + 4);
	string usesArray2[] = {"i", "j"};
	set<string> uses2(usesArray2, usesArray2 + 2);
	stmt2->setModifies(mods2);
	stmt2->setUses(uses2);
	stmt2->setTNodeRef(while1);
	int children2[] = {3, 4, 7};
	stmt2->setChildren(set<int>(children2, children2+3));
	stable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsAfter(4);
	set<string> mods3 = set<string>();
	mods3.emplace("k");
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg2);
	stmt3->setParent(2);
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(WHILE_STMT_);
	stmt4->setFollowsBefore(3);
	set<string> mods4 = set<string>();
	mods4.emplace("i");
	mods4.emplace("j");
	set<string> uses4 = set<string>();
	uses4.emplace("j");
	stmt4->setModifies(mods4);
	stmt4->setUses(uses4);
	stmt4->setTNodeRef(while2);
	stmt4->setParent(2);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsAfter(6);
	set<string> mods5= set<string>();
	mods5.emplace("i");
	stmt5->setModifies(mods5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(4);
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	set<string> mods6= set<string>();
	mods6.emplace("j");
	stmt6->setModifies(mods6);
	stmt6->setTNodeRef(assg4);
	stmt6->setParent(4);
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	stmt7->setFollowsBefore(4);
	set<string> mods7= set<string>();
	mods7.emplace("b");
	stmt7->setModifies(mods7);
	stmt7->setTNodeRef(assg5);
	stmt7->setParent(2);
	stable->addStmt(stmt7);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* va = new Variable("a");
	va->addModifyingStmt(1);
	va->addTNode(a1);
	vtable->addVariable(va);

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(2);
	vi->addModifyingStmt(5);
	vi->addUsingStmt(2);
	vi->addTNode(i1);
	vi->addTNode(i2);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(2);
	vj->addModifyingStmt(4);
	vj->addModifyingStmt(6);
	vj->addUsingStmt(4);
	vj->addTNode(j1);
	vj->addTNode(j2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(2);
	vk->addModifyingStmt(3);
	vk->addTNode(k1);
	vtable->addVariable(vk);

	Variable* vb = new Variable("b");
	vb->addModifyingStmt(2);
	vb->addModifyingStmt(7);
	vb->addTNode(b1);
	vtable->addVariable(vb);
}

void ParentStarClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParentStarClauseTest );

void ParentStarClauseTest::testParentStarFixedFixedPass() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("5");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	//Results r1 = m1->evaluate();
	//CPPUNIT_ASSERT(r1.isClausePassed());
}

void ParentStarClauseTest::testParentStarFixedFixedFail() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("4");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("7");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	//Results r1 = m1->evaluate();
	//CPPUNIT_ASSERT(!r1.isClausePassed());

}

void ParentStarClauseTest::testParentStarSynFixedPass() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("5");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	/**
	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 2);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "2");
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "4");
	**/
}

void ParentStarClauseTest::testParentStarSynFixedFail() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("1");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	//Results r1 = m1->evaluate();
	//CPPUNIT_ASSERT(!r1.isClausePassed());
}

void ParentStarClauseTest::testParentStarFixedSynPass() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	/**
	Results r1 = m1->evaluate();
	
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 5);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "3");
	CPPUNIT_ASSERT(r1.getSinglesResults().at(1) == "4");
	CPPUNIT_ASSERT(r1.getSinglesResults().at(2) == "5");
	CPPUNIT_ASSERT(r1.getSinglesResults().at(3) == "6");
	CPPUNIT_ASSERT(r1.getSinglesResults().at(4) == "7");
	**/
}

void ParentStarClauseTest::testParentStarFixedSynPassWithWhile() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());

	/**
	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "4");
	**/
}

void ParentStarClauseTest::testParentStarFixedSynFail() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	/**
	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
	**/
}

void ParentStarClauseTest::testParentStarFixedSynFailWithWhile() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("4");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());

	/**
	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
	**/
}

void ParentStarClauseTest::testParentStarSynSynPass() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());
	/**
	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getPairResults().size() == 7);
	pair<string, string> pair0("2","3");
	pair<string, string> pair1("2","4");
	pair<string, string> pair2("2","5");
	pair<string, string> pair3("2","6");
	pair<string, string> pair4("2","7");
	pair<string, string> pair5("4","5");
	pair<string, string> pair6("4","6");
	CPPUNIT_ASSERT(r1.getPairResults().at(0) == pair0);
	CPPUNIT_ASSERT(r1.getPairResults().at(1) == pair1);
	CPPUNIT_ASSERT(r1.getPairResults().at(2) == pair2);
	CPPUNIT_ASSERT(r1.getPairResults().at(3) == pair3);
	CPPUNIT_ASSERT(r1.getPairResults().at(4) == pair4);
	CPPUNIT_ASSERT(r1.getPairResults().at(5) == pair5);
	CPPUNIT_ASSERT(r1.getPairResults().at(6) == pair6);
	**/

}

void ParentStarClauseTest::testParentStarSynSynPassWithWhile() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());

	/**
	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getPairResults().size() == 1);
	pair<string, string> pair0("2","4");
	CPPUNIT_ASSERT(r1.getPairResults().at(0) == pair0);
	**/
}

void ParentStarClauseTest::testParentStarInvalid() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_ASSIGN);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(!m1->isValid());
}