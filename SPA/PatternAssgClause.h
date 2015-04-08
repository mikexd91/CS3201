#pragma once
#include <string>
#include "PatternClause.h"

using namespace std;

class PatternAssgClause :
	public PatternClause {

public:
	PatternAssgClause(string syn);
	~PatternAssgClause(void);

	Results evaluate(void);

};

