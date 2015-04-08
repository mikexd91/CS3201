#include "ParentClause.h"
#include "Utils.h"

ParentClause::ParentClause(void):Clause(PARENT_){
}

ParentClause::~ParentClause(void){
}

bool ParentClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = ((firstType == stringconst::ARG_WHILE);
	bool secondArg = ((secondType == stringconst::ARG_WHILE);
	return (firstArg && secondArg);
}

Results ParentClause::evaluate(void) {
	return Results();
}