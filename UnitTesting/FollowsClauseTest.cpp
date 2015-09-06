#include <cppunit/config/SourcePrefix.h>
#include "FollowsClauseTest.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/Utils.h"
#include "../SPA/StmtTable.h"
#include "boost\unordered_set.hpp"

#include <string>

using namespace std;
using boost::unordered_set;

void FollowsClauseTest::setUp() {
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
	unordered_set<string> mods1 = unordered_set<string>();
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
	unordered_set<string> mods2 = unordered_set<string>();
	mods2.emplace(jvar);
	stmt2->setModifies(mods2);
	stmt2->setTNodeRef(assg2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2);
	string kvar = "k";
	unordered_set<string> mods3 = unordered_set<string>();
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
	unordered_set<string> mods4 = unordered_set<string>();
	mods4.emplace(wvar);
	stmt4->setModifies(mods4);
	unordered_set<string> uses4 = unordered_set<string>();
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
	unordered_set<string> mods5 = unordered_set<string>();
	mods5.emplace(xvar);
	stmt5->setModifies(mods5);
	unordered_set<string> uses5 = unordered_set<string>();
	uses5.emplace(wvar);
	uses5.emplace(kvar);
	stmt5->setUses(uses5);
	stmt5->setTNodeRef(assg5);
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(ASSIGN_STMT_);
	stmt6->setFollowsBefore(5);
	unordered_set<string> mods6 = unordered_set<string>();
	mods6.emplace(ivar);
	stmt6->setModifies(mods6);
	unordered_set<string> uses6 = unordered_set<string>();
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

void FollowsClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( FollowsClauseTest );





// Test argument-type combinations of Follows(a,b) where a and b are unfixed
void FollowsClauseTest::testIsFollows() { 
	// Test Follows(a1, a2) where a1 and a2 are both assign
	Results* result = new Results();
	FollowsClause* fol = new FollowsClause();
	fol->setFirstArg("a1");
	fol->setSecondArg("a2");
	
	fol->setFirstArgFixed(false);
	fol->setSecondArgFixed(false);

	fol->setFirstArgType(stringconst::ARG_ASSIGN);
	fol->setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r = fol->evaluate();

	string res1 = "1";
	string res2 = "2";
	string res3 = "3";

	/*
	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getPairResults().size() == 2);
	CPPUNIT_ASSERT(r.getPairResults().at(1).first == res2);
	CPPUNIT_ASSERT(r.getPairResults().at(1).second == res3);
	
	// Test Follows(a1, a1) where a1 is assign
	fol.setFirstArg("a1");
	fol.setSecondArg("a1");

	fol.setFirstArgType(stringconst::ARG_ASSIGN);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);

	Results r2 = fol.evaluate();

	CPPUNIT_ASSERT(!r2.isClausePassed());
	CPPUNIT_ASSERT(r2.getPairResults().size() == 0);
	
	// Test Follows(a1, s1) where a1 is assign and s1 is statement
	fol.setFirstArg("a1");
	fol.setSecondArg("s1");

	fol.setFirstArgType(stringconst::ARG_ASSIGN);
	fol.setSecondArgType(stringconst::ARG_STATEMENT);

	Results r3 = fol.evaluate();

	CPPUNIT_ASSERT(r3.isClausePassed());
	CPPUNIT_ASSERT(r3.getPairResults().size() == 2);
	CPPUNIT_ASSERT(r3.getPairResults().at(1).first == res2);
	CPPUNIT_ASSERT(r3.getPairResults().at(1).second == res3);

	// Test Follows(s1, a1) where a1 is assign and s1 is statement
	fol.setFirstArg("s1");
	fol.setSecondArg("a1");

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);

	Results r4 = fol.evaluate();

	CPPUNIT_ASSERT(r4.isClausePassed());
	CPPUNIT_ASSERT(r4.getPairResults().size() == 2);
	CPPUNIT_ASSERT(r4.getPairResults().at(1).first == res1);
	CPPUNIT_ASSERT(r4.getPairResults().at(1).second == res2);

	// Test Follows(s,s) where s is a statement
	fol.setFirstArg("s");
	fol.setSecondArg("s");

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_STATEMENT);

	Results r5 = fol.evaluate();
	CPPUNIT_ASSERT(!r5.isClausePassed());

	// Test Follows(_,_) where _ is a generic statement
	FollowsClause fol2 = *new FollowsClause();

	fol2.setFirstArgFixed(false);
	fol2.setSecondArgFixed(false);

	fol2.setFirstArgType(stringconst::ARG_GENERIC);
	fol2.setSecondArgType(stringconst::ARG_GENERIC);

	Results r6 = fol2.evaluate();
	CPPUNIT_ASSERT(r6.isClausePassed());
	//cout << r6.getPairResults().size() << "lala";

	// test follow (s, _)
	fol2.setFirstArg("s");
	fol2.setFirstArgFixed(false);
	fol2.setSecondArgFixed(false);

	fol2.setFirstArgType(stringconst::ARG_STATEMENT);
	fol2.setSecondArgType(stringconst::ARG_GENERIC);

	Results r7 = fol2.evaluate();
	CPPUNIT_ASSERT(r6.isClausePassed());

	//cout <<	"singles Result size: " << r7.getSinglesResults().size() << endl;
	vector<string> resultSet = r7.getSinglesResults();
	/*for (size_t i = 0; i < resultSet.size(); i++) {
		cout << "result: " << resultSet.at(i) << "!";
	}*/

	//CPPUNIT_ASSERT(r7.getSinglesResults().size() == 2);
}

// Test augument-type combinations of Follows (1, a) where a is unfixed
void FollowsClauseTest::testIsFollows2() {
	FollowsClause fol = *new FollowsClause();
	
	// Test Follows(1, a) where a is an assign type
	fol.setFirstArg("1");
	fol.setSecondArg("a");
	
	fol.setFirstArgFixed(true);
	fol.setSecondArgFixed(false);

	fol.setFirstArgType(stringconst::ARG_ASSIGN);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r = fol.evaluate();

	string res1 = "1";
	string res2 = "2";
	string res3 = "3";

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getSinglesResults().at(0) == res2);

	// Test Follows(2, a) where a is an assign type
	fol.setFirstArg("2");
	fol.setSecondArg("a");

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r1 = fol.evaluate();

	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == res3);

	// Test Follows(2, a) where a is an assign type
	fol.setFirstArg("3");
	fol.setSecondArg("a");

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r2 = fol.evaluate();

	CPPUNIT_ASSERT(!r2.isClausePassed());

	// Test Follows(2,_) where _ is a generic type
	fol.setFirstArg("2");
	fol.setSecondArg("_");

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_GENERIC);

	Results r3 = fol.evaluate();

	CPPUNIT_ASSERT(r3.isClausePassed());

	return ;
}

// Test augument-type combinations of Follows (a, 1) where a is unfixed
void FollowsClauseTest::testIsFollows3() {
	FollowsClause fol = *new FollowsClause();
	
	// Test Follows(a, 1) where a is an assign type
	fol.setFirstArg("a");
	fol.setSecondArg("2");
	
	fol.setFirstArgFixed(false);
	fol.setSecondArgFixed(true);

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r = fol.evaluate();

	string res1 = "1";
	string res2 = "2";
	string res3 = "3";

	CPPUNIT_ASSERT(r.isClausePassed());
	CPPUNIT_ASSERT(r.getSinglesResults().at(0) == res1);

	// Test Follows(a, 1) where a is an assign type
	fol.setFirstArg("a");
	fol.setSecondArg("1");

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r2 = fol.evaluate();

	CPPUNIT_ASSERT(!r2.isClausePassed());

	// Test Follows(_,3) where _ is a generic type
	fol.setFirstArg("_");
	fol.setSecondArg("3");

	fol.setFirstArgType(stringconst::ARG_GENERIC);
	fol.setSecondArgType(stringconst::ARG_STATEMENT);

	Results r3 = fol.evaluate();
	/*
	cout <<	"singles Result size: " << r3.getSinglesResults().size() << endl;
	vector<string> resultSet = r3.getSinglesResults();
	for (size_t i = 0; i < resultSet.size(); i++) {
		cout << "result: " << resultSet.at(i) << "!";
	}
	*/
	CPPUNIT_ASSERT(r3.isClausePassed());
	
	return ;
}

// Test Follows (1, 2)
void FollowsClauseTest::testIsFollows4() {
	FollowsClause fol = *new FollowsClause();
	
	// Test Follows(a, 1) where a is an assign type
	fol.setFirstArg("1");
	fol.setSecondArg("2");
	
	fol.setFirstArgFixed(true);
	fol.setSecondArgFixed(true);

	Results r = fol.evaluate();

	string res1 = "1";
	string res2 = "2";
	string res3 = "3";

	CPPUNIT_ASSERT(r.isClausePassed());

	// Test Follows(1, 3) where a is an assign type
	fol.setFirstArg("1");
	fol.setSecondArg("3");
	
	fol.setFirstArgFixed(true);
	fol.setSecondArgFixed(true);

	Results r2 = fol.evaluate();

	CPPUNIT_ASSERT(!r2.isClausePassed());

	// Test out of bounds
	fol.setFirstArg("-1");
	fol.setSecondArg("0");

	fol.setFirstArgFixed(true);
	fol.setSecondArgFixed(true);

	Results r3 = fol.evaluate();

	CPPUNIT_ASSERT(!r3.isClausePassed());

	return;
}

void FollowsClauseTest::testIsFollowsStar() {
	
	/*Follows fol = *new Follows();
	CPPUNIT_ASSERT(fol.isFollowsStar(1, 2));
	CPPUNIT_ASSERT(fol.isFollowsStar(1, 3));
	CPPUNIT_ASSERT(fol.isFollowsStar(2, 3));
	CPPUNIT_ASSERT(!fol.isFollowsStar(2, 1));
	CPPUNIT_ASSERT(!fol.isFollowsStar(3, 3));*/

	return;
}