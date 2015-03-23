#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

#include "ParentTable.h"

// constructor: instantiates an empty parent table
ParentTable::ParentTable() {


}

// adds statement to table
void addStmt(int stmtNum, vector<int> children) {


}

// get list of statements stmtNum follows
vector<int> getStmtChildren(int stmtNum) {


}

// get list of statements followed by stmtNum
vector<int> getStmtParents(int stmtNum) {


}