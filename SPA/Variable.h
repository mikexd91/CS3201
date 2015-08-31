#pragma once

#include <string>
#include "TNode.h"
#include <set>

using namespace std;

class Variable {
public:

	// constructor
	Variable(const string& varName);

	// getters
	string getName();
	set<int>& getUsedByStmts();
	set<int>& getModifiedByStmts();
	set<string>& getUsedByProc();
	set<string>& getModifiedByProc();
	set<TNode*>& getTNodes();

	// setters
	void addUsingStmt(int stmt);
	void addModifyingStmt(int stmt);
	void addUsingProc(string proc);
	void addModifyingProc(string proc);
	void addTNode(TNode *node);

private:
	string _name;
	set<int> _usedBy;
	set<int> _modifiedBy;
	set<string> _usedByProc;
	set<string> _modifiedByProc;
	set<TNode*> _nodes;
};

