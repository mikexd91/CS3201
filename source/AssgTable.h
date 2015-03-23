#pragma once

#include <vector>
#include <boost/unordered_map.hpp>

using namespace std;
using namespace boost;

class AssgTable {

public:
	AssgTable();	// constructor: instantiates an empty AssgTable

	void addStmt(int stmtNum, vector<int> uses, vector<int> modifies); // adds statement to table

	vector<int> getStmtUses(int stmtNum);		// get list of statements stmtNum uses
	vector<int> getStmtModifies(int stmtNum);	// get list of statements stmtNum modifies

private:
	//unordered_map<int, vector<unordered_map<>>> _AssgTable;

}