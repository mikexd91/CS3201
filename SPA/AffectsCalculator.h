#pragma once
#include "CFG.h"
#include "CallGNode.h"
#include "StmtTable.h"
#include "AssgGNode.h"
#include "WhileGNode.h"
#include "IfGNode.h"
#include "ProcTable.h"
#include "boost/unordered_map.hpp"

using boost::unordered_map;
//when the start point is not defined
class AffectsCalculator {

public:
	AffectsCalculator(void);
	~AffectsCalculator(void);
	unordered_set<vector<string>> computeAllS1AndS2(void);

private:
	typedef unordered_map<string, unordered_set<int>> State;
	StmtTable* stmtTable;
	ProcTable* procTable;
	CFG* cfg;
	unordered_set<vector<string>> results;
	//key is variable, value are stmtNums that modify it
	State globalState;

	void updateStateForCall(CallGNode*, State&);
	void updateStateForWhile(WhileGNode*, State&);
	void updateStateForIf(IfGNode*, State&); 
	State recurseIf(GNode*, State); 
	void updateStateForAssign(AssgGNode*, State&);
	GNode* evaluateNode(GNode* node, State&);
	State mergeStates(State, State);
	State recurseWhile(GNode*, State);
};