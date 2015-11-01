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

struct BasicAffectsStarTermination : public std::exception {
	const char * what () const throw () {
    return "we can terminate Affects*";
  }
};

//thrown to break out of recursion so that we will return to the top level
struct AffectsStarTermination : public BasicAffectsStarTermination {
  const char * what () const throw () {
    return "we can terminate Affects*";
  }
};

//thrown to break out of recursion when state is empty
//only thrown for fixed syn and fixed fixed, since it is not possible for such a case to happen w syn syn
struct EmptyStateTermination : public BasicAffectsStarTermination {
  const char * what () const throw () {
    return "The state is empty, let's stop rolling";
  }
};

//when the start point is not defined
class AffectsStarCalculator {

public:
	AffectsStarCalculator(void);
	~AffectsStarCalculator(void);
	bool computeFixedFixed(string, string);
	unordered_set<string> computeFixedSyn(string);
	unordered_set<vector<string>> computeSynSyn(bool);

private:
	typedef unordered_map<string, unordered_set<int>> State;
	typedef unordered_map<int, unordered_set<int>> AffectsStarResult;
	enum AffectsStarResultType {FIXED_FIXED, FIXED_SYN, SYN_SYN};

	StmtTable* stmtTable;
	ProcTable* procTable;
	CFG* cfg;
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
	bool isEmpty(State);

	//For Affect*(1,2)
	int s1Num;
	int s2Num;
	bool result;
	bool isStart;
};

