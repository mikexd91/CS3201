#pragma once
#include <string>
#include <vector>
#include "Clause.h"
using std::string;
using std::vector;
class Query
{
private:
	string selectSynonym;
	int numClauses;
	vector<Clause> clauseList;
public:
	Query(void);
	~Query(void);
	
	void setSelectSynonym(string);
	void setClauseList(vector<Clause>);
	void addClause(Clause);

	string getSelectSynonym();
	vector<Clause> getClauseList();
	Clause getClause(int);
};