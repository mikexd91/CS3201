#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

#include "Follows.h"
#include "FollowsTable.h"

// constructor: instantiates an empty follows table
Follows::Follows() {


}

// checks if s2 follows s1
bool isFollows(int stmtNum1, int stmtNum2) {
	FollowsTable followsTable = FollowsTable();
	vector<int> results = followsTable.getStmtFollows(stmtNum1);
	vector<int>::iterator it;
	it = find(results.begin(), results.end(), stmtNum2);

	if (results.empty() || (it != results.end())) {
		return false;
	} else {
		return true;
	}
}

// gets list of statements that follows stmtNum
vector<int> getFollows(int stmtNum) {
	FollowsTable followsTable = FollowsTable();
	vector<int> results = followsTable.getStmtFollows(stmtNum);
	return results;
}

// gets list of statements that stmtNum follows
vector<int> getFollowedBy(int stmtNum) {
	FollowsTable followsTable = FollowsTable();
	vector<int> results = followsTable.getStmtFollowedBy(stmtNum);
	return results;
}

// gets lists of all possible follows
vector<vector<int>> getAllFollows(int stmtNum1, int stmtNum2) {
	// iterate through followsTable
	// call getFollows for each item
	// store them in a vector<vector<int>> list
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