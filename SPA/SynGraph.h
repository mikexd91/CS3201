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
#include <boost/heap/priority_queue.hpp>

#include <utility>

using namespace boost;
using boost::unordered_map;
using boost::unordered_set;

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

struct EdgeCmp
{
	bool operator()(pair<SynEdge*, SynNode*> a, pair<SynEdge*, SynNode*> b) const
	{
		SynEdge* edge1 = a.first;
		SynEdge* edge2 = b.first;
		int edge1Weight = edge1->getWeight();
		int edge2Weight = edge2->getWeight();
		return edge1Weight > edge2Weight;
	}
};

class SynGraph
{
public:
	// For pairs, first of type int always refer to index, 
	// second int type refers to what I am storing together 
	// with that index
	typedef vector<pair<SynEdge*, SynNode*>> EdgeList;
	typedef unordered_map<string, SynNode*> SynTable;
	typedef unordered_map<string, EdgeList> GraphTable;
	typedef vector<pair<int, int>> ComponentWeight;
	typedef unordered_map<int, unordered_set<string>> ComponentSyns;
	typedef boost::heap::priority_queue<pair<SynEdge*, SynNode*>, heap::compare<EdgeCmp>> PriorityQ;

	SynGraph(void);
	~SynGraph(void);

	vector<int> reorderQuery(Query*);
 
private:
	vector<Clause*> originalClauseList;
	SynTable synTable;
	GraphTable graphTable;
	ComponentWeight compWeightList;		// Stores the component index and the component's weight
	ComponentSyns componentSyn;	
	
	int componentIndex;
	
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
	void updateExistNodeAndCreateNewNode(string, string, Clause*, int);
	
	void setComponents(void);
	void setComponentNumByDFS(SynNode*, string, int);
	
	void arrangeComponentClauses(Query*, vector<int>&);
	vector<Clause*> getSingleSynClause(string);
	pair<SynNode*, SynEdge*> getStartingSyn(int);
	unordered_set<string> getDesiredSingleSyn(unordered_set<string>);
	pair<SynNode*, SynEdge*> getStartingSynonymForSingleSyn(unordered_set<string>);
	pair<SynNode*, SynEdge*> getSingleSynEdge(string);
	bool undesiredClauseCheck(SynEdge*);
	pair<SynNode*, SynEdge*> getStartingSynonymForMultiSyn(unordered_set<string>);
	void structureClausesByPrimAlgorithm(vector<Clause*>&, vector<int>&, pair<SynNode*, SynEdge*>);
	Clause* formClause(SynEdge*);
};