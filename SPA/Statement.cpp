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
	follows = DEFAULT;
	followedBy = DEFAULT;
	childOf = DEFAULT;
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

// get stmt that this stmt follows
int Statement::getFollows() {
	return follows;
}

// get stmt that follows after this stmt
int Statement::getFollowedBy() {
	return followedBy;
}

// get childrent nodes
const Statement::ChildrenSet& Statement::getChildren() {
	return parentOf;
}

// get immediate parent node
int Statement::getParent() {
	return childOf;
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

void Statement::setFollows(int follows) {
	this->follows = follows;
}

void Statement::setFollowedBy(int followedBy) {
	this->followedBy = followedBy;
}

void Statement::setParentOf(const set<int> &parentOfSet) {
	parentOf = parentOfSet;
}

void Statement::setChildOf(int childOf) {
	this->childOf = childOf;
}

void Statement::setNext(int nextSet) {
	this->next = next;
}

void Statement::setPrev(int prevSet) {
	this->prev = prev;
}