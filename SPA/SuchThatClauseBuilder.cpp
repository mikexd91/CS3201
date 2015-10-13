#pragma once

#include "SuchThatClauseBuilder.h"
#include "SuchThatClause.h"
#include "FollowsClause.h"
#include "ParentClause.h"
#include "ModifiesClause.h"
#include "UsesClause.h"
#include "FollowsStarClause.h"
#include "ParentStarClause.h"
#include "CallsClause.h"
#include "CallsStarClause.h"
#include "NextClause.h"
#include "AffectsClause.h"

SuchThatClauseBuilder::SuchThatClauseBuilder(ClauseType clauseType) {
	this->clauseType = clauseType;
}


SuchThatClauseBuilder::~SuchThatClauseBuilder(void)
{
}

void SuchThatClauseBuilder::setArg(int argNum, string arg) {
	switch (argNum) {
	case 1 :
		this->firstArg = arg;
		break;
	case 2 :
		this->secondArg = arg;
		break;
	default :
		// fail
		return;
	}
}

void SuchThatClauseBuilder::setArgType(int argNum, string argType) {
	switch (argNum) {
	case 1 :
		this->firstArgType = argType;
		break;
	case 2 :
		this->secondArgType = argType;
		break;
	default :
		// fail
		return;
	}
}

void SuchThatClauseBuilder::setArgFixed(int argNum, bool argFixed) {
	switch (argNum) {
	case 1 :
		this->firstArgFixed = argFixed;
		break;
	case 2 :
		this->secondArgFixed = argFixed;
		break;
	default :
		// fail
		return;
	}
}

SuchThatClause* SuchThatClauseBuilder::build() {
	SuchThatClause* clause;
	if (this->clauseType == FOLLOWSSTAR_){
		clause = new FollowsStarClause();
	
	} else if (this->clauseType == PARENTSTAR_){
		clause = new ParentStarClause();
	
	} else if (this->clauseType == FOLLOWS_){
		clause = new FollowsClause();
		
	} else if (this->clauseType == PARENT_){
		clause = new ParentClause();
		
	} else if (this->clauseType == MODIFIES_){
		clause = new ModifiesClause();
		
	} else if (this->clauseType == USES_){
		clause = new UsesClause();
		
	//INSERT WHEN CLAUSES ARE DONE
	} else if (this->clauseType == CALLS_){
		clause = new CallsClause();

	} else if (this->clauseType == CALLSSTAR_){
		clause = new CallsStarClause();
		
	} else if (this->clauseType == NEXT_){
		clause = new NextClause();
			
	} else if (this->clauseType == AFFECTS_){
		clause = new AffectsClause();
	
	/*	
	} else if (this->clauseType == NEXTSTAR_){
		clause = new NextStarClause();
		
	} else if (this->clauseType == AFFECTSSTAR_){
		clause = new AffectsStarClause();
		
	*/
	} else {
		// fail
		return nullptr;
	}

	clause->setFirstArg(this->firstArg);
	clause->setFirstArgFixed(this->firstArgFixed);
	clause->setFirstArgType(this->firstArgType);
	clause->setSecondArg(this->secondArg);
	clause->setSecondArgFixed(this->secondArgFixed);
	clause->setSecondArgType(this->secondArgType);

	return clause;
}
