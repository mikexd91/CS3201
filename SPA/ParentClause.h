#pragma once
#include "Clause.h"
class ParentClause : public Clause{
public:
	ParentClause(void);
	~ParentClause(void);
	bool isValid(void);
	Results evaluate(void);
};

