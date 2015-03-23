#include "Query.h"
#include "Clause.h"
#include <string>
#include <vector>

Query::Query(void){

}


Query::~Query(void)
{
}

void Query::setSelectSynonym(string synonym){
	this->selectSynonym = synonym;
}

void Query::setClauseList(vector<Clause> list){
	this->clauseList = list;
	int sizeOf = list.size();
	this->numClauses = sizeOf;
}

void Query::addClause(Clause item){
	this->clauseList.push_back(item);
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

int Query::getNumClauses(){
	return this->numClauses;
}