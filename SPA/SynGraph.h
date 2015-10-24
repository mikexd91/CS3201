#pragma once
#include "SynEdge.h"
#include "SynNode.h"
#include "Clause.h"
#include "Query.h"
#include "Utils.h"
#include "FollowsClause.h"
#include "FollowsStarClause.h"
#include "ParentClause.h"
#include "ParentStarClause.h"
#include "UsesClause.h"
#include "ModifiesClause.h"
#include "PatternAssgClause.h"
#include "PatternIfClause.h"
#include "PatternWhileClause.h"
#include "PatternClause.h"
#include "CallsClause.h"
#include "CallsStarClause.h"
#include "WithClause.h"
#include "AffectsClause.h"
#include "NextClause.h"
#include "NextStarClause.h"
#include "SuchThatClause.h"
#include "WithClause.h"

#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include "boost/unordered_set.hpp"
#include <boost/lexical_cast.hpp>

#include <utility>

using boost::unordered_map;

struct componentCmp
{
    bool operator()(pair<int, int> a, pair<int, int> b)
    {
		return a.second < b.second;
    }
};

struct clauseCmp
{
	bool operator()(Clause* a, Clause* b)
	{
		return a->getWeight() < b->getWeight();
	}
};

class SynGraph
{
public:
	typedef vector<pair<SynEdge, SynNode>> EdgeList;

	SynGraph(void);
	~SynGraph(void);

	void reorderQuery(Query*);

private:
	unordered_map<string, SynNode> synTable;
	unordered_map<string, EdgeList> graphTable;
	vector<pair<int, int>> componentWeightList;
	set<pair<int, vector<Clause*>>> componentClause;
	vector<int> componentOrder;
	vector<int> orderedClauseList;
	vector<Clause*> originalClauseList;

	int fixedCount;
	int componentIndex;

	// Create Synonyms, create Edges, link synonyms
	// Fill in graphTable
	void populateGraphTable(Query*);

	// populateGraphTable helper functions:
	void createSelectSynNodes(Query*);
	bool isSelectSyn(vector<StringPair>);
	void createClauseSynNodes(Query*);
	vector<string> getClauseSynonym(Clause*, ClauseType);
	vector<string> getSynFromSuchThatClause(SuchThatClause*);
	vector<string> getSynFromPatternClause(PatternClause*);
	vector<string> getSynFromWithClause(WithClause*);
	void createFixedArgNode(Clause*, int);
	void createUnfixedArgNode(Clause*, int, vector<string>); // Generics are handled here
	void linkSingleUnfixedArgNode(Clause*, int, string);

	// Find disjointed sets, set componentIndex
	void setComponents(void);
	unordered_set<pair<int, int>> dfs(SynNode, string, int);
	vector<Clause*> setClauseWeight(unordered_set<pair<int, int>>);
	void orderClauses(vector<Clause*>);
	void orderComponents(void);
	void orderComponentClauses(void);

	/*
	bool isSelectSyn(vector<StringPair>);
	
	void createSelectSynNodes(Query*);
	void createClauseSynNodes(Query*);
	void createFixedArgSynNode(Clause*, int);
	void createUnfixedArgSynNode(Clause*, int, vector<string>);
	void findDisjointedComponents(void);

	vector<string> getSynonyms(Clause*);
	vector<string> getSynFromPatternClause(PatternClause*);
	vector<string> getSynFromSuchThatClause(SuchThatClause*);
	*/
	//vector<string> getSelectSyn(Query);
	//void createSynNodes(vector<string>, vector<Clause*>);
	//void buildGraph(vector<string>, vector<Clause*>);
};