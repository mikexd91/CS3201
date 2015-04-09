#include <cppunit/config/SourcePrefix.h>
#include "PatternAssgClauseTest.h"
#include "../SPA/PatternAssgClause.h"
#include "../SPA/AST.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"

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
	set<string> uses1 = set<string>();
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
	set<string> uses2 = set<string>();
	uses2.emplace(jvar);
	stmt2->setModifies(uses2);
	stmt2->setTNodeRef(assg2);
	stable->addStmt(stmt2);
	
	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(ASSIGN_STMT_);
	stmt3->setFollowsBefore(2);
	string kvar = "k";
	set<string> uses3 = set<string>();
	uses3.emplace(kvar);
	stmt3->setModifies(uses3);
	stmt3->setTNodeRef(assg3);
	stable->addStmt(stmt3);
}

void PatternAssgClauseTest::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PatternAssgClauseTest );

void PatternAssgClauseTest::evaluateVarWildExprWild() {
	
	PatternAssgClause* p1 = new PatternAssgClause("a");
	p1->setVar("_");
	p1->setVarFixed(true);
	p1->setExpression("_");
	CPPUNIT_ASSERT(p1->isValid());
	
	Results r1 = p1->evaluate();
	string syn1 = "a";

	//cout << r1.getFirstClauseSyn() << endl;
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 3);
	
	vector<string> v = r1.getSinglesResults();
	for (int i = 0; i < v.size(); i++) {
		long long num = i + 1;
		CPPUNIT_ASSERT(v.at(i) == to_string(num));
	}

	return;
}

void PatternAssgClauseTest::evaulateVarWildExpr() {
	
	PatternAssgClause* p1 = new PatternAssgClause("a");
	p1->setVar("_");
	p1->setVarFixed(true);
	p1->setExpression("_\"1 2 +\"_");
	CPPUNIT_ASSERT(p1->isValid());

	Results r1 = p1->evaluate();
	
	string syn1 = "a";
	long long num = 1;

	cout << r1.getFirstClauseSyn() << endl;
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == to_string(num));
	
	return;
}

void PatternAssgClauseTest::evaluateVarFixedExprWild() {
	
	PatternAssgClause* p1 = new PatternAssgClause("a");
	p1->setVar("i");
	p1->setVarFixed(true);
	p1->setExpression("_");
	CPPUNIT_ASSERT(p1->isValid());

	Results r1 = p1->evaluate();
	
	string syn1 = "a";
	long long num = 1;

	cout << r1.getFirstClauseSyn() << endl;
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == to_string(num));
	
	return;
}

void PatternAssgClauseTest::evaluateVarFixedExpr() {
	
	PatternAssgClause* p1 = new PatternAssgClause("a");
	p1->setVar("i");
	p1->setVarFixed(true);
	p1->setExpression("_\"1 2 +\"_");
	CPPUNIT_ASSERT(p1->isValid());

	Results r1 = p1->evaluate();
	
	string syn1 = "a";
	long long num = 1;

	cout << r1.getFirstClauseSyn() << endl;
	CPPUNIT_ASSERT(r1.isClausePassed());
	CPPUNIT_ASSERT(r1.getFirstClauseSyn() == syn1);
	CPPUNIT_ASSERT(r1.getSinglesResults().size() == 1);
	CPPUNIT_ASSERT(r1.getSinglesResults().at(0) == to_string(num));

	PatternAssgClause* p2 = new PatternAssgClause("a");
	p2->setVar("i");
	p2->setVarFixed(true);
	p2->setExpression("_\"3 4 +\"_");
	CPPUNIT_ASSERT(p2->isValid());

	Results r2 = p2->evaluate();

	CPPUNIT_ASSERT(!r2.isClausePassed());
	
	return;
}