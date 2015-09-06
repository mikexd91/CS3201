#include "FollowsStarClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include <iostream>
#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

using namespace stringconst;
using namespace boost;
using namespace std;

FollowsStarClause::FollowsStarClause(void):Clause(FOLLOWSSTAR_){
}

FollowsStarClause::~FollowsStarClause(void){
}

bool FollowsStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) 
		|| (firstType == stringconst::ARG_ASSIGN) 
		|| (firstType == stringconst::ARG_WHILE) 
		|| (firstType == stringconst::ARG_PROGLINE) 
		|| (firstType == stringconst::ARG_GENERIC);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) 
		|| (secondType == stringconst::ARG_ASSIGN) 
		|| (secondType == stringconst::ARG_WHILE) 
		|| (secondType == stringconst::ARG_PROGLINE) 
		|| (secondType == stringconst::ARG_GENERIC);
	return (firstArg && secondArg);
}

bool FollowsStarClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	return false;
};

bool FollowsStarClause::evaluateS1GenericS2Generic() {
	return false;
};

bool FollowsStarClause::evaluateS1GenericS2Fixed(string s2){
	return false;
};

bool FollowsStarClause::evaluateS1FixedS2Generic(string s1) {
	return false;
}

unordered_set<string> FollowsStarClause::getAllS2WithS1Fixed(string s1) {
	return unordered_set<string>();
}

unordered_set<string> FollowsStarClause::getAllS2() {
	return unordered_set<string>();
}

unordered_set<string> FollowsStarClause::getAllS1WithS2Fixed(string s2) {
	return unordered_set<string>();
}

unordered_set<string> FollowsStarClause::getAllS1() {
	return unordered_set<string>();
}

Results::ResultsTable* FollowsStarClause::getAllS1AndS2() {
	return new Results::ResultsTable;
}


/**
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
	res->setNumOfSyn(0);
	if (getSecondStmtType() != ARG_GENERIC) {
		res->setNumOfSyn(1);
		res->setFirstClauseSyn(secondStmtSyn);
	}

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
	res->setNumOfSyn(0);
	if (getFirstStmtType() != ARG_GENERIC) {
		res->setNumOfSyn(1);
		res->setFirstClauseSyn(firstStmtSyn);
	}

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
	bool isFirstGeneric = getFirstStmtType() == ARG_GENERIC;
	bool isSecondGeneric = getSecondStmtType() == ARG_GENERIC;
	bool isBothGeneric = isFirstGeneric && isSecondGeneric;

	Results* res = new Results();
	res->setNumOfSyn(0);
	if (isBothGeneric) {
		res->setNumOfSyn(0);
	} else if (isFirstGeneric) {
		res->setFirstClauseSyn(secondStmtSyn);
		res->setNumOfSyn(1);
	} else if (isSecondGeneric) {
		res->setFirstClauseSyn(firstStmtSyn);
		res->setNumOfSyn(1);
	} else {
		res->setFirstClauseSyn(firstStmtSyn);
		res->setSecondClauseSyn(secondStmtSyn);
		res->setNumOfSyn(2);
	}

	// check if same, unless if _,_
	if ((getFirstStmtType() != ARG_GENERIC) 
		|| (getSecondStmtType() != ARG_GENERIC)) {
		if (firstStmtSyn == secondStmtSyn) {
			return *res;
		}
	}

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

	set<string>* singles = new set<string>();
	set<pair<string, string>>* pairs = new set<pair<string, string>>();

	BOOST_FOREACH(auto p, allStmt1) {
		int firstStmtNum = p->getStmtNum();

		BOOST_FOREACH(auto q, allStmt2) {
			int secondStmtNum = q->getStmtNum();
			if (isFollowsStar(firstStmtNum, secondStmtNum)) {
				// put in a set first, then from set put in vector.
				if (isBothGeneric) {
					pair<string, string> pr = pair<string, string>(lexical_cast<string>(firstStmtNum), lexical_cast<string>(secondStmtNum));
					pairs->emplace(pr);
				} else if (isFirstGeneric) {
					singles->emplace(lexical_cast<string>(secondStmtNum));
				} else if (isSecondGeneric) {
					singles->emplace(lexical_cast<string>(firstStmtNum));
				} else {
					pair<string, string> pr = pair<string, string>(lexical_cast<string>(firstStmtNum), lexical_cast<string>(secondStmtNum));
					pairs->emplace(pr);
				}
			}
		}
	}

	// transfer from set to vector
	if (isBothGeneric) {
		transferPairsToResult(pairs, res);
	} else if (isFirstGeneric) {
		transferSinglesToResult(singles, res);
	} else if (isSecondGeneric) {
		transferSinglesToResult(singles, res);
	} else {
		transferPairsToResult(pairs, res);
	}
	
	return *res;
}

void FollowsStarClause::transferSinglesToResult(set<string>* singles, Results* res) {
	
	BOOST_FOREACH(auto p, *singles) {
		res->addSingleResult(p);
	}
	res->setClausePassed(res->getSinglesResults().size() > 0);
}

void FollowsStarClause::transferPairsToResult(set<pair<string, string>>* pairs, Results* res) {
	
	BOOST_FOREACH(auto p, *pairs) {
		res->addPairResult(p.first, p.second);
	}
	res->setClausePassed(res->getPairResults().size() > 0);
}

bool FollowsStarClause::isFollowsStar(int stmtNum1, int stmtNum2) {

	if (stmtNum2 <= stmtNum1) {
		return false;
	}
	
	StmtTable* stable = StmtTable::getInstance();
	Statement* stmt1 = stable->getStmtObj(stmtNum1);

	if (stmt1 == nullptr) {
		return false;
	}
	
	int nextFollows = stmt1->getFollowsAfter();

	while (nextFollows != -1) {
		if (nextFollows == stmtNum2) {
			return true;
		}
		if (stable->getStmtObj(nextFollows) == nullptr) {
			return false;
		}
		nextFollows = stable->getStmtObj(nextFollows)->getFollowsAfter();
	}

	return false;
}**/

