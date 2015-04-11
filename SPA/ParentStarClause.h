#pragma once

#include "Clause.h"
#include "ParentClause.h"

class ParentStarClause : public ParentClause{
public:
	ParentStarClause(void);
	~ParentStarClause(void);

	bool isValid(void);
	Results evaluate(void);

private:
	// ONLY EVALUATES PROTOTYPE CASES (only while statements)
	Results evaluateS1WildS2Wild();										// Case: Parent*(s1,s2) - stmt1 wild, stmt2 wild
	Results evaluateS1WildS2Fixed();									// Case: Parent*(s1,2) - stmt1 wild, stmt2 fixed
	Results evaluateS1FixedS2Wild();									// Case: Parent*(1,s2) - stmt1 fixed, stmt2 wild
	Results evaluateS1FixedS2Fixed();									// Case: Parent*(1,2) - stmt1 fixed, stmt2 fixed

	// Other Private Functions
	void recurParentCheckS1WildS2Wild(Results&, string, string);
	void recurParentCheckS1WildS2Fixed(Results&, string);
	void recurParentCheckS1FixedS2Wild(Results&, string, string);
	void recurParentCheckS1FixedS2Fixed(Results&, string, string);
};