#pragma once

#include <set>
#include "TNode.h"

/*
// -------------------------- Procedure --------------------------- 
// Procedure object created by PDR to use to add to statement table
// 1. PDR create procedure object
// 2. Set all necessary components
//		a. setTNodeRef
//		b. setUses
//		c. setModifies
//		d. setCalls
//		e. setCalledBy
// 3. Add to ProcTable:
// -----------------------------------------------------------------
*/

class Procedure {

public:

	// TYPEDEF
	typedef set<string> UsesSet;
	typedef set<string> ModifiesSet;
	typedef set<Procedure*> CallsSet;
	typedef set<Procedure*> CalledBySet;

	// CONSTRUCTOR
	Procedure();
	Procedure(TNode *tRef);

	// GETTERS
	const TNode* getTNodeRef();				// get reference to proc TNode
	const UsesSet& getUses();				// get set of variables proc uses
	const ModifiesSet& getModifies();		// get set of variables proc modifies
	const CallsSet& getCalls();				// get set of procs this proc calls
	const CalledBySet& getCalledBy();		// get set of procs that call this proc

	// SETTERS
	void setTNodeRef(TNode *ref);
	void setUses(const set<string> &useSet);
	void setModifies(const set<string> &modifiesSet);
	void setCalls(const set<Procedure*> &callsSet);
	void setCalledBy(const set<Procedure*> &calledBySet);

private:
	// PRIVATE ATTRIBUTES
	TNode		*TNodeRef;		// TNode reference

	// PRIVATE SETS
	UsesSet			uses;
	ModifiesSet		modifies;
	CallsSet		calls;
	CalledBySet	calledBy;

}