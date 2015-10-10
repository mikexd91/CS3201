#pragma once
#include "SuchThatClause.h"
#include "CFG.h"
#include "StmtTable.h"

struct GNodeContainer {
	GNode* node;
	int count;
	GNodeContainer(GNode* n= NULL, int c=0):
		node(n),
        count(c){}
}

class AffectsClause 
	: public SuchThatClause {

public:
	AffectsClause(void);
	~AffectsClause(void);
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
	CFG* cfg;
	StmtTable* stmtTable;
	stack<GNodeContainer> nodeStack;

};