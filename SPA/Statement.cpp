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
	follows = -1;
	followedBy = -1;
	parentOf = -1;
	childOf = -1;
	next = -1;
	prev = -1;
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

// get set of stmts that this stmt follows
int Statement::getFollows() {
	return follows;
}

// get set of stmts that follows this stmt
int Statement::getFollowedBy() {
	return followedBy;
}

// get set of stmts that are parents of this stmt
int Statement::getParentOf() {
	return parentOf;
}

// get set of stmts that are children of this stmt
int Statement::getChildOf() {
	return childOf;
}

// get set of stmts that are next of this stmt
int Statement::getNext() {
	return next;
}

// get set of stmts whose next is this stmt
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

void Statement::setParentOf(int parentOf) {
	this->parentOf = parentOf;
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