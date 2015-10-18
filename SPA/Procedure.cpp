#pragma once

#include <set>
#include "Procedure.h"
#include "TNode.h"

using namespace std;

/* ----------------------------- */
/* -------- CONSTRUCTORS ------- */
/* ----------------------------- */
// empty contructor (null attributes)
Procedure::Procedure() {}

Procedure::Procedure(const string &name) {
	procName = name;
}

Procedure::Procedure(const string &name, TNode *tRef) {
	procName = name;
	TNodeRef = tRef;
}


/* ----------------------------- */
/* ---------- GETTERS ---------- */
/* ----------------------------- */
// get name of proc
string Procedure::getProcName() {
	return procName;
}

// get reference to proc TNode
TNode* Procedure::getTNodeRef() {
	return TNodeRef;
}

ProcGNode* Procedure::getGNodeRef() {
	return GNodeRef;
}

const Procedure::ContainsStmtSet& Procedure::getContainStmts() {
	return stmts;
}

// get set of variables proc uses
const Procedure::UsesSet& Procedure::getUses() {
	return uses;
}

// get set of variables proc modifies
const Procedure::ModifiesSet& Procedure::getModifies() {
	return modifies;
}

// get set of procs this proc calls
const Procedure::CallsSet& Procedure::getCalls() {
	return calls;
}

// get set of procs that call this proc
const Procedure::CalledBySet& Procedure::getCalledBy() {
	return calledBy;
}


/* ----------------------------- */
/* ---------- SETTERS ---------- */
/* ----------------------------- */
void Procedure::setProcName(const string &name) {
	procName = name;
}

void Procedure::setTNodeRef(TNode *ref) {
	TNodeRef = ref;
}

void Procedure::setGNodeRef(ProcGNode* ref) {
	GNodeRef = ref;
}

void Procedure::setContainStmts(const unordered_set<int> &containsStmtSet) {
	stmts = containsStmtSet;
}

void Procedure::setUses(const unordered_set<string> &useSet) {
	uses = useSet;
}

void Procedure::setModifies(const unordered_set<string> &modifiesSet) {
	modifies = modifiesSet;
}

void Procedure::setCalls(const unordered_set<Procedure*> &callsSet) {
	calls = callsSet;
}

void Procedure::setCalledBy(const unordered_set<Procedure*> &calledBySet) {
	calledBy = calledBySet;
}