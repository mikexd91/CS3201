#pragma once

#include "GNode.h"
#include "IfGNode.h"
#include <boost/unordered_set.hpp>

using boost::unordered_set;

class DummyGNode : public GNode {

public:

	// constructor
	DummyGNode();

	// getters
	// dummy nodes exist for connecting the if and else branches of if
	// stmts
	int getIfParentStmt();
	int getElseParentStmt();
	IfGNode* getEntrance();
	unordered_set<int> getPrevStmts();

	// setters
	void setIfParentStmt(int);
	void setElseParentStmt(int);
	void setEntrance(IfGNode*);
	void setPrevStmts(unordered_set<int>);

private:
	IfGNode* entrance;
	unordered_set<int> prevStmts;
};
