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
	void evaluateVarWildExprWild(vector<int>&, unordered_set<string>);
	Results evaulateVarWildExpr(vector<int>& assgNums, string expr, Results);
	Results evaluateVarFixedExprWild(vector<int>& assgNums, Results);
	Results evaluateVarFixedExpr(vector<int>& assgNums, string expr, Results);
	Results evaluateVarExprWild(vector<int>& assgNums, vector<string>& varNames, Results);
	Results evaluateVarExpr(vector<int>& assgNums, vector<string>& varNames, string expr, Results);
	// ---- end old stuff

	vector<int> getAssgNums(Results, string);
	bool matchExpr(AssgNode* assg, string expr);
	bool matchVar(AssgNode* assg, string var);

	bool isExprWild();
	bool isVarWild();
	
	string _expr;
};

