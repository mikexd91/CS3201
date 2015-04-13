#include "UsesClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "Results.h"

#include <set>
#include "boost\lexical_cast.hpp"

using namespace boost;
using namespace std;

StmtTable* stmtTable = StmtTable::getInstance();
VarTable* varTable = VarTable::getInstance();

UsesClause::UsesClause(void):Clause(USES_){
}

UsesClause::~UsesClause(void){
}

bool UsesClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_VARIABLE);
	return (firstArg && secondArg);
}

// ONLY EVALUATES PROTOTYPE CASES (dealing with assg stmts and vars)
Results UsesClause::evaluate(void) {
	// arg properties
	bool firstArgIsFixed = this->getFirstArgFixed();
	bool secondArgIsFixed = this->getSecondArgFixed();
	
	// CASES
	// Case: Uses(s,v) - stmt wild, var wild
	if(firstArgIsFixed==false && secondArgIsFixed==false) {
		return evaluateStmtWildVarWild();
		
	// Case: Uses(s,'x') - stmt wild, var fixed
	} else if(firstArgIsFixed==false && secondArgIsFixed==true) {
		return evaluateStmtWildVarFixed();

	// Case: Uses(1,v) - stmt fixed, var wild
	} else if(firstArgIsFixed==true && secondArgIsFixed==false) {
		return evaluateStmtFixedVarWild();

	// Case: Uses(1,'x') - stmt fixed, var fixed
	} else {
		return evaluateStmtFixedVarFixed();
	}
}


// PRIVATE FUNCTIONS
// Case: Uses(s,v) - stmt wild, var wild
Results UsesClause::evaluateStmtWildVarWild() {
	Results res = Results();
	// set synonyms
	res.setNumOfSyn(2);
	res.setFirstClauseSyn(this->getFirstArg());
	res.setSecondClauseSyn(this->getSecondArg());

	// generate all possible combinations using stmtTable as reference
	StmtTable::StmtTableIterator stmtTIter;
	// iterate through stmt table
	for(stmtTIter=stmtTable->getIterator(); stmtTIter!=stmtTable->getEnd(); stmtTIter++) {
		Statement* currentStmt = stmtTIter->second;

		// check if current stmt conforms to specific stmt type, if not, skip to next statement
		if(!Utils::isSameType(this->firstArgType, currentStmt->getType())) {
			continue;
		}

		// for each stmt generate result pair for vars that it uses
		Statement::UsesSet currentUseSet = currentStmt->getUses();
		Statement::UsesSet::iterator useIter;
		for(useIter=currentUseSet.begin(); useIter!=currentUseSet.end(); useIter++) {
			string stmtNum = lexical_cast<string>(currentStmt->getStmtNum());
			string var = *useIter;

			res.addPairResult(stmtNum, var);
		}
	}

	// checks if result is empty
	if(res.getPairResults().size() != 0) {
		res.setClausePassed(true);
	}

	return res;
}

// Case: Uses(s,'x') - stmt wild, var fixed
Results UsesClause::evaluateStmtWildVarFixed() {
	Results res = Results();
	// set synonyms
	res.setNumOfSyn(1);
	res.setFirstClauseSyn(this->getFirstArg());

	// get the fixed var and usedby
	Variable* fixedVar = varTable->getVariable(this->getSecondArgType());
	set<int>::iterator stmtIter;
	set<int> stmtSet = fixedVar->getUsedByStmts();

	// check set for results
	if(stmtSet.size() != 0) {
		res.setClausePassed(true);
		for(stmtIter=stmtSet.begin(); stmtIter!=stmtSet.end(); stmtIter++) {
			Statement* currentStmt = stmtTable->getStmtObj(*stmtIter);
			// check if current stmt conforms to specific stmt type
			if(Utils::isSameType(this->firstArgType, currentStmt->getType())) {
				res.addSingleResult(lexical_cast<string>(*stmtIter));
			}
		}
	}

	return res;
}

// Case: Uses(1,v) - stmt fixed, var wild
Results UsesClause::evaluateStmtFixedVarWild() {
	Results res = Results();
	// set synonyms
	res.setNumOfSyn(1);
	res.setFirstClauseSyn(this->getSecondArg());

	// get relevant stmts
	string firstArgType = this->getFirstArgType();
	set<Statement*>::iterator stmtIter;
	set<Statement*> stmtSet;
	if(firstArgType == stringconst::ARG_WHILE) {				// only while stmts
		stmtSet = stmtTable->getWhileStmts();
	} else if(firstArgType == stringconst::ARG_ASSIGN) {		// only assign stmts
		stmtSet = stmtTable->getAssgStmts();
	} else {													// all types of stmts
		// TODO for all stmts
	}

	int stmtNum = lexical_cast<int>(this->getFirstArg());

	// check stmts
	for(stmtIter=stmtSet.begin(); stmtIter!=stmtSet.end(); stmtIter++) {
		// current stmt
		Statement* currentStmt = *stmtIter;
		if(currentStmt->getStmtNum() == stmtNum) {
			// get set of variables current stmt uses
			Statement::UsesSet currentUses = currentStmt->getUses();

			// check if stmt uses any variable
			if(currentUses.size() != 0) {
				res.setClausePassed(true);

				// add all pairs into results
				Statement::UsesSet::iterator setIter;
				for(setIter=currentUses.begin(); setIter!=currentUses.end(); setIter++) {
					res.addSingleResult(*setIter);
				}
			}
		}
	}

	return res;
}

// Case: Uses(1,'x') - stmt fixed, var fixed
Results UsesClause::evaluateStmtFixedVarFixed() {
	Results res = Results();
	// set synonyms
	res.setNumOfSyn(0);

	// get relevant stmts
	string firstArgType = this->getFirstArgType();
	set<Statement*>::iterator stmtIter;
	set<Statement*> stmtSet;
	if(firstArgType == stringconst::ARG_WHILE) {				// only while stmts
		stmtSet = stmtTable->getWhileStmts();
	} else if(firstArgType == stringconst::ARG_ASSIGN) {		// only assign stmts
		stmtSet = stmtTable->getAssgStmts();
	} else {													// all types of stmts
		// TODO for all stmts
	}

	int stmtNum = stoi(this->getFirstArg(), nullptr);

	// check stmts
	for(stmtIter=stmtSet.begin(); stmtIter!=stmtSet.end(); stmtIter++) {
		// current stmt
		Statement* currentStmt = *stmtIter;
		if(currentStmt->getStmtNum() == stmtNum) {
			// get set of variables current stmt uses
			Statement::UsesSet currentUses = currentStmt->getUses();
					
			// checks if current stmt uses variable
			if(currentUses.find(this->getSecondArg()) != currentUses.end()) {
				res.setClausePassed(true);
			}
			break;
		}
	}

	return res;
}