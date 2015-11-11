#include "CallGNode.h"
#include "DesignExtractor.h"
#include "ProcTable.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "WhileNode.h"
#include "IfNode.h"
#include "boost/foreach.hpp"
#include "InvalidCodeException.h"

DesignExtractor::DesignExtractor() {

}

void DesignExtractor::executeSecondPass() {
	checkCyclicCalls();
	populateModUsesCalls();
	populateFollowStar();
	populateParentStar();
}

void DesignExtractor::checkCyclicCalls(){
	ProcTable* procTable = ProcTable::getInstance();
	unordered_set<Procedure*> procs = procTable->getAllProcs();
	//for each procedure, check whether there is a cyclic call
	unordered_set<Procedure*> visitedProcs = unordered_set<Procedure*>();
	unordered_set<Procedure*> recurseProcs = unordered_set<Procedure*>();
	BOOST_FOREACH(Procedure* proc, procs) {
		if (isCyclicCall(visitedProcs, recurseProcs, proc)) {
			throw InvalidCodeException("Cyclic calls detected!");
		}
	}
}

bool DesignExtractor::isCyclicCall(unordered_set<Procedure*> visitedProcs, unordered_set<Procedure*> recurseProcs, Procedure* currentProc) {
	if(visitedProcs.find(currentProc) == visitedProcs.end()) {
		// Mark the current node as visited and part of recursion stack
		visitedProcs.insert(currentProc);
		recurseProcs.insert(currentProc);

		// Recur for all the vertices adjacent to this vertex
		Procedure::CallsSet calledProcs = currentProc->getCalls();
		BOOST_FOREACH(Procedure* calledProc, calledProcs) {
			if ( visitedProcs.find(calledProc) == visitedProcs.end() && isCyclicCall(visitedProcs, recurseProcs, calledProc)) {
				return true;
			} else if (recurseProcs.find(calledProc) != recurseProcs.end()) {
				return true;
			}
		}
 
	}
	recurseProcs.erase(currentProc);  // remove the vertex from recursion stack
	return false;
}


void DesignExtractor::populateFollowStar() {
	ProcTable* procTable = ProcTable::getInstance();
	AST* ast = AST::getInstance();

	//for each procedure
	for (auto procIter = procTable->getIterator(); procIter != procTable->getEnd(); procIter++) {
		string procName = procIter->first;
		ProcNode* procNode = ast->getProcNode(procName);
		//do bfs on procNode
		StmtLstNode* stmtLstNode = procNode->getStmtLstNode();
		queue<StmtLstNode*> stmtLstQueue = queue<StmtLstNode*>();
		stmtLstQueue.push(stmtLstNode);
		while(!stmtLstQueue.empty()) {
			StmtLstNode* currentStmtLst = stmtLstQueue.front();
			stmtLstQueue.pop();
			populateFollowInStmtLst(currentStmtLst, stmtLstQueue);
		}
	}
}

void DesignExtractor::populateParentStar() {

	ProcTable* procTable = ProcTable::getInstance();
	AST* ast = AST::getInstance();

	//for each procedure
	for (auto procIter = procTable->getIterator(); procIter != procTable->getEnd(); procIter++) {
		string procName = procIter->first;
		ProcNode* procNode = ast->getProcNode(procName);
		//do dfs on procNode
		StmtLstNode* stmtLstNode = procNode->getStmtLstNode();
		vector<TNode*> stmts = stmtLstNode->getChildren();
		vector<int> currentParents = vector<int>();
		for (auto stmtIter = stmts.begin(); stmtIter != stmts.end(); stmtIter++) {
			//children of stmtList should be statements...
			StmtNode* stmt = (StmtNode*) *stmtIter;
			recurseParentStar(stmt, currentParents);
		}
	}
}

void DesignExtractor::populateFollowInStmtLst(StmtLstNode* stmtLstNode, queue<StmtLstNode*>& stmtLstQueue) {
	StmtTable* stmtTable = StmtTable::getInstance();
	vector<TNode*> stmts = stmtLstNode->getChildren();
	vector<int> prevStmtNums = vector<int>();
	for (auto stmtIter = stmts.begin(); stmtIter != stmts.end(); stmtIter++) {
		StmtNode* stmtNode = (StmtNode*) *stmtIter;
		int currentStmtNum = stmtNode->getStmtNum();
		Statement* currentStmt = stmtTable->getStmtObj(currentStmtNum);
		//if there are previous statements
		//we need to set the follows star before of the current statement, and the follows star after of the previous stmt
		if (!prevStmtNums.empty()) {
			for (auto prevStmtIter = prevStmtNums.begin(); prevStmtIter != prevStmtNums.end(); prevStmtIter++) {
				int prevStmtNum = *prevStmtIter;
				Statement* prevStmt = stmtTable->getStmtObj(prevStmtNum);
				//set follows star after of earlier statement
				Statement::FollowsStarAfterSet followsStarAfter = prevStmt->getFollowsStarAfter();
				followsStarAfter.insert(currentStmtNum);
				prevStmt->setFollowsStarAfter(followsStarAfter);
				//set follows star before of current statement
				Statement::FollowsStarBeforeSet followsStarBefore = currentStmt->getFollowsStarBefore();
				followsStarBefore.insert(prevStmtNum);
				currentStmt->setFollowsStarBefore(followsStarBefore);
			}
		}
		//if it has statement list node(s)
		//push the statement list nodes into nextStmtLst to be evaluated in the next iteration
		if (stmtNode->getNodeType() == WHILE_STMT_) {
			WhileNode* whileNode = (WhileNode*) stmtNode;
			stmtLstQueue.push(whileNode->getStmtLstNode());
		} else if (stmtNode->getNodeType() == IF_STMT_) {
			IfNode* ifNode = (IfNode*) stmtNode;
			stmtLstQueue.push(ifNode->getThenStmtLstNode());
			stmtLstQueue.push(ifNode->getElseStmtLstNode());
		}
		prevStmtNums.push_back(currentStmtNum);
	}
}

void DesignExtractor::recurseParentStar(StmtNode* stmtNode, vector<int>& currentParents){
	//add stmt to parent
	StmtTable* stmtTable = StmtTable::getInstance();
	Statement* currentStmt = stmtTable->getStmtObj(stmtNode->getStmtNum());
	//set children star and parent star
	if (!currentParents.empty()) {
		//for each of the current parent,
		for (auto parentIter = currentParents.begin(); parentIter != currentParents.end(); parentIter++) {
			Statement* parentStmt = stmtTable->getStmtObj(*parentIter);
			//set children star
			Statement::ChildrenStarSet childrenStar = parentStmt->getChildrenStar();
			childrenStar.insert(currentStmt->getStmtNum());
			parentStmt->setChildrenStar(childrenStar);
			//set parent star
			Statement::ParentStarSet parentStar = currentStmt->getParentStar();
			parentStar.insert(parentStmt->getStmtNum());	
			currentStmt->setParentStar(parentStar);
		}
	}
	if(stmtNode->getNodeType() == WHILE_STMT_) {
		//has children
		WhileNode* whileNode = (WhileNode*) stmtNode;
		currentParents.push_back(whileNode->getStmtNum());
		StmtLstNode* stmtLstNode = whileNode->getStmtLstNode();
		vector<TNode*> stmts = stmtLstNode->getChildren();
		for (auto stmtIter = stmts.begin(); stmtIter != stmts.end(); stmtIter++) {
			//children of stmtList should be statements...
			StmtNode* stmt = (StmtNode*) *stmtIter;
			recurseParentStar(stmt, currentParents);
		}
		//remove stmt from current parents as all its children has been iterated through
		currentParents.pop_back();
	} else if (stmtNode->getNodeType() == IF_STMT_){
		IfNode* ifNode = (IfNode*) stmtNode;
		currentParents.push_back(ifNode->getStmtNum());
		StmtLstNode* thenStmtLstNode = ifNode->getThenStmtLstNode();
		vector<TNode*> thenStmts = thenStmtLstNode->getChildren();
		for (auto stmtIter = thenStmts.begin(); stmtIter != thenStmts.end(); stmtIter++) {
			//children of stmtList should be statements...
			StmtNode* stmt = (StmtNode*) *stmtIter;
			//iter on all its children
			recurseParentStar(stmt, currentParents);
		}
		StmtLstNode* elseStmtLstNode = ifNode->getElseStmtLstNode();
		vector<TNode*> elseStmts = elseStmtLstNode->getChildren();
		for (auto stmtIter = elseStmts.begin(); stmtIter != elseStmts.end(); stmtIter++) {
			//children of stmtList should be statements...
			StmtNode* stmt = (StmtNode*) *stmtIter;
			//iter on all its children
			recurseParentStar(stmt, currentParents);
		}
		//remove stmt from current parents as all its children has been iterated
		currentParents.pop_back();
	}	
}

void DesignExtractor::populateModUsesCalls() {
	StmtTable* stmtTable = StmtTable::getInstance();
	ProcTable* procTable = ProcTable::getInstance();
	unordered_set<Statement*> callsStmt = stmtTable->getCallStmts();

	BOOST_FOREACH(auto stmt, callsStmt) {
		string procName = stmt->getCalls();
		stmt->setUses(procTable->getProcObj(procName)->getUses());
		stmt->setModifies(procTable->getProcObj(procName)->getModifies());
	}

	BOOST_FOREACH(auto stmt, callsStmt) {
		populateCallsParents(stmt);
		populateCallsVarTable(stmt);
	}
}

void DesignExtractor::populateCallsParents(Statement* callStmt) {
	StmtTable* stmtTable = StmtTable::getInstance();
	unordered_set<string> uses = callStmt->getUses();
	unordered_set<string> modifies = callStmt->getModifies();

	bool flag = true;
	int parent;
	
	if(callStmt->getParent() != -1) {
		parent = callStmt->getParent();
	} else {
		flag = false;	
	}

	while(flag) {
		Statement* parentStmt = stmtTable->getStmtObj(parent);
		unordered_set<string> parentUses = parentStmt->getUses();
		unordered_set<string> parentMods = parentStmt->getModifies();
		parentUses.insert(uses.begin(), uses.end());
		parentMods.insert(modifies.begin(), modifies.end());
		parentStmt->setUses(parentUses);
		parentStmt->setModifies(parentMods);

		populateParentCallsVarTable(parent, uses, modifies);

		if(parentStmt->getParent() != -1) {
			parent = parentStmt->getParent();
		} else {
			flag = false;
		}
	}
}

void DesignExtractor::populateCallsVarTable(Statement* callStmt) {
	VarTable* varTable = VarTable::getInstance();
	unordered_set<string> uses = callStmt->getUses();
	unordered_set<string> mods = callStmt->getModifies();
	int stmtNum = callStmt->getStmtNum();
	
	BOOST_FOREACH(auto u, uses) {
		Variable* useVar = varTable->getVariable(u);
		useVar->addUsingStmt(stmtNum);
	}

	BOOST_FOREACH(auto m, mods) {
		Variable* modVar = varTable->getVariable(m);
		modVar->addModifyingStmt(stmtNum);
	}
}

void DesignExtractor::populateParentCallsVarTable(int stmtNum, unordered_set<string> uses, unordered_set<string> mods) {
	VarTable* varTable = VarTable::getInstance();
	BOOST_FOREACH(auto u, uses) {
		Variable* var = varTable->getVariable(u);
		var->addUsingStmt(stmtNum);
	}

	BOOST_FOREACH(auto m, mods) {
		Variable* var = varTable->getVariable(m);
		var->addModifyingStmt(stmtNum);
	}
}

void DesignExtractor::constructBip() {
	StmtTable* stmtTable = StmtTable::getInstance();

	unordered_set<Statement*> callStmts = stmtTable->getCallStmts();
	BOOST_FOREACH(auto c, callStmts) {
		breakBonds(c);
	}

	setCallsBipList(callStmts);
	setProcBipPrevList();

	CFGbip* cfg = CFGbip::getInstance();
	vector<ProcGNode*> allProcs = cfg->getAllProcedures();
	BOOST_FOREACH(auto n, allProcs) {
		populateEndProcs(n);
	}
	BOOST_FOREACH(auto n, allProcs) {
		setEndProcChildrenList(n);
	}
}

void DesignExtractor::setEndProcChildrenList(ProcGNode* procNode) {
	EndGNode* endNode = (EndGNode*) procNode->getEndNode();
	StmtTable* stmtTable = StmtTable::getInstance();

	unordered_set<int> prevStmts = endNode->getPrevStmts();		//endNode's previous
	vector<GNode*> children = endNode->getChildren();			//endNode's children

	BOOST_FOREACH(auto child, children) {
		switch(child->getNodeType()) {
			case ASSIGN_: case WHILE_: case CALL_: case IF_: 
				addOthersPrevBip(child, prevStmts);
				break;
			
			case DUMMY_: 
				addDummyPrevBip(child, prevStmts);
				break;
			
			default:
				break;
		}
	}
}

void DesignExtractor::addDummyPrevBip(GNode* child, unordered_set<int> prevStmts) {
	DummyGNode* dum = (DummyGNode*) child;
	bool flag = true;

	while(flag) {
		unordered_set<int> dumPrev = dum->getPrevStmts();
		dumPrev.insert(prevStmts.begin(), prevStmts.end());
		dum->setPrevStmts(dumPrev);

		switch(dum->getChild()->getNodeType()) {
			case DUMMY_:
				dum = (DummyGNode*) dum->getChild();
				break;
			case ASSIGN_: case WHILE_: case IF_: case CALL_: {
				flag = false;
				GNode* othersChild = dum->getChild();
				addOthersPrevBip(othersChild, dum->getPrevStmts());
				break;
			}
			default:
				flag = false;
				break;
		}
	}
}

void DesignExtractor::addOthersPrevBip(GNode* child, unordered_set<int> prevStmts) {
	StmtTable* stmtTable = StmtTable::getInstance();
	Statement* stmt = stmtTable->getStmtObj(child->getStartStmt());

	unordered_set<int> stmtPrevBip = stmt->getPrevBip();
	stmtPrevBip.insert(prevStmts.begin(), prevStmts.end());
	stmt->setPrevBip(stmtPrevBip);

	BOOST_FOREACH(auto prev, prevStmts) {
		Statement* individualPrev = stmtTable->getStmtObj(prev);
		unordered_set<int> nextSet = individualPrev->getNextBip();
		nextSet.insert(stmt->getStmtNum());
		individualPrev->setNextBip(nextSet);
	}
}

void DesignExtractor::populateEndProcs(ProcGNode* procNode) {
	EndGNode* end = (EndGNode*) procNode->getEndNode();

	bool flag = true;
	stack<EndGNode*> stack;

	while(flag) {
		if(!end->getPrevStmts().empty()) {
			break;
		}

		GNode* parent = end->getParent();
		stack.push(end);
		if(parent->getNodeType() == END_) {
			end = (EndGNode*) parent;
		} else if(parent->getNodeType() == DUMMY_) {
			DummyGNode* dumParent = (DummyGNode*) parent;
			while(!stack.empty()) {
				EndGNode* top = stack.top();
				unordered_set<int> prev = top->getPrevStmts();
				prev.insert(dumParent->getPrevStmts().begin(), dumParent->getPrevStmts().end());
				top->setPrevStmts(prev);
				stack.pop();
			}
			
			flag = false;
		} else {
			int prevStmtNum = parent->getEndStmt();
			while(!stack.empty()) {
				EndGNode* top = stack.top();
				unordered_set<int> prev = top->getPrevStmts();
				prev.insert(prevStmtNum);
				top->setPrevStmts(prev);
				stack.pop();
			}
			
			flag = false;
		}
	}
}

void DesignExtractor::setCallsBipList(unordered_set<Statement*> callStmts) {
	BOOST_FOREACH(auto c, callStmts) {
		CallGNode* callNode = (CallGNode*) c->getGBipNodeRef();
		ProcGNode* calledProc = (ProcGNode*) callNode->getChild();
		GNode* callNextNode = calledProc->getChild();
		
		unordered_set<int> callNextSet;
		callNextSet.insert(callNextNode->getStartStmt());
		c->setNextBip(callNextSet);
	}
}

void DesignExtractor::setProcBipPrevList() {
	StmtTable* stmtTable = StmtTable::getInstance();
	CFGbip* cfg = CFGbip::getInstance();
	vector<ProcGNode*> allProcs = cfg->getAllProcedures();

	BOOST_FOREACH(auto p, allProcs) {
		GNode* childNode = p->getChild();
		Statement* stmt = stmtTable->getStmtObj(childNode->getStartStmt());

		unordered_set<int> previousBip;
		vector<GNode*> parents = p->getParents();
		BOOST_FOREACH(auto call, parents) {
			previousBip.insert(call->getStartStmt());
		}

		stmt->setPrevBip(previousBip);
	}
}

void DesignExtractor::breakBonds(Statement* callStmt) {
	CFGbip* cfg = CFGbip::getInstance();
	StmtTable* stmtTable = StmtTable::getInstance();

	GNode* callNode = callStmt->getGBipNodeRef();
	GNode* originalChild = callNode->getChildren().at(0);

	ProcGNode* calledProc = cfg->getProcedure(callStmt->getCalls());
	EndGNode* calledEnd = (EndGNode*) calledProc->getEndNode();

	callNode->setFirstChild(calledProc);

	switch(originalChild->getNodeType()) {
		case ASSIGN_: case CALL_: case IF_: case END_:
			originalChild->setFirstParent(calledEnd);
			break;
		case WHILE_: case DUMMY_:
			if(originalChild->getParents().at(0) == callNode) {
				originalChild->setFirstParent(calledEnd);
			} else {
				originalChild->setSecondParent(calledEnd);
			}
			break;
		default:
			break;
	}

	vector<GNode*> calledProcParents = calledProc->getParents();
	vector<GNode*> calledEndChildren = calledEnd->getChildren();

	calledProcParents.push_back(callNode);
	calledEndChildren.push_back(originalChild);

	calledProc->setParents(calledProcParents);
	calledEnd->setChildren(calledEndChildren);

	unordered_set<int> callsNext;
	callStmt->setNextBip(callsNext);

	switch(originalChild->getNodeType()) {
		case CALL_:  case ASSIGN_: case IF_:
			removeOthersPrevBip(originalChild);
			break;
		case WHILE_:
			removeWhilePrevBip(originalChild->getStartStmt(), callNode->getStartStmt());
			break;
		case DUMMY_:
			removeDummyPrevBip(originalChild, callNode->getStartStmt());
			break;
		default:
			break;
	}
}

void DesignExtractor::removeOthersPrevBip(GNode* origChild) {
	StmtTable* stmtTable = StmtTable::getInstance();
	unordered_set<int> prev;
	Statement* stmt = stmtTable->getStmtObj(origChild->getStartStmt());
	stmt->setPrevBip(prev);
}

void DesignExtractor::removeDummyPrevBip(GNode* origChild, int stmtToErase) {
	StmtTable* stmtTable = StmtTable::getInstance();
	DummyGNode* dum = (DummyGNode*) origChild;

	bool flag = true;

	while(flag) {
		unordered_set<int> prevStmts = dum->getPrevStmts();
		prevStmts.erase(stmtToErase);
		dum->setPrevStmts(prevStmts);

		if(dum->getChild()->getNodeType() == ASSIGN_ || dum->getChild()->getNodeType() == CALL_ || dum->getChild()->getNodeType() == IF_ || dum->getChild()->getNodeType() == WHILE_) {
			Statement* stmt = stmtTable->getStmtObj(dum->getChild()->getStartStmt());
			unordered_set<int> stmtPrevBip = stmt->getPrevBip();
			stmtPrevBip.erase(stmtToErase);
			stmt->setPrevBip(stmtPrevBip);
			flag = false;
		}

		if(dum->getChild()->getNodeType() == DUMMY_) {
			dum = (DummyGNode*) dum->getChild();
		}

		if(dum->getChild()->getNodeType() == END_) {
			flag = false;
		}
	}
}

void DesignExtractor::removeWhilePrevBip(int stmtNum, int stmtToErase) {
	StmtTable* stmtTable = StmtTable::getInstance();
	Statement* stmt = stmtTable->getStmtObj(stmtNum);
	unordered_set<int> prevBip = stmt->getPrevBip();
	prevBip.erase(stmtToErase);
	stmt->setPrevBip(prevBip);
}