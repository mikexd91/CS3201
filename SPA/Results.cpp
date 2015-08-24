#pragma once
#include "Results.h"
#include <iostream>

using namespace std;

Results::Results(void)
{
	this->clausePassed = false;
}

Results::~Results(void)
{
}

bool Results::isClausePass() {
	return this->clausePassed;
}

void Results::setClausePass() {

}
	
void Results::resetClausePass() {

}

// checks if syn exist in resultsTable
bool Results::hasResults(string syn) {
	return false;
}

// for clauses with 2 or more synonyms
Results::ResultsTable Results::selectMultiSyn(unordered_set<string> synList) {
	return ResultsTable();
}
	
// for clauses with 1 synonym
unordered_set<string>* Results::selectSyn(string syn) {
	return new unordered_set<string>();
}
	
// for clauses with 2 or more synonyms
bool Results::insertMultiResult(ResultsRow results) {
	return false;
}
// for clauses with 1 synonym
bool Results::insertResult(string syn, string value) {
	return false;
}
// called after all results have been inserted. push tells me what to delete
bool Results::push() {
	return false;
}

// Testing
int Results::getResultsTableSize() {
	return 0;
}
