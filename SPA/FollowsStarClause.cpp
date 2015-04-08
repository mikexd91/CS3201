#include "FollowsStarClause.h"

FollowsStarClause::FollowsStarClause(void):Clause(FOLLOWSSTAR_){
}

FollowsStarClause::~FollowsStarClause(void){
}

bool FollowsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	return ((firstType == stringconst::ARG_STATEMENT) && (secondType == stringconst::ARG_STATEMENT));
}

Results FollowsStarClause::evaluate(void) {
	return Results();
}