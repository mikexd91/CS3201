#include <cppunit/config/SourcePrefix.h>
#include "UsesClauseTest.h"
#include "../SPA/UsesClause.h"
#include "../SPA/AST.h"
#include "../SPA/WhileNode.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"

using namespace stringconst;
using namespace std;

void UsesClauseTest::setUp() {
	/* testing this source

	procedure chocs {
		while i {
			k = 3;
			while j {
				i=1+2;
				j=2+3+i;
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
	VarNode* i3 = new VarNode("i");
	OpNode* plus2_2 = new OpNode("+");
	ConstNode* three2 = new ConstNode("3");
	ConstNode* two2 = new ConstNode("2");
	plus2_2->linkRightNode(three2);
	plus2_2->linkLeftNode(two2);
	plus2_1->linkRightNode(i3);
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
	string usesArray1[] = {"i", "j"};
	set<string> mods1(modifiesArray1, modifiesArray1 + 3);
	set<string> use1(usesArray1, usesArray1 + 2);
	stmt1->setModifies(mods1);
	stmt1->setUses(use1);
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
	string usesArray3[] = {"i", "j"};
	set<string> use3(usesArray3, usesArray3 + 2);
	set<string> mods3(modifiesArray3, modifiesArray3 + 2);
	stmt3->setModifies(mods3);
	stmt3->setUses(use3);
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
	set<string> uses5 = set<string>();
	mods5.emplace(jvar);
	uses5.emplace(ivar);
	stmt5->setModifies(mods5);
	stmt5->setUses(uses5);
	stmt5->setTNodeRef(assg3);
	stmt5->setParent(3);
	stable->addStmt(stmt5);

	// to set up the vartable manually
	VarTable* vtable = VarTable::getInstance();

	Variable* vi = new Variable("i");
	vi->addModifyingStmt(1);
	vi->addModifyingStmt(4);
	vi->addUsingStmt(1);
	vi->addUsingStmt(3);
	vi->addUsingStmt(5);
	vi->addTNode(i1);
	vi->addTNode(i2);
	vtable->addVariable(vi);

	Variable* vj = new Variable("j");
	vj->addModifyingStmt(1);
	vj->addModifyingStmt(3);
	vj->addModifyingStmt(5);
	vj->addUsingStmt(1);
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


void UsesClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( UsesClauseTest );

void UsesClauseTest::testUsesAssignFixedFixedPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("3");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_STATEMENT);
	c->setSecondArg("j");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	CPPUNIT_ASSERT(r.isClausePassed());
}

void UsesClauseTest::testUsesAssignFixedFixedFail() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("4");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_STATEMENT);
	c->setSecondArg("k");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	CPPUNIT_ASSERT(!r.isClausePassed());
}

void UsesClauseTest::testUsesAssignSynFixedPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("a");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_ASSIGN);
	c->setSecondArg("i");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();
	
	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 1);
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "5") != singleResults.end());
}

void UsesClauseTest::testUsesGenericSynFixedPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("_");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_GENERIC);
	c->setSecondArg("i");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();
	
	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 0);
}

void UsesClauseTest::testUsesAssignSynFixedFail() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("a");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_ASSIGN);
	c->setSecondArg("k");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();
	
	CPPUNIT_ASSERT(!r.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 0);
}

void UsesClauseTest::testUsesAssignFixedSynPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("5");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_ASSIGN);
	c->setSecondArg("v");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();
	
	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 1);
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "i") != singleResults.end());
}

void UsesClauseTest::testUsesGenericFixedSynPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("5");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_ASSIGN);
	c->setSecondArg("_");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();
	
	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 1);
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "i") != singleResults.end());
}

void UsesClauseTest::testUsesAssignFixedSynFail() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("4");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_ASSIGN);
	c->setSecondArg("v");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();
	
	CPPUNIT_ASSERT(!r.isClausePassed());
	CPPUNIT_ASSERT(singleResults.size() == 0);
}

void UsesClauseTest::testUsesAssignSynSynPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("a");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_ASSIGN);
	c->setSecondArg("v");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<pair<string,string>> pairResults = r.getPairResults();
	
	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(pairResults.size() == 1);
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("5", "i")) != pairResults.end());
}

void UsesClauseTest::testUsesWhileSynSynPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("w");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_WHILE);
	c->setSecondArg("v");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<pair<string,string>> pairResults = r.getPairResults();

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(pairResults.size() == 4);
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("1", "i")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("1", "j")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("3", "i")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("3", "j")) != pairResults.end());
}

void UsesClauseTest::testUsesFirstGenericPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("_");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_GENERIC);
	c->setSecondArg("v");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getNumOfSyn() == 1); 
	CPPUNIT_ASSERT(singleResults.size() == 2);
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "i") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "j") != singleResults.end());
}

void UsesClauseTest::testUsesSecondGenericPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("s");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_STATEMENT);
	c->setSecondArg("_");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<string> singleResults = r.getSinglesResults();

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getNumOfSyn() == 1); 
	CPPUNIT_ASSERT(singleResults.size() == 3);
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "1") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "3") != singleResults.end());
	CPPUNIT_ASSERT(find(singleResults.begin(), singleResults.end(), "5") != singleResults.end());
}

void UsesClauseTest::testUsesGenericFixedPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("_");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_GENERIC);
	c->setSecondArg("i");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getNumOfSyn() == 0); 
}

void UsesClauseTest::testUsesFixedGenericFail() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("2");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_STATEMENT);
	c->setSecondArg("_");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	CPPUNIT_ASSERT(!r.isClausePassed());
}

void UsesClauseTest::testUsesGenericSynSynPass() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("_");
	c->setFirstArgFixed(false);
	c->setFirstArgType(ARG_GENERIC);
	c->setSecondArg("_");
	c->setSecondArgFixed(false);
	c->setSecondArgType(ARG_GENERIC);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	vector<pair<string,string>> pairResults = r.getPairResults();

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getNumOfSyn() == 2); 
	CPPUNIT_ASSERT(pairResults.size() == 5);
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("1", "i")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("1", "j")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("3", "i")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("3", "j")) != pairResults.end());
	CPPUNIT_ASSERT(find(pairResults.begin(), pairResults.end(), pair<string,string>("5", "i")) != pairResults.end());
}

void UsesClauseTest::testUsesStmtOverflow() {
	UsesClause* c = new UsesClause();
	c->setFirstArg("99");
	c->setFirstArgFixed(true);
	c->setFirstArgType(ARG_STATEMENT);
	c->setSecondArg("j");
	c->setSecondArgFixed(true);
	c->setSecondArgType(ARG_VARIABLE);
	CPPUNIT_ASSERT(c->isValid());

	Results r = c->evaluate();
	CPPUNIT_ASSERT(!r.isClausePassed());
}