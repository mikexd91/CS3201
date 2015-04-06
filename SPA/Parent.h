#pragma once
#include <set>

using namespace std;

class Parent
{
public:
	Parent(void);
	~Parent(void);

	bool isParent(int stmtNum1, int stmtNum2);		// checks if s2 follows s1

	int getParent(int stmtNum);						// gets immdiate parent of stmtNum
	set<int> getChild(int stmtNum);					// gets immediate children of stmtNum

	// set<set<int>> getAllParent(int stmtNum1, int stmtNum2);	// gets lists of all possible Parents

	bool isParentStar(int stmtNum1, int stmtNum2);	// checks if Parent*(s1, s2)

	set<int> getParentStar(int stmtNum);			// get set of stmts that are parents of this stmt
	set<int> getChildStar(int stmtNum);				// get set of stmts that are children of this stmt

	// set<set<int>> getAllParentStar(int stmtNum1, int stmtNum2);	// gets list of all possible Parent*
};

