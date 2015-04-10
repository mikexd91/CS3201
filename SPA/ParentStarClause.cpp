#include "ParentStarClause.h"
#include "Utils.h"

ParentStarClause::ParentStarClause(void):ParentClause(){
}

ParentStarClause::~ParentStarClause(void){
}

bool ParentStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = ((firstType == stringconst::ARG_WHILE) || (firstType == stringconst::ARG_STATEMENT));
	bool secondArg = ((secondType == stringconst::ARG_WHILE) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_STATEMENT));
	return (firstArg && secondArg);
}

Results ParentStarClause::evaluate(void) {


	return Results();
}

// Case: Parent*(s1,s2) - stmt1 wild, stmt2 wild
Results ParentStarClause::evaluateS1WildS2Wild() {

	return Results();
}

// Case: Parent*(s1,2) - stmt1 wild, stmt2 fixed
Results ParentStarClause::evaluateS1WildS2Fixed() {

	return Results();
}

// Case: Parent*(1,s2) - stmt1 fixed, stmt2 wild
Results ParentStarClause::evaluateS1FixedS2Wild() {

	return Results();
}

// Case: Parent*(1,2) - stmt1 fixed, stmt2 fixed
Results ParentStarClause::evaluateS1FixedS2Fixed() {

	return Results();
}