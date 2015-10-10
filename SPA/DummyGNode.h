#pragma once

#include "GNode.h"

class DummyGNode : public GNode {

public:

	// constructor
	DummyGNode();

	// getters
	// dummy nodes exist for connecting the if and else branches of if
	// stmts
	int getIfParentStmt();
	int getElseParentStmt();

	// setters
	void setIfParentStmt(int);
	void setElseParentStmt(int);

private:

};
