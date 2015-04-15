#include "FollowsStarClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace stringconst;
using namespace boost;

FollowsStarClause::FollowsStarClause(void):Clause(FOLLOWSSTAR_){
}

FollowsStarClause::~FollowsStarClause(void){
}

bool FollowsStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE) || (firstType == stringconst::ARG_PROGLINE);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_WHILE) || (secondType == stringconst::ARG_PROGLINE);
	return (firstArg && secondArg);
}

int FollowsStarClause::getFirstStmtNum() {
	return atoi(getFirstArg().c_str());
}

int FollowsStarClause::getSecondStmtNum() {
	return atoi(getSecondArg().c_str());
}

string FollowsStarClause::getFirstStmtSyn() {
	return getFirstArg();
}

string FollowsStarClause::getSecondStmtSyn() {
	return getSecondArg();
}

string FollowsStarClause::getFirstStmtType() {
	return getFirstArgType();
}

string FollowsStarClause::getSecondStmtType() {
	return getSecondArgType();
}

Results FollowsStarClause::evaluate(void) {

	if (firstArgFixed) {
		if (secondArgFixed) {
			return evaluateFollowsStarFixedFixed(getFirstStmtNum(), getSecondStmtNum());
		} else {
			return evaluateFollowsStarFixedSyn(getFirstStmtNum(), getSecondStmtSyn());
		}
	} else {
		if (secondArgFixed) {
			return evaluateFollowsStarSynFixed(getFirstStmtSyn(), getSecondStmtNum());
		} else {
			return evaluateFollowsStarSynSyn(getFirstStmtSyn(), getSecondStmtSyn());
		}
	}

}

Results FollowsStarClause::evaluateFollowsStarFixedFixed(int firstStmtNum, int secondStmtNum) {
	Results* res = new Results();
	res->setNumOfSyn(0);

	res->setClausePassed(isFollowsStar(firstStmtNum, secondStmtNum));

	return *res;
}

Results FollowsStarClause::evaluateFollowsStarFixedSyn(int firstStmtNum, string secondStmtSyn) {
	Results* res = new Results();
	res->setNumOfSyn(1);
	res->setFirstClauseSyn(secondStmtSyn);

	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allStmt2;	
	
	if (getSecondStmtType() == stringconst::ARG_ASSIGN) {
		allStmt2 = stable->getAssgStmts();
	} else if (getSecondStmtType() == stringconst::ARG_WHILE) {
		allStmt2 = stable->getWhileStmts();
	} else {
		allStmt2 = stable->getAllStmts();
	}
	
	BOOST_FOREACH(auto p, allStmt2) {
		int secondStmtNum = p->getStmtNum();
		if (isFollowsStar(firstStmtNum, secondStmtNum)) {
			res->addSingleResult(lexical_cast<string>(secondStmtNum));
		}
	}

	res->setClausePassed(res->getSinglesResults().size() > 0);

	return *res;
}

Results FollowsStarClause::evaluateFollowsStarSynFixed(string firstStmtSyn, int secondStmtNum) {
	Results* res = new Results();
	res->setNumOfSyn(1);
	res->setFirstClauseSyn(firstStmtSyn);

	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allStmt1;	
	
	if (getFirstStmtType() == stringconst::ARG_ASSIGN) {
		allStmt1 = stable->getAssgStmts();
	} else if (getFirstStmtType() == stringconst::ARG_WHILE) {
		allStmt1 = stable->getWhileStmts();
	} else {
		allStmt1 = stable->getAllStmts();
	}

	BOOST_FOREACH(auto p, allStmt1) {
		int firstStmtNum = p->getStmtNum();
		if (isFollowsStar(firstStmtNum, secondStmtNum)) {
			res->addSingleResult(lexical_cast<string>(firstStmtNum));
		}
	}

	res->setClausePassed(res->getSinglesResults().size() > 0);

	return *res;
}

Results FollowsStarClause::evaluateFollowsStarSynSyn(string firstStmtSyn, string secondStmtSyn) {
	Results* res = new Results();
	res->setFirstClauseSyn(firstStmtSyn);
	res->setSecondClauseSyn(secondStmtSyn);
	res->setNumOfSyn(2);

	StmtTable* stable = StmtTable::getInstance();
	set<Statement*> allStmt1;
	set<Statement*> allStmt2;
	
	if (getFirstStmtType() == ARG_ASSIGN) {
		allStmt1 = stable->getAssgStmts();
	} else if (getFirstStmtType() == ARG_WHILE) {
		allStmt1 = stable->getWhileStmts();
	} else {
		allStmt1 = stable->getAllStmts();
	}

	if (getSecondStmtType() == ARG_ASSIGN) {
		allStmt2 = stable->getAssgStmts();
	} else if (getSecondStmtType() == ARG_WHILE) {
		allStmt2 = stable->getWhileStmts();
	} else {
		allStmt2 = stable->getAllStmts();
	}

	//cout << endl << "all stmt size " << allStmt.size() << endl;

	BOOST_FOREACH(auto p, allStmt1) {
		int firstStmtNum = p->getStmtNum();

		BOOST_FOREACH(auto q, allStmt2) {
		//for (size_t i = 0; i < allVarNames->size(); i++) {
			int secondStmtNum = q->getStmtNum();
			if (isFollowsStar(firstStmtNum, secondStmtNum)) {
				res->addPairResult(lexical_cast<string>(firstStmtNum), lexical_cast<string>(secondStmtNum));
			}
		}
	}

	res->setClausePassed(res->getPairResults().size() > 0);

	return *res;
}

bool FollowsStarClause::isFollowsStar(int stmtNum1, int stmtNum2) {

	if (stmtNum2 <= stmtNum1) {
		return false;
	}
	
	StmtTable* stable = StmtTable::getInstance();
	Statement* stmt1 = stable->getStmtObj(stmtNum1);
	
	int nextFollows = stmt1->getFollowsAfter();

	while (nextFollows != -1) {
		if (nextFollows == stmtNum2) {
			return true;
		}
		nextFollows = stable->getStmtObj(nextFollows)->getFollowsAfter();
	}

	return false;
}

