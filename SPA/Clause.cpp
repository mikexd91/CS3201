#include "Clause.h"
#include <iostream>

Clause::Clause() {
	weight = 0;
}

Clause::~Clause(void) {
	
}

ClauseType Clause::getClauseType()
{
	return this->clauseType;
}

void Clause::setWeight(int weight) {
	this->weight = weight;
}

int Clause::getWeight() {
	if (weight > 0) {
		return weight;
	} else if (clauseType == WITH_) {
		return 1;
	} else if (clauseType == CALLS_) {
		return 4;
	} else if (clauseType == FOLLOWS_) {
		return 7;
	} else if (clauseType == PARENT_) {
		return 10;
	} else if (clauseType == CALLSSTAR_) {
		return 13;
	} else if (clauseType == FOLLOWSSTAR_) {
		return 16;
	} else if (clauseType == PARENTSTAR_) {
		return 19;
	} else if (clauseType == PATTERNASSG_) {
		return 22;
	} else if (clauseType == PATTERNIF_) {
		return 25;
	} else if (clauseType == PATTERNWHILE_) {
		return 28;
	} else if (clauseType == USES_) {
		return 31;
	} else if (clauseType == MODIFIES_) {
		return 34;
	} else if (clauseType == NEXT_) {
		return 37;
	} else if (clauseType == NEXTSTAR_) {
		return 40;
	} else if (clauseType == AFFECTS_) {
		return 43;
	} else if (clauseType == AFFECTSSTAR_) {
		return 46;
	} else {
		return 999;
	}
}