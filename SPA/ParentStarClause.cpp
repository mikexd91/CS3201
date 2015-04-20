#include "ParentStarClause.h"
#include "Utils.h"
#include <iostream>

#include "boost\lexical_cast.hpp"

using namespace std;
using namespace boost;
using namespace stringconst;

ParentStarClause::ParentStarClause(void):ParentClause(){
}

ParentStarClause::~ParentStarClause(void){
}

bool ParentStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = ((firstType == ARG_GENERIC) || (firstType == ARG_WHILE) || (firstType == ARG_STATEMENT) || (firstType == ARG_PROGLINE) || (firstType == ARG_ASSIGN));
	bool secondArg = ((secondType == ARG_GENERIC) || (secondType == ARG_WHILE) || (secondType == ARG_ASSIGN) || (secondType == ARG_STATEMENT) || (secondType == ARG_PROGLINE));
	return (firstArg && secondArg);
}

// ONLY EVALUATES PROTOTYPE CASES (dealing with while stmts, if stmts not included)
Results ParentStarClause::evaluate(void) {
	// arg properties
	bool firstArgIsFixed = this->getFirstArgFixed();
	bool secondArgIsFixed = this->getSecondArgFixed();

	// CASES
	// Case: Parent*(s1,s2) - stmt1 wild, stmt2 wild
	if(firstArgIsFixed==false && secondArgIsFixed==false) {
		return evaluateS1WildS2Wild();

	// Case: Parent*(s1,2) - stmt1 wild, stmt2 fixed
	} else if(firstArgIsFixed==false && secondArgIsFixed==true) {
		return evaluateS1WildS2Fixed();

	// Case: Parent*(1,s2) - stmt1 fixed, stmt2 wild
	} else if(firstArgIsFixed==true && secondArgIsFixed==false) {
		return evaluateS1FixedS2Wild();

	// Case: Parent*(1,2) - stmt1 fixed, stmt2 fixed
	} else {
		return evaluateS1FixedS2Fixed();
	}
}

// Case: Parent*(s1,s2) - stmt1 wild, stmt2 wild
Results ParentStarClause::evaluateS1WildS2Wild() {
	Results res = Results();
	// set synonyms
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	if(firstType==ARG_GENERIC && secondType==ARG_GENERIC) {
		res.setNumOfSyn(0);
	} else if(firstType==ARG_GENERIC || secondType==ARG_GENERIC) {
		res.setNumOfSyn(1);
		if(firstType != ARG_GENERIC) {
			res.setFirstClauseSyn(this->getFirstArg());
		} else {
			res.setFirstClauseSyn(this->getSecondArg());
		}
	} else {
		res.setNumOfSyn(2);
		res.setFirstClauseSyn(this->getFirstArg());
		res.setSecondClauseSyn(this->getSecondArg());
	}

	if((this->getFirstArg()==this->getSecondArg()) && this->getFirstArg()!="_") {
		return res;
	}

	// recursively determine result for each possible s1, s2 pairs of statements
	string firstArgType = this->getFirstArgType();
	string secondArgType = this->getSecondArgType();

	// get the right statement set
	set<Statement*> s1Set, s2Set;
	set<Statement*>::iterator s1Iter, s2Iter;

	// TODO did not include if statements
	if(firstArgType == ARG_STATEMENT || firstArgType == ARG_GENERIC) {
		s1Set = stmtTable->getAllStmts();
	} else if(firstArgType == ARG_WHILE) {
		s1Set = stmtTable->getWhileStmts();
	}

	if(secondArgType == ARG_STATEMENT || secondArgType == ARG_GENERIC) {
		s2Set = stmtTable->getAllStmts();
	} else if(secondArgType == ARG_WHILE) {
		s2Set = stmtTable->getWhileStmts();
	} else if(secondArgType == ARG_ASSIGN) {
		s2Set = stmtTable->getAssgStmts();
	}

	for(s1Iter=s1Set.begin(); s1Iter!=s1Set.end(); s1Iter++) {
		for(s2Iter=s2Set.begin(); s2Iter!=s2Set.end(); s2Iter++) {
			// skip if both stmts are the same
			if(*s1Iter == *s2Iter) {
				continue;
			}

			string currentS1 = lexical_cast<string>((*s1Iter)->getStmtNum());
			string currentS2 = lexical_cast<string>((*s2Iter)->getStmtNum());

			recurParentCheckS1WildS2Wild(res, currentS1, currentS2, currentS1, currentS2);
		}
	}

	vector<string> temp1 = res.getSinglesResults();
	vector<pair<string,string>> temp2 = res.getPairResults();
	Utils::removeVectorDupes(temp1);
	Utils::removeVectorDupes(temp2);
	res.setSingleResult(temp1);
	res.setPairResult(temp2);

	return res;
}

void ParentStarClause::recurParentCheckS1WildS2Wild(Results& res, string s1, string s2, string originS1, string originS2) {

	// base case 1 - stmts are direct parent/child
	if(isParent(s1, s2)) {
		res.setClausePassed(true);

		// add result depending on whether generics are involved
		string firstType = this->getFirstArgType();
		string secondType = this->getSecondArgType();
		if(firstType == ARG_GENERIC && secondType != ARG_GENERIC) {
			res.addSingleResult(originS2);
		} else if(firstType != ARG_GENERIC && secondType == ARG_GENERIC) {
			res.addSingleResult(originS1);
		} else if(firstType != ARG_GENERIC && secondType != ARG_GENERIC) {
			res.addPairResult(originS1, originS2);
		}

	// base case 2 - checking same stmt
	} else if(s1 == s2) {
		return;
	} else {
		// get all children of first arg
		set<int> argChildren = stmtTable->getStmtObj(lexical_cast<int>(s1))->getChildren();

		// base case 2 - s1 has no children
		if(argChildren.size() == 0) {
			return;

		// recursive case - for each child check evaluation
		} else {
			// iterate through children and recursively check for parent
			set<int>::iterator setIter;
			for(setIter=argChildren.begin(); setIter!=argChildren.end(); setIter++) {
				string currentStmt = lexical_cast<string>(*setIter);

				recurParentCheckS1WildS2Wild(res, currentStmt, s2, originS1, originS2);
			}
		}
	}
}

// Case: Parent*(s1,2) - stmt1 wild, stmt2 fixed
Results ParentStarClause::evaluateS1WildS2Fixed() {
	Results res = Results();
	// set synonyms
	if(this->getFirstArgType() == ARG_GENERIC) {
		res.setNumOfSyn(0);
	} else {
		res.setNumOfSyn(1);
		res.setFirstClauseSyn(this->getFirstArg());
	}

	string secondArg = this->getSecondArg();

	// recursively determine result for each possible s1 statement
	string firstArgType = this->getFirstArgType();

	// get the right statement set
	if(firstArgType == ARG_WHILE) {
		set<Statement*> whileSet = stmtTable->getWhileStmts();
		set<Statement*>::iterator stmtIter;
		
		for(stmtIter=whileSet.begin(); stmtIter!=whileSet.end(); stmtIter++) {
			string currentStmtNum = lexical_cast<string>((*stmtIter)->getStmtNum());
			recurParentCheckS1WildS2Fixed(res, currentStmtNum, currentStmtNum);
		}

	} else if(firstArgType == ARG_STATEMENT || firstArgType == ARG_GENERIC) {
		StmtTable::StmtTableIterator stmtIter;

		for(stmtIter=stmtTable->getIterator(); stmtIter!=stmtTable->getEnd(); stmtIter++) {
			string currentStmtNum = lexical_cast<string>(stmtIter->first);
			recurParentCheckS1WildS2Fixed(res, currentStmtNum, currentStmtNum);
		}	
	}

	vector<string> temp1 = res.getSinglesResults();
	vector<pair<string,string>> temp2 = res.getPairResults();
	Utils::removeVectorDupes(temp1);
	Utils::removeVectorDupes(temp2);
	res.setSingleResult(temp1);
	res.setPairResult(temp2);

	return res;
}

void ParentStarClause::recurParentCheckS1WildS2Fixed(Results &res, string s1, string originS1) {
	string secondArg = this->getSecondArg();

	// base case 1 - stmts are direct parent/child
	if(isParent(s1, secondArg)) {
		res.setClausePassed(true);

		// add if wild is not generic
		if(this->getFirstArgType() != ARG_GENERIC) {
			res.addSingleResult(originS1);
		}
	} else {
		// get all children of first arg (type does not matter)
		Statement* currStmt = stmtTable->getStmtObj(lexical_cast<int>(s1));

		// base case 2 - s1 doesn't exist
		if(currStmt == nullptr) {
			return;
		}

		set<int> argChildren = currStmt->getChildren();

		// base case 3 - s1 has no children
		if(argChildren.size() == 0) {
			return;

		// recursive case - for each child check evaluation
		} else {
			// iterate through children and recursively check for parent
			set<int>::iterator setIter;
			for(setIter=argChildren.begin(); setIter!=argChildren.end(); setIter++) {
				string currentStmt = lexical_cast<string>(*setIter);

				recurParentCheckS1WildS2Fixed(res, currentStmt, originS1);
			}
		}
	}
}

// Case: Parent*(1,s2) - stmt1 fixed, stmt2 wild
Results ParentStarClause::evaluateS1FixedS2Wild() {
	Results res = Results();
	// set synonyms
	if(this->getSecondArgType() == ARG_GENERIC) {
		res.setNumOfSyn(0);
	} else {
		res.setNumOfSyn(1);
		res.setFirstClauseSyn(this->getSecondArg());
	}

	string firstArg = this->getFirstArg();

	// recursively determine result for each possible s2 statement
	string secondArgType = this->getSecondArgType();

	// get the right statement set
	if(secondArgType == ARG_ASSIGN) {
		set<Statement*> assignSet = stmtTable->getAssgStmts();
		set<Statement*>::iterator stmtIter;
		
		for(stmtIter=assignSet.begin(); stmtIter!=assignSet.end(); stmtIter++) {
			string currentStmtNum = lexical_cast<string>((*stmtIter)->getStmtNum());
			recurParentCheckS1FixedS2Wild(res, firstArg, currentStmtNum, currentStmtNum);
		}

	} else if(secondArgType == ARG_WHILE) {
		set<Statement*> whileSet = stmtTable->getWhileStmts();
		set<Statement*>::iterator stmtIter;
		
		for(stmtIter=whileSet.begin(); stmtIter!=whileSet.end(); stmtIter++) {
			string currentStmtNum = lexical_cast<string>((*stmtIter)->getStmtNum());
			recurParentCheckS1FixedS2Wild(res, firstArg, currentStmtNum, currentStmtNum);
		}

	} else if(secondArgType == ARG_STATEMENT || secondArgType == ARG_GENERIC) {
		StmtTable::StmtTableIterator stmtIter;

		for(stmtIter=stmtTable->getIterator(); stmtIter!=stmtTable->getEnd(); stmtIter++) {
			string currentStmtNum = lexical_cast<string>(stmtIter->first);
			recurParentCheckS1FixedS2Wild(res, firstArg, currentStmtNum, currentStmtNum);
		}		
	}

	vector<string> temp1 = res.getSinglesResults();
	vector<pair<string,string>> temp2 = res.getPairResults();
	Utils::removeVectorDupes(temp1);
	Utils::removeVectorDupes(temp2);
	res.setSingleResult(temp1);
	res.setPairResult(temp2);

	return res;
}

void ParentStarClause::recurParentCheckS1FixedS2Wild(Results &res, string s1, string s2, string originS2) {

	// base case 1 - stmts are direct parent/child
	if(isParent(s1, s2)) {
		res.setClausePassed(true);
		
		// add result if wild is not a generic
		if(this->getSecondArgType() != ARG_GENERIC) {
			res.addSingleResult(originS2);
		}
	} else {
		// get all children of first arg
		Statement* currStmt = stmtTable->getStmtObj(lexical_cast<int>(s1));

		// base case 2 - s1 doesn't exist
		if(currStmt == nullptr) {
			return;
		}

		set<int> argChildren = currStmt->getChildren();

		// base case 3 - s1 has no children
		if(argChildren.size() == 0) {
			return;

		// recursive case - for each child check evaluation
		} else {
			// iterate through children and recursively check for parent
			set<int>::iterator setIter;
			for(setIter=argChildren.begin(); setIter!=argChildren.end(); setIter++) {
				string currentStmt = lexical_cast<string>(*setIter);

				recurParentCheckS1FixedS2Wild(res, currentStmt, s2, originS2);
			}
		}
	}
}

// Case: Parent*(1,2) - stmt1 fixed, stmt2 fixed
Results ParentStarClause::evaluateS1FixedS2Fixed() {
	Results res;
	// set synonyms
	res.setNumOfSyn(0);

	string firstArg = this->getFirstArg();
	string secondArg = this->getSecondArg();

	// recursively determine result
	recurParentCheckS1FixedS2Fixed(res, firstArg, secondArg);

	vector<string> temp1 = res.getSinglesResults();
	vector<pair<string,string>> temp2 = res.getPairResults();
	Utils::removeVectorDupes(temp1);
	Utils::removeVectorDupes(temp2);
	res.setSingleResult(temp1);
	res.setPairResult(temp2);

	return res;
}

void ParentStarClause::recurParentCheckS1FixedS2Fixed(Results &res, string s1, string s2) {
	// base case 1 - stmts are direct parent/child
	if(isParent(s1, s2)) {
		res.setClausePassed(true);
	// base case 2 - checking same stmt
	} else if(s1 == s2) {
		return;
	} else {
		// get all children of first arg
		Statement* currStmt = stmtTable->getStmtObj(lexical_cast<int>(s1));
		
		// base case 2 - s1 doesn't exist
		if(currStmt == nullptr) {
			return;
		}

		set<int> argChildren = currStmt->getChildren();

		// base case 3 - s1 has no children
		if(argChildren.size() == 0) {
			return;

		// recursive case - for each child check evaluation
		} else {
			// iterate through children and recursively check for parent
			set<int>::iterator setIter;
			for(setIter=argChildren.begin(); setIter!=argChildren.end(); setIter++) {
				string currentStmt = lexical_cast<string>(*setIter);

				recurParentCheckS1FixedS2Fixed(res, currentStmt, s2);
			}
		}
	}
}