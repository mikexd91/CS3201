#pragma once

#include "CNode.h"

class DummyCNode : public CNode {

public:

	// constructor
	DummyCNode();

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
