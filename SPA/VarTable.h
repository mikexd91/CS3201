#pragma once

#include "../include/boost/unordered_map.hpp"
#include "TNode.h"
#include <set>
#include "Variable.h"
#include <unordered_map>

using namespace std;

class VarTable {
	/*

	Conceptual VarTable:
	------------------------------------------------------------------------------------------
	|_[key]_________|_____________________|_____________________|____________________________|
	| variable name | used by these stmts | modf by these stmts | contained in these TNodes  |
	|_[key]_________|_____________________|_____________________|____________________________|
	------------------------------------------------------------------------------------------

	Actual Implementation:
	4 separate tables - 

	All-Vars Table - contains all the vars that are added
	-----------------
	|_[key]_________|
	| name (string) |
	-----------------

	Used-By Table - contains all the vars whose used-by has been set 
	----------------------------------------
	|_[key]_________|______________________|
	| name (string) | used by (set<int>)   | 
	----------------------------------------

	Modified-By Table - contains all the vars whose modified-by has been set 
	----------------------------------------
	|_[key]_________|______________________|
	| name (string) | modf by (set<int>)   |
	----------------------------------------

	TNode-Reference Table - contains all the vars whose TNode has been set
	----------------------------------------
	|_[key]_________|______________________|
	| name (string) | TNodes (set<TNode>)  |
	----------------------------------------

	*/

	//typedef set<int>	UsedByTableRow;
	//typedef set<int>	ModifiedByTableRow;
	//typedef set<TNode>	TNodeRefTableRow;

	//typedef set<string>											AllVarsTable;
	//typedef boost::unordered_map<string, UsedByTableRow>		UsedByTable;
	//typedef boost::unordered_map<string, ModifiedByTableRow>	ModifiedByTable;
	//typedef boost::unordered_map<string, TNodeRefTableRow>		TNodeRefTable;

	typedef unordered_map<string, Variable*>::iterator VarIter;

public:
	
	// getters
	static VarTable* getInstance();
	Variable* getVariable(const string& varName);
	VarIter getIterator();

	// general methods
	bool contains(const string& varName);

private:
	// constructor
	VarTable();	

	static VarTable* _instance;
	unordered_map<string, Variable*> _table;
};