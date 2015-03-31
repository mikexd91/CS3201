#pragma once
#include <string>
#include <vector>
#include <map>
#include "StringPair.h"
#include "Clause.h"

using std::map;
using std::string;
using std::vector;
class Query
{
private:
	int numClauses;
	vector<string> selectList;
	vector<Clause> clauseList;
	map<string, string> declarationList;
public:
	Query(void);
	~Query(void);
	
	void setSelectList(vector<string>);
	void addSelectSynonym(string);
	void setClauseList(vector<Clause>);
	void addClause(Clause);
	void setDeclarationList(map<string, string>);
	void addDeclaration(StringPair);

	vector<string> getSelectList(void);
	string getSelectSynonym(int);
	vector<Clause> getClauseList(void);
	Clause getClause(int);
	int getNumClauses(void);
	map<string, string> getDeclarationList(void);
	bool checkQueryValidity(void);

};