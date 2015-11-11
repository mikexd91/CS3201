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

struct BasicAffectsStarBipTermination : public std::exception {
	const char * what () const throw () {
    return "we can terminate Affects*";
  }
};

//thrown to break out of recursion so that we will return to the top level
struct AffectsStarBipTermination : public BasicAffectsStarBipTermination {
  const char * what () const throw () {
    return "we can terminate Affects*";
  }
};

//thrown to break out of recursion when state is empty
//only thrown for fixed syn and fixed fixed, since it is not possible for such a case to happen w syn syn
struct EmptyStateBipTermination : public BasicAffectsStarBipTermination {
  const char * what () const throw () {
    return "The state is empty, let's stop rolling";
  }
};

//when the start point is not defined
class AffectsStarBipCalculator {

public:
	AffectsStarBipCalculator(void);
	~AffectsStarBipCalculator(void);
	bool computeFixedFixed(string, string);
	unordered_set<string> computeFixedSyn(string);
	unordered_set<vector<string>> computeSynSyn(bool);

private:
	typedef unordered_map<string, unordered_set<int>> State;
	typedef unordered_map<int, unordered_set<int>> AffectsStarBipResult;
	enum AffectsStarBipResultType {FIXED_FIXED, FIXED_SYN, SYN_SYN};

	StmtTable* stmtTable;
	ProcTable* procTable;
	CFGbip* cfg;
	//state: key is variable, value are stmtNums that modify it
	State globalState;
	//store the statement number and the other statements that affects* it
	AffectsStarBipResult globalResult;
	AffectsStarBipResultType type;

	void updateStateForCall(CallGNode*, State&);
	void updateStateForWhile(WhileGNode*, State&);
	void updateStateForIf(IfGNode*, State&); 
	State recurseIf(GNode*, State); 
	void updateStateForAssign(AssgGNode*, State&);
	GNode* evaluateNode(GNode* node, State&);
	State mergeStates(State, State);
	State recurseWhile(WhileGNode*, State);
	bool areResultsEqual(AffectsStarBipResult, AffectsStarBipResult);
	bool isEmpty(State);
	void updateStateBeyondEnd(GNode*, State);

	//BIP stuff
	stack<int> parentCallStmts;

	//For Affect*(1,2)
	int s1Num;
	int s2Num;
	bool result;
	bool isStart;
	unordered_set<string> singleSynResult;
};

