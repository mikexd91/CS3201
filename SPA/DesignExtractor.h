#pragma once
#include "Utils.h"
#include "AST.h"
#include "CFGbip.h"
#include "EndGNode.h"
#include "ProcGNode.h"
#include <string>
#include <vector>

using std::string;
using boost::unordered_set;

class DesignExtractor {
public: 
	DesignExtractor();
	void executeSecondPass();
	void constructBip();

private:
	void checkCyclicCalls();
	void populateParentStar();
	void populateFollowStar();
	void recurseParentStar(StmtNode*, vector<int>&);
	void populateFollowInStmtLst(StmtLstNode*, queue<StmtLstNode*>&);
	void populateModUsesProc();
	void populateModUsesCalls();
	bool isCyclicCall(unordered_set<Procedure*>, unordered_set<Procedure*>, Procedure*);

	void breakBonds(Statement*);
	unordered_set<string> recurseModifies(Procedure*);
	unordered_set<string> recurseUses(Procedure*);
};
