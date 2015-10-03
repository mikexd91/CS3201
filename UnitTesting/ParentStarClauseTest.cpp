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
#include "../SPA/SuchThatClauseBuilder.h"

#include <iostream>
#include <string>
#include <algorithm>

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
	unordered_set<string> mods1(modifiesArray1, modifiesArray1 + 1);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(assg1);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(WHILE_STMT_);
	stmt2->setFollowsBefore(1);
	string modifiesArray2[] = {"k", "i", "j", "b"};
	unordered_set<string> mods2(modifiesArray2, modifiesArray2 + 4);
	string usesArray2[] = {"i", "j"};
	unordered_set<string> uses2(usesArray2, usesArray2 + 2);
	stmt2->setModifies(mods2);
	stmt2->setUses(uses2);
	stmt2->setTNodeRef(while1);
	int children2[] = {3, 4, 7};
	stmt2->setChildren(unordered_set<int>(children2, children2+3));
	int childrenStar2[] = {3, 4, 5, 6, 7};
	stmt2->setChildrenStar(unordered_set<int>(childrenStar2, childrenStar2+5));
	stable->addStmt(stmt2);

	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsAfter(4);
	unordered_set<string> mods3 = unordered_set<string>();
	mods3.emplace("k");
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg2);
	stmt3->setParent(2);
	int parentStar3[] = {2};
	stmt3->setParentStar(unordered_set<int>(parentStar3, parentStar3+1));
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(WHILE_STMT_);
	stmt4->setFollowsBefore(3);
	unordered_set<string> mods4 = unordered_set<string>();
	mods4.emplace("i");
	mods4.emplace("j");
	unordered_set<string> uses4 = unordered_set<string>();
	uses4.emplace("j");
	stmt4->setModifies(mods4);
	stmt4->setUses(uses4);
	stmt4->setTNodeRef(while2);
	stmt4->setParent(2);
	int parentStar4[] = {2};
	stmt4->setParentStar(unordered_set<int>(parentStar4, parentStar4+1));
	int children4[] = { 5, 6 };
	stmt4->setChildren(unordered_set<int>(children4, children4+2));
	int childrenStar4[] = { 5, 6 };
	stmt4->setChildrenStar(unordered_set<int>(childrenStar4, childrenStar4+2));
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsAfter(6);
	unordered_set<string> mods5= unordered_set<string>();
	mods5.emplace("i");
	stmt5->setModifies(mods5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(4);
	int parentStar5[] = {2, 4};
	stmt5->setParentStar(unordered_set<int>(parentStar5, parentStar5+2));
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	unordered_set<string> mods6= unordered_set<string>();
	mods6.emplace("j");
	stmt6->setModifies(mods6);
	stmt6->setTNodeRef(assg4);
	stmt6->setParent(4);
	int parentStar6[] = {2, 4};
	stmt6->setParentStar(unordered_set<int>(parentStar6, parentStar6+2));
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(ASSIGN_STMT_);
	stmt7->setFollowsBefore(4);
	unordered_set<string> mods7= unordered_set<string>();
	mods7.emplace("b");
	stmt7->setModifies(mods7);
	stmt7->setTNodeRef(assg5);
	stmt7->setParent(2);
	int parentStar7[] = {2};
	stmt6->setParentStar(unordered_set<int>(parentStar7, parentStar7+1));
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
	Result res = Result();
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "2");
	parentStarBuilder->setArgFixed(1, true);
	parentStarBuilder->setArgType(1, ARG_STATEMENT);
	parentStarBuilder->setArg(2, "5");
	parentStarBuilder->setArgFixed(2, true);
	parentStarBuilder->setArgType(2, ARG_STATEMENT);
	ParentStarClause* m1 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

void ParentStarClauseTest::testParentStarFixedFixedFail() {
	Result res = Result();
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "4");
	parentStarBuilder->setArgFixed(1, true);
	parentStarBuilder->setArgType(1, ARG_STATEMENT);
	parentStarBuilder->setArg(2, "7");
	parentStarBuilder->setArgFixed(2, true);
	parentStarBuilder->setArgType(2, ARG_STATEMENT);
	ParentStarClause* m1 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);
}

void ParentStarClauseTest::testParentStarFixedFixedFailSameStmt() {
	Result res = Result();
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "4");
	parentStarBuilder->setArgFixed(1, true);
	parentStarBuilder->setArgType(1, ARG_STATEMENT);
	parentStarBuilder->setArg(2, "4");
	parentStarBuilder->setArgFixed(2, true);
	parentStarBuilder->setArgType(2, ARG_STATEMENT);
	ParentStarClause* m1 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(!result);

}

void ParentStarClauseTest::testParentStarGenericGenericPass() {
	// Parent*(_,_)
	Result res = Result();
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "_");
	parentStarBuilder->setArgFixed(1, false);
	parentStarBuilder->setArgType(1, ARG_GENERIC);
	parentStarBuilder->setArg(2, "_");
	parentStarBuilder->setArgFixed(2, false);
	parentStarBuilder->setArgType(2, ARG_GENERIC);
	ParentStarClause* m1 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 0);
}

//Parent*(_,s)
void ParentStarClauseTest::testParentStarGenericStatementPass() {
	Result res = Result();
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "_");
	parentStarBuilder->setArgFixed(1, false);
	parentStarBuilder->setArgType(1, ARG_GENERIC);
	parentStarBuilder->setArg(2, "s");
	parentStarBuilder->setArgFixed(2, false);
	parentStarBuilder->setArgType(2, ARG_STATEMENT);
	ParentStarClause* m1 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 5);
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 5);
	CPPUNIT_ASSERT(s.find("3") != s.end());
	CPPUNIT_ASSERT(s.find("4") != s.end());
	CPPUNIT_ASSERT(s.find("5") != s.end());
	CPPUNIT_ASSERT(s.find("6") != s.end());
	CPPUNIT_ASSERT(s.find("7") != s.end());
}

//Parent*(_,w)
void ParentStarClauseTest::testParentStarGenericWhilePass() {

	Result res = Result();
	SuchThatClauseBuilder* parentStarBuilder = new SuchThatClauseBuilder(PARENTSTAR_);
	parentStarBuilder->setArg(1, "_");
	parentStarBuilder->setArgFixed(1, false);
	parentStarBuilder->setArgType(1, ARG_GENERIC);
	parentStarBuilder->setArg(2, "w");
	parentStarBuilder->setArgFixed(2, false);
	parentStarBuilder->setArgType(2, ARG_WHILE);
	ParentStarClause* m1 = (ParentStarClause*) parentStarBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 1);
	unordered_set<string> s = res.getSyn("w");
	CPPUNIT_ASSERT(s.size() == 1);
	CPPUNIT_ASSERT(s.find("4") != s.end());
}

/**
void ParentStarClauseTest::testParentStarSynFixedPass() {
	Result res = Result();
	SuchThatClauseBuilder* parentBuilder = new SuchThatClauseBuilder(PARENT_);
	parentBuilder->setArg(1, "s");
	parentBuilder->setArgFixed(1, false);
	parentBuilder->setArgType(1, ARG_STATEMENT);
	parentBuilder->setArg(2, "5");
	parentBuilder->setArgFixed(2, true);
	parentBuilder->setArgType(2, ARG_STATEMENT);
	ParentClause* m1 = (ParentClause*) parentBuilder->build();
	CPPUNIT_ASSERT(m1->isValid());

	bool result = m1->evaluate(&res);
	CPPUNIT_ASSERT(result);
	CPPUNIT_ASSERT(res.getResultTableSize() == 2);
	CPPUNIT_ASSERT(res.isSynPresent("s"));
	unordered_set<string> s = res.getSyn("s");
	CPPUNIT_ASSERT(s.size() == 2);
	CPPUNIT_ASSERT(s.find("2") != s.end());
	CPPUNIT_ASSERT(s.find("4") != s.end());
}


void ParentStarClauseTest::testParentStarSynFixedPassWithGeneric() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("_");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_GENERIC);
	m1->setSecondArg("5");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	
	Results r1 = m1->evaluate();
	vector<string> singleResults = r1.getSinglesResults();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 0);
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

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
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

	Results r1 = m1->evaluate();
	vector<string> singleResults = r1.getSinglesResults();

	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 5);
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "3") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "4") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "5") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "6") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "7") != singleResults.end());
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

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "4");
}

void ParentStarClauseTest::testParentStarFixedSynPassWithGeneric() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("_");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	vector<string> singleResults = r1.getSinglesResults();

	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 0);
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

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
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

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
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

	Results r1 = m1->evaluate();
	vector<pair<string, string>> pairResults = r1.getPairResults();

	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getPairResults().size() == 7);
	pair<string, string> pair0("2","3");
	pair<string, string> pair1("2","4");
	pair<string, string> pair2("2","5");
	pair<string, string> pair3("2","6");
	pair<string, string> pair4("2","7");
	pair<string, string> pair5("4","5");
	pair<string, string> pair6("4","6");
	pair<string, string> pair7("2","2");
	pair<string, string> pair8("4","4");
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair0) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair1) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair2) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair3) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair4) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair5) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair6) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair7) == pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair8) == pairResults.end());

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

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getPairResults().size() == 1);
	pair<string, string> pair0("2","4");
	CPPUNIT_ASSERT(r1.getPairResults().at(0) == pair0);
}
**/

/**
void ParentStarClauseTest::testParentStarInvalid() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_ASSIGN);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
}

void ParentStarClauseTest::testParentStarStmtOverflow() {
	ParentStarClause* m1 = new ParentStarClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("-100");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
}
**/