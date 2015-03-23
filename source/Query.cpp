#include "Query.h"
#include <string>
#include <vector>

Query::Query(void)
{
}

Query::~Query(void)
{
}

void Query::setSelectSynonym(string synonym){
	this->selectSynonym = synonym;
}

void Query::setClauseList(vector<Clause> list){
	this->clauseList = list;
}

void Query::addClause(Clause clause){
	this->clauseList.push_back(clause);
}

string Query::getSelectSynonym(){
	return this->selectSynonym;
}

vector<Clause> Query::getClauseList(){
	return this->clauseList;
}

Clause Query::getClause(int index){
	return this->clauseList.at(index);
}