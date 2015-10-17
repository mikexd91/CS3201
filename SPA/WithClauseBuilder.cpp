#include "WithClauseBuilder.h"


WithClauseBuilder::WithClauseBuilder(ClauseType clauseType)
{
	this->clauseType = clauseType;
}

WithClauseBuilder::~WithClauseBuilder(void)
{
}

void WithClauseBuilder::setRefType(int argNum, RefType type)
{
	switch (argNum) {
		case 1 :
			this->firstRefType = type;
			break;

		case 2 :
			this->secondRefType = type;
			break;

		default :
			return;
	}
}

void WithClauseBuilder::setEntity(int argNum, string entity)
{
	switch (argNum) {
	case 1 :
		this->firstEntity = entity;
		break;

	case 2 :
		this->secondEntity = entity;
		break;

	default :
		return;
	}
}

void WithClauseBuilder::setEntityType(int argNum, string type)
{
	switch (argNum) {
	case 1 :
		this->firstEntityType = type;
		break;

	case 2 :
		this->secondEntityType = type;
		break;

	default :
		return;
	}
}

void WithClauseBuilder::setAttrType(int argNum, AttrType type)
{
	switch (argNum) {
	case 1 :
		this->firstAttrType = type;
		break;

	case 2 :
		this->secondAttrType = type;
		break;

	default :
		return;
	}
}

WithClause* WithClauseBuilder::build()
{
	// make the with clause
	WithClause* w = new WithClause(clauseType);

	// create the left side ref
	// left is first
	WithClauseRef firstRef = WithClauseRef(firstRefType);
	firstRef.setEntity(firstEntity);
	firstRef.setEntityType(firstEntityType);
	firstRef.setAttrType(firstAttrType);
	w->setLeftRef(firstRef);

	// create the right side ref
	// right is second
	WithClauseRef secondRef = WithClauseRef(secondRefType);
	secondRef.setEntity(secondEntity);
	secondRef.setEntityType(secondEntityType);
	secondRef.setAttrType(secondAttrType);
	w->setRightRef(secondRef);

	return w;
}
