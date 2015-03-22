#pragma once

#include <stdio.h>
#include <iostream>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

#include "StmtTable.h"
#include "AST.h"
#include "TNode.h"
// to include CFG
// to include GNode

// constructor: instantiates an empty statement table
StmtTable::StmtTable() {

}

// adds statement to table
void StmtTable::addStmt(int stmtNum, string type) {


}

// get type of statement
string StmtTable::getStmtType(int stmtNum) {


}

// get AST node rep of statement
TNode * getASTReference(int stmtNum) {


}

/*
// get CFG node rep of statement
GNode * getCFGReference(int stmtNum) {


}*/

// sets the AST ref node of statement
void setASTReference(int stmtNum, TNode node) {


}

/*
// sets the CFG ref node of statement
void setCFGReference(int stmtNum, GNode node) {


}*/