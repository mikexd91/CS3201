#include "FollowsClause.h"

FollowsClause::FollowsClause(void):Clause(FOLLOWS_){
}

FollowsClause::~FollowsClause(void){
}

bool FollowsClause::checkValidFollows(void){
	return false;
}

Results FollowsClause::evaluate(void) {
	return Results();
}