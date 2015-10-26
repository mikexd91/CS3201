#pragma once
#include "SingleSynInsert.h"
#include "MultiSynInsert.h"
#include "Result.h"
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
	NEXT_,
	NEXTSTAR_,
	AFFECTS_,
	AFFECTSSTAR_,
	PATTERNASSG_,
	PATTERNIF_,
	PATTERNWHILE_,
	WITH_
};

class Clause {

public:
	Clause(void);
	~Clause(void);

	ClauseType getClauseType(void);
	
	virtual bool evaluate(Result*)=0;

	//Check the validity of the specific clause
	//Substitute the original clause isValid for this one
	virtual bool isValid(void)=0;

protected:
	ClauseType clauseType;
};
