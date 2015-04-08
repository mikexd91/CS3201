#include "UsesClause.h"
#include "Utils.h"
UsesClause::UsesClause(void):Clause(USES_){
}

UsesClause::~UsesClause(void){
}

bool UsesClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_WHILE);
	return (firstArg && secondArg);
}

Results UsesClause::evaluate(void) {
	return Results();
}
