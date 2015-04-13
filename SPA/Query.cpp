#include "Query.h"
#include "Clause.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include <string>
#include <vector>
#include "boost/unordered_map.hpp"

using boost::unordered_map;
using namespace stringconst;

Query::Query(void){
	this->clauseList = vector<Clause*>();
	this->declarationList = boost::unordered_map<string, string>();
}

Query::~Query(void){
}

void Query::setSelectList(vector<StringPair> list){
	this->selectList = list;
}

void Query::addSelectSynonym(StringPair synonym){
	this->selectList.push_back(synonym);
}

void Query::setClauseList(vector<Clause*> list){
	this->clauseList = list;
	int sizeOf = list.size();
	this->numClauses = sizeOf;
}

void Query::addClause(Clause* item){
	this->clauseList.push_back(item);
	this->numClauses = this->numClauses + 1;
}

void Query::setDeclarationList(unordered_map<string, string> map){
	this->declarationList = map;
}

void Query::addDeclaration(StringPair item){
	this->declarationList.emplace(item.getFirst(), item.getSecond());
}

vector<StringPair> Query::getSelectList(){
	return this->selectList;
}

vector<Clause*> Query::getClauseList(){
	return this->clauseList;
}

int Query::getNumClauses(){
	return this->numClauses;
}

unordered_map<string, string> Query::getDeclarationList(void){
	return this->declarationList;
}