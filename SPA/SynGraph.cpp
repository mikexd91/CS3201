#include "SynGraph.h"

using namespace stringconst;

SynGraph::SynGraph(void) {
	synTable = unordered_map<string, SynNode>();
	graphTable = unordered_map<string, EdgeList>();
	vector<int> componentWeightList = vector<int>();
	vector<int> componentOrder = vector<int>();
	vector<int> orderedClauseList = vector<int>();

	fixedCount = 1;
	componentIndex = 1;
}

SynGraph::~SynGraph(void) {

}

void SynGraph::reorderQuery(Query* q) {
	originalClauseList = q->getClauseList();
	populateGraphTable(q);
	setComponents();
	orderComponents();
	orderComponentClauses();

	// get component index, starting from smallest weighted component
	// set q with this component's clauseList
	// do the same for the other components

	// after testing, add in empty clauses for results optimisation
}

void SynGraph::populateGraphTable(Query* q) {
	createSelectSynNodes(q);
	createClauseSynNodes(q);
}

void SynGraph::setComponents() {
	BOOST_FOREACH(auto i, synTable) {
		string syn = i.first;
		SynNode node1 = i.second;
		bool componentNotSet = (node1.getComponentNum() == -1);
		if (componentNotSet) {
			unordered_set<pair<int, int>> clauseIndexSet = dfs(node1, syn, componentIndex);
			vector<Clause*> clauseList = setClauseWeight(clauseIndexSet);
			orderClauses(clauseList);
			componentClause.insert(make_pair(componentIndex, clauseList));
			componentIndex++;
		}
	}
}

void SynGraph::orderClauses(vector<Clause*> clauseList) {
	std::sort(clauseList.begin(), clauseList.end(), clauseCmp());
}

vector<Clause*> SynGraph::setClauseWeight(unordered_set<pair<int, int>> clauseSet) {
	vector<Clause*> clauseList = vector<Clause*>();
	BOOST_FOREACH(auto i, clauseSet) {
		Clause* c = originalClauseList.at(i.first);
		c->setWeight(i.second);
		clauseList.push_back(c);
	}
	return clauseList;
}

void SynGraph::orderComponents() {
	std::sort(componentWeightList.begin(), componentWeightList.end(), componentCmp());
}

void SynGraph::orderComponentClauses() {
	 vector <bool> componentChecked (componentIndex);
	 vector<int> clauseIndexOrdered = vector<int>();

}

// Helper function for setComponents() START
unordered_set<pair<int, int>> SynGraph::dfs(SynNode node1, string syn, int index) {
	unordered_set<pair<int, int>> clauseSet = unordered_set<pair<int, int>>();
	int componentWeight = 0;
	queue<SynNode> q;
	q.push(node1);
	componentWeight += node1.getWeight();

	while (q.size() > 0) {
		SynNode node = q.front();
		q.pop();
		if (node.getComponentNum() == -1) {
			node.setComponentNum(index);
			string nodeName = node.getSynonym();
			EdgeList edgelist = graphTable[nodeName];
			
			// In the case of select synonym, it might not
			// have any edge in it's edgelist yet
			BOOST_FOREACH(auto i, edgelist) {
				SynEdge edge = i.first;
				SynNode neighbour = i.second;
				componentWeight += edge.getWeight();

				//  check if it is another synonym
				if (!neighbour.isSame(node)) {
					// neighbour is another synonym,
					// this clause has 2 synonyms
					q.push(neighbour);
					
					// Add clause and clause weight to clauseSet
					int clauseWeight = edge.getWeight() + 
									node.getWeight() + 
									neighbour.getWeight();
					clauseSet.insert(make_pair(edge.getClauseIndex(), clauseWeight));

				} else {
					// neighbour is me, this clause only has 1 synonym
					int clauseWeight = edge.getWeight() + node.getWeight();
					clauseSet.insert(make_pair(edge.getClauseIndex(), clauseWeight));
				}
			}
		}
	}
	componentWeightList.push_back(make_pair(index, componentWeight));
	return clauseSet;
}

// Helper function for setComponent() END //

// Helper functions for populateGraphTable START //
vector<string> SynGraph::getSynFromSuchThatClause(SuchThatClause* stc) {
	bool isFirstArgSyn = ((stc->getFirstArgFixed()) == false) && 
						(stc->getFirstArgType() != ARG_GENERIC);
	bool isSecondArgSyn = ((stc->getSecondArgFixed()) == false) &&
						(stc->getSecondArgType() != ARG_GENERIC);
	vector<string> synList = vector<string>();
	if (isFirstArgSyn) {
		string syn1 = stc->getFirstArg();
		synList.push_back(syn1);
	}
	if (isSecondArgSyn) {
		string syn2 = stc->getSecondArg();
		synList.push_back(syn2);
	}
	return synList;
}

vector<string> SynGraph::getSynFromPatternClause(PatternClause* pc) {
	vector<string>synList = vector<string>();
	synList.push_back(pc->getSyn());
	bool isVarArgSyn = (pc->isVarFixed() == false) &&
						(pc->getVarType() != ARG_GENERIC);
	if (isVarArgSyn) {
		string syn = pc->getVar();
		synList.push_back(syn);
	}
	return synList;
}

vector<string> SynGraph::getSynFromWithClause(WithClause* wc) {
	return vector<string>();
}

vector<string> SynGraph::getClauseSynonym(Clause* c, ClauseType cType) {
	vector<string> synList = vector<string>();
	bool isSuchThatClauseType = (cType != WITH_) && 
		(cType != PATTERNASSG_) &&
		(cType != PATTERNIF_) &&
		(cType != PATTERNWHILE_);

	bool isPatternClauseType = (cType == PATTERNASSG_) ||
		(cType == PATTERNIF_) ||
		(cType == PATTERNWHILE_);

	if (isSuchThatClauseType) {
		SuchThatClause* stc = (SuchThatClause*) c;
		synList = getSynFromSuchThatClause(stc);
	
	} else if (isPatternClauseType) {
		PatternClause* pc = (PatternClause*) c;
		synList = getSynFromPatternClause(pc);

	} else { // If is a With clause
		WithClause* wc = (WithClause*) c;
		synList = getSynFromWithClause(wc);
	}
	return synList;
}

void SynGraph::createFixedArgNode(Clause* c, int cIndex) {
	// Create and set SynNode
	// Fixed arg have a weight of 0
	string syn = boost::lexical_cast<string>(fixedCount);
	SynNode synNode = SynNode();
	synNode.setSynonym(syn);
	synNode.setWeight(0);
	synNode.setComponentNum(componentIndex);
	fixedCount++;
	componentIndex++;

	// Create SynEdge and link to itself
	SynEdge edge = SynEdge(c->getWeight(), cIndex);
	EdgeList edgeList = EdgeList();
	edgeList.push_back(make_pair(edge, synNode));

	// Insert into tables
	synTable.insert(make_pair(syn, synNode));
	graphTable.insert(make_pair(syn, edgeList));
}

void SynGraph::linkSingleUnfixedArgNode(Clause* c, int cIndex, string syn) {
	bool isSynExist = (synTable.find(syn) != synTable.end());
	if (isSynExist) {
		// If synonym already exist, add another edge to it
		SynNode node = synTable[syn];
		EdgeList edgeList = graphTable[syn];
		SynEdge edge = SynEdge(c->getWeight(), cIndex);
		edgeList.push_back(make_pair(edge, node));

	} else {
		// if synonym does not exist
		SynNode synNode = SynNode();
		synNode.setSynonym(syn);
		synNode.setWeight(1);
		synNode.setComponentNum(-1);

		// Create SynEdge
		SynEdge edge1 = SynEdge(c->getWeight(), cIndex);
		EdgeList edgeList1 = EdgeList();
		edgeList1.push_back(make_pair(edge1, synNode));

		// Insert into tables
		synTable.insert(make_pair(syn, synNode));
		graphTable.insert(make_pair(syn, edgeList1));
	}
}

void SynGraph::createUnfixedArgNode(Clause* c, int cIndex, 
									vector<string> synList) {
	if (synList.size() == 1) {
		linkSingleUnfixedArgNode(c, cIndex, synList.at(0));
	
	} else {
		string syn1 = synList.at(0);
		string syn2 = synList.at(1);
		bool isSyn1Exist = (synTable.find(syn1) != synTable.end());
		bool isSyn2Exist = (synTable.find(syn2) != synTable.end());
		
		if (isSyn1Exist && isSyn2Exist) {
			// If both synonyms exist, add edge to both SynNode
			SynNode node1 = synTable[syn1];
			SynNode node2 = synTable[syn2];
			EdgeList edgeList1 = graphTable[syn1];
			EdgeList edgeList2 = graphTable[syn2];
			SynEdge edge = SynEdge(c->getWeight(), cIndex);
			edgeList1.push_back(make_pair(edge, node2));
			edgeList2.push_back(make_pair(edge, node1));
		
		} else if (isSyn1Exist && !isSyn2Exist) {
			// If only syn1 exist
			// create SynNode for syn2 and add edge to both SynNode
			SynNode node1 = synTable[syn1];
			EdgeList edgeList1 = graphTable[syn1];
			SynEdge edge = SynEdge(c->getWeight(), cIndex);
			
			SynNode node2 = SynNode();
			node2.setSynonym(syn2);
			node2.setWeight(1);
			node2.setComponentNum(-1);

			EdgeList edgeList2 = EdgeList();
			edgeList2.push_back(make_pair(edge, node1));
			edgeList1.push_back(make_pair(edge, node2));

		} else if (!isSyn1Exist && isSyn2Exist) {
			// If only syn2 exist
			// create SynNode for syn1 and add edge to both SynNode
			SynNode node2 = synTable[syn2];
			EdgeList edgeList2 = graphTable[syn2];
			SynEdge edge = SynEdge(c->getWeight(), cIndex);

			SynNode node1 = SynNode();
			node1.setSynonym(syn1);
			node1.setWeight(1);
			node1.setComponentNum(-1);

			EdgeList edgeList1 = EdgeList();
			edgeList1.push_back(make_pair(edge, node2));
			edgeList2.push_back(make_pair(edge, node1));

		} else {
			// Both syn1 and syn2 does not exist yet
			// create SynNode for both synonyms and add edge to both SynNode
			SynNode node1 = SynNode();
			node1.setSynonym(syn1);
			node1.setWeight(1);
			node1.setComponentNum(-1);

			SynNode node2 = SynNode();
			node2.setSynonym(syn2);
			node2.setWeight(1);
			node2.setComponentNum(-1);

			SynEdge edge = SynEdge(c->getWeight(), cIndex);
			EdgeList edgeList1 = EdgeList();
			EdgeList edgeList2 = EdgeList();
			edgeList1.push_back(make_pair(edge, node2));
			edgeList2.push_back(make_pair(edge, node1));
		}
	}
}

void SynGraph::createClauseSynNodes(Query* q) {
	vector<Clause*> clauseList = q->getClauseList();
	int clauseIndex = 0;
	BOOST_FOREACH(auto i, clauseList) {
		Clause* c = i;
		ClauseType cType = i->getClauseType();
		vector<string> synList = getClauseSynonym(c, cType);

		// Clause returns a boolean
		if (synList.size() == 0) {
			createFixedArgNode(c, clauseIndex);

		} else { // Clause needs to populate a synonym
			createUnfixedArgNode(c, clauseIndex, synList);
		}
		clauseIndex++;
	}
}

bool SynGraph::isSelectSyn(vector<StringPair> selectList) {
	if (selectList.at(0).getSecond() != ARG_BOOLEAN) {
		return true;
	} else {
		return false;
	}
}

void SynGraph::createSelectSynNodes(Query* q) {
	vector<StringPair> selectList = q->getSelectList();
	bool hasSelectSyn = isSelectSyn(selectList);
	// Case of "Select a s.t. ... "
	if (hasSelectSyn) {
		BOOST_FOREACH(auto i, selectList) {
			// Create and set SynNode
			// Select synonyms have a weight of 5
			string syn = i.getFirst();
			SynNode synNode = SynNode();
			synNode.setSynonym(syn);
			synNode.setWeight(5);
			synNode.setComponentNum(-1);

			// Set tables
			synTable.insert(make_pair(syn, synNode));
			graphTable.insert(make_pair(syn, EdgeList())); // Select synonyms have no Edges
		}
	}
	// Case of "Select BOOLEAN" will not have a SynNode
}

// Helper functions for populateGraphTable END //
