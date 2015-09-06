#pragma once
#include "Clause.h"
#include "Results.h"
#include "TNode.h"

class FollowsClause : public Clause
{
private:
	//e.g. Parent(string,string)
	virtual bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Parent(_,_)
	virtual bool evaluateS1GenericS2Generic();
	//e.g. Parent(_,string)
	virtual bool evaluateS1GenericS2Fixed(string);
	//Parent(string,_)
	virtual bool evaluateS1FixedS2Generic(string);
	//Parent(string,s2)
	virtual unordered_set<string> getAllS2WithS1Fixed(string);
	//Parent(_,s2)
	virtual unordered_set<string> getAllS2();
	//Parent(s1,string)
	virtual unordered_set<string> getAllS1WithS2Fixed(string);
	//Parent(s1,__)
	virtual unordered_set<string> getAllS1();
	//Parent(s1,s2)
	virtual unordered_set<unordered_map<string, string>> getAllS1AndS2();

public:
	FollowsClause(void);
	~FollowsClause(void);

	bool isValid(void);
	bool evaluate(Results* results);
};

