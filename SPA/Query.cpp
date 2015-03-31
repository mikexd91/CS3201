#include "Query.h"
#include "Clause.h"
#include "PQLExceptions.h"
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
	this->numClauses++;
}

void Query::setDeclarationList(map<string, string> map){
	this->declarationList = map;
}

void Query::addDeclaration(StringPair item){
	this->declarationList.insert(item.getFirst(), item.getSecond());
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

map<string, string> Query::getDeclarationList(void){
	return this->declarationList;
}

bool Query::checkQueryValidity(void){
	vector<Clause> clauses = this->getClauseList();
	map<string, string> declarations = this->getDeclarationList();
	for (int i=0; i<clauses.size(); i++){
		Clause current = clauses.at(i);
		bool validClause = current.checkClauseValidity(declarations);
		if (!validClause){
			//break loop cause dont need to check the rest
			throw InvalidDeclarationException();
			return false;
		}
		//else check all other clauses in the list
	}
}