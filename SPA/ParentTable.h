#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

class ParentTable {

public:
	ParentTable();	// constructor: instantiates an empty parent table

	void addStmt(int stmtNum, vector<int> children); // adds statement to table

	vector<int> getStmtChildren(int stmtNum);		// get list of statements stmtNum follows
	vector<int> getStmtParents(int stmtNum);		// get list of statements followed by stmtNum

private:
	//unordered_map<int, vector<unordered_map<>>> _ParentOfTable;
	//unordered_map<int, vector<unordered_map<>>> _ChildrenTable;

};