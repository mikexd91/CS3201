#include "FollowsStarClause.h"
#include "Utils.h"
FollowsStarClause::FollowsStarClause(void):Clause(FOLLOWSSTAR_){
}

FollowsStarClause::~FollowsStarClause(void){
}

bool FollowsStarClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_STATEMENT) || (secondType == stringconst::ARG_ASSIGN) || (secondType == stringconst::ARG_WHILE);
	return (firstArg && secondArg);
}

Results FollowsStarClause::evaluate(void) {
	return Results();
}