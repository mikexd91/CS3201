#include "Follows.h"
#include <set>

using namespace std;

set<Statement*>::iterator setIter;						// stmt set iterator
set<int>::iterator intIter;								// int set iterator

Follows::Follows(void)
{
}


Follows::~Follows(void)
{
}

Statement* getStmtObj(int stmtnum, set<Statement*> set) {
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

vector<int> Follows::getFollows(int stmtNum) {

}

vector<int> Follows::getFollowedBy(int stmtNum) {

}

vector<vector<int>> Follows::getAllFollows(int stmtNum1, int stmtNum2) {

}

bool Follows::isFollowsStar(int stmtNum1, int stmtNum2) {

}

vector<int> Follows::getFollowsStar(int stmtNum) {

}

vector<int> Follows::getFollowedStarBy(int stmtNum) {

}

vector<vector<int>> Follows::getAllFollowsStar(int stmtNum1, int stmtNum2) {

}
