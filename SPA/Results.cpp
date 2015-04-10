#pragma once
#include "Results.h"
#include <algorithm>

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

void Results::setSingleResult(vector<string> v) {
	this->singleResults = v;
}

void Results::setPairResult(vector<pair<string, string>> v) {
	this->pairResults = v;
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

void Results::getSingleIntercept(Results &res1, Results &res2) {
	string syn1 = res1.getFirstClauseSyn();
	string syn2 = res2.getFirstClauseSyn();

	if (syn1 == syn2) {
		vector<string> res1List = res1.getSinglesResults();
		vector<string> res2List = res2.getSinglesResults();
		vector<string> temp = *new vector<string>();

		for (size_t i = 0; i < res1List.size(); i++) {
			for (size_t j = 0; j < res2List.size(); j++) {
				if (res1List.at(i) == res2List.at(j)) {
					temp.push_back(res1List.at(i));
				}
			}
		}
	
		res1.setSingleResult(temp);
		res2.setSingleResult(temp);
	}
}

void Results::getSinglePairIntercept(Results &res1, Results &res2) {

}

void Results::getPairIntercept(Results &res1, Results &res2) {
	string res1Syn1 = res1.getFirstClauseSyn();
	string res1Syn2 = res1.getSecondClauseSyn();

	string res2Syn1 = res2.getFirstClauseSyn();
	string res2Syn2 = res2.getSecondClauseSyn();

	if (res1Syn1 == res2Syn1) {			// assign or statement

	} else if (res1Syn2 == res2Syn1) {	// assign

	} else if (res1Syn2 == res2Syn2) {	// variable

	} else {
		// error
	}
}

void Results::getIntersect(Results &res) {
	int nSynRes1 = this->getNumOfSyn();
	int nSynRes2 = res.getNumOfSyn();

	if (nSynRes1 == 1 && nSynRes2 == 1) {
		getSingleIntercept(*this, res);

	} else if (nSynRes1 == 1 && nSynRes2 == 2) {
		getSinglePairIntercept(*this, res);

	} else if (nSynRes1 == 2 && nSynRes2 == 1) {
		res.getIntersect(*this);

	} else if (nSynRes1 == 2 && nSynRes1 == 2) {
		getPairIntercept(*this, res);

	} else {
		// error
	}
}

set<string> Results::getSelectSynResult(string syn) {
	return set<string>();
}

bool Results::usesSelectSyn(string syn) {
	return false;
}