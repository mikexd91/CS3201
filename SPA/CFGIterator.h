#pragma once

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "CFG.h"
#include <stack>
#include "WhileGNode.h"
#include "StmtTable.h"
#include "IfGNode.h"

struct GNodeContainer {
	GNode* node;
	int count;
	GNodeContainer(GNode* n= NULL, int c=0):
		node(n),
        count(c){}
};


class CFGIterator {

public:

	~CFGIterator();
	CFGIterator(GNode* start);
	GNode* getNextNode();
	bool isInWhileLoop();
	WhileGNode* getCurrentWhileNode();
	void skipWhileLoop(WhileGNode* node);
	bool toConsiderElseStmt();
	IfGNode* getCurrentIfNode();
	void skipThenStmt(IfGNode* node);
	bool isStart();

	
private:
	GNode* startNode;
	GNode* nextNode;
	stack<GNodeContainer> nodeStack;
	int numIter;
};
