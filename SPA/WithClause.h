#pragma once

#include "Clause.h"
#include "Result.h"
#include "ProcTable.h"
#include "VarTable.h"
#include "ConstTable.h"
#include "StmtTable.h"
#include "Statement.h"
#include "boost\unordered_set.hpp"
#include "WithClauseRef.h"

using std::string;
using boost::unordered_set;


class WithClause : public Clause {

public:
	WithClause(ClauseType);
	~WithClause(void);

	bool isValid(void);

	//access
	WithClauseRef getLeftRef();
	WithClauseRef getRightRef();

	//set
	void setLeftRef(WithClauseRef);
	void setRightRef(WithClauseRef);

	bool evaluate(Result*);

private:
	WithClauseRef leftRef;
	WithClauseRef rightRef;
	
	bool evalSynSyn(WithClauseRef, WithClauseRef, Result*);
	
	bool evalValueInt(WithClauseRef, WithClauseRef, Result*);
	bool evalSynInt(WithClauseRef, WithClauseRef, Result*);
	bool evalStmtInt(WithClauseRef, WithClauseRef, Result*);
	
	bool evalVNameString(WithClauseRef, WithClauseRef, Result*);
	bool evalPNameString(WithClauseRef, WithClauseRef, Result*);
	
	bool evalPNamePName(WithClauseRef, WithClauseRef, Result*);
	bool evalVNameVName(WithClauseRef, WithClauseRef, Result*);
	bool evalValueValue(WithClauseRef, WithClauseRef, Result*);
	bool evalStmtStmt(WithClauseRef, WithClauseRef, Result*);
};

