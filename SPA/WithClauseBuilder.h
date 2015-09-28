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
	void setAttrType(int, AttrType);

	WithClause* build();

private:
	ClauseType clauseType;

	RefType firstRefType;
	string firstEntity;
	string firstEntityType;
	AttrType firstAttrType;

	RefType secondRefType;
	string secondEntity;
	string secondEntityType;
	AttrType secondAttrType;

/* how to use
basically a With clause is made up of two references equated to each other:
	Ref = Ref

one Ref is made up of an entity and an optional attribute
	entity.attribute

the entity can be a synonym, a string or an integer type.
this is stated in the RefType
RefType:
	ATTRREF_ - a synonym with an attribute - eg s.stmt# = c.value
	SYNONYM_ - a synonym only - eg n1 = n2
	IDENT_ - a string identifier - eg "proc1" = "v"
	INTEGER_ - an integer - eg 1 = 20


the attribute is only given to synonym entities
attributes have types, as stated below
AttrType:
	PROCNAME_ - procedure name, a string - eg p.procName
	VARNAME_ - variable name, a string - eg v.varName
	VALUE_ - the value of a constant, an integer - eg c.value
	STMTNUM_ - the statement number of a statement, an integer - eg s.stmt#

Example usage of with clause:
// using p.progName = v.varName as the clause

// make the builder with the with clausetype
WithClauseBuilder* w = new WithClauseBuilder(ClauseType::With_);

// set the reftype of the first arg, the left side, to Attribute ref
w->setRefType(1, RefType::ATTRREF_);

// set the left side entity to p
w->setEntity(1, "p");

// set the left side entity type to procedure
w->setEntityType(1, stringconst::ARG_PROCEDURE);

// set the left side attribute type to procName
w->setAttrType(1, AttrType::PROCNAME_);

// set the reftype of the second arg, the right side, to Attribute ref
w->setRefType(2, RefType::ATTRREF_);

// set the right side entity to v
w->setEntity(2, "v");

// set the right side entity type to variable
w->setEntityType(2, stringconst::ARG_VARIABLE);

// set the right side attribute type to varName
w->setAttrType(2, AttrType::VARNAME_);

// finally get the WithClause back
WithClause wc = w->build();


*/
};

