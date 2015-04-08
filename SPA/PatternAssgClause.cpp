#include "PatternAssgClause.h"
#include "Utils.h"

using namespace stringconst;

PatternAssgClause::PatternAssgClause(string syn) 
	: PatternClause() {
	firstArgType = ARG_ASSIGN;
	firstArg = syn;
	firstArgFixed = false;
}

PatternAssgClause::~PatternAssgClause(void) {
}

Results PatternAssgClause::evaluate(void) {


	return Results();
}
