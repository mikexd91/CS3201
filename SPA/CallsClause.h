#pragma once
#include "Clause.h"
#include "ProcTable.h"
#include "Results.h"

class CallsClause : public Clause {

public:
	CallsClause(void);
	~CallsClause(void);
	bool isValid(void);
	
private:
	bool isCalls(string proc1, string proc2);
	bool isProcObjNull(string proc);

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
	Results::ResultsTable* getAllS1AndS2();
};

