#pragma once
#include "Clause.h"
class FollowsStarClause : public Clause{
public:
	FollowsStarClause(void);
	~FollowsStarClause(void);

	Results evaluate(void);
};

