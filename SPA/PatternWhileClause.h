#pragma once
#include <string>
#include "PatternClause.h"
#include "WhileNode.h"

using namespace std;

class PatternWhileClause :
	public PatternClause {

public:
	PatternWhileClause(const string&, const string&, const string&);
	~PatternWhileClause(void);

	string getStmtList();
	bool isValid();

protected:
	// ---- new stuff ----
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
	// ---- end new stuff -

private:
	// ---- old stuff
	unordered_set<string> evaluateVarWild();
	unordered_set<string> evaluateVarFixed();
	Results::ResultsTable* evaluateVarSyn();
	// ---- end old stuff

	bool matchStmtList(WhileNode*, string);
	bool matchVar(WhileNode*, string);

	bool isStmtListWild();
	bool isVarWild();

	string _stmtList;
};

