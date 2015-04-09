#pragma once
#include "Results.h"

Results::Results(void)
{
	clausePassed = false;
}

Results::~Results(void)
{
}

void Results::setClausePassed(bool passed) {
	this->clausePassed = passed;
}

void Results::setNumOfSyn(int n) {
	this->numOfSyn = n;
}

void Results::addSingleResult(string s) {
	this->singleResults.push_back(s);
}

void Results::addPairResult(string s1, string s2) {
	pair<string, string> pair = make_pair(s1, s2);
	this->pairResults.push_back(pair);
}

void Results::setFirstClauseSyn(string s1) {
	this->firstClauseSyn = s1;
}

void Results::setSecondClauseSyn(string s2) {
	this->secondClauseSyn = s2;
}

bool Results::isClausePassed() {
	return this->clausePassed;
}

string Results::getFirstClauseSyn() {
	return this->firstClauseSyn;
}

string Results::getSecondClauseSyn() {
	return this->secondClauseSyn;
}

vector<string> Results::getSinglesResults() {
	return this->singleResults;
}

vector<pair<string, string>> Results::getPairResults() {
	return this->pairResults;
}

int Results::getNumOfSyn() {
	return this->numOfSyn;
}

void Results::getIntersect(Results res) {
	
}

set<string> Results::getSelectSynResult(string syn) {
	return set<string>();
}

bool Results::usesSelectSyn(string syn) {
	return false;
}