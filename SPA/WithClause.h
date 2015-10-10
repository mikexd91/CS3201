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

	ClauseType getClauseType();

	//access
	WithClauseRef getLeftRef();
	WithClauseRef getRightRef();

	//set
	void setLeftRef(WithClauseRef);
	void setRightRef(WithClauseRef);

	bool evaluate(Result*);

private:
	ClauseType clauseType;
	WithClauseRef leftRef;
	WithClauseRef rightRef;
};

