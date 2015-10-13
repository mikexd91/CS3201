#include "WithClauseRef.h"

WithClauseRef::WithClauseRef(void)
{
}

WithClauseRef::WithClauseRef(RefType refType)
{
	this->refType = refType;
}

WithClauseRef::~WithClauseRef(void)
{
}

void WithClauseRef::setEntity(string entity)
{
	this->entity = entity;
}

void WithClauseRef::setEntityType(string entityType)
{
	this->entityType = entityType;
}

void WithClauseRef::setAttrType(AttrType attrType)
{
	this->attrType = attrType;
}

RefType WithClauseRef::getRefType()
{
	return this->refType;
}

string WithClauseRef::getEntity()
{
	return this->entity;
}

string WithClauseRef::getEntityType()
{
	return this->entityType;
}

AttrType WithClauseRef::getAttrType()
{
	return this->attrType;
}

ReturnType WithClauseRef::getReturnType()
{
	// switch case to say what type it returns based on the ref construction
	switch (getRefType()) {
		case ATTRREF_ :
			switch (getAttrType()) {
				case PROCNAME_ :
				case VARNAME_ :
					return RETURNSTRING_;

				case CONSTVALUE_ :
				case STMTNUM_ :
					return RETURNINTEGER_;

				default :
					return RETURNINVALID_;
			}

		case SYNONYM_ :
			// since its only progline which represents ints, then it's only int
			return RETURNINTEGER_;

		case IDENT_ :
			// idents are always string
			return RETURNSTRING_;

		case INTEGER_ :
			return RETURNINTEGER_;

		default :
			return RETURNINVALID_;
	}
}
