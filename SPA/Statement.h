#pragma once

#include <set>
#include "TNode.h"
//#include "GNode.h"
#include <boost\unordered_set.hpp>

using boost::unordered_set;

/*
// --------------------------- Statement --------------------------- 
// Statement object created by PDR to use to add to statement table
// 1. PDR create statement object
// 2. Set all necessary components
//		a. setNodeType
//		b. setStmtNum
//		c. setTNodeRef
//		d. setGNodeRef
//		e. setUses
//		f. setModifies
//		g. setFollowsBefore
//		h. setFollowedAfter
//		i. setFollowsStarBefore
//		j. setFollowsStarAfter
//		k. setChildren
//		l. setChildrenStar
//		m. setParent
//		n. setParentStar
//		o. setNext
//		p. setPrev
// 3. Add to StmtTable:
// -----------------------------------------------------------------
*/

#define DEFAULT -1

class Statement {

public:

	// TYPEDEF
	typedef unordered_set<string> UsesSet;
	typedef unordered_set<string> ModifiesSet;
	typedef unordered_set<int> ChildrenSet;
	typedef unordered_set<int> ParentStarSet;
	typedef unordered_set<int> ChildrenStarSet;
	typedef unordered_set<int> FollowsStarBeforeSet;
	typedef unordered_set<int> FollowsStarAfterSet;

	// CONSTRUCTOR
	Statement();
	//Statement(NodeType type, int stmtNum, TNode *tRef, GNode *gRef);

	// GETTERS
	NodeType getType();													// get stmt type
	int getStmtNum();													// get stmt num
	TNode* getTNodeRef();												// get reference to stmt TNode
	//GNode* getGNodeRef();												// get reference to stmt GNode
	const UsesSet& getUses();											// get set of variables stmt uses
	const ModifiesSet& getModifies();									// get set of variables stmt modifies
	int getFollowsAfter();												// get stmt that follows after this stmt
	int getFollowsBefore();												// get stmt that follows before this stmt
	string getCalls();													// get proc name that stmt calls
	const ChildrenSet& getChildren();									// get set of child nodes of this stmt
	const ChildrenStarSet& getChildrenStar();							// get set of children star stmts of this stmt
	const ParentStarSet& getParentStar();								// get set of parent star stmts of this stmt
	int getParent();													// get parent of this stmt
	int getNext();														// get stmt that is next of this stmt
	int getPrev();														// get stmt whose next is this stmt
	const FollowsStarBeforeSet& Statement::getFollowsStarBefore();		// get set of follows star stmt before this stmt
	const FollowsStarAfterSet& Statement::getFollowsStarAfter();		// get set of follows star stmt after this stmt


	// SETTERS
	void setType(NodeType nodeType);
	void setStmtNum(int num);
	void setTNodeRef(TNode *ref);
	//void setGNodeRef(GNode *ref);
	void setUses(const UsesSet &usesSet);
	void setModifies(const ModifiesSet &modifiesSet);
	void setFollowsAfter(int followsAfter);
	void setFollowsBefore(int followsBefore);
	void setCalls(string calls);
	void setChildren(const ChildrenSet &childrenSet);
	void setParent(int parent);
	void setNext(int next);
	void setPrev(int prev);
	void setParentStar(const ParentStarSet&);
	void setChildrenStar(const ChildrenStarSet&);
	void setFollowsStarBefore(const FollowsStarBeforeSet&);
	void setFollowsStarAfter(const FollowsStarAfterSet&);

private:
	// PRIVATE ATTRIBUTES
	NodeType	type;			// node type
	int			stmtNum;		// stmt number
	TNode		*TNodeRef;		// TNode reference
	//GNode		*GNodeRef;		// GNode reference

	// PRIVATE RELATIONS ATTRIBUTES
	UsesSet			uses;
	ModifiesSet		modifies;
	int				followsAfter;
	int				followsBefore;
	string			calls;
	ChildrenSet		children;
	int				parent;
	int				next;
	int				prev;
	ChildrenStarSet childrenStar;
	ParentStarSet   parentStar;
	FollowsStarBeforeSet followsStarBefore;
	FollowsStarAfterSet followsStarAfter;

};