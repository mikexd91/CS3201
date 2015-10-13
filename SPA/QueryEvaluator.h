#pragma once
#include "Query.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "ConstTable.h"
#include "ProcTable.h"
#include "Result.h"
#include "boost/unordered_set.hpp"

//Query object to be passed between parser and evaluator

class QueryEvaluator
{
private:
	StmtTable* stmtTable;
	VarTable* varTable;
	ConstTable* constTable;
	ProcTable* procTable;
	vector<Clause*> clauseList;
	vector<StringPair> selectList;

	void setClauseList(vector<Clause*> clauseList);
	void setSelectList(vector<StringPair> selectList);

	unordered_set<string> getTableValues(string type);
	unordered_set<string> getAllAssign();
	unordered_set<string> getAllStmt();
	unordered_set<string> getAllWhile();
	unordered_set<string> getAllIf();
	unordered_set<string> getAllCall();
	unordered_set<string> getAllVar();
	unordered_set<string> getAllProc();
	unordered_set<string> getAllConst();

	string boolToString(bool);
	void getRemainingSynValuesFromTable(Result &obj);
	Result* evaluateClauses(Result* obj, vector<Clause*> clauseList);
	unordered_set<string> stringVectorToSet(vector<string> &varVector);
	unordered_set<string> intVectorToSet(vector<int> &vec);
	void insertSetValues(string syn, unordered_set<string> values, Result &obj);
	unordered_set<string> printSingleSynValues(Result &obj, string syn);
	unordered_set<string> printTupleSynValues(Result &obj, vector<StringPair> selectList);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	// Evaluate all clauses in a query. Return values of the from table of the right type if there are no clauses.
	Result* evaluateQuery(Query query);
	// Return values to be printed.
	unordered_set<string> getValuesToPrint(Result* obj, vector<StringPair> selectList); 
};