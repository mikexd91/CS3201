#pragma once
#include <string>
#include <vector>
#include "Clause.h"
using std::string;
using std::vector;
class Query
{
private:
	int numClauses;
	vector<string> selectList;
	vector<Clause> clauseList;

public:
	Query(void);
	~Query(void);
	
	void setSelectList(vector<string>);
	void addSelectSynonym(string);
	void setClauseList(vector<Clause>);
	void addClause(Clause);
	
	vector<string> getSelectList();
	string getSelectSynonym(int);
	vector<Clause> getClauseList();
	Clause getClause(int);
	int getNumClauses();
	bool checkQueryValidity();
};