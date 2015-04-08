#include "ModifiesClause.h"


ModifiesClause::ModifiesClause(void):Clause(MODIFIES_){
}

ModifiesClause::~ModifiesClause(void){
}

Results ModifiesClause::evaluate(void) {
	return Results();
}