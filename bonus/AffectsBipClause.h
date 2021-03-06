#pragma once
#include "SuchThatClause.h"
#include "CFGbip.h"
#include "StmtTable.h"

class AffectsBipClause 
	: public SuchThatClause {

public:
	AffectsBipClause(void);
	~AffectsBipClause(void);
	bool isValid(void);

protected:
	//e.g. Affects(string,string)
	bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Affects(_,_)
	bool evaluateS1GenericS2Generic();
	//e.g. Affects(_,string)
	bool evaluateS1GenericS2Fixed(string);
	//Affects(string,_)
	bool evaluateS1FixedS2Generic(string);
	//Affects(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string);
	//Affects(_,s2)
	unordered_set<string> getAllS2();
	//Affects(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string);
	//Affects(s1,__)
	unordered_set<string> getAllS1();
	//Affects(s1,s2)
	unordered_set<vector<string>> getAllS1AndS2();

private:
	StmtTable* stmtTable;
	ProcTable* procTable;
	bool modcheck(string, GNode*, unordered_set<int>*);
	bool modcheck(string, GNode*, unordered_set<int>*, int);
	void modadd(string, GNode*, unordered_set<int>*, unordered_set<int>*);
	void modadd(string, GNode*, unordered_set<int>*, unordered_set<int>*, int);
	void print(unordered_set<int>);
};