#include "ModifiesClause.h"
#include "Utils.h"

ModifiesClause::ModifiesClause(void):Clause(MODIFIES_){
}

ModifiesClause::~ModifiesClause(void){
}

bool ModifiesClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_VARIABLE);
	return (firstArg && secondArg);
}

Results ModifiesClause::evaluate(void) {
	return Results();
}