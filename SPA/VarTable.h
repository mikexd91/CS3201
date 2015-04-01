#pragma once

#include "../include/boost/unordered_map.hpp"
#include "TNode.h"
#include <set>
#include "Variable.h"
#include <boost\unordered_map.hpp>

using namespace std;

class VarTable {
	/*

	Conceptual VarTable:
	------------------------------------------------------------------------------------------
	|_[key]_________|_____________________|_____________________|____________________________|
	| variable name | used by these stmts | modf by these stmts | contained in these TNodes  |
	|_[key]_________|_____________________|_____________________|____________________________|
	------------------------------------------------------------------------------------------

	*/

	typedef boost::unordered_map<string, Variable*> Table;
	typedef Table::iterator VarIter;

public:
	
	// getters
	static VarTable& getInstance();
	Variable* getVariable(const string& varName);
	VarIter getIterator();

	// setters
	void addVariable(Variable *var);

	// general methods
	bool contains(const string& varName);

private:
	static bool _hasInstance;
	static VarTable _instance;
	static Table _table;
	
	// constructor
	VarTable();	

};