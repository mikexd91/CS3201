#pragma once

#include <string>
#include "TNode.h"
#include <set>
#include <boost\unordered_set.hpp>

using namespace std;
using boost::unordered_set;

class Variable {
public:

	// constructor
	Variable(const string& varName);

	// getters
	string getName();
	unordered_set<int>& getUsedByStmts();
	unordered_set<int>& getModifiedByStmts();
	unordered_set<string>& getUsedByProc();
	unordered_set<string>& getModifiedByProc();
	unordered_set<TNode*>& getTNodes();

	// setters
	void addUsingStmt(int stmt);
	void addModifyingStmt(int stmt);
	void addUsingProc(string proc);
	void addModifyingProc(string proc);
	void addTNode(TNode *node);

private:
	string _name;
	unordered_set<int> _usedBy;
	unordered_set<int> _modifiedBy;
	unordered_set<string> _usedByProc;
	unordered_set<string> _modifiedByProc;
	unordered_set<TNode*> _nodes;
};

