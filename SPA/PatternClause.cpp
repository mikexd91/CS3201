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

bool PatternClause::getVarFixed() {
	return getSecondArgFixed();
}

string PatternClause::getExpression() {
	return _expr;
}

void PatternClause::setVar(string var) {
	setSecondArg(var);
}

void PatternClause::setVarFixed(bool fixed) {
	setSecondArgFixed(fixed);
}

void PatternClause::setExpression(string expr) {
	_expr = expr;
}
