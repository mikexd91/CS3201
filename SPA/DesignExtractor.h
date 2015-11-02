#pragma once

#include "Utils.h"
#include "AST.h"
#include "CFGbip.h"
#include "DummyGNode.h"
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
	bool isCyclicCall(unordered_set<Procedure*>, unordered_set<Procedure*>, Procedure*);

	void populateModUsesCalls();
	void populateCallsParents(Statement*);
	void populateCallsVarTable(Statement*);
	void populateParentCallsVarTable(int, unordered_set<string>, unordered_set<string>);

	void populateEndProcs(ProcGNode*);
	void setEndProcChildrenList(ProcGNode*);
	void setCallsBipList(unordered_set<Statement*>);
	void setProcBipPrevList();
	void breakBonds(Statement*);

	void removeWhilePrevBip(int, int);
	void removeDummyPrevBip(GNode*, int);
	void removeOthersPrevBip(GNode*);

	void addOthersPrevBip(GNode*, unordered_set<int>);
	void addDummyPrevBip(GNode*, unordered_set<int>);
};
