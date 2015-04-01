#pragma once

#include <set>
#include "TNode.h"
//#include "GNode.h"

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
//		g. setFollows
//		h. setFollowedBy
//		i. setParentOf
//		j. setChildOf
//		k. setNext
//		l. setPrev
// 3. Add to StmtTable:
// -----------------------------------------------------------------
*/

#define DEFAULT -1

class Statement {

public:

	// TYPEDEF
	typedef set<string> UsesSet;
	typedef set<string> ModifiesSet;
	typedef set<int> ChildrenSet;

	// CONSTRUCTOR
	Statement();
	//Statement(NodeType type, int stmtNum, TNode *tRef, GNode *gRef);

	// GETTERS
	NodeType getType();					// get stmt type
	int getStmtNum();					// get stmt num
	TNode* getTNodeRef();				// get reference to stmt TNode
	//GNode* getGNodeRef();				// get reference to stmt GNode
	const UsesSet& getUses();			// get set of variables stmt uses
	const ModifiesSet& getModifies();	// get set of variables stmt modifies
	int getFollows();					// gets all those after a in Follows(a, b)
	int getFollowedBy();				// gets all those before b in Follows(a, b)
	const ChildrenSet& getChildren();	// get childrent nodes
	int getParent();					// get immediate parent node
	int getNext();						// get stmt that is next of this stmt
	int getPrev();						// get stmt whose next is this stmt

	// SETTERS
	void setType(NodeType nodeType);
	void setStmtNum(int num);
	void setTNodeRef(TNode *ref);
	//void setGNodeRef(GNode *ref);
	void setUses(const UsesSet &usesSet);
	void setModifies(const ModifiesSet &modifiesSet);
	void setFollows(int follows);
	void setFollowedBy(int followedBy);
	void setParentOf(const ChildrenSet &parentOfSet);
	void setChildOf(int childOf);
	void setNext(int next);
	void setPrev(int prev);

private:
	// PRIVATE ATTRIBUTES
	NodeType	type;			// node type
	int			stmtNum;		// stmt number
	TNode		*TNodeRef;		// TNode reference
	//GNode		*GNodeRef;		// GNode reference

	// PRIVATE RELATIONS ATTRIBUTES
	UsesSet			uses;
	ModifiesSet		modifies;
	int				follows;
	int				followedBy;
	ChildrenSet		parentOf;
	int				childOf;
	int				next;
	int				prev;

};