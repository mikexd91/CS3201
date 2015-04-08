#include "FollowsClause.h"
#include "Utils.h"
FollowsClause::FollowsClause(void):Clause(FOLLOWS_){
}

FollowsClause::~FollowsClause(void){
}

bool FollowsClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	return ((firstType == stringconst::ARG_STATEMENT) && (secondType == stringconst::ARG_STATEMENT));
}

Results FollowsClause::evaluate(void) {
	return Results();
}