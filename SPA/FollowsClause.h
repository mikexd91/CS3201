#pragma once
#include "Clause.h"
#include "TNode.h"

class FollowsClause : public Clause
{
public:
	FollowsClause(void);
	~FollowsClause(void);

	bool isValid(void)=0;

	Results evaluate(void);
	//string getFollows(string stmtNum, string unfixedStmtType);			// gets list of statements that follows stmtNum
	//string getFollowedBy(string stmtNum, string unfixedStmtType);		// gets list of statements that stmtNum follows
	//bool checkIsSameType(NodeType type, string stmtType);
	//void followsBothUnfixedArg(string firstArgType, string secondArgType, Results &resObj);
	//void followsWithOneUnderscore(string firstArgType, string secondArgType, Results &resObj);

protected:
	
	bool evaluateS1FixedS2Fixed(string, string)=0;
	//e.g. Parent(_,_)
	bool evaluateS1GenericS2Generic()=0;
	//e.g. Parent(_,string)
	bool evaluateS1GenericS2Fixed(string)=0;
	//Parent(string,_)
	bool evaluateS1FixedS2Generic(string)=0;
	//Parent(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string)=0;
	//Parent(_,s2)
	unordered_set<string> getAllS2()=0;
	//Parent(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string)=0;
	//Parent(s1,__)
	unordered_set<string> getAllS1()=0;
	//Parent(s1,s2)
	Results::ResultsTable* getAllS1AndS2()=0;

private:

	bool isFollows(string stmtNum1, string stmtNum2);
};

