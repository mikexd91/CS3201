#pragma once

#include <set>
#include "TNode.h"
//#include "GNode.h"

/*
// --------------------------- Statement --------------------------- 
// Statement object created by PDR to use to add to statement table
// 1. PDR create statement objecy
// 2. Set all necessary components
//		a. setNodeType
//		b. setStmtNum
//		c. setTNodeRef
//		d. setGNodeRef
//		e. setUses
//		f. setModifies
//		g. setFollows
//		h. setFollowedBy
//		i. setParentOf
//		j. setChildOf
//		k. setNext
//		l. setPrev
// 3. Add to StmtTable:
// -----------------------------------------------------------------
*/

class Statement {

	// DEFINE NODE TYPE
	enum NodeType { ASSIGN, CALL, WHILE, IF };

public:
	// TYPEDEF
	typedef set<string> UsesSet;
	typedef set<string> ModifiesSet;
	typedef set<int> FollowsSet;
	typedef set<int> FollowedBySet;
	typedef set<int> ParentOfSet;
	typedef set<int> ChildOfSet;
	typedef set<int> NextSet;
	typedef set<int> PrevSet;

	// CONSTRUCTOR
	Statement();
	//Statement(NodeType type, int stmtNum, TNode *tRef, GNode *gRef);

	// GETTERS
	const NodeType getType();				// get stmt type
	const int getStmtNum();					// get stmt num
	const TNode* getTNodeRef();				// get reference to stmt TNode
	//const GNode* getGNodeRef();			// get reference to stmt GNode
	const UsesSet& getUses();				// get set of variables stmt uses
	const ModifiesSet& getModifies();		// get set of variables stmt modifies
	const FollowsSet& getFollows();			// get set of stmts that this stmt follows
	const FollowedBySet& getFollowedBy();	// get set of stmts that follows this stmt
	const ParentOfSet& getParentOf();		// get set of stmts that are parents of this stmt
	const ChildOfSet& getChildOf();			// get set of stmts that are children of this stmt
	const NextSet& getNext();				// get set of stmts that are next of this stmt
	const PrevSet& getPrev();				// get set of stmts whose next is this stmt

	// SETTERS
	void setType(NodeType nodeType);
	void setStmtNum(int num);
	void setTNodeRef(const TNode *ref);
	//void setGNodeRef(const GNode *ref);
	void setUses(const set<string> &useSet);
	void setModifies(const set<string> &modifiesSet);
	void setFollows(const set<int> &followsSet);
	void setFollowedBy(const set<int> &followedBySet);
	void setParentOf(const set<int> &parentOfSet);
	void setChildOf(const set<int> &childOfSet);
	void setNext(const set<int> &nextSet);
	void setPrev(const set<int> &prevSet);

private:
	// PRIVATE ATTRIBUTES
	NodeType	type;			// node type
	int			stmtNum;		// stmt number
	TNode		*TNodeRef;		// TNode reference
	//GNode		*GNodeRef;		// GNode reference

	// PRIVATE SETS
	UsesSet			uses;
	ModifiesSet		modifies;
	FollowsSet		follows;
	FollowedBySet	followedBy;
	ParentOfSet		parentOf;
	ChildOfSet		childOf;
	NextSet			next;
	PrevSet			prev;

}