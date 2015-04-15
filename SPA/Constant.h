#pragma once

#include <set>
#include "TNode.h"

/*
// -------------------------- Constant ----------------------------- 
// Constant object created by PDR to use to add to statement table
// 1. PDR create Constant object
// 2. Set all necessary components
//		a. setValue
//		b. setTNodeRef
//		c. setAppearsIn
// 3. Add to ConstTable:
// -----------------------------------------------------------------
*/

class Constant {

public:
	// TYPEDEF
	typedef set<int> AppearsInSet;

	// CONSTRUCTOR
	Constant();
	Constant(const string &name);
	Constant(const string &name, TNode *tRef);

	// GETTERS
	string* getConstName();							// get name of proc
	TNode* getTNodeRef();							// get reference to proc TNode
	const AppearsInSet& getAppearsIn();				// get set of variables proc uses

	// SETTERS
	void setConstName(const string &name);
	void setTNodeRef(TNode *ref);
	void setAppearsIn(const AppearsInSet &stmtSet);

private:
	// PRIVATE ATTRIBUTES
	string		constName;		// const name
	TNode		*TNodeRef;		// TNode reference

	// PRIVATE SETS
	AppearsInSet			appearsIn;

}