#include "DesignExtractor.h"
#include "ProcTable.h"
#include "StmtTable.h"
#include "WhileNode.h"
#include "IfNode.h"

DesignExtractor::DesignExtractor() {

}

void DesignExtractor::executeSecondPass() {
	populateFollowStar();
	populateParentStar();
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
