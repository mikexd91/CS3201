#pragma once

#include <boost/unordered_map.hpp>

using namespace boost;

#include "AST.h"
#include "TNode.h"
// to include CFG
// to include GNode

class StmtTable {

public:
	StmtTable();	// constructor: instantiates an empty statement table

	void addStmt(int stmtNum, string type); // adds statement to table

	string getStmtType(int stmtNum);		// get type of statement
	TNode * getASTReference(int stmtNum);	// get AST node rep of statement
	//GNode * getCFGReference(int stmtNum);	// get CFG node rep of statement

	void setASTReference(int stmtNum, TNode node);	// sets the AST ref node of statement
	//void setCFGReference(int stmtNum, GNode node);	// sets the CFG ref node of statement

private:
	//unordered_map<int, vector<unordered_map<>>> _stmtTable;

};