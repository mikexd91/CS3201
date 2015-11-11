#pragma once
#include <exception>
#include "CFGbip.h"
#include "CallGNode.h"
#include "StmtTable.h"
#include "AssgGNode.h"
#include "WhileGNode.h"
#include "IfGNode.h"
#include "ProcTable.h"
#include "boost/unordered_map.hpp"
#include <stack>

using boost::unordered_map;

//directly thrown when there's a success!
struct BasicAffectsBipTermination : public std::exception {
  const char * what () const throw () {
    return "Success!";
  }
};

//thrown to break out of recursion so that we will return to the top level
//used for Affects(_,_), where we want to terminate after finding a single pair
struct AffectsBipTermination : BasicAffectsBipTermination {
  const char * what () const throw () {
    return "We found a pair, we can terminate Affects(_,_) , and this is a bad hack.";
  }
};

//when the start point is not defined
class AffectsBipCalculator {

public:
	AffectsBipCalculator(void);
	~AffectsBipCalculator(void);
	//Affects(s1, s2)
	unordered_set<vector<string>> computeAllS1AndS2(bool);
	//Affects(s1,_)
	unordered_set<string> computeAllS1(void);
	//Affects(_,s2)
	unordered_set<string> computeAllS2(void);
	//AFfects(_,_)
	bool computeS1GenericS2Generic(void);
	//Affects(1,_)
	bool computeS1FixedS2Generic(string);
	bool computeFixedFixed(string, string);
	unordered_set<string> computeFixedSyn(string);

private:
	typedef unordered_map<string, unordered_set<int>> State;
	enum AffectsResultType {S1_ONLY, S2_ONLY, S1_AND_S2, GENERIC_GENERIC, FIXED_GENERIC, FIXED_FIXED, FIXED_SYN};

	StmtTable* stmtTable;
	ProcTable* procTable;
	CFGbip* cfg;
	//for s1, _ and _, s1
	unordered_set<string> singleSynResults;
	unordered_set<vector<string>> multiSynResults;
	//state: key is variable, value are stmtNums that modify it
	State globalState;
	AffectsResultType type;
	bool isStart;

	//for Affects(1,_), we need to store 1
	int s1Num;
	int s2Num;
	bool result; //it can terminate prematurely and fail

	//BIP stuff
	stack<int> parentCallStmts;

	void updateStateForCall(CallGNode*, State&);
	void updateStateForWhile(WhileGNode*, State&);
	void updateStateForIf(IfGNode*, State&); 
	State recurseIf(GNode*, State); 
	void updateStateForAssign(AssgGNode*, State&);
	GNode* evaluateNode(GNode* node, State&);
	State mergeStates(State, State);
	State recurseWhile(WhileGNode*, State);
	bool toProceed(State state);
	void updateStateBeyondEnd(GNode*, State);
};

