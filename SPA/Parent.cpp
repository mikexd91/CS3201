#pragma once
#include "Parent.h"
#include "StmtTable.h"

using namespace std;
// constructor: instantiates an empty parent table
Parent::Parent() {
	
}

// checks if s1 is parent of s2
bool isParent(int stmtNum1, int stmtNum2) {
	ParentTable parentTable = ParentTable(); // need to clarify this
	vector<int> results = parentTable.getStmtChildren(stmtNum1);
	vector<int>::iterator it;
	it = find(results.begin(), results.end(), stmtNum2);

	if (results.empty() || (it != results.end())) {
		return false;
	} else {
		return true;
	}
}

// get list of parents of stmtNum
vector<int> getParent(int stmtNum) {
	ParentTable parentTable = ParentTable();
	vector<int> results = parentTable.getStmtParents(stmtNum);
	return results;
}

// get list of children of stmtNum
vector<int> getChild(int stmtNum) {
	ParentTable parentTable = ParentTable();
	vector<int> results = parentTable.getStmtChildren(stmtNum);
	return results;
}

/*
// gets lists of all possible parent-child combinations
vector<vector<int>> getAllParent(syn syn1, syn syn2) {


} */

// checks if s1 is parent* of s2
bool isParentStar(int stmtNum1, int stmtNum2) {
	// get parent of stmtNum2
	// check if parent == stmtNum1
	// if stmtNum 1 parent returns none, return false
	// if yes, return true
	// else, get parent of stmtNum1 (do recursion)
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