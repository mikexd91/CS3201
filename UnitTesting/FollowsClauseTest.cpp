#include <cppunit/config/SourcePrefix.h>
#include "FollowsClauseTest.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/Utils.h"
#include "../SPA/StmtTable.h"

#include <iostream>
#include <string>

using namespace std;

void FollowsClauseTest::setUp() {
	/* testing this source
	procedure zumba {
		i = 1;	//1
		j = 2;	//2
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
	VarNode* i = new VarNode("i");
	ConstNode* one = new ConstNode("1");
	assg1->linkVarNode(i);
	assg1->linkExprNode(one);
	procsl->linkStmtNode(assg1);

	AssgNode* assg2 = new AssgNode(2);
	VarNode* j = new VarNode("j");
	ConstNode* two = new ConstNode("2");
	assg2->linkVarNode(j);
	assg2->linkExprNode(two);
	procsl->linkStmtNode(assg2);

	AssgNode* assg3 = new AssgNode(3);
	VarNode* k = new VarNode("k");
	ConstNode* three = new ConstNode("3");
	assg3->linkVarNode(j);
	assg3->linkExprNode(three);
	procsl->linkStmtNode(assg3);

	ast->addProcNode(proc);

	// to set up the stmttable manually
	StmtTable* stable = StmtTable::getInstance();

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(ASSIGN_STMT_);
	stmt1->setFollowsAfter(2);
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(ASSIGN_STMT_);
	stmt2->setFollowsBefore(1);
	stmt2->setFollowsAfter(3);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2);
	stable->addStmt(stmt3);
}

void FollowsClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( FollowsClauseTest );

// Test argument-type combinations of Follows(a,b) where a and b are unfixed
void FollowsClauseTest::testIsFollows() { 
	// Test Follows(a1, a2) where a1 and a2 are both assign
	FollowsClause fol = *new FollowsClause();
	fol.setFirstArg("a1");
	fol.setSecondArg("a2");
	
	fol.setFirstArgFixed(false);
	fol.setSecondArgFixed(false);

	fol.setFirstArgType(stringconst::ARG_ASSIGN);
	fol.setSecondArgType(stringconst::ARG_ASSIGN);
	
	Results r = fol.evaluate();

	string res1 = "1";
	string res2 = "2";
	string res3 = "3";

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

	CPPUNIT_ASSERT(r7.getSinglesResults().size() == 2);
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