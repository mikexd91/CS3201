#pragma once
#include "Clause.h"

class FollowsClause : public Clause
{
public:
	FollowsClause(void);
	~FollowsClause(void);
	bool isValid(void);
	Results evaluate(void);

};

