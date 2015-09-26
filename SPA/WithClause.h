#pragma once

#include "Clause.h"
#include "Results.h"
#include "boost\unordered_set.hpp"
#include "WithClauseRef.h"

using std::string;
using boost::unordered_set;


class WithClause : public Clause {

public:
	WithClause(void);
	~WithClause(void);

	bool isValid(void);

	//access
	WithClauseRef getLeftRef();
	WithClauseRef getRightRef();

	//set
	void setLeftRef(WithClauseRef);
	void setRightRef(WithClauseRef);

private:
	bool evaluate(Results*);

	WithClauseRef leftRef;
	WithClauseRef rightRef;
};

