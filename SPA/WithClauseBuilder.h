#pragma once

#include "WithClause.h"

class WithClauseBuilder
{
public:
	WithClauseBuilder(ClauseType clauseType);
	~WithClauseBuilder(void);

	// setting the ref type
	void setRefType(int, RefType);

	// setting the entity 
	void setEntity(int, string);
	void setEntityType(int, string);

	// setting the attribute
	void setAttr(int, string);
	void setAttrType(int, AttrType);

	WithClause* build();

private:
	ClauseType clauseType;

	RefType firstRefType;
	string firstEntity;
	string firstEntityType;
	string firstAttr;
	AttrType firstAttrType;

	RefType secondRefType;
	string secondEntity;
	string secondEntityType;
	string secondAttr;
	AttrType secondAttrType;
};

