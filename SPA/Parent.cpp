#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

#include "Parent.h"

// constructor: instantiates an empty parent table
Parent::Parent() {


}

// checks if s1 is parent of s2
bool isParent(int stmtNum1, int stmtNum2) {


}

// get list of parents of stmtNum
vector<int> getParent(int stmtNum) {


}

// get list of children of stmtNum
vector<int> getChild(int stmtNum) {


}

/*
// gets lists of all possible parent-child combinations
vector<vector<int>> getAllParent(syn syn1, syn syn2) {


} */

// checks if s1 is parent* of s2
bool isParentStar(int stmtNum1, int stmtNum2) {


}

// get list of parents* of stmtNum
vector<int> getParentStar(int stmtNum) {


}

// get list of children* of stmtNum
vector<int> getChildStarBy(int stmtNum) {


}

/*
// gets lists of all possible parent-child* combinations
vector<vector<int>> getAllParentStar(syn syn1, syn syn2) {


} */