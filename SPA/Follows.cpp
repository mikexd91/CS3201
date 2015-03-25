#include "Follows.h"
#include <set>

using namespace std;

<<<<<<< HEAD
set<Statement*>::iterator setIter;						// stmt set iterator
set<int>::iterator intIter;								// int set iterator

Follows::Follows(void)
{
}
=======
#include "Follows.h"

/*
// constructor: instantiates an empty follows table
Follows::Follows() {
>>>>>>> 6ef66e5d505603d41310ab5f77408e4d9b36deb8


Follows::~Follows(void)
{
}

<<<<<<< HEAD

Statement* getStmtObj(int stmtnum) {
	StmtTable* table = StmtTable.getInstance();
	const set<Statement*> set = table->getAssgStmts();
	for (setIter=set.begin(); setIter!=set.end(); setIter++) {
		Statement* stmt = *setIter;
		int stmtnum2 = stmt->getStmtNum();
		if (stmtnum2 == stmtnum) {
			return stmt;
			//stmt->getFollowedBy();
		}
	}
}

bool Follows::isFollows(int stmtNum1, int stmtNum2) {
	StmtTable* table = StmtTable.getInstance();
	const set<Statement*> assignStmts = table->getAssgStmts();
	// Statement* stmtObj = getStmtObj(stmtNum1, assignStmts);
}

// gets all those before a in Follows(a, b)
vector<int> Follows::getFollows(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	 //FollowsSet& stmtsList = stmtObj->getFollows();
=======
// checks if s2 follows s1
bool isFollows(int stmtNum1, int stmtNum2) {
	//FollowsTable followsTable = FollowsTable();
	//vector<int> results = followsTable.getStmtFollows(stmtNum1);
	//vector<int>::iterator it;
	//it = find(results.begin(), results.end(), stmtNum2);

	//if (results.empty() || (it != results.end())) {
		//return false;
	//} else {
		//return true;
	//}
}

// gets list of statements that follows stmtNum
vector<int> getFollows(int stmtNum) {
	//FollowsTable followsTable = FollowsTable();
	//vector<int> results = followsTable.getStmtFollows(stmtNum);
	//return results;
}

// gets list of statements that stmtNum follows
vector<int> getFollowedBy(int stmtNum) {
	//FollowsTable followsTable = FollowsTable();
	//vector<int> results = followsTable.getStmtFollowedBy(stmtNum);
	//return results;
>>>>>>> 6ef66e5d505603d41310ab5f77408e4d9b36deb8
}

// gets all those after b in Follows(a, b)
vector<int> Follows::getFollowedBy(int stmtNum) {
	Statement* stmtObj = getStmtObj(stmtNum);
	//FollowsSet& stmtsList = stmtObj->getFollowedBy();
}

vector<vector<int>> Follows::getAllFollows(int stmtNum1, int stmtNum2) {

}

bool Follows::isFollowsStar(int stmtNum1, int stmtNum2) {

}

vector<int> Follows::getFollowsStar(int stmtNum) {

}

vector<int> Follows::getFollowedStarBy(int stmtNum) {

}

<<<<<<< HEAD
vector<vector<int>> Follows::getAllFollowsStar(int stmtNum1, int stmtNum2) {

}
=======
}*/
>>>>>>> 6ef66e5d505603d41310ab5f77408e4d9b36deb8
