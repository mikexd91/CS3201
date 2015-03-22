#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

#include "FollowsTable.h"

// constructor: instantiates an empty follows table
FollowsTable::FollowsTable() {


}

// checks if s2 follows s1
bool isFollows(int stmtNum1, int stmtNum2) {


}

// gets list of statements that follows stmtNum
vector<int> getFollows(int stmtNum) {


}

// gets list of statements that stmtNum follows
vector<int> getFollowedBy(int stmtNum) {


}

// gets lists of all possible follows
vector<vector<int>> getAllFollows(int stmtNum1, int stmtNum2) {


}

// checks if s2 followsStar s1
bool isFollowsStar(int stmtNum1, int stmtNum2) {


}

// gets list of statements that follows* stmtNum
vector<int> getFollowsStar(int stmtNum) {


}

// gets list of statements that stmtNum follows*
vector<int> getFollowedStarBy(int stmtNum) {


}

// gets list of all possible follows*
vector<vector<int>> getAllFollowsStar(int stmtNum1, int stmtNum2) {


}