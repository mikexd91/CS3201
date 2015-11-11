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
	bool validResults;

	void setClauseList(vector<Clause*> clauseList);
	void setSelectList(vector<StringPair> selectList);

	string boolToString(bool);

	unordered_set<string> printSingleSynValues(Result &obj, StringPair syn);
	unordered_set<string> printTupleSynValues(Result &obj, vector<StringPair> selectList);

	bool evalNumClauses(Query*, int, int, Result*);
	bool evalNoClause(Query*, Result*);
	void extractSingleSynonymResults(Result*, Result*, string);
	void extractTupleSynonymResults(Result*, Result*, vector<string>);
	bool isTuplePresent(Result*, vector<string>);

public:
	QueryEvaluator(void);
	~QueryEvaluator(void);

	Result* evalOptimisedQuery(Query*, vector<int>*);
	unordered_set<string> printValues(Result*, vector<StringPair>);
	unordered_set<string> getValuesToPrint(Result* obj, vector<StringPair> selectList);
	void copyResult(Result*, Result*, string);
	void mergeMultiResult(Result*, Result*, vector<string>);
	
	// print invalid bool queries
	unordered_set<string> printInvalid(Query*);
};