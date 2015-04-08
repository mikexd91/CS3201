#include "Query.h"
#include "Clause.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include <string>
#include <vector>
using namespace stringconst;
Query::Query(void){
}

Query::~Query(void){
}

void Query::setSelectList(vector<StringPair> list){
	this->selectList = list;
}

void Query::addSelectSynonym(StringPair synonym){
	this->selectList.push_back(synonym);
}

void Query::setClauseList(vector<Clause> list){
	this->clauseList = list;
	int sizeOf = list.size();
	this->numClauses = sizeOf;
}

void Query::addClause(Clause item){
	this->clauseList.push_back(item);
	this->numClauses = this->numClauses + 1;
}

void Query::setDeclarationList(map<string, string> map){
	this->declarationList = map;
}

void Query::addDeclaration(StringPair item){
	this->declarationList.insert(item.getFirst(), item.getSecond());
}

vector<StringPair> Query::getSelectList(){
	return this->selectList;
}

vector<Clause> Query::getClauseList(){
	return this->clauseList;
}

int Query::getNumClauses(){
	return this->numClauses;
}

map<string, string> Query::getDeclarationList(void){
	return this->declarationList;
}