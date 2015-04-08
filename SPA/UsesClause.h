#pragma once
#include "Clause.h"
class UsesClause : public Clause{
public:
	UsesClause(void);
	~UsesClause(void);

	Results evaluate(void);
};

