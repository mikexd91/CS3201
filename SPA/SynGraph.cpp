#include "SynGraph.h"

using namespace stringconst;

SynGraph::SynGraph(void) {
	synTable = SynTable();
	graphTable = GraphTable();
	compClauseMap = ComponentMap();
	compWeightList = ComponentWeight();

	componentIndex = 0;
}

SynGraph::~SynGraph(void) {

}

Query* SynGraph::reorderQuery(Query* q) {
	originalClauseList = q->getClauseList();
	populateGraphTable(q);
	setComponents();

	// get component index, starting from smallest weighted component
	// set q with this component's clauseList
	// do the same for the other components
	buildQuery(q);
	// after testing, add in empty clauses for results optimisation
	return q;
}

void SynGraph::buildQuery(Query* q) {
	// order component in ascending weight order
	std::sort(compWeightList.begin(), compWeightList.end(), componentCmp());
	int clauseIndex = 0;
	vector<Clause*> modifiedClause = vector<Clause*>();
	vector<Clause*>::iterator iter = modifiedClause.begin();
	BOOST_FOREACH(auto i, compWeightList) {
		int componentNum = i.first;
		vector<Clause*> clauselist = compClauseMap[componentNum];
		modifiedClause.insert(iter + clauselist.size(), clauselist.begin(), clauselist.end());
		iter += clauselist.size();
	}
	q->setClauseList(modifiedClause);
}

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
	return vector<string>();
}

// Creates and set fixed arg SynNodes and 
// add it to componentMap and componentWeight
void SynGraph::createFixedArgNode(Clause* c, int cIndex, int fixedCount) {
	// Create and set SynNode
	// Fixed arg have a weight of 0
	// and they are a component by themselves
	string syn = boost::lexical_cast<string>(fixedCount);
	SynNode synNode = SynNode();
	synNode.setSynonym(syn);
	synNode.setWeight(0);
	synNode.setComponentNum(componentIndex);

	// Create SynEdge and link to itself
	SynEdge edge = SynEdge(c->getWeight(), cIndex);
	EdgeList edgeList = EdgeList();
	edgeList.push_back(make_pair(edge, synNode));

	// Insert into tables
	synTable.insert(make_pair(syn, synNode));
	graphTable.insert(make_pair(syn, edgeList));

	// Add to componentMap and componentWeight
	storeFixedArgClauseCompWeight(componentIndex, syn);
	vector<Clause*> clauseList = edgeToClause(syn);
	compClauseMap[componentIndex] = clauseList;
	componentIndex++;
}

// As a fixed clause is a component on it's own, this function
// stores the mapping of the weight of a fixed clause component to 
// the component number
void SynGraph::storeFixedArgClauseCompWeight(int index, string syn) {
	int weight = 0;
	SynNode node = synTable[syn];
	weight += node.getWeight();

	EdgeList edgelist = graphTable[syn];
	SynEdge edge = edgelist.at(0).first;
	weight += edge.getWeight();

	compWeightList.push_back(make_pair(index, weight));
}

vector<Clause*> SynGraph::edgeToClause(string syn) {
	EdgeList edgelist = graphTable[syn];
	SynEdge edge = edgelist.at(0).first;
	Clause* c = originalClauseList.at(edge.getClauseIndex());
	vector<Clause*> clauselist = vector<Clause*>();
	clauselist.push_back(c);
	return clauselist;
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










// Set the component number of synNodes that do not have a component
// number yet. SynNodes that can be reached from another synNode are
// in the same component.
void SynGraph::setComponents() {
	BOOST_FOREACH(auto i, synTable) {
		string syn = i.first;
		SynNode node1 = i.second;
		bool componentNotSet = (node1.getComponentNum() == -1);
		if (componentNotSet) {
			// get clauses and their weight from traversing the graph
			// set the clauses with their respective weight
			// sort clauselist (component's clauses) by ascending weight order
			// map componentIndex to clauselist
			ClauseWeight clauseWeightMatchList = setComponentNumByDFS(node1, syn, componentIndex);
			vector<Clause*> clauseList = setClauseWeight(clauseWeightMatchList);
			std::sort(clauseList.begin(), clauseList.end(), clauseCmp());
			compClauseMap[componentIndex] = clauseList;
			componentIndex++;
		}
	}
}

vector<Clause*> SynGraph::setClauseWeight(ClauseWeight clauseSet) {
	vector<Clause*> clauseList = vector<Clause*>();
	BOOST_FOREACH(auto i, clauseSet) {
		Clause* c = originalClauseList.at(i.first);
		c->setWeight(i.second);
		clauseList.push_back(c);
	}
	return clauseList;
}

// This function sets the componentIndex of the SynNode as it traverse the graph.
// It also sets the component's weight at the end of traversal
SynGraph::ClauseWeight SynGraph::setComponentNumByDFS(SynNode node1, string syn, int index) {
	ClauseWeight clauseSet = ClauseWeight();
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
	compWeightList.push_back(make_pair(index, componentWeight));
	return clauseSet;
}

// Helper function for setComponent() END //
