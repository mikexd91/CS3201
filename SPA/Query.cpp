#include "Query.h"
#include "Clause.h"
#include <string>
#include <vector>
using namespace stringconst;
Query::Query(void){
}

Query::~Query(void){
}

void Query::setSelectList(vector<string> list){
	this->selectList = list;
}

void Query::addSelectSynonym(string synonym){
	this->selectList.push_back(synonym);
}

void Query::setClauseList(vector<Clause> list){
	this->clauseList = list;
	int sizeOf = list.size();
	this->numClauses = sizeOf;
}

void Query::addClause(Clause item){
	this->clauseList.push_back(item);
}

vector<string> Query::getSelectList(){
	return this->selectList;
}

string Query::getSelectSynonym(int num){
	return this->selectList.at(num);
}

vector<Clause> Query::getClauseList(){
	return this->clauseList;
}

Clause Query::getClause(int index){
	return this->clauseList.at(index);
}

int Query::getNumClauses(){
	return this->numClauses;
}