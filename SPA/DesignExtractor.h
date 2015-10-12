#pragma once
#include "Utils.h"
#include "AST.h"
#include <string>
#include <vector>

using std::string;
using boost::unordered_set;

class DesignExtractor {
public: 
	DesignExtractor();
	void executeSecondPass();

private:
	void checkCyclicCalls();
	void populateParentStar();
	void populateFollowStar();
	void recurseParentStar(StmtNode*, vector<int>&);
	void populateFollowInStmtLst(StmtLstNode*, queue<StmtLstNode*>&);
	void populateModUses();
	unordered_set<string> recurseModifies(Procedure*);
	unordered_set<string> recurseUses(Procedure*);
};
