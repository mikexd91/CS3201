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
#include "boost/foreach.hpp"
#include "boost/unordered_map.hpp"
#include <utility>
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
	void createSelectSynNodes(Query*);
	void createClauseSynNodes(Query*);
	void createFixedSynNode(Clause*, int);
	void createUnfixedSynNode(Clause*, int, vector<string>);
	void findDisjointedComponents(void);

	vector<string> getSynonyms(Clause*);
	vector<string> getSynFromPatternClause(PatternClause*);
	vector<string> getSynFromSuchThatClause(SuchThatClause*);

	//vector<string> getSelectSyn(Query);
	//void createSynNodes(vector<string>, vector<Clause*>);
	//void buildGraph(vector<string>, vector<Clause*>);
};