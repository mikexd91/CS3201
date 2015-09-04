#pragma once

#include "Clause.h"
#include "ParentClause.h"

class ParentStarClause : public ParentClause{
public:
	ParentStarClause(void);
	~ParentStarClause(void);

	bool isValid(void);

protected:
	//e.g. Parent(string,string)
	bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Parent(_,_)
	bool evaluateS1GenericS2Generic();
	//e.g. Parent(_,string)
	bool evaluateS1GenericS2Fixed(string);
	//Parent(string,_)
	bool evaluateS1FixedS2Generic(string);
	//Parent(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string);
	//Parent(_,s2)
	unordered_set<string> getAllS2();
	//Parent(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string);
	//Parent(s1,__)
	unordered_set<string> getAllS1();
	//Parent(s1,s2)
	Results::ResultsTable* getAllS1AndS2();


private:
	/**
	// ONLY EVALUATES PROTOTYPE CASES (only while statements)
	Results evaluateS1WildS2Wild();										// Case: Parent*(s1,s2) - stmt1 wild, stmt2 wild
	Results evaluateS1WildS2Fixed();									// Case: Parent*(s1,2) - stmt1 wild, stmt2 fixed
	Results evaluateS1FixedS2Wild();									// Case: Parent*(1,s2) - stmt1 fixed, stmt2 wild
	Results evaluateS1FixedS2Fixed();									// Case: Parent*(1,2) - stmt1 fixed, stmt2 fixed

	// Other Private Functions
	void recurParentCheckS1WildS2Wild(Results&, string, string, string, string);
	void recurParentCheckS1WildS2Fixed(Results&, string, string);
	void recurParentCheckS1FixedS2Wild(Results&, string, string, string);
	void recurParentCheckS1FixedS2Fixed(Results&, string, string);
	**/
};