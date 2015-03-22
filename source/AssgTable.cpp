#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

#include "AssgTable.h"

// constructor: instantiates an empty AssgTable
AssgTable::AssgTable() {


}

// adds statement to table
void addStmt(int stmtNum, vector<int> uses, vector<int> modifies) {


}

// get list of statements stmtNum uses
vector<int> getStmtUses(int stmtNum) {


}

// get list of statements stmtNum modifies
vector<int> getStmtModifies(int stmtNum) {


}