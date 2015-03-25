#pragma once

#include <set>
#include "Statement.h"
#include "TNode.h"
//#include "GNode.h"

using namespace std;

/* ----------------------------- */
/* -------- CONSTRUCTORS ------- */
/* ----------------------------- */
// empty contructor (null attributes)
Statement::Statement() {}

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
const NodeType Statement::getType() {
	return type;
}

// get stmt num
const int Statement::getStmtNum() {
	return stmtNum;
}

// get reference to stmt TNode
const TNode* Statement::getTNodeRef() {
	return TNodeRef;
}

/*
// get reference to stmt GNode
const GNode* Statement::getGNodeRef() {
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
const Statement::FollowsSet& Statement::getFollows() {
	return follows;
}

// get set of stmts that follows this stmt
const Statement::FollowedBySet& Statement::getFollowedBy() {
	return followedBy;
}

// get set of stmts that are parents of this stmt
const Statement::ParentOfSet& Statement::getParentOf() {
	return parentOf;
}

// get set of stmts that are children of this stmt
const Statement::ChildOfSet& Statement::getChildOf() {
	return childOf;
}

// get set of stmts that are next of this stmt
const Statement::NextSet& Statement::getNext() {
	return next;
}

// get set of stmts whose next is this stmt
const Statement::PrevSet& Statement::getPrev() {
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
//void Statement::setGNodeRef(const GNode *ref) {
	GNodeRef = ref;
} */

void Statement::setUses(const set<string> &useSet) {
	uses = useSet;
}

void Statement::setModifies(const set<string> &modifiesSet) {
	modifies = modifiesSet;
}

void Statement::setFollows(const set<int> &followsSet) {
	follows = followsSet;
}

void Statement::setFollowedBy(const set<int> &followedBySet) {
	followedBy = followedBySet;
}

void Statement::setParentOf(const set<int> &parentOfSet) {
	parentOf = parentOfSet;
}

void Statement::setChildOf(const set<int> &childOfSet) {
	childOf = childOfSet;
}

void Statement::setNext(const set<int> &nextSet) {
	next = nextSet;
}

void Statement::setPrev(const set<int> &prevSet) {
	prev = prevSet;
}