#include "UsesClause.h"
#include "Utils.h"
#include "StmtTable.h"
#include "Results.h"

#include <set>

StmtTable* stmtTable = StmtTable::getInstance();

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
	// first arg properties
	string firstArgType = this->getFirstArgType();
	bool firstArgIsFixed = this->getFirstArgFixed();

	// second arg properties
	string secondArgType = this->getSecondArgType();
	bool secondArgIsFixed = this->getSecondArgFixed();
	
	// ASSIGN CASES
	if(firstArgType==stringconst::ARG_ASSIGN && secondArgType==stringconst::ARG_VARIABLE) {
		// get set of assignment stmts and iterator
		set<Statement*> assgStmtSet = stmtTable->getAssgStmts();
		set<Statement*>::iterator assgIter;

		// Assign Case: Uses(a,v) - assign wild, var wild
		if(firstArgIsFixed==false && secondArgIsFixed==false) {

		
		// Assign Case: Uses(a,'x') - assign wild, var fixed
		} else if(firstArgIsFixed==false && secondArgIsFixed==true) {


		// Assign Case: Uses(1,v) - assign fixed, var wild
		} else if(firstArgIsFixed==true && secondArgIsFixed==false) {


		// Assign Case: Uses(1,'x') - assign fixed, var fixed
		} else {
			int stmtNum = stoi(this->getFirstArg(), nullptr);
			for(assgIter=assgStmtSet.begin(); assgIter!=assgStmtSet.end(); assgIter++) {
				// current stmt
				Statement* currentStmt = *assgIter;
				if(currentStmt->getStmtNum() == stmtNum) {
					// get set of variables current stmt uses
					Statement::UsesSet currentUses = currentStmt->getUses();
					
					// checks if current stmt uses variable
					if(currentUses.find(this->getSecondArg()) != currentUses.end()) {
						Results res = Results();
						res.setClauseTrue();
					}
				}
			}
		}
	}

	return Results();
}
