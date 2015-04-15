#pragma once

#include <set>
#include "TNode.h"

/*
// -------------------------- Constant ----------------------------- 
// Constant object created by PDR to use to add to statement table
// 1. PDR create Constant object
// 2. Set all necessary components
//		a. setValue
//		b. addTNodeRef
//		c. setAppearsIn
// 3. Add to ConstTable:
// -----------------------------------------------------------------
*/

class Constant {

public:

	// CONSTRUCTOR
	Constant();
	Constant(const string &name);

	// GETTERS
	string getConstName();						// get name of const
	int getValue();								// get value of const
	const set<TNode*>& getTNodes();				// get set of references to TNodes
	const set<int>& getAppearsIn();				// get set of stmt numbers const appears in

	// SETTERS
	void setConstName(const string &name);
	void addTNodeRef(TNode *node);
	void addAppearsIn(int stmtNum);

private:
	// PRIVATE ATTRIBUTES
	string		constName;		// const name
	int			value;			// const value
	TNode		*TNodeRef;		// TNode reference

	// PRIVATE SETS
	set<TNode*>			nodes;
	set<int>			appearsIn;

};