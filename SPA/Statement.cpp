#pragma once

#include "Statement.h"
#include "TNode.h"
//#include "GNode.h"

using namespace std;

/* ----------------------------- */
/* -------- CONSTRUCTORS ------- */
/* ----------------------------- */
// contructor with no preset attributes
// (null attributes or -1 for primitives and 0 for stmtNum)
Statement::Statement() {
	uses = UsesSet();
	modifies = ModifiesSet();
	followsAfter = DEFAULT;
	followsBefore = DEFAULT;
	calls = DEFAULT;
	children = ChildrenSet();
	parent = DEFAULT;
	next = NextSet();
	prev = PreviousSet();
	parentStar = ParentStarSet();
	childrenStar = ChildrenStarSet();
	followsStarBefore = FollowsStarBeforeSet();
	followsStarAfter = FollowsStarAfterSet();
}

/*
// sets basic attributes
Statment::Statement(NodeType type, int stmtNum, TNode *tRef, GNode *gRef) {
	this->type = type;
	this->stmtNum = stmtNum;
	this->tNodeRef = tRef;
	this->gRef = gRef;
}*/


/* ----------------------------- */
/* ---------- GETTERS ---------- */
/* ----------------------------- */
// get stmt type
NodeType Statement::getType() {
	return type;
}

// get stmt num
int Statement::getStmtNum() {
	return stmtNum;
}

// get reference to stmt TNode
TNode* Statement::getTNodeRef() {
	return TNodeRef;
}

/*
// get reference to stmt GNode
GNode* Statement::getGNodeRef() {
	return GNodeRef;
} */

// get set of variables stmt uses
const Statement::UsesSet& Statement::getUses() {
	return uses;
}

// get set of variables stmt modifies
const Statement::ModifiesSet& Statement::getModifies() {
	return modifies;
}

// get stmt that follows after this stmt
int Statement::getFollowsAfter() {
	return followsAfter;
}

// get stmt that follows before this stmt
int Statement::getFollowsBefore() {
	return followsBefore;
}

// get proc name that stmt calls
string Statement::getCalls() {
	return calls;
}

// get set of child nodes of this stmt
const Statement::ChildrenSet& Statement::getChildren() {
	return children;
}

// get parent of this stmt
int Statement::getParent() {
	return parent;
}

// get stmt that is next of this stmt
const Statement::NextSet& Statement::getNext() {
	return next;
}

// get stmt whose next is this stmt
const Statement::PreviousSet& Statement::getPrev() {
	return prev;
}

// get parent star of this stmt
const Statement::ParentStarSet& Statement::getParentStar() {
	return parentStar;
}

// get children star of this stmt
const Statement::ChildrenStarSet& Statement::getChildrenStar() {
	return childrenStar;
}


// get all previous follows of this stmt
const Statement::FollowsStarBeforeSet& Statement::getFollowsStarBefore() {
	return followsStarBefore;
}

// get all next follows of this stmt
const Statement::FollowsStarAfterSet& Statement::getFollowsStarAfter() {
	return followsStarAfter;
}

/* ----------------------------- */
/* ---------- SETTERS ---------- */
/* ----------------------------- */
void Statement::setType(NodeType nodeType) {
	type = nodeType;
}

void Statement::setStmtNum(int num) {
	stmtNum = num;
}

void Statement::setTNodeRef(TNode *ref) {
	TNodeRef = ref;
}

/*
//void Statement::setGNodeRef(GNode *ref) {
	GNodeRef = ref;
} */

void Statement::setUses(const unordered_set<string> &useSet) {
	uses = useSet;
}

void Statement::setModifies(const unordered_set<string> &modifiesSet) {
	modifies = modifiesSet;
}

void Statement::setFollowsAfter(int followsAfter) {
	this->followsAfter = followsAfter;
}

void Statement::setFollowsBefore(int followsBefore) {
	this->followsBefore = followsBefore;
}

void Statement::setCalls(string procName) {
	this->calls = procName;
}

void Statement::setChildren(const unordered_set<int> &childrenSet) {
	children = childrenSet;
}

void Statement::setParent(int parent) {
	this->parent = parent;
}

void Statement::setNext(const unordered_set<int> &nextSet) {
	this->next = nextSet;
}

void Statement::setPrev(const unordered_set<int> &prevSet) {
	this->prev = prevSet;
}

void Statement::setParentStar(const unordered_set<int> &parentStarSet) {
	parentStar = parentStarSet;
}

void Statement::setChildrenStar(const unordered_set<int> &childrenStarSet) {
	childrenStar = childrenStarSet;
}

void Statement::setFollowsStarBefore(const FollowsStarBeforeSet &followsStarBeforeSet) {
	followsStarBefore = followsStarBeforeSet;
}
void Statement::setFollowsStarAfter(const FollowsStarAfterSet &followsStarAfterSet) {
	followsStarAfter = followsStarAfterSet;
}
