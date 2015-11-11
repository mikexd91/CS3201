#pragma once
#include <string>

using namespace std;

enum RefType {
	ATTRREF_,
	SYNONYM_,
	IDENT_,
	INTEGER_
};

enum AttrType {
	PROCNAME_,
	VARNAME_,
	CONSTVALUE_,
	STMTNUM_,
	NULLATTR_
};

enum ReturnType {
	RETURNINTEGER_,
	RETURNSTRING_,
	RETURNINVALID_
};

class WithClauseRef {

public:
	WithClauseRef(void);
	WithClauseRef(RefType);
	~WithClauseRef(void);

	void setEntity(string);
	void setEntityType(string);
	void setAttr(string);
	void setAttrType(AttrType);

	RefType getRefType();
	string getEntity();
	string getEntityType();
	AttrType getAttrType();

	ReturnType getReturnType();

private:
	RefType refType;
	string entity;
	string entityType;
	AttrType attrType;
};

