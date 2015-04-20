#include <cppunit/config/SourcePrefix.h>
#include "ParentClauseTest.h"
#include "../SPA/ParentClause.h"
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

void ParentClauseTest::setUp() {
	/* testing this source

	procedure chocs {
		while i {
			k = 3;
			while j {
				i=1+2;
				j=2+3+4;
			}
		}	
		}
	*/

	// to set up the ast manually
	AST* ast = AST::getInstance();

	ProcNode* proc = new ProcNode("chocs");
	StmtLstNode* procsl = new StmtLstNode();
	proc->linkStmtLstNode(procsl);

	WhileNode* while1 = new WhileNode(1);
	VarNode* i1 = new VarNode("i");
	while1->linkVarNode(i1);
	StmtLstNode* whilesl1 = new StmtLstNode();
	while1->linkStmtLstNode(whilesl1);
	procsl->linkStmtNode(while1);

	AssgNode* assg1 = new AssgNode(2);
	VarNode* k1 = new VarNode("k");
	assg1->linkVarNode(k1);
	assg1->linkExprNode(new ConstNode("3"));
	procsl->linkStmtNode(assg1);

	WhileNode* while2 = new WhileNode(3);
	VarNode* j1 = new VarNode("j");
	while2->linkVarNode(j1);
	StmtLstNode* whilesl2 = new StmtLstNode();
	while2->linkStmtLstNode(whilesl2);
	whilesl1->linkStmtNode(while1);

	AssgNode* assg2 = new AssgNode(4);
	VarNode* i2 = new VarNode("i");
	assg2->linkVarNode(i2);
	OpNode* plus1 = new OpNode("+");
	ConstNode* one1 = new ConstNode("1");
	ConstNode* two1 = new ConstNode("2");
	plus1->linkLeftNode(one1);
	plus1->linkRightNode(two1);
	assg2->linkExprNode(plus1);
	whilesl2->linkStmtNode(assg2);

	AssgNode* assg3 = new AssgNode(2);
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
	assg3->linkVarNode(j2);
	assg3->linkExprNode(plus2_1);
	procsl->linkStmtNode(assg3);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(WHILE_STMT_);
	string modifiesArray1[] = {"k", "i", "j"};
	set<string> mods1(modifiesArray1, modifiesArray1 + 3);
	stmt1->setModifies(mods1);
	stmt1->setTNodeRef(while1);
	int children1[] = {2, 3};
	stmt1->setChildren(set<int>(children1, children1+2));
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsAfter(3);
	string kvar = "k";
	set<string> mods2 = set<string>();
	mods2.emplace(kvar);
	stmt2->setModifies(mods2);
	stmt2->setTNodeRef(assg1);
	stmt2->setParent(1);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(WHILE_STMT_);
	stmt3->setFollowsBefore(2);
	string modifiesArray3[] = {"i", "j"};
	set<string> mods3(modifiesArray1, modifiesArray1 + 2);
	stmt3->setModifies(mods3);
	stmt3->setTNodeRef(assg3);
	stmt3->setParent(1);
	int children3[] = {4, 5};
	stmt3->setChildren(set<int>(children3, children3+2));
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(ASSIGN_STMT_);
	stmt4->setFollowsAfter(5);
	string ivar = "i";
	set<string> mods4 = set<string>();
	mods4.emplace(ivar);
	stmt4->setModifies(mods4);
	stmt4->setTNodeRef(assg2);
	stmt4->setParent(3);
	stable->addStmt(stmt4);

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(ASSIGN_STMT_);
	stmt5->setFollowsBefore(4);
	string jvar = "j";
	set<string> mods5 = set<string>();
	mods5.emplace(jvar);
	stmt5->setModifies(mods5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(3);
	stable->addStmt(stmt5);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(1);
	vi->addModifyingStmt(4);
	vi->addUsingStmt(1);
	vi->addTNode(i1);
	vi->addTNode(i2);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(1);
	vj->addModifyingStmt(3);
	vj->addModifyingStmt(5);
	vj->addUsingStmt(3);
	vj->addTNode(j1);
	vj->addTNode(j2);
	vtable->addVariable(vj);

	Variable* vk = new Variable("k");
	vk->addModifyingStmt(1);
	vk->addModifyingStmt(2);
	vk->addTNode(k1);
	vtable->addVariable(vk);
}

void ParentClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( ParentClauseTest );


void ParentClauseTest::testParentFixedFixedPass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("2");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	r1.isClausePassed();
	//CPPUNIT_ASSERT(r1.isClausePassed());
}

void ParentClauseTest::testParentFixedFixedFail() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("3");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());

}

void ParentClauseTest::testParentSynFixedPass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("4");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "s");
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "3");
}

void ParentClauseTest::testParentSynFixedFail() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("1");
	m1->setSecondArgFixed(true);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 0);
}

void ParentClauseTest::testParentFixedSynPass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "s");
	
	vector<string> results = r1.getSinglesResults();
	CPPUNIT_ASSERT(results.size() == 2);
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "2") != results.end());
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "3") != results.end());
}

void ParentClauseTest::testParentFixedSynPassWithWhile() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("1");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("w");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_WHILE);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "w");
	vector<string> results = r1.getSinglesResults();
	CPPUNIT_ASSERT(results.size() == 1);
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "3") != results.end());
}

void ParentClauseTest::testParentFixedSynFail() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("2");
	m1->setFirstArgFixed(true);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(!r1.isClausePassed());
}

void ParentClauseTest::testParentFixedSynFailWithWhile() {
	ParentClause* m1 = new ParentClause();
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

void ParentClauseTest::testParentSynSynPass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s1");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	vector<pair<string, string>> resultVector = r1.getPairResults();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "s1");
	CPPUNIT_ASSERT(r1.getSecondClauseSyn() == "s2");
	CPPUNIT_ASSERT(resultVector.size() == 4);
	pair<string, string> pair0("1","2");
	pair<string, string> pair1("1","3");
	pair<string, string> pair2("3","4");
	pair<string, string> pair3("3","5");
	CPPUNIT_ASSERT(find(resultVector.begin(), resultVector.end(), pair0) != resultVector.end());
	CPPUNIT_ASSERT(find(resultVector.begin(), resultVector.end(), pair1) != resultVector.end());
	CPPUNIT_ASSERT(find(resultVector.begin(), resultVector.end(), pair2) != resultVector.end());
	CPPUNIT_ASSERT(find(resultVector.begin(), resultVector.end(), pair3) != resultVector.end());
}

void ParentClauseTest::testParentFirstUnderscorePass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("_");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_GENERIC);
	m1->setSecondArg("s2");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_STATEMENT);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 4);
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "s2");
	vector<string> results = r1.getSinglesResults();
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "2") != results.end());
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "3") != results.end());
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "4") != results.end());
	CPPUNIT_ASSERT(find(results.begin(), results.end(), "5") != results.end());
}

void ParentClauseTest::testParentSecondUnderscorePass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("s2");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_STATEMENT);
	m1->setSecondArg("_");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 1);
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "s2");
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 2);
	/*CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == "1");
	CPPUNIT_ASSERT(r1.getSinglesResults().at(1) == "3");*/
}

void ParentClauseTest::testParentBothUnderscorePass() {
	ParentClause* m1 = new ParentClause();
	m1->setFirstArg("_");
	m1->setFirstArgFixed(false);
	m1->setFirstArgType(ARG_GENERIC);
	m1->setSecondArg("_");
	m1->setSecondArgFixed(false);
	m1->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(m1->isValid());

	Results r1 = m1->evaluate();
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getNumOfSyn() == 0);
}


void ParentClauseTest::testParentSynSynPassWithWhile() {
	ParentClause* m1 = new ParentClause();
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
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == "s1");
	CPPUNIT_ASSERT(r1.getSecondClauseSyn() == "s2");
	pair<string, string> pair0("1","3");
	CPPUNIT_ASSERT(r1.getPairResults().at(0) == pair0);
}