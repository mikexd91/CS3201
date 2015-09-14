#pragma once
#include "Results.h"
#include "Utils.h"

enum ClauseType{
	FOLLOWS_,
	FOLLOWSSTAR_,
	PARENT_,
	PARENTSTAR_,
	USES_,
	MODIFIES_,
	CALLS_,
	CALLSSTAR_,
	PATTERNASSG_,
	PATTERNIF_,
	PATTERNWHILE_
};

class Clause {

public:
	Clause(void);
	~Clause(void);

	ClauseType getClauseType();
	
	virtual bool evaluate(Results*)=0;

	//Check the validity of the specific clause
	//Substitute the original clause isValid for this one
	virtual bool isValid(void)=0;
};
