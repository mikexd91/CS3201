#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

class Parent {

public:
	Parent();		// constructor: instantiates an empty parent table

	bool isParent(int stmtNum1, int stmtNum2);	// checks if s1 is parent of s2

	vector<int> getParent(int stmtNum);			// get list of parents of stmtNum
	vector<int> getChild(int stmtNum);			// get list of children of stmtNum

	//vector<vector<int>> getAllParent(syn syn1, syn syn2);			// gets lists of all possible parent-child combinations

	bool isParentStar(int stmtNum1, int stmtNum2);	// checks if s1 is parent* of s2

	vector<int> getParentStar(int stmtNum);		// get list of parents* of stmtNum
	vector<int> getChildStarBy(int stmtNum);	// get list of children* of stmtNum

	//vector<vector<int>> getAllParentStar(syn syn1, syn syn2);		// gets lists of all possible parent-child* combinations

private:
	//unordered_map<int, vector<unordered_map<>>> _parentOfTable;
	//unordered_map<int, vector<unordered_map<>>> _childOfTable;

}