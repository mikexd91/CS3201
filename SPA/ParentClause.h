#pragma once
#include "Clause.h"
class ParentClause : public Clause{
public:
	ParentClause(void);
	~ParentClause(void);

	Results evaluate(void);
};

