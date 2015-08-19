#pragma once
#include "Results.h"

using namespace std;

bool clausePassed = false;

Results::Results(void)
{
}

Results::~Results(void)
{
}

bool Results::isClausePass() {
	return this->clausePassed;
}

void Results::setClausePass() {
	this->clausePassed = true;
}

void Results::resetClausePass() {
	this->clausePassed = false;
}

vector<bool> isExist(vector<string> synList) {
	return vector<bool>();
}

vector<unordered_map<string, string>> selectMultiSyn(vector<string> synList) {
	return vector<unordered_map<string, string>>();
}

vector<string> selectSyn(string syn) {
	return vector<string>();
}

vector<unordered_map<string, string>> selectSynWhere(string syn, string val, vector<unordered_map<string, string>>) {
	return vector<unordered_map<string, string>>();
}

bool insertMultiResult(unordered_map<string, string> results) {
	return false;
}

bool insertResult(string syn, string value) {
	return false;
}

bool delMultiResult(unordered_map<string, string> results) {
	return false;
}

bool delResult(string syn, string value) {
	return false;
}

/*
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

void Results::setSingleResult(vector<string>& v) {
	this->singleResults = v;
}

void Results::setPairResult(vector<pair<string, string>>& v) {
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

		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {
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
	string res1Syn1 = res1.getFirstClauseSyn();
	string res2Syn1 = res2.getFirstClauseSyn();
	string res2Syn2 = res2.getSecondClauseSyn();
	vector<string> res1List = res1.getSinglesResults();
	vector<pair<string, string>> res2List = res2.getPairResults();
	set<string> temp1 = *new set<string>();
	set<pair<string, string>> temp2 = *new set<pair<string, string>>();

	bool modifiedFlag = false;

	if (res1Syn1 == res2Syn1) {
		modifiedFlag = true;
		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {

				if (res1List.at(i) == res2List.at(j).first) {
					temp1.emplace(res1List.at(i));
					temp2.emplace(res2List.at(j));
				}
			}
		}

	} else if (res1Syn1 == res2Syn2) {
		modifiedFlag = true;
		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {
				
				if (res1List.at(i) == res2List.at(j).second) {
					temp1.emplace(res1List.at(i));
					temp2.emplace(res2List.at(j));
				}
			}
		}
	} 

	if (modifiedFlag) {
		res1.setSingleResult(vector<string>());
		res2.setPairResult(vector<pair<string, string>>());
		
		for (set<string>::iterator iter = temp1.begin(); iter != temp1.end(); iter++) {
			res1.addSingleResult(*iter);
		}

		for (set<pair<string, string>>::iterator iter = temp2.begin(); iter != temp2.end(); iter++) {
			pair<string, string> p = *iter;
			res2.addPairResult(p.first, p.second);
		}
	}
}

void Results::getPairIntercept(Results &res1, Results &res2) {
	string res1Syn1 = res1.getFirstClauseSyn();
	string res1Syn2 = res1.getSecondClauseSyn();
	string res2Syn1 = res2.getFirstClauseSyn();
	string res2Syn2 = res2.getSecondClauseSyn();

	vector<pair<string, string>> res1List = res1.getPairResults();
	vector<pair<string, string>> res2List = res2.getPairResults();
	set<pair<string, string>> temp1 = *new set<pair<string, string>>();
	set<pair<string, string>> temp2 = *new set<pair<string, string>>();

	bool resultsModifiedFlag = false;

	if (res1Syn1 == res2Syn1 && res1Syn2 == res2Syn2) {
		resultsModifiedFlag = true;
		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {

				if ((res1List.at(i).first == res2List.at(j).first) &&
					(res1List.at(i).second == res2List.at(j).second)){
					temp1.emplace(res1List.at(i));
					temp2.emplace(res2List.at(j));
				}
			}
		}

	} else if (res1Syn1 == res2Syn1) {
		resultsModifiedFlag = true;
		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {

				if (res1List.at(i).first == res2List.at(j).first) {
					temp1.emplace(res1List.at(i));
					temp2.emplace(res2List.at(j));
				}
			}
		}

	} else if (res1Syn2 == res2Syn1) {
		resultsModifiedFlag = true;
		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {

				if (res1List.at(i).second == res2List.at(j).first) {
					temp1.emplace(res1List.at(i));
					temp2.emplace(res2List.at(j));
				}
			}
		}

	} else if (res1Syn2 == res2Syn2) {
		resultsModifiedFlag = true;
		for (size_t  i = 0; i < res1List.size(); i++) {
			for (size_t  j = 0; j < res2List.size(); j++) {

				if (res1List.at(i).second == res2List.at(j).second) {
					temp1.emplace(res1List.at(i));
					temp2.emplace(res2List.at(j));
				}
			}
		}

	} else {
		// error
	}

	if (resultsModifiedFlag) {
		res1.setPairResult(vector<pair<string, string>>());
		res2.setPairResult(vector<pair<string, string>>());

		for (set<pair<string, string>>::iterator iter = temp1.begin(); iter != temp1.end(); iter++) {
			pair<string, string> p = *iter;
			res1.addPairResult(p.first, p.second);
		}

		for (set<pair<string, string>>::iterator iter = temp2.begin(); iter != temp2.end(); iter++) {
			pair<string, string> p = *iter;
			res2.addPairResult(p.first, p.second);
		}
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
	string syn1 = this->getFirstClauseSyn();
	string syn2 = this->getSecondClauseSyn();
	int numSyn = this->getNumOfSyn();
	set<string>* results = new set<string>();
	
	if (numSyn == 1 && syn == syn1) {
		vector<string> singleList = this->getSinglesResults();
		for (vector<string>::iterator iter = singleList.begin(); iter != singleList.end(); iter++) {
			results->insert(*iter);
		}
		return *results;

	} else if (numSyn == 2 && syn == syn1) {
		vector<pair<string, string>> pairList = this->getPairResults();
		for (size_t  i = 0; i < pairList.size(); i++) {
			pair<string, string> p = pairList.at(i);
			results->insert(p.first);
		}
		return *results;

	} else if (numSyn == 2 && syn == syn2) {
		vector<pair<string, string>> pairList = this->getPairResults();
		for (size_t  i = 0; i < pairList.size(); i++) {
			pair<string, string> p = pairList.at(i);
			results->insert(p.second);
		}
		return *results;

	} else {
		return *results;
	}
}

bool Results::usesSyn(string syn) {
	int nSyn = this->getNumOfSyn();
	string syn1 = this->getFirstClauseSyn();
	string syn2 = this->getSecondClauseSyn();

	if (nSyn == 1 && syn1 == syn) {
		return true;

	} else if (nSyn == 2 && syn1 == syn) {
		return true;
	
	} else if (nSyn == 2 && syn2 == syn) {
		return true;
	} else {
		return false;
	}
}
*/