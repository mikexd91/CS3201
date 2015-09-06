#pragma once

#include "Constant.h"
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

using namespace std;

/* ----------------------------- */
/* -------- CONSTRUCTORS ------- */
/* ----------------------------- */
// empty contructor (null attributes)
Constant::Constant() {
	appearsIn = unordered_set<int>();
	nodes = unordered_set<TNode*>();
}

Constant::Constant(const string &name) {
	Constant();
	constName = name;
	value = lexical_cast<int>(name);
	appearsIn = unordered_set<int>();
	nodes = unordered_set<TNode*>();
}


/* ----------------------------- */
/* ---------- GETTERS ---------- */
/* ----------------------------- */
// get name of const
string Constant::getConstName() {
	return constName;
}

// get value of const
int Constant::getValue() {
	return value;
}

// get set of references to TNodes
const unordered_set<TNode*>& Constant::getTNodes() {
	return nodes;
}

// get set of stmt numbers const appears in
const unordered_set<int>& Constant::getAppearsIn() {
	return appearsIn;
}


/* ----------------------------- */
/* ---------- SETTERS ---------- */
/* ----------------------------- */
void Constant::setConstName(const string &name) {
	constName = name;
}

void Constant::addTNodeRef(TNode *node) {
	nodes.emplace(node);
}

void Constant::addAppearsIn(int stmtNum) {
	appearsIn.emplace(stmtNum);
}

void Constant::setAppearsIn(const unordered_set<int> &stmts) {
	BOOST_FOREACH(auto s, stmts) {
		appearsIn.emplace(s);
	}
}