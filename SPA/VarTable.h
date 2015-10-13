#pragma once

#include "TNode.h"
#include "Variable.h"
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"

using namespace boost;

class VarTable {
	/*

	Conceptual VarTable:
	------------------------------------------------------------------------------------------
	|_[key]_________|_____________________|_____________________|____________________________|
	| variable name | used by these stmts | modf by these stmts | contained in these TNodes  |
	|_[key]_________|_____________________|_____________________|____________________________|
	------------------------------------------------------------------------------------------

	*/

public:
	~VarTable();
	
	// getters
	static VarTable* getInstance();
	Variable* getVariable(const string& varName);
	unordered_map<string, Variable*>::iterator getIterator();
	unordered_map<string, Variable*>::iterator getEnd();
	const unordered_set<string>& getAllVarNames();
	const unordered_set<Variable*>& getAllVariables();

	// setters
	void addVariable(Variable *var);
	static void reset();

	// general methods
	bool contains(const string& varName);

private:
	static bool _hasInstance;
	static VarTable* _instance;
	boost::unordered_map<string, Variable*> _table;
	
	// constructor
	VarTable();	

};