#pragma once
#include "Clause.h"
class FollowsStarClause : public Clause{
public:
	FollowsStarClause(void);
	~FollowsStarClause(void);
	bool isValid(void);
	Results evaluate(void);
};

