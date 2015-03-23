#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

class FollowsTable {

public:
	FollowsTable();	// constructor: instantiates an empty follows table

	void addStmt(int stmtNum, vector<int> follows); // adds statement to table

	vector<int> getStmtFollows(int stmtNum);		// get list of statements stmtNum follows
	vector<int> getStmtFollowedBy(int stmtNum);		// get list of statements followed by stmtNum

private:
	//unordered_map<int, vector<unordered_map<>>> _FollowsTable;
	//unordered_map<int, vector<unordered_map<>>> _FollowedByTable;

};