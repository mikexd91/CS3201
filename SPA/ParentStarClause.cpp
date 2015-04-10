#include "ParentStarClause.h"
#include "Utils.h"

ParentStarClause::ParentStarClause(void):ParentClause(){
}

ParentStarClause::~ParentStarClause(void){
}

bool ParentStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_WHILE);
	return (firstArg && secondArg);
}

Results ParentStarClause::evaluate(void) {


	return Results();
}