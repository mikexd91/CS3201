#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

class Follows {

public:
	Follows();		// constructor: instantiates an empty follows table

	bool isFollows(int stmtNum1, int stmtNum2);		// checks if s2 follows s1

	vector<int> getFollows(int stmtNum);			// gets list of statements that follows stmtNum
	vector<int> getFollowedBy(int stmtNum);			// gets list of statements that stmtNum follows

	vector<vector<int>> getAllFollows(int stmtNum1, int stmtNum2);	// gets lists of all possible follows

	bool isFollowsStar(int stmtNum1, int stmtNum2);	// checks if s2 followsStar s1

	vector<int> getFollowsStar(int stmtNum);		// gets list of statements that follows* stmtNum
	vector<int> getFollowedStarBy(int stmtNum);		// gets list of statements that stmtNum follows*

	vector<vector<int>> getAllFollowsStar(int stmtNum1, int stmtNum2);	// gets list of all possible follows*

private:
	//unordered_map<int, vector<unordered_map<>>> _followsTable;
	//unordered_map<int, vector<unordered_map<>>> _followedByTable;

};