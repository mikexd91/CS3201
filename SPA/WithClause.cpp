#include "WithClause.h"
#include "Utils.h"

WithClause::WithClause(ClauseType clauseType) {
	this->clauseType = clauseType;
}

WithClause::~WithClause(void)
{
}

bool WithClause::isValid(void)
{
	// TODO
	// must be same return type on both side
	bool returnTypesMatch = getLeftRef().getReturnType() == getRightRef().getReturnType();

	return returnTypesMatch;
}

WithClauseRef WithClause::getLeftRef()
{
	return this->leftRef;
}

WithClauseRef WithClause::getRightRef()
{
	return this->rightRef;
}

void WithClause::setLeftRef(WithClauseRef leftRef)
{
	this->leftRef = leftRef;
}

void WithClause::setRightRef(WithClauseRef rightRef)
{
	this->rightRef = rightRef;
}

bool WithClause::evaluate(Result*)
{
	// TODO
	// 
	return false;
}
