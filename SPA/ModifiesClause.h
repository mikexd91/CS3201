#pragma once
#include "Clause.h"
class ModifiesClause : public Clause{
public:
	ModifiesClause(void);
	~ModifiesClause(void);

	bool isValid(void);
	Results evaluate(void);
};

