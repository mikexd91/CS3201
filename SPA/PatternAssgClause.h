#pragma once
#include <string>
#include "PatternClause.h"
#include "AssgNode.h"

using namespace std;

class PatternAssgClause :
	public PatternClause {

public:
	PatternAssgClause(const string& syn);
	PatternAssgClause(const string& syn, const string& var, const string& expr);
	~PatternAssgClause(void);
	
	string getExpression();
	bool isValid();

	void setExpression(string expr);

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
	unordered_set<string> evaluateVarWildExprWild();
	unordered_set<string> evaulateVarWildExprFixed();
	unordered_set<string> evaluateVarFixedExprWild();
	unordered_set<string> evaluateVarFixedExprFixed();
	Results::ResultsTable* evaluateVarSynExprWild();
	Results::ResultsTable* evaluateVarSynExprFixed();
	// ---- end old stuff

	vector<int> getAssgNums(Results, string);
	bool matchExpr(AssgNode* assg, string expr);
	bool matchVar(AssgNode* assg, string var);

	bool isExprWild();
	bool isVarWild();
	
	string _expr;
};

