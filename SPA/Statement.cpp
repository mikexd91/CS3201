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
	followsAfter = DEFAULT;
	followsBefore = DEFAULT;
	parent = DEFAULT;
	next = DEFAULT;
	prev = DEFAULT;
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

// get set of child nodes of this stmt
const Statement::ChildrenSet& Statement::getChildren() {
	return children;
}

// get parent of this stmt
int Statement::getParent() {
	return parent;
}

// get stmt that is next of this stmt
int Statement::getNext() {
	return next;
}

// get stmt whose next is this stmt
int Statement::getPrev() {
	return prev;
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

void Statement::setUses(const set<string> &useSet) {
	uses = useSet;
}

void Statement::setModifies(const set<string> &modifiesSet) {
	modifies = modifiesSet;
}

void Statement::setFollowsAfter(int followsAfter) {
	this->followsAfter = followsAfter;
}

void Statement::setFollowsBefore(int followsBefore) {
	this->followsBefore = followsBefore;
}

void Statement::setChildren(const set<int> &childrenSet) {
	children = childrenSet;
}

void Statement::setParent(int parent) {
	this->parent = parent;
}

void Statement::setNext(int nextSet) {
	this->next = next;
}

void Statement::setPrev(int prevSet) {
	this->prev = prev;
}