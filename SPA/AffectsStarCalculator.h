#pragma once
#include <exception>
#include "CFG.h"
#include "CallGNode.h"
#include "StmtTable.h"
#include "AssgGNode.h"
#include "WhileGNode.h"
#include "IfGNode.h"
#include "ProcTable.h"
#include "boost/unordered_map.hpp"

using boost::unordered_map;

//thrown to break out of recursion so that we will return to the top level
//used for Affects(_,_), where we want to terminate after finding a single pair
struct AffectsTermination : public std::exception {
  const char * what () const throw () {
    return "We found a pair, we can terminate Affects(_,_) , and this is a bad hack.";
  }
};

//when the start point is not defined
class AffectsStarCalculator {

public:
	AffectsStarCalculator(void);
	~AffectsStarCalculator(void);
	bool computeFixedFixed(string, string);

private:
	typedef unordered_map<string, unordered_set<int>> State;
	typedef unordered_map<int, unordered_set<int>> AffectsStarResult;
	enum AffectsStarResultType {FIXED_FIXED};

	StmtTable* stmtTable;
	ProcTable* procTable;
	CFG* cfg;
	//for s1, _ and _, s1
	unordered_set<string> singleSynResults;
	unordered_set<vector<string>> multiSynResults;
	//state: key is variable, value are stmtNums that modify it
	State globalState;
	//store the statement number and the other statements that affects* it
	AffectsStarResult globalResult;
	AffectsStarResultType type;

	void updateStateForCall(CallGNode*, State&);
	void updateStateForWhile(WhileGNode*, State&);
	void updateStateForIf(IfGNode*, State&); 
	State recurseIf(GNode*, State); 
	void updateStateForAssign(AssgGNode*, State&);
	GNode* evaluateNode(GNode* node, State&);
	State mergeStates(State, State);
	State recurseWhile(WhileGNode*, State);
	bool areResultsEqual(AffectsStarResult, AffectsStarResult);

	//For Affect*(1,2)
	GNode* s1GNode;
	GNode* s2GNode;
	bool result;
};

