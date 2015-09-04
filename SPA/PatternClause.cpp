#include "PatternClause.h"
#include "Utils.h"

using namespace stringconst;

PatternClause::PatternClause()
	: Clause(PATTERN_) {
}

PatternClause::~PatternClause(void) {
}

string PatternClause::getSynonym() {
	return getFirstArg();
}

string PatternClause::getVar() {
	return getSecondArg();
}

string PatternClause::getVarType() {
	return getSecondArgType();
}

bool PatternClause::getVarFixed() {
	return getSecondArgFixed();
}

void PatternClause::setVar(string var) {
	setSecondArg(var);
}

void PatternClause::setVarType(string varType) {
	setSecondArgType(varType);
}

void PatternClause::setVarFixed(bool fixed) {
	setSecondArgFixed(fixed);
}
