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
#include "SuchThatClause.h"
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include <boost/lexical_cast.hpp>

using boost::unordered_map;

class SynGraph
{
public:
	SynGraph(void) : fixedCount(0) {};
	~SynGraph(void);

	typedef vector<pair<SynEdge, SynNode>> EdgeList;
	void reorderQuery(Query*);

private:
	unordered_map<string,EdgeList> graphTable;
	vector<int> componentWeightList;
	vector<int> componentOrder;
	vector<int> orderedClauseList;

	int fixedCount;

	bool isSelectSyn(vector<StringPair>);
	void populateGraphTable(Query*);
	vector<string> getSynonyms(Clause*);
	void findDisjointedComponents(void);

	vector<string>getFollowsSyn(Clause*);
	vector<string>getFollowsStarSyn(Clause* c);
	vector<string>getParentSyn(Clause*);
	vector<string>getParentStarSyn(Clause*);
	vector<string>getUsesSyn(Clause*);
	vector<string>getModifiesSyn(Clause*);
	vector<string>getCallsSyn(Clause*);
	vector<string>getCallsStarSyn(Clause*);
	vector<string>getNextSyn(Clause*);
	vector<string>getNextStarSyn(Clause*);
	/*
	vector<string>getAffectSyn(Clause*);
	vector<string>getAffectsStarSyn(Clause*);
	vector<string>getPatternAssignSyn(Clause*);
	vector<string>getPatternIfSyn(Clause*);
	vector<string>getPatternWhileSyn(Clause*);
	vector<string>getWithSyn(Clause*);
	*/



	//vector<string> getSelectSyn(Query);
	//void createSynNodes(vector<string>, vector<Clause*>);
	//void buildGraph(vector<string>, vector<Clause*>);
};