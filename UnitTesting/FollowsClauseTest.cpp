#include <cppunit/config/SourcePrefix.h>
#include "FollowsClauseTest.h"
#include "../SPA/Follows.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/FollowsClause.h"
#include "../SPA/Utils.h"

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

void FollowsClauseTest::testIsFollows() { 

	FollowsClause fol = *new FollowsClause();
	fol.setFirstArg("s");
	fol.setSecondArg("1");
	
	fol.setFirstArgFixed(true);
	fol.setSecondArgFixed(false);

	fol.setFirstArgType(stringconst::ARG_STATEMENT);
	fol.setSecondArgType(stringconst::ARG_STATEMENT);

	//string gtFollows = fol.getFollows("1", stringconst::ARG_STATEMENT);
	string gtFollowsBy = fol.getFollowedBy("4", stringconst::ARG_STATEMENT);

	//cout << "JIAWEI: ";
	//cout << gtFollowsBy;
	//cout << " end";
	
	//CPPUNIT_ASSERT(fol.isFollows("1", "2"));
	//CPPUNIT_ASSERT(resObj.getSinglesResults().at(0) == res);
	//CPPUNIT_ASSERT(!fol.isFollows(2, 1));

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