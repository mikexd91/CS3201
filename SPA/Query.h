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
	vector<StringPair> selectList;
	vector<Clause> clauseList;
	map<string, string> declarationList;
public:
	Query(void);
	~Query(void);
	
	void setSelectList(vector<StringPair>);
	void addSelectSynonym(StringPair);
	void setClauseList(vector<Clause>);
	void addClause(Clause);
	void setDeclarationList(map<string, string>);
	void addDeclaration(StringPair);

	vector<StringPair> getSelectList(void); // stringPair <synonym, synonym type>
	vector<Clause> getClauseList(void);
	int getNumClauses(void);
	map<string, string> getDeclarationList(void);
};