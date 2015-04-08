#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "StringPair.h"
#include "Clause.h"

using std::unordered_map;
using std::string;
using std::vector;
class Query
{
private:
	int numClauses;
	vector<StringPair> selectList;
	vector<Clause> clauseList;
	unordered_map<string, string> declarationList;
public:
	Query(void);
	~Query(void);
	
	void setSelectList(vector<StringPair>);
	void addSelectSynonym(StringPair);
	void setClauseList(vector<Clause>);
	void addClause(Clause);
	void setDeclarationList(unordered_map<string, string>);
	void addDeclaration(StringPair);

	vector<StringPair> getSelectList(void); // stringPair <synonym, synonym type>
	vector<Clause> getClauseList(void);
	int getNumClauses(void);
	unordered_map<string, string> getDeclarationList(void);
};