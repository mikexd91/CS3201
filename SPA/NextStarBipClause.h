/*
 * NextStarBipClause.h
 *
 *  Created on: 2 Nov, 2015
 *      Author: Leon
 */

#pragma once

#include "CFGbip.h"
#include "DummyGNode.h"
#include "EndGNode.h"
#include "SuchThatClause.h"
#include "StmtTable.h"

using namespace boost;

class NextStarBipClause
	: public SuchThatClause {

public:
	NextStarBipClause(void);
	~NextStarBipClause(void);

	bool isValid(void);

protected:

	//e.g. Parent(string,string)
	bool evaluateS1FixedS2Fixed(string, string);
	//e.g. Parent(_,_)
	bool evaluateS1GenericS2Generic();
	//e.g. Parent(_,string)
	bool evaluateS1GenericS2Fixed(string);
	//Parent(string,_)
	bool evaluateS1FixedS2Generic(string);
	//Parent(string,s2)
	unordered_set<string> getAllS2WithS1Fixed(string);
	//Parent(_,s2)
	unordered_set<string> getAllS2();
	//Parent(s1,string)
	unordered_set<string> getAllS1WithS2Fixed(string);
	//Parent(s1,__)
	unordered_set<string> getAllS1();
	//Parent(s1,s2)
	unordered_set<vector<string>> getAllS1AndS2();

private:
	CFGbip* cfg;
	bool match;
	unordered_set<string> results;
	unordered_set<vector<string>> resultsPair;

	void dfsFind(Statement*, string, vector<string>, stack<int>);
	void dfsFind(GNode*, string, vector<string>, stack<int>);

	void dfsFindNext(Statement*, vector<string>, stack<int>, string);
	void dfsFindNext(GNode*, vector<string>, stack<int>, string);

	bool contains(vector<string>, string);

	int getNodePosition(vector<GNode*>, GNode*);
	
	GNode* traverseDummyToGetAnything(GNode*);

	unordered_set<Statement*> getNeededStmts(string);
	bool isNeededArgType(string, int);
	int getChildNodeType(GNode*);
	bool isNodeType(GNode*, GType);
};