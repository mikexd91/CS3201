#include <cppunit/config/SourcePrefix.h>
#include "PQLIntegration.h"
#include "../SPA/PQLController.h"
#include "../SPA/AST.h"
#include "../SPA/WhileNode.h"
#include "../SPA/AssgNode.h"
#include "../SPA/ConstNode.h"
#include "../SPA/OpNode.h"
#include "../SPA/StmtTable.h"
#include "../SPA/VarTable.h"
#include "../SPA/Utils.h"
#include "../SPA/ConstTable.h"
#include "../SPA/IfNode.h"

#include <iostream>
#include <string>
#include <algorithm>
#include <boost\foreach.hpp>
#include <boost\unordered_set.hpp>
#include "../SPA/ProcTable.h"
#include "../SPA/CFG.h"
#include "../SPA/CallNode.h"

using namespace stringconst;
using namespace std;
using boost::unordered_set;

void PQLIntegration::setUp() {

	/* testing this source

	procedure Pizza {
		if good then {						//1
			eaten = eaten + 1;				//2
		} else {							
			drink = more * (beer - water);	//3
			call YourMom;					//4
		}
	}

	procedure YourMom {
		while nagging {						//5
			nagging = nagging + 1;			//6
			while i {						//7
				mom = nagging;				//8
			}
		}
		full = drink + eaten;				//9
	}

	*/

//-------------  SET UP AST -------------//
	AST* ast = AST::getInstance();

// procedure Pizza
	ProcNode* pizzaProc = new ProcNode("Pizza");
	ast->addProcNode(pizzaProc);
	StmtLstNode* pizzasl = new StmtLstNode();
	pizzaProc->linkStmtLstNode(pizzasl);
	
	IfNode* if1 = new IfNode(1);
	pizzasl->linkStmtNode(if1);
	VarNode* good1 = new VarNode("good");
	if1->linkVarNode(good1);
	StmtLstNode* thensl1 = new StmtLstNode();
	if1->linkThenStmtLstNode(thensl1);
	StmtLstNode* elsesl1 = new StmtLstNode();
	if1->linkElseStmtLstNode(elsesl1);

	AssgNode* assg2 = new AssgNode(2);
	thensl1->linkStmtNode(assg2);
	VarNode* eaten1 = new VarNode("eaten");
	assg2->linkVarNode(eaten1);
	OpNode* plus1 = new OpNode("+");
	assg2->linkExprNode(plus1);
	VarNode* eaten2 = new VarNode("eaten");
	plus1->linkLeftNode(eaten2);
	ConstNode* one1 = new ConstNode("1");
	plus1->linkRightNode(one1);

	AssgNode* assg3 = new AssgNode(3);
	elsesl1->linkStmtNode(assg3);
	VarNode* drink1 = new VarNode("drink");
	assg3->linkVarNode(drink1);
	OpNode* times1 = new OpNode("*");
	assg3->linkExprNode(times1);
	VarNode* more1 = new VarNode("more");
	times1->linkLeftNode(more1);
	OpNode* minus1 = new OpNode("-");
	times1->linkRightNode(minus1);
	VarNode* beer1 = new VarNode("beer");
	minus1->linkLeftNode(beer1);
	VarNode* water1 = new VarNode("water");
	minus1->linkRightNode(water1);

	CallNode* call4 = new CallNode(4, "YourMom");
	pizzasl->linkStmtNode(call4);

// procedure YourMom
	ProcNode* yourmomProc = new ProcNode("YourMom");
	ast->addProcNode(yourmomProc);
	StmtLstNode* yourmomsl = new StmtLstNode();
	yourmomProc->linkStmtLstNode(yourmomsl);
	
	WhileNode* while5 = new WhileNode(5);
	yourmomsl->linkStmtNode(while5);
	VarNode* nagging1 = new VarNode("nagging");
	while5->linkVarNode(nagging1);
	StmtLstNode* while5sl = new StmtLstNode();
	while5->linkStmtLstNode(while5sl);

	AssgNode* assg6 = new AssgNode(6);
	while5sl->linkStmtNode(assg6);
	VarNode* nagging2 = new VarNode("nagging");
	assg6->linkVarNode(nagging2);
	OpNode* plus2 = new OpNode("+");
	assg6->linkExprNode(plus2);
	VarNode* nagging3 = new VarNode("nagging");
	plus2->linkLeftNode(nagging3);
	ConstNode* one2 = new ConstNode("1");
	plus2->linkRightNode(one2);

	WhileNode* while7 = new WhileNode(7);
	while5sl->linkStmtNode(while7);
	VarNode* i1 = new VarNode("i");
	while7->linkVarNode(i1);
	StmtLstNode* while7sl = new StmtLstNode();
	while7->linkStmtLstNode(while7sl);

	AssgNode* assg8 = new AssgNode(8);
	while7sl->linkStmtNode(assg8);
	VarNode* mom1 = new VarNode("mom");
	assg8->linkVarNode(mom1);
	VarNode* nagging4 = new VarNode("nagging");
	assg8->linkExprNode(nagging4);

	AssgNode* assg9 = new AssgNode(9);
	yourmomsl->linkStmtNode(assg9);
	VarNode* full1 = new VarNode("full");
	assg9->linkVarNode(full1);
	OpNode* plus3 = new OpNode("+");
	assg9->linkExprNode(plus3);
	VarNode* drink2 = new VarNode("drink");
	plus3->linkLeftNode(drink2);
	VarNode* eaten3 = new VarNode("eaten");
	plus3->linkRightNode(eaten3);
//-------------  END OF AST -------------//


//-------------  SET UP STMT ------------//
	StmtTable* stable = StmtTable::getInstance();

	//--- stmt sample ---//
	//Statement* stmt1 = new Statement();
	//stmt1->setStmtNum(1);
	//stmt1->setType(IF_STMT_);
	//stmt1->setTNodeRef();
	//stmt1->setCalls();
	//string stmt1usesarr[] = {"eaten", "more"};
	//stmt1->setUses(Statement::UsesSet(stmt1usesarr, stmt1usesarr+len));
	//string stmt1modsarr[] = {"eaten"};
	//stmt1->setModifies(Statement::ModifiesSet(stmt1modsarr, stmt1modsarr+len));
	//stmt1->setFollowsAfter();
	//stmt1->setFollowsBefore();
	//stmt1->setFollowsStarAfter();
	//stmt1->setFollowsStarBefore();
	//stmt1->setParent();
	//stmt1->setChildren();
	//stmt1->setParentStar();
	//stmt1->setChildrenStar();
	//stmt1->setNext();
	//stmt1->setPrev();
	//stable->addStmt(stmt1);

	Statement* stmt1 = new Statement();
	stmt1->setStmtNum(1);
	stmt1->setType(NodeType::IF_STMT_);
	stmt1->setTNodeRef(if1);
	//stmt1->setCalls();
	string stmt1UsesArr[] = {"eaten", "more", "beer", "water", "nagging", "i", "drink"};
	stmt1->setUses(Statement::UsesSet(stmt1UsesArr, stmt1UsesArr+ sizeof(stmt1UsesArr)/sizeof(*stmt1UsesArr)));
	string stmt1ModsArr[] = {"eaten", "drink", "nagging", "mom", "full"};
	stmt1->setModifies(Statement::ModifiesSet(stmt1ModsArr, stmt1ModsArr+ sizeof(stmt1ModsArr)/sizeof(*stmt1ModsArr)));
	//stmt1->setFollowsAfter();
	//stmt1->setFollowsBefore();
	//stmt1->setFollowsStarAfter();
	//stmt1->setFollowsStarBefore();
	//stmt1->setParent();
	int stmt1ChildrenArr[] = {2, 3, 4};
	stmt1->setChildren(Statement::ChildrenSet(stmt1ChildrenArr, stmt1ChildrenArr+ sizeof(stmt1ChildrenArr)/sizeof(*stmt1ChildrenArr)));
	//stmt1->setParentStar();
	int stmt1ChildrenStarArr[] = {2, 3, 4};
	stmt1->setChildrenStar(Statement::ChildrenSet(stmt1ChildrenStarArr, stmt1ChildrenStarArr+ sizeof(stmt1ChildrenStarArr)/sizeof(*stmt1ChildrenStarArr)));
	int stmt1NextArr[] = {2, 3};
	stmt1->setNext(Statement::NextSet(stmt1NextArr, stmt1NextArr+ sizeof(stmt1NextArr)/sizeof(*stmt1NextArr)));
	//stmt1->setPrev();
	stable->addStmt(stmt1);

	Statement* stmt2 = new Statement();
	stmt2->setStmtNum(2);
	stmt2->setType(NodeType::ASSIGN_STMT_);
	stmt2->setTNodeRef(assg2);
	//stmt2->setCalls();
	string stmt2UsesArr[] = {"eaten"};
	stmt2->setUses(Statement::UsesSet(stmt2UsesArr, stmt2UsesArr+ sizeof(stmt2UsesArr)/sizeof(*stmt2UsesArr)));
	string stmt2ModsArr[] = {"eaten"};
	stmt2->setModifies(Statement::ModifiesSet(stmt2ModsArr, stmt2ModsArr+ sizeof(stmt2ModsArr)/sizeof(*stmt2ModsArr)));
	//stmt2->setFollowsAfter();
	//stmt2->setFollowsBefore();
	//stmt2->setFollowsStarAfter();
	//stmt2->setFollowsStarBefore();
	stmt2->setParent(1);
	//int stmt2ChildrenArr[] = {2, 3, 4};
	//stmt2->setChildren(Statement::ChildrenSet(stmt2ChildrenArr, stmt2ChildrenArr+ 3));
	int stmt2ParentStarArr[] = {1};
	stmt2->setParentStar(Statement::ParentStarSet(stmt2ParentStarArr, stmt2ParentStarArr+ sizeof(stmt2ParentStarArr)/sizeof(*stmt2ParentStarArr)));
	//int stmt2ChildrenStarArr[] = {2, 3, 4};
	//stmt2->setChildrenStar(Statement::ChildrenSet(stmt2ChildrenStarArr, stmt2ChildrenStarArr+ 3));
	//int stmt2NextArr[] = {2, 3};
	//stmt2->setNext(Statement::NextSet(stmt2NextArr, stmt2NextArr+ 2));
	int stmt2PrevArr[] = {1};
	stmt2->setPrev(Statement::PreviousSet(stmt2PrevArr, stmt2PrevArr+ sizeof(stmt2PrevArr)/sizeof(*stmt2PrevArr)));
	stable->addStmt(stmt2);

	cout << "aqwertyu" << endl;

	Statement* stmt3 = new Statement();
	stmt3->setStmtNum(3);
	stmt3->setType(NodeType::ASSIGN_STMT_);
	stmt3->setTNodeRef(assg3);
	//stmt3->setCalls();
	string stmt3UsesArr[] = {"more", "beer", "water"};
	//cout << "asd " << sizeof(stmt3UsesArr)/sizeof(*stmt3UsesArr) << " dsa" << endl;
	stmt3->setUses(Statement::UsesSet(stmt3UsesArr, stmt3UsesArr+ sizeof(stmt3UsesArr)/sizeof(*stmt3UsesArr)));
	string stmt3ModsArr[] = {"drink"};
	stmt3->setModifies(Statement::ModifiesSet(stmt3ModsArr, stmt3ModsArr+ sizeof(stmt3ModsArr)/sizeof(*stmt3ModsArr)));
	stmt3->setFollowsAfter(4);
	//stmt3->setFollowsBefore();
	int stmt3FollowsStarAfterArr[] = {4};
	stmt3->setFollowsStarAfter(Statement::FollowsStarAfterSet(stmt3FollowsStarAfterArr, stmt3FollowsStarAfterArr+ sizeof(stmt3FollowsStarAfterArr)/sizeof(*stmt3FollowsStarAfterArr)));
	//stmt3->setFollowsStarBefore();
	stmt3->setParent(1);
	//int stmt3ChildrenArr[] = {2, 3, 4};
	//stmt3->setChildren(Statement::ChildrenSet(stmt3ChildrenArr, stmt3ChildrenArr+ 3));
	int stmt3ParentStarArr[] = {1};
	stmt3->setParentStar(Statement::ParentStarSet(stmt3ParentStarArr, stmt3ParentStarArr+ sizeof(stmt3UsesArr)/sizeof(*stmt3UsesArr)));
	//int stmt3ChildrenStarArr[] = {2, 3, 4};
	//stmt3->setChildrenStar(Statement::ChildrenSet(stmt3ChildrenStarArr, stmt3ChildrenStarArr+ 3));
	int stmt3NextArr[] = {4};
	stmt3->setNext(Statement::NextSet(stmt3NextArr, stmt3NextArr+ sizeof(stmt3NextArr)/sizeof(*stmt3NextArr)));
	int stmt3PrevArr[] = {1};
	stmt3->setPrev(Statement::PreviousSet(stmt3PrevArr, stmt3PrevArr+ sizeof(stmt3UsesArr)/sizeof(*stmt3UsesArr)));
	stable->addStmt(stmt3);

	Statement* stmt4 = new Statement();
	stmt4->setStmtNum(4);
	stmt4->setType(NodeType::CALL_STMT_);
	stmt4->setTNodeRef(call4);
	stmt4->setCalls("YourMom");
	string stmt4UsesArr[] = {"nagging", "i", "drink", "eaten"};
	stmt4->setUses(Statement::UsesSet(stmt4UsesArr, stmt4UsesArr+ sizeof(stmt4UsesArr)/sizeof(*stmt4UsesArr)));
	string stmt4ModsArr[] = {"nagging", "mom", "full"};
	stmt4->setModifies(Statement::ModifiesSet(stmt4ModsArr, stmt4ModsArr+ sizeof(stmt4ModsArr)/sizeof(*stmt4ModsArr)));
	//stmt4->setFollowsAfter();
	stmt4->setFollowsBefore(3);
	//stmt4->setFollowsStarAfter();
	int stmt4FollowsStarBeforeArr[] = {3};
	stmt4->setFollowsStarBefore(Statement::FollowsStarBeforeSet(stmt4FollowsStarBeforeArr, stmt4FollowsStarBeforeArr+ sizeof(stmt4FollowsStarBeforeArr)/sizeof(*stmt4FollowsStarBeforeArr)));
	stmt4->setParent(1);
	//int stmt4ChildrenArr[] = {2, 3, 4};
	//stmt4->setChildren(Statement::ChildrenSet(stmt4ChildrenArr, stmt4ChildrenArr+ 3));
	int stmt4ParentStarArr[] = {1};
	stmt4->setParentStar(Statement::ParentStarSet(stmt4ParentStarArr, stmt4ParentStarArr+ sizeof(stmt4ParentStarArr)/sizeof(*stmt4ParentStarArr)));
	//int stmt4ChildrenStarArr[] = {2, 3, 4};
	//stmt4->setChildrenStar(Statement::ChildrenSet(stmt4ChildrenStarArr, stmt4ChildrenStarArr+ 3));
	//int stmt4NextArr[] = {2, 3};
	//stmt4->setNext(Statement::NextSet(stmt4NextArr, stmt4NextArr+ 2));
	int stmt4PrevArr[] = {3};
	stmt4->setPrev(Statement::PreviousSet(stmt4PrevArr, stmt4PrevArr+ sizeof(stmt4PrevArr)/sizeof(*stmt4PrevArr)));
	stable->addStmt(stmt4);

	cout << "L" << endl;

	Statement* stmt5 = new Statement();
	stmt5->setStmtNum(5);
	stmt5->setType(NodeType::WHILE_STMT_);
	stmt5->setTNodeRef(while5);
	//stmt5->setCalls("YourMom");
	string stmt5UsesArr[] = {"nagging", "i"};
	stmt5->setUses(Statement::UsesSet(stmt5UsesArr, stmt5UsesArr+ sizeof(stmt5UsesArr)/sizeof(*stmt5UsesArr)));
	string stmt5ModsArr[] = {"nagging", "mom"};
	stmt5->setModifies(Statement::ModifiesSet(stmt5ModsArr, stmt5ModsArr+ sizeof(stmt5ModsArr)/sizeof(*stmt5ModsArr)));
	stmt5->setFollowsAfter(9);
	//stmt5->setFollowsBefore(3);
	int stmt5FollowsStarAfterArr[] = {4};
	stmt5->setFollowsStarAfter(Statement::FollowsStarAfterSet(stmt5FollowsStarAfterArr, stmt5FollowsStarAfterArr+ sizeof(stmt5FollowsStarAfterArr)/sizeof(*stmt5FollowsStarAfterArr)));
	//int stmt5FollowsStarBeforeArr[] = {3};
	//stmt5->setFollowsStarBefore(Statement::FollowsStarBeforeSet(stmt3FollowsStarBeforeArr, stmt3FollowsStarBeforeArr+ sizeof(stmt3FollowsStarBeforeArr)/sizeof(*stmt3FollowsStarBeforeArr)));
	//stmt5->setParent(1);
	int stmt5ChildrenArr[] = {6, 7};
	stmt5->setChildren(Statement::ChildrenSet(stmt5ChildrenArr, stmt5ChildrenArr+ sizeof(stmt5ChildrenArr)/sizeof(*stmt5ChildrenArr)));
	//int stmt5ParentStarArr[] = {1};
	//stmt5->setParentStar(Statement::ParentStarSet(stmt5ParentStarArr, stmt5ParentStarArr+ sizeof(stmt5ParentStarArr)/sizeof(*stmt5ParentStarArr)));
	int stmt5ChildrenStarArr[] = {6, 7, 8};
	stmt5->setChildrenStar(Statement::ChildrenSet(stmt5ChildrenStarArr, stmt5ChildrenStarArr+ sizeof(stmt5ChildrenStarArr)/sizeof(*stmt5ChildrenStarArr)));
	int stmt5NextArr[] = {6};
	stmt5->setNext(Statement::NextSet(stmt5NextArr, stmt5NextArr+ sizeof(stmt5NextArr)/sizeof(*stmt5NextArr)));
	//int stmt5PrevArr[] = {3};
	//stmt5->setPrev(Statement::PreviousSet(stmt5PrevArr, stmt5PrevArr+ sizeof(stmt5PrevArr)/sizeof(*stmt5PrevArr)));
	stable->addStmt(stmt5);

	Statement* stmt6 = new Statement();
	stmt6->setStmtNum(6);
	stmt6->setType(NodeType::ASSIGN_STMT_);
	stmt6->setTNodeRef(assg6);
	//stmt6->setCalls("YourMom");
	string stmt6UsesArr[] = {"nagging"};
	stmt6->setUses(Statement::UsesSet(stmt6UsesArr, stmt6UsesArr+ sizeof(stmt6UsesArr)/sizeof(*stmt6UsesArr)));
	string stmt6ModsArr[] = {"nagging"};
	stmt6->setModifies(Statement::ModifiesSet(stmt6ModsArr, stmt6ModsArr+ sizeof(stmt6ModsArr)/sizeof(*stmt6ModsArr)));
	stmt6->setFollowsAfter(7);
	//stmt6->setFollowsBefore(3);
	int stmt6FollowsStarAfterArr[] = {7};
	stmt6->setFollowsStarAfter(Statement::FollowsStarAfterSet(stmt6FollowsStarAfterArr, stmt6FollowsStarAfterArr+ sizeof(stmt6FollowsStarAfterArr)/sizeof(*stmt6FollowsStarAfterArr)));
	//int stmt6FollowsStarBeforeArr[] = {3};
	//stmt6->setFollowsStarBefore(Statement::FollowsStarBeforeSet(stmt3FollowsStarBeforeArr, stmt3FollowsStarBeforeArr+ sizeof(stmt3FollowsStarBeforeArr)/sizeof(*stmt3FollowsStarBeforeArr)));
	stmt6->setParent(5);
	//int stmt6ChildrenArr[] = {6, 7};
	//stmt6->setChildren(Statement::ChildrenSet(stmt6ChildrenArr, stmt6ChildrenArr+ sizeof(stmt6ChildrenArr)/sizeof(*stmt6ChildrenArr)));
	int stmt6ParentStarArr[] = {5};
	stmt6->setParentStar(Statement::ParentStarSet(stmt6ParentStarArr, stmt6ParentStarArr+ sizeof(stmt6ParentStarArr)/sizeof(*stmt6ParentStarArr)));
	int stmt6ChildrenStarArr[] = {6, 7, 8};
	stmt6->setChildrenStar(Statement::ChildrenSet(stmt6ChildrenStarArr, stmt6ChildrenStarArr+ sizeof(stmt6ChildrenStarArr)/sizeof(*stmt6ChildrenStarArr)));
	int stmt6NextArr[] = {7, 9};
	stmt6->setNext(Statement::NextSet(stmt6NextArr, stmt6NextArr+ sizeof(stmt6NextArr)/sizeof(*stmt6NextArr)));
	int stmt6PrevArr[] = {7, 5};
	stmt6->setPrev(Statement::PreviousSet(stmt6PrevArr, stmt6PrevArr+ sizeof(stmt6PrevArr)/sizeof(*stmt6PrevArr)));
	stable->addStmt(stmt6);

	Statement* stmt7 = new Statement();
	stmt7->setStmtNum(7);
	stmt7->setType(NodeType::WHILE_STMT_);
	stmt7->setTNodeRef(while7);
	//stmt7->setCalls("YourMom");
	string stmt7UsesArr[] = {"i", "nagging"};
	stmt7->setUses(Statement::UsesSet(stmt7UsesArr, stmt7UsesArr+ sizeof(stmt7UsesArr)/sizeof(*stmt7UsesArr)));
	string stmt7ModsArr[] = {"mom"};
	stmt7->setModifies(Statement::ModifiesSet(stmt7ModsArr, stmt7ModsArr+ sizeof(stmt7ModsArr)/sizeof(*stmt7ModsArr)));
	//stmt7->setFollowsAfter(7);
	stmt7->setFollowsBefore(6);
	//int stmt7FollowsStarAfterArr[] = {7};
	//stmt3->setFollowsStarAfter(Statement::FollowsStarAfterSet(stmt7FollowsStarAfterArr, stmt7FollowsStarAfterArr+ sizeof(stmt7FollowsStarAfterArr)/sizeof(*stmt7FollowsStarAfterArr)));
	int stmt7FollowsStarBeforeArr[] = {6};
	stmt7->setFollowsStarBefore(Statement::FollowsStarBeforeSet(stmt7FollowsStarBeforeArr, stmt7FollowsStarBeforeArr+ sizeof(stmt7FollowsStarBeforeArr)/sizeof(*stmt7FollowsStarBeforeArr)));
	stmt7->setParent(5);
	int stmt7ChildrenArr[] = {8};
	stmt7->setChildren(Statement::ChildrenSet(stmt7ChildrenArr, stmt7ChildrenArr+ sizeof(stmt7ChildrenArr)/sizeof(*stmt7ChildrenArr)));
	int stmt7ParentStarArr[] = {5};
	stmt7->setParentStar(Statement::ParentStarSet(stmt7ParentStarArr, stmt7ParentStarArr+ sizeof(stmt7ParentStarArr)/sizeof(*stmt7ParentStarArr)));
	int stmt7ChildrenStarArr[] = {8};
	stmt7->setChildrenStar(Statement::ChildrenSet(stmt7ChildrenStarArr, stmt7ChildrenStarArr+ sizeof(stmt7ChildrenStarArr)/sizeof(*stmt7ChildrenStarArr)));
	int stmt7NextArr[] = {6, 8};
	stmt7->setNext(Statement::NextSet(stmt7NextArr, stmt7NextArr+ sizeof(stmt7NextArr)/sizeof(*stmt7NextArr)));
	int stmt7PrevArr[] = {6, 8};
	stmt7->setPrev(Statement::PreviousSet(stmt7PrevArr, stmt7PrevArr+ sizeof(stmt7PrevArr)/sizeof(*stmt7PrevArr)));
	stable->addStmt(stmt7);
	
	cout << "sdfghjk" << endl;

	Statement* stmt8 = new Statement();
	stmt8->setStmtNum(8);
	stmt8->setType(NodeType::ASSIGN_STMT_);
	stmt8->setTNodeRef(assg8);
	//stmt8->setCalls("YourMom");
	string stmt8UsesArr[] = {"nagging"};
	stmt8->setUses(Statement::UsesSet(stmt8UsesArr, stmt8UsesArr+ sizeof(stmt8UsesArr)/sizeof(*stmt8UsesArr)));
	string stmt8ModsArr[] = {"mom"};
	stmt8->setModifies(Statement::ModifiesSet(stmt8ModsArr, stmt8ModsArr+ sizeof(stmt8ModsArr)/sizeof(*stmt8ModsArr)));
	//stmt8->setFollowsAfter(7);
	//stmt8->setFollowsBefore(6);
	//int stmt8FollowsStarAfterArr[] = {7};
	//stmt3->setFollowsStarAfter(Statement::FollowsStarAfterSet(stmt8FollowsStarAfterArr, stmt8FollowsStarAfterArr+ sizeof(stmt8FollowsStarAfterArr)/sizeof(*stmt8FollowsStarAfterArr)));
	//int stmt8FollowsStarBeforeArr[] = {6};
	//stmt8->setFollowsStarBefore(Statement::FollowsStarBeforeSet(stmt8FollowsStarBeforeArr, stmt8FollowsStarBeforeArr+ sizeof(stmt8FollowsStarBeforeArr)/sizeof(*stmt8FollowsStarBeforeArr)));
	stmt8->setParent(7);
	//int stmt8ChildrenArr[] = {8};
	//stmt8->setChildren(Statement::ChildrenSet(stmt8ChildrenArr, stmt8ChildrenArr+ sizeof(stmt8ChildrenArr)/sizeof(*stmt8ChildrenArr)));
	int stmt8ParentStarArr[] = {5, 7};
	stmt8->setParentStar(Statement::ParentStarSet(stmt8ParentStarArr, stmt8ParentStarArr+ sizeof(stmt8ParentStarArr)/sizeof(*stmt8ParentStarArr)));
	//int stmt8ChildrenStarArr[] = {8};
	//stmt8->setChildrenStar(Statement::ChildrenSet(stmt8ChildrenStarArr, stmt8ChildrenStarArr+ sizeof(stmt8ChildrenStarArr)/sizeof(*stmt8ChildrenStarArr)));
	int stmt8NextArr[] = {7};
	stmt8->setNext(Statement::NextSet(stmt8NextArr, stmt8NextArr+ sizeof(stmt8NextArr)/sizeof(*stmt8NextArr)));
	int stmt8PrevArr[] = {7};
	stmt8->setPrev(Statement::PreviousSet(stmt8PrevArr, stmt8PrevArr+ sizeof(stmt8PrevArr)/sizeof(*stmt8PrevArr)));
	stable->addStmt(stmt8);

	Statement* stmt9 = new Statement();
	stmt9->setStmtNum(9);
	stmt9->setType(NodeType::ASSIGN_STMT_);
	stmt9->setTNodeRef(assg9);
	//stmt9->setCalls("YourMom");
	string stmt9UsesArr[] = {"drink", "eat"};
	stmt9->setUses(Statement::UsesSet(stmt9UsesArr, stmt9UsesArr+ sizeof(stmt9UsesArr)/sizeof(*stmt9UsesArr)));
	string stmt9ModsArr[] = {"full"};
	stmt9->setModifies(Statement::ModifiesSet(stmt9ModsArr, stmt9ModsArr+ sizeof(stmt9ModsArr)/sizeof(*stmt9ModsArr)));
	//stmt9->setFollowsAfter(7);
	stmt9->setFollowsBefore(5);
	//int stmt9FollowsStarAfterArr[] = {7};
	//stmt3->setFollowsStarAfter(Statement::FollowsStarAfterSet(stmt9FollowsStarAfterArr, stmt9FollowsStarAfterArr+ sizeof(stmt9FollowsStarAfterArr)/sizeof(*stmt9FollowsStarAfterArr)));
	int stmt9FollowsStarBeforeArr[] = {5};
	stmt9->setFollowsStarBefore(Statement::FollowsStarBeforeSet(stmt9FollowsStarBeforeArr, stmt9FollowsStarBeforeArr+ sizeof(stmt9FollowsStarBeforeArr)/sizeof(*stmt9FollowsStarBeforeArr)));
	stmt9->setParent(7);
	//int stmt9ChildrenArr[] = {8};
	//stmt9->setChildren(Statement::ChildrenSet(stmt9ChildrenArr, stmt9ChildrenArr+ sizeof(stmt9ChildrenArr)/sizeof(*stmt9ChildrenArr)));
	//int stmt9ParentStarArr[] = {5, 7};
	//stmt9->setParentStar(Statement::ParentStarSet(stmt9ParentStarArr, stmt9ParentStarArr+ sizeof(stmt9ParentStarArr)/sizeof(*stmt9ParentStarArr)));
	//int stmt9ChildrenStarArr[] = {8};
	//stmt9->setChildrenStar(Statement::ChildrenSet(stmt9ChildrenStarArr, stmt9ChildrenStarArr+ sizeof(stmt9ChildrenStarArr)/sizeof(*stmt9ChildrenStarArr)));
	//int stmt9NextArr[] = {7};
	//stmt9->setNext(Statement::NextSet(stmt9NextArr, stmt9NextArr+ sizeof(stmt9NextArr)/sizeof(*stmt9NextArr)));
	int stmt9PrevArr[] = {6};
	stmt9->setPrev(Statement::PreviousSet(stmt9PrevArr, stmt9PrevArr+ sizeof(stmt9PrevArr)/sizeof(*stmt9PrevArr)));
	stable->addStmt(stmt9);
//-------------  END OF STMT ------------//


//-------------  SET UP PROC ------------//
	ProcTable* ptable = ProcTable::getInstance();

//-------------  END OF PROC ------------//


//-------------  SET UP VAR -------------//
	VarTable* vtable = VarTable::getInstance();

//-------------  END OF VAR -------------//


//-------------  SET UP CONST -----------//
	ConstTable* ctable = ConstTable::getInstance();

//-------------  END OF CONST -----------//
}

void PQLIntegration::tearDown() {
	// to clear the pkb
	AST::reset();
	StmtTable::getInstance()->clearTable();
	VarTable::reset();
	ConstTable::getInstance()->clearTable();
	ProcTable::getInstance()->clearTable();
	CFG::reset();
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( PQLIntegration );

void PQLIntegration::testSelectOnly() {
	cout << "select only";

	// DOES NOT PASS
	// because the values of a are not set in results object.

	string QUERY_STRING = "assign a; Select a";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	cout << "size1 for select only: " << r.size() << endl;
	CPPUNIT_ASSERT(6 == r.size());

	// new test for if
	string QUERY_STRING2 = "if x; Select x";
	pcc = new PQLController();
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);

	CPPUNIT_ASSERT(1 == r2.size());
}

void PQLIntegration::testSelectModifies() {
	//string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\")";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	//r = pcc->parse(QUERY_STRING);
	//CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(7 == r2.size());

	//string QUERY_STRING3 = "if i; variable v; Select a such that Modifies(i, v)";
	//unordered_set<string> r3;
	//r3 = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(0 == r3.size());

	string QUERY_STRING4 = "variable v; Select v such that Modifies(_, v) and Modifies(_, v)";
	unordered_set<string> r4;
	r4 = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r4.size());
}

	/* OLd source

	procedure chocs {
		a=4;				//1
		while i {			//2
			k = 3;			//3
			while j {		//4
				i=1;		//5
				j=2; 		//6
			}
			b=5;			//7
			j=2+3+4;		//8
			z=(10+11)*12; 	//9
		}
		--- new stuff TODO ---
		if a then {			//10
		} else {
		}
	}
	*/

void PQLIntegration::testSelectUses() {
	PQLController* pcc = new PQLController();

	//string QUERY_STRING = "while w; variable v; Select v such that Uses(w, v)";
	//unordered_set<string> r;
	//r = pcc->parse(QUERY_STRING);
	//CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING2 = "assign aaaa; variable verynice; Select verynice such that Uses(aaaa, verynice)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	//CPPUNIT_ASSERT(0 == r2.size());

	string QUERY_STRING3 = "stmt s; variable v; Select v such that Uses(s, v)";
	unordered_set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(3 == r3.size());

	string QUERY_STRING4 = "variable v; Select v such that Uses(_, v)";
	unordered_set<string> r4;
	r4 = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(3 == r4.size());
}

void PQLIntegration::testSelectFollows() {
	PQLController* pcc = new PQLController();
	unordered_set<string> r;

	string QUERY_STRING = "assign a, a1; Select a such that Follows(a, a1)";
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(3 == r.size());

	string QUERY_STRING2 = "stmt s; Select s such that Follows(s, _)";
	r = pcc->parse(QUERY_STRING2);
	// answer supposed to be 1,3,4,5,7
    CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING3 = "stmt s; Select s such that Follows(_, s)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(5 == r.size());

	//string QUERY_STRING4 = "assign a; Select a such that Follows(_, _)";
	//r = pcc->parse(QUERY_STRING4);
	//CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectFollowsStar() {
	string QUERY_STRING = "assign a, a1; Select a1 such that Follows*(a, a1)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(3 == r.size());

	string QUERY_STRING2 = "stmt s; Select s such that Follows*(s, _)";
	r = pcc->parse(QUERY_STRING2);
	// answer: 1,3,4,5,7
	/*BOOST_FOREACH(auto p, r) {
		cout << p << " ";
	}*/
	CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING3 = "stmt s; Select s such that Follows*(_, s)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(5 == r.size());

	string QUERY_STRING4 = "assign a; Select a such that Follows*(_, _)";
	r = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectParent() {
	PQLController* pcc = new PQLController();
	unordered_set<string> r;

	string QUERY_STRING = "while w; assign a; Select a such that Parent(w, a)";
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(6 == r.size());

	string QUERY_STRING2 = "while w; Select w such that Parent(w, _)";
	r = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING3 = "while w; Select w such that Parent(_, w)";
	r = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(1 == r.size());

	// don't work because a not present in clauses
	//string QUERY_STRING4 = "assign a; Select a such that Parent(_, _)";
	//r = pcc->parse(QUERY_STRING4);
	//CPPUNIT_ASSERT(6 == r.size());

	//string QUERY_STRING5 = "assign a; Select a such that Parent(3, _)";
	//r = pcc->parse(QUERY_STRING5);
	//CPPUNIT_ASSERT(0 == r.size());

	//string QUERY_STRING6 = "assign a; Select a such that Parent(2, _)";
	//r = pcc->parse(QUERY_STRING6);
	//CPPUNIT_ASSERT(6 == r.size());

}

void PQLIntegration::testSelectParentStar() {
	string QUERY_STRING = "while w; Select w such that Parent*(w, 6)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(2 == r.size()); 

	string QUERY_STRING2 = "while w; Select w such that Parent*(w, _)";
	r = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(2 == r.size());

	string QUERY_STRING3 = "while w; Select w such that Parent*(_, w)";
	r = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING4 = "assign a; Select a such that Parent*(_, _)";
	r = pcc->parse(QUERY_STRING4);
	CPPUNIT_ASSERT(6 == r.size());
}

void PQLIntegration::testSelectPatternAssg() {
	string QUERY_STRING = "assign a; Select a pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a pattern a(v, _\"4\"_)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	//cout << r2.size() << endl;
	CPPUNIT_ASSERT(2 == r2.size());

	string QUERY_STRING3 = "assign a; variable v; Select a pattern a(v, _\"2 + 3 + 4\"_)";
	unordered_set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	CPPUNIT_ASSERT(1 == r3.size());
	CPPUNIT_ASSERT("8" == *r3.begin());
}

void PQLIntegration::testSelectModifiesPattern() {
	string QUERY_STRING = "assign a; Select a such that Modifies(a, \"a\") pattern a(\"a\", _)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);

	CPPUNIT_ASSERT(1 == r.size());

	string QUERY_STRING2 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"2 + 3 + 4\"_)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING2);
	CPPUNIT_ASSERT(1 == r2.size());

	//TODO: fix parsing of brackets in query parser
	string QUERY_STRING3 = "assign a; variable v; Select a such that Modifies(a, v) pattern a(v, _\"( 10 + 11 ) * 12\"_)";
	unordered_set<string> r3;
	r3 = pcc->parse(QUERY_STRING3);
	//CPPUNIT_ASSERT(1 == r3.size());
}

void PQLIntegration::testSelectFollowsPattern() {
	string QUERY_STRING = "assign a1, a2; Select a1 such that Follows(a1, a2) pattern a2(v, _\"2 + 3\"_)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());
}

void PQLIntegration::testSelectUsesPattern() {
	string QUERY_STRING = "variable v; assign a; Select v such that Uses(_, v) pattern a(v, _)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	// answer: i, j
	BOOST_FOREACH(auto p, r) {
		cout << p << " ";
	}
	//CPPUNIT_ASSERT(2 == r.size());
}

void PQLIntegration::testSelectConstant() {
	string QUERY_STRING = "constant c; Select c such that Follows(1, 2)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(8 == r.size());

	string QUERY_STRING_2 = "constant c; assign a; Select c such that Follows(a, 1)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING_2);
	CPPUNIT_ASSERT(0 == r2.size());
}

void PQLIntegration::testSelectProgLine() {
	string QUERY_STRING = "prog_line p; Select p such that Follows(1, p)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(1 == r.size());
	//CPPUNIT_ASSERT(r.find("2") != r.end());

	string QUERY_STRING_2 = "prog_line p; Select p such that Follows(9, p)";
	unordered_set<string> r2;
	r2 = pcc->parse(QUERY_STRING_2);
	CPPUNIT_ASSERT(1 == r2.size());
}

void PQLIntegration::testFailParent(){
	cout << "will throw exception" << endl;
	string QUERY_STRING = "assign a; variable v; Select a such that Parent(v, a)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(0 == r.size());
}

void PQLIntegration::testFailUses(){
	cout << "will throw exception" << endl;
	string QUERY_STRING = "variable v, v1; Select v such that Uses(v, v1)";
	PQLController* pcc = new PQLController();
	unordered_set<string> r;
	r = pcc->parse(QUERY_STRING);
	CPPUNIT_ASSERT(0 == r.size());
}