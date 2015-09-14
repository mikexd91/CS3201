#include "PatternClauseBuilder.h"


PatternClauseBuilder::PatternClauseBuilder(ClauseType clauseType) {
	this->clauseType = clauseType;
}


PatternClauseBuilder::~PatternClauseBuilder(void) {
	// empty
}

void PatternClauseBuilder::setSynonym(string syn) {
	this->syn = syn;
}

void PatternClauseBuilder::setVar(string var) {
	this->var = var;
}

void PatternClauseBuilder::setVarType(string varType) {
	this->varType = varType;
}

void PatternClauseBuilder::setVarFixed(bool varFixed) {
	this->varFixed = varFixed;
}

void PatternClauseBuilder::setExpr(int exprNum, string expr) {
	switch (exprNum) {
	case 1 :
		this->firstExpr = expr;
		break;
	case 2 :
		this->secondExpr = expr;
		break;
	default :
		// fail
		return;
	}
}

PatternClause* PatternClauseBuilder::build() {
	PatternClause* clause;
	if (this->clauseType == PATTERNASSG_){
		clause = new PatternAssgClause(this->syn, this->var, this->firstExpr);
		
	} else if (this->clauseType == PATTERNWHILE_){
		clause = new PatternWhileClause(this->syn, this->var, this->firstExpr);
		
	} else if (this->clauseType == PATTERNIF_){
		clause = new PatternIfClause(this->syn, this->var, this->firstExpr, this->secondExpr);

	} else {
		// fail
		return nullptr;
	}

	clause->setVarFixed(this->varFixed);
	clause->setVarType(this->varType);

	return clause;

}

