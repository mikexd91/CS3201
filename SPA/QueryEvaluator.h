#pragma once
#include "Query.h"
#include <set>
#include "boost/unordered_set.hpp"

//Query object to be passed between parser and evaluator
using namespace std;
using boost::unordered_set;

namespace SynListConstants {
	const enum Category {
		ALL_IN_CLAUSE = 0,
		HALF_IN_CLAUSE = 1,
		NONE_IN_CLAUSE = 2
	};
}

class QueryEvaluator
{
private:
	StmtTable* stmtTable;
	VarTable* varTable;
	ConstTable* constTable;
	ProcTable* procTable;
	vector<Clause*> clauseList;
	vector<StringPair> selectList;
	SynListConstants::Category category;
	bool allSynInClauseFlag;

	void setClauseList(vector<Clause*> clauseList);
	void setSelectList(vector<StringPair> selectList);
	void setNotAllSynInClauseFlag();
	void resetAllSynInClauseFlag();

	bool isAllSynInClauseFlag();

	SynListConstants::Category getCategory();
	unordered_set<string> getTableValues(string type);
	unordered_set<string> getAllAssign();
	unordered_set<string> getAllStmt();
	unordered_set<string> getAllWhile();
	unordered_set<string> getAllIf();
	unordered_set<string> getAllCall();
	unordered_set<string> getAllVar();
	unordered_set<string> getAllProc();
	unordered_set<string> getAllConst();

	void getAllSynValuesFromTable(Results &obj);
	void getRemainingSynValuesFromTable(Results &obj);
	Results* evaluateClauses(Results* obj, vector<Clause*> clauseList);
	unordered_set<string> stringVectorToSet(vector<string> &varVector);
	unordered_set<string> intVectorToSet(vector<int> &vec);
	void insertSetValues(string syn, unordered_set<string> values, Results &obj);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	// Evaluate all clauses in a query. Return values of the from table of the right type if there are no clauses.
	Results* evaluateQuery(Query query);
	// Used by preprocessor to set category of synonyms in selectList
	void setCategory(SynListConstants::Category category);
	// Return values to be printed.
	unordered_set<string> getValuesToPrint(Results* obj, vector<StringPair> selectList); 
};