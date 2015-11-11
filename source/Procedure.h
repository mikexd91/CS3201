#pragma once

#include <set>
#include "TNode.h"
#include "ProcGNode.h"
#include <boost\unordered_set.hpp>

using boost::unordered_set;

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
	typedef unordered_set<int> ContainsStmtSet;
	typedef unordered_set<string> UsesSet;
	typedef unordered_set<string> ModifiesSet;
	typedef unordered_set<Procedure*> CallsSet;
	typedef unordered_set<Procedure*> CalledBySet;

	// CONSTRUCTOR
	Procedure();
	Procedure(const string &name);
	Procedure(const string &name, TNode *tRef);

	// GETTERS
	string getProcName();						// get name of proc
	TNode* getTNodeRef();						// get reference to proc TNode
	ProcGNode* getGNodeRef();					// get reference to proc GNode
	ProcGNode* getGBipNodeRef();
	const ContainsStmtSet& getContainStmts();	// get set of statements in this proc
	const UsesSet& getUses();					// get set of variables proc uses
	const ModifiesSet& getModifies();			// get set of variables proc modifies
	const CallsSet& getCalls();					// get set of procs this proc calls
	const CalledBySet& getCalledBy();			// get set of procs that call this proc

	// SETTERS
	void setProcName(const string &name);
	void setTNodeRef(TNode *ref);
	void setGNodeRef(ProcGNode *ref);
	void setGBipNodeRef(ProcGNode *ref);
	void setContainStmts(const unordered_set<int> &stmtsSet);
	void setUses(const unordered_set<string> &useSet);
	void setModifies(const unordered_set<string> &modifiesSet);
	void setCalls(const unordered_set<Procedure*> &callsSet);
	void setCalledBy(const unordered_set<Procedure*> &calledBySet);

private:
	// PRIVATE ATTRIBUTES
	string		procName;		// proc name
	TNode		*TNodeRef;		// TNode reference
	ProcGNode	*GNodeRef;		// GNode reference
	ProcGNode	*GBipNodeRef;

	// PRIVATE SETS
	ContainsStmtSet	stmts;
	UsesSet			uses;
	ModifiesSet		modifies;
	CallsSet		calls;
	CalledBySet		calledBy;

};