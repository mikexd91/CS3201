#pragma once
#include "Clause.h"
#include "ProcTable.h"
#include "SuchThatClause.h"

class CallsStarClause : public SuchThatClause {

public:
	CallsStarClause(void);
	~CallsStarClause(void);
	bool isValid(void);

private:
	bool isCallsStar(Procedure&, Procedure&);
	bool isProcObjNull(string);
	bool isCalls(Procedure&, Procedure&);
	void insertS1AndS2IntoResTable(string, unordered_set<string>&, unordered_set<vector<string>>&);
	void getAllS1WithS2FixedHelper(Procedure&, unordered_set<string>&);
	void getAllS2WithS1FixedHelper(Procedure&, unordered_set<string>&);

protected:
	ProcTable* procTable;

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
	unordered_set<vector<string>> getAllS1AndS2();
};

