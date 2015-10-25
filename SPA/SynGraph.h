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
	// For pairs, first of type int always refer to index, 
	// second int type refers to what I am storing together 
	// with that index
	typedef vector<pair<SynEdge, SynNode>> EdgeList;
	typedef unordered_map<string, SynNode> SynTable;
	typedef unordered_map<string, EdgeList> GraphTable;
	typedef unordered_map<int, vector<Clause*>> ComponentMap;
	typedef vector<pair<int, int>> ComponentWeight;
	typedef unordered_set<pair<int, int>> ClauseWeight;

	SynGraph(void);
	~SynGraph(void);

	Query* reorderQuery(Query*); 		// Reorders the query for optimisation
	//Query* optimiseForResult(Query*);	// Add in empty clauses to denote the start of a component

private:
	vector<Clause*> originalClauseList;
	SynTable synTable;
	GraphTable graphTable;
	ComponentMap compClauseMap;			// Stores the component index and the ClauseList under it
	ComponentWeight compWeightList;		// Stores the component index and the component's weight

	int componentIndex;

	void buildQuery(Query*);
	void populateGraphTable(Query*);
	void createSelectSynNodes(Query*);
	void createClauseSynNodes(Query*);
	vector<string> getClauseSynonym(Clause*, ClauseType);
	vector<string> getSynFromSuchThatClause(SuchThatClause*);
	vector<string> getSynFromPatternClause(PatternClause*);
	vector<string> getSynFromWithClause(WithClause*);
	void createFixedArgNode(Clause*, int, int);
	void storeFixedArgClauseCompWeight(int, string);
	vector<Clause*> edgeToClause(string);
	void linkSingleUnfixedArgNode(Clause*, int, string);
	void createUnfixedArgNode(Clause*, int, vector<string>);
	void setComponents(void);
	vector<Clause*> setClauseWeight(ClauseWeight);
	ClauseWeight setComponentNumByDFS(SynNode, string, int);
	
};