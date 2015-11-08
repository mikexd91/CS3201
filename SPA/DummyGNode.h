#pragma once

#include "GNode.h"
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
	GNode* getIfParent();
	GNode* getElseParent();
	GNode* getChild();
	GNode* getEntrance();
	unordered_set<int> getPrevStmts();

	// setters
	void setIfParent(GNode*);
	void setElseParent(GNode*);
	void setEntrance(GNode*);
	void setPrevStmts(unordered_set<int>);

private:
	GNode* entrance;
	unordered_set<int> prevStmts;
};
