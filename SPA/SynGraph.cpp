#include "SynGraph.h"

using namespace stringconst;

SynGraph::SynGraph(void) {
	synTable = SynTable();
	graphTable = GraphTable();
	compWeightList = ComponentWeight();
	componentSyn = ComponentSyns();

	componentIndex = 0;
}

SynGraph::~SynGraph(void) {

}

vector<int> SynGraph::reorderQuery(Query* q) {
	vector<int> queryCompSize;
	// Case of no clauses
	if (q->getClauseList().size() == 0) {
		queryCompSize.push_back(0);
		return queryCompSize;

	} else if (q->getClauseList().size() == 1) {
		// Case of 1 clause
		queryCompSize.push_back(1);
		return queryCompSize;

	} else {
		// Case of more than 1 clause
		originalClauseList = q->getClauseList();
		populateGraphTable(q);
		setComponents();
		arrangeComponentClauses(q, queryCompSize);
		return queryCompSize;
	}
}

// ------------------------------arrangeComponentClauses and private functions ----------------------------//

void SynGraph::arrangeComponentClauses(Query* q, vector<int>& queryCompSize) {
	vector<Clause*> componentClauseList;
	BOOST_FOREACH(auto i, compWeightList) {
		int compIndex = i.first;
		unordered_set<string> compSyn = componentSyn[compIndex];
		if (compSyn.size() == 0) {
			// Case of a fixed clause in a component
			// fixed clause synonym is the compIndex (an integer)
			EdgeList edgelist = graphTable[boost::lexical_cast<std::string>(compIndex)];
			SynEdge* edge = edgelist.at(0).first;
			Clause* c = formClause(edge);
			componentClauseList.push_back(c);
			queryCompSize.push_back(1);

		} else if (compSyn.size() == 1) {
			// Case of a single synonym clause(s) 
			// in a component
			string syn1 = *(compSyn.begin());
			vector<Clause*> listOfClauses = getSingleSynClause(syn1);
			componentClauseList.insert(componentClauseList.end(), listOfClauses.begin(), listOfClauses.end());
			queryCompSize.push_back(listOfClauses.size());
		
		} else {
			// Case of more than 1 synonym
			// in a component
			pair<SynNode*, SynEdge*> link = getStartingSyn(compIndex);
			structureClausesByPrimAlgorithm(componentClauseList, queryCompSize, link);

			/*
			BOOST_FOREACH(auto i, componentClauseList) {
				cout << "(clause type)arrangeComponentClauses: ";
				cout << i->getClauseType();
				cout << " | ";
				cout << endl;
			}
	
			BOOST_FOREACH(auto i, queryCompSize) {
				cout << "component size: ";
				cout << i;
				cout << " | ";
				cout << endl;
			}
			*/
		}
	}
	q->setClauseList(componentClauseList);
}

vector<Clause*> SynGraph::getSingleSynClause(string syn) {
	vector<Clause*> listOfClauses = vector<Clause*>();
	SynNode* node1 = synTable[syn];
	EdgeList edgelist = graphTable[syn];
	BOOST_FOREACH(auto i, edgelist) {
		SynNode* node2 = i.second;
		SynEdge* edge = i.first;
		if (node1->isSame(*node2)) {
			Clause* c = formClause(edge);
			listOfClauses.push_back(c);
		}
	}
	std::sort(listOfClauses.begin(), listOfClauses.end(), clauseCmp());
	return listOfClauses;
}

// CLAUSES WE DONT WANT: NEXT*, AFFECTS AND AFFECTS*
// This synonym will be the starting synonym used for prim's algo
pair<SynNode*, SynEdge*> SynGraph::getStartingSyn(int compIndex) {
	unordered_set<string> compSynSet = componentSyn[compIndex];
	// desiredSingleSyn set does not have next*, affects and affects* syn
	unordered_set<string> desiredSingleSyn = getDesiredSingleSyn(compSynSet);

	pair<SynNode*, SynEdge*> syn;
	if (desiredSingleSyn.size() > 0) {
		syn = getStartingSynonymForSingleSyn(desiredSingleSyn);

	} else {
		syn = getStartingSynonymForMultiSyn(compSynSet);

	}
	return syn;
}

unordered_set<string> SynGraph::getDesiredSingleSyn(unordered_set<string> synSet) {
	unordered_set<string> desiredSingleSynSet;
	BOOST_FOREACH(auto i, synSet) {
		string syn = i;
		EdgeList edgelist = graphTable[syn];
		BOOST_FOREACH(auto j, edgelist) {
			bool isSingleSynArgClause = (synTable[syn])->isSame(*(j.second));
			bool isUndesiredClause = undesiredClauseCheck(j.first);
			if (isSingleSynArgClause && !isUndesiredClause) {
				desiredSingleSynSet.insert(syn);
			}
		}
	}
	return desiredSingleSynSet;
}

// REFACTOR THIS FUNCTION!!!
pair<SynNode*, SynEdge*> SynGraph::getStartingSynonymForSingleSyn(unordered_set<string> synSet) {
	int count = 0;
	BOOST_FOREACH(auto i, synSet) {
		string syn = i;
		EdgeList edgelist = graphTable[syn];
		BOOST_FOREACH(auto j, edgelist) {
			// Check if this node has any neighbour
			// that is not Affects, Affects* or next*
			bool isUndesiredClause = undesiredClauseCheck(j.first);
			if (!isUndesiredClause) {
				// check if this is a single syn arg clause
				bool isSingleSynArgClause = (synTable[syn])->isSame(*(j.second));
				if (isSingleSynArgClause) {
					if (count > 0) {
						// Case of having a neighbour that is not Affects, Affects* or next*
						pair<SynNode*, SynEdge*> link = make_pair(synTable[syn], j.first);
						return link;
					
					} else {
						count++;
					}
				} else {
					// Case of having a neighbour that is not Affects, Affects* or next*
					pair<SynNode*, SynEdge*> link = getSingleSynEdge(syn);
					return link;
				}
			}
		}
		// reset count to 0 for the next syn
		count = 0;
	}
	// Case of all neighbours are undesired clauses,
	// return any single syn arg clause
	BOOST_FOREACH(auto i, synSet) {
		string syn2 = i;
		EdgeList edgelist = graphTable[syn2];
		BOOST_FOREACH(auto j, edgelist) {
			bool isSingleSynArgClause = (synTable[syn2])->isSame(*(j.second));
			if (isSingleSynArgClause) {
				pair<SynNode*, SynEdge*> link = make_pair(synTable[syn2], j.first);
				return link;
			}
		}
	}
}

pair<SynNode*, SynEdge*> SynGraph::getSingleSynEdge(string syn) {
	pair<SynNode*, SynEdge*> link = pair<SynNode*, SynEdge*>();
	SynNode* node1 = synTable[syn];
	EdgeList edgelist = graphTable[syn];
	BOOST_FOREACH(auto i, edgelist) {
		SynEdge* edge = i.first;
		SynNode* node2 = i.second;
		if (node1->isSame(*node2)) {
			link = make_pair(node1, edge);
		}
	}
	return link;
}

bool SynGraph::undesiredClauseCheck(SynEdge* edge) {
	int clauseIndex = edge->getClauseIndex();
	Clause* c = originalClauseList.at(clauseIndex);
	ClauseType cType = c->getClauseType();

	if (cType == AFFECTS_ || cType == AFFECTSSTAR_ || cType == NEXTSTAR_) {
		return true;
	} else {
		return false;
	}
}

// REFACTOR THIS FUNCTION!!!
pair<SynNode*, SynEdge*> SynGraph::getStartingSynonymForMultiSyn(unordered_set<string> synSet) {
	int count = 0;
	BOOST_FOREACH(auto i, synSet) {
		string syn = i;
		EdgeList edgelist = graphTable[syn];
		BOOST_FOREACH(auto j, edgelist) {
			SynEdge* edge = j.first;
			bool isUndesiredClause = undesiredClauseCheck(j.first);
			if (!isUndesiredClause) {
				if (count > 0) {
					pair<SynNode*, SynEdge*> link = make_pair(synTable[syn], j.first);
					return link;

				} else {
					count++;
				}
			}
		}
		// reset count to 0 for the next syn
		count = 0;
	}

	// Case of all neighbours are undesired clauses,
	// return any multi syn arg clause
	string syn2 = *(synSet.begin());
	EdgeList edgelist2 = graphTable[syn2];
	pair<SynNode*, SynEdge*> link = make_pair(edgelist2.at(0).second, edgelist2.at(0).first);
	return link;
}

void SynGraph::structureClausesByPrimAlgorithm(vector<Clause*>& componentClauseList, 
	vector<int>& queryCompSize, pair<SynNode*, SynEdge*> link) {

	int clauseCount = 0;
	Clause* c1 = formClause(link.second);
	link.second->setVisited();
	componentClauseList.push_back(c1);
	clauseCount++;

	PriorityQ pQueue;
	string currentSyn = (link.first)->getSynonym();
	EdgeList edgelist1 = graphTable[currentSyn];
	BOOST_FOREACH(auto i, edgelist1) {
		SynEdge* edge1 = i.first;
		bool isEdgeVisited = (i.first)->isVisited();
		if (!isEdgeVisited) {
			pQueue.push(i);
		} else {
			SynNode* node2 = i.second;
			bool isSingleSynClause = (link.first)->isSame(*node2);
			// When starting with a multi syn clause
			if (!isSingleSynClause) {
				string syn2 = node2->getSynonym();
				EdgeList edgelist2 = graphTable[syn2];
				BOOST_FOREACH(auto j, edgelist2) {
					bool isEdgeVisited = (j.first)->isVisited();
					if (!isEdgeVisited) {
						pQueue.push(j);
					}
				}
			}
		}
	}

	while (pQueue.size() > 0) {
		pair<SynEdge*, SynNode*> clauseLink = pQueue.top();
		pQueue.pop();
		bool isEdgeVisited = (clauseLink.first)->isVisited();
		
		if (!isEdgeVisited) {
			Clause* c2 = formClause(clauseLink.first);
			(clauseLink.first)->setVisited();
			componentClauseList.push_back(c2);
			clauseCount++;

			string syn2 = (clauseLink.second)->getSynonym();
			EdgeList edgelist2 = graphTable[syn2];
			BOOST_FOREACH(auto i, edgelist2) {
				bool isEdgeVisited = (i.first)->isVisited();
				if (!isEdgeVisited) {
					pQueue.push(i);
				}
			}
		}
	}
	queryCompSize.push_back(clauseCount);
	
	/*
	BOOST_FOREACH(auto i, componentClauseList) {
		cout << "(clause type)prim algo: ";
		cout << i->getClauseType();
		cout << " | ";
		cout << endl;
	}
	
	BOOST_FOREACH(auto i, queryCompSize) {
		cout << "component size: ";
		cout << i;
		cout << " | ";
		cout << endl;
	}
	*/
}

Clause* SynGraph::formClause(SynEdge* edge) {
	int clauseIndex = edge->getClauseIndex();
	Clause* c = originalClauseList.at(clauseIndex);
	return c;
}

// --------------------------------populateGraphTable and private functions --------------------------//

// Populates synTable and graphTable
void SynGraph::populateGraphTable(Query* q) {
	createSelectSynNodes(q);
	createClauseSynNodes(q);
}

void SynGraph::createSelectSynNodes(Query* q) {
	vector<StringPair> selectList = q->getSelectList();
	bool hasSelectSyn = (selectList.at(0).getSecond() != ARG_BOOLEAN);
	// Case of "Select a s.t. ... "
	if (hasSelectSyn) {
		BOOST_FOREACH(auto i, selectList) {
			// Create and set SynNode
			// Select synonyms have a weight of 5
			// and component number -1 to define 
			// no set component yet

			string syn = i.getFirst();
			SynNode* synNode = new SynNode();
			synNode->setSynonym(syn);
			synNode->setWeight(1);
			synNode->setComponentNum(-1);

			// Set tables
			synTable.insert(make_pair(syn, synNode));
			graphTable.insert(make_pair(syn, EdgeList())); // Select synonyms have no Edges
		}
	}
	// Case of "Select BOOLEAN" will not have a SynNode
}

void SynGraph::createClauseSynNodes(Query* q) {
	vector<Clause*> clauseList = q->getClauseList();
	int clauseIndex = 0;
	int fixedCount = 0;
	BOOST_FOREACH(auto i, clauseList) {
		Clause* c = i;
		ClauseType cType = i->getClauseType();
		vector<string> synList = getClauseSynonym(c, cType);

		// Clause returns a boolean
		if (synList.size() == 0) {
			createFixedArgNode(c, clauseIndex, fixedCount);
			fixedCount++;

		} else { // Clause has synonym(s)
			createUnfixedArgNode(c, clauseIndex, synList);
		}
		clauseIndex++;
	}
}

// Retrieves the synonym arguments in a Clause
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
	vector<string> synList;
	WithClauseRef leftSide = wc->getLeftRef();
	WithClauseRef rightSide = wc->getRightRef();

	bool isLeftSyn = (leftSide.getRefType() == ATTRREF_ || 
						leftSide.getRefType() == SYNONYM_);
	bool isRightSyn = (rightSide.getRefType() == ATTRREF_ || 
						rightSide.getRefType() == SYNONYM_);
	bool isLeftFixed = (leftSide.getRefType() == IDENT_ || 
						leftSide.getRefType() == INTEGER_);
	bool isRightFixed =(rightSide.getRefType() == IDENT_ || 
						leftSide.getRefType() == INTEGER_);
	
	// get multi synonym
	if (isLeftSyn && isRightSyn) {
		string syn1 = leftSide.getEntity();
		string syn2 = rightSide.getEntity();
		string synArr [] = { syn1,syn2 };
		synList.insert (synList.begin(), synArr, synArr+2);
		return synList;
	} 

	// get single synonym
	if (isLeftSyn) {
		string syn1 = leftSide.getEntity();
		synList.push_back(syn1);
		return synList;
	}
	if (isRightSyn) {
		string syn2 = rightSide.getEntity();
		synList.push_back(syn2);
		return synList;
	}
	return synList;
}

// Creates and set fixed arg SynNodes and 
// add it to componentMap and componentWeight
void SynGraph::createFixedArgNode(Clause* c, int cIndex, int fixedCount) {
	// Create and set SynNode
	// Fixed arg have a weight of 0
	// and they are a component by themselves
	string syn = boost::lexical_cast<string>(fixedCount);
	SynNode* synNode = new SynNode();
	synNode->setSynonym(syn);
	synNode->setWeight(0);
	synNode->setComponentNum(componentIndex);

	// Create SynEdge and link to itself
	SynEdge* edge = new SynEdge(c->getWeight(), cIndex);
	EdgeList edgeList = EdgeList();
	edgeList.push_back(make_pair(edge, synNode));

	// Insert into tables
	synTable.insert(make_pair(syn, synNode));
	graphTable.insert(make_pair(syn, edgeList));

	// Add to componentMap and componentWeight
	storeFixedArgClauseCompWeight(componentIndex, syn);
	
	componentIndex++;
}

// As a fixed clause is a component on it's own, this function
// stores the mapping of the weight of a fixed clause component to 
// the component number
void SynGraph::storeFixedArgClauseCompWeight(int index, string syn) {
	int weight = 0;
	SynNode* node = synTable[syn];
	weight += node->getWeight();

	EdgeList edgelist = graphTable[syn];
	SynEdge* edge = edgelist.at(0).first;
	weight += edge->getWeight();

	compWeightList.push_back(make_pair(index, weight));
}

vector<Clause*> SynGraph::edgeToClause(string syn) {
	EdgeList edgelist = graphTable[syn];
	SynEdge* edge = edgelist.at(0).first;
	Clause* c = originalClauseList.at(edge->getClauseIndex());
	vector<Clause*> clauselist = vector<Clause*>();
	clauselist.push_back(c);
	return clauselist;
}

void SynGraph::linkSingleUnfixedArgNode(Clause* c, int cIndex, string syn) {
	bool isSynExist = (synTable.find(syn) != synTable.end());
	if (isSynExist) {
		// If synonym already exist, add another edge to it
		SynNode* node = synTable[syn];
		EdgeList edgeList = graphTable[syn];
		SynEdge* edge = new SynEdge(c->getWeight(), cIndex);
		graphTable[syn].push_back(make_pair(edge, node));
		//edgeList.push_back(make_pair(edge, node));

	} else {
		// if synonym does not exist
		SynNode* synNode = new SynNode();
		synNode->setSynonym(syn);
		synNode->setWeight(1);
		synNode->setComponentNum(-1);

		// Create SynEdge
		SynEdge* edge1 = new SynEdge(c->getWeight(), cIndex);
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
			SynNode* node1 = synTable[syn1];
			SynNode* node2 = synTable[syn2];

			SynEdge* edge = new SynEdge(c->getWeight(), cIndex);
			graphTable[syn1].push_back(make_pair(edge, node2));
			graphTable[syn2].push_back(make_pair(edge, node1));
		
		} else if (isSyn1Exist && !isSyn2Exist) {
			// If only syn1 exist
			// create SynNode for syn2 and add edge to both SynNode
			updateExistNodeAndCreateNewNode(syn1, syn2, c, cIndex);
			
		} else if (!isSyn1Exist && isSyn2Exist) {
			// If only syn2 exist
			// create SynNode for syn1 and add edge to both SynNode
			updateExistNodeAndCreateNewNode(syn2, syn1, c, cIndex);
			
		} else {
			// Both syn1 and syn2 does not exist yet
			// create SynNode for both synonyms and add edge to both SynNode
			SynNode* node1 = new SynNode();
			node1->setSynonym(syn1);
			node1->setWeight(1);
			node1->setComponentNum(-1);
			synTable[syn1] = node1;

			SynNode* node2 = new SynNode();
			node2->setSynonym(syn2);
			node2->setWeight(1);
			node2->setComponentNum(-1);
			synTable[syn2] = node2;

			SynEdge* edge = new SynEdge(c->getWeight(), cIndex);
			EdgeList edgeList1 = EdgeList();
			EdgeList edgeList2 = EdgeList();
			edgeList1.push_back(make_pair(edge, node2));
			edgeList2.push_back(make_pair(edge, node1));
			graphTable[syn1] = edgeList1;
			graphTable[syn2] = edgeList2;
		}
	}
}

void SynGraph::updateExistNodeAndCreateNewNode(string existingSyn, 
	string nwSyn, Clause* c, int cIndex) {

	SynNode* existingNode = synTable[existingSyn];
	SynEdge* edge = new SynEdge(c->getWeight(), cIndex);

	SynNode* nwNode = new SynNode();
	nwNode->setSynonym(nwSyn);
	nwNode->setWeight(1);
	nwNode->setComponentNum(-1);
	synTable[nwSyn] = nwNode;

	EdgeList nwEdgelist = EdgeList();
	nwEdgelist.push_back(make_pair(edge, existingNode));
	graphTable[nwSyn] = nwEdgelist;
	graphTable[existingSyn].push_back(make_pair(edge, nwNode));
}

// -------------------------------------setComponents() and private functions -----------------------------------------//

// Set the component number of synNodes that do not have a component
// number yet. SynNodes that can be reached from another synNode are
// in the same component.
void SynGraph::setComponents() {
	BOOST_FOREACH(auto i, synTable) {
		string syn = i.first;
		SynNode* node1 = i.second;
		bool componentNotSet = (node1->getComponentNum() == -1);
		if (componentNotSet) {
			setComponentNumByDFS(node1, syn, componentIndex);
			componentIndex++;
		}
	}
	std::sort(compWeightList.begin(), compWeightList.end(), componentCmp());
}

// This function sets the componentIndex of the SynNode as it traverse the graph.
// It also sets the component's weight at the end of traversal and fills
// up componentSyn map
void SynGraph::setComponentNumByDFS(SynNode* node1, string syn, int index) {
	//ClauseWeight clauseSet = ClauseWeight();
	int componentWeight = 0;
	queue<SynNode*> q;
	q.push(node1);
	componentWeight += node1->getWeight();

	while (q.size() > 0) {
		SynNode* node = q.front();
		q.pop();
		if (node->getComponentNum() == -1) {
			node->setComponentNum(index);
			string nodeName = node->getSynonym();
			EdgeList edgelist = graphTable[nodeName];
			
			// In the case of select synonym, it might not
			// have any edge in it's edgelist yet
			BOOST_FOREACH(auto i, edgelist) {
				SynEdge* edge = i.first;
				SynNode* neighbour = i.second;
				componentWeight += edge->getWeight();
				componentSyn[index].insert(nodeName);

				//  check if it is another synonym
				if (!neighbour->isSame(*node)) {
					q.push(neighbour);					
				} 
			}
		}
	}

	// Select synonyms are not a component
	if (graphTable[syn].size() > 0) {
		compWeightList.push_back(make_pair(index, componentWeight));
	}
}

