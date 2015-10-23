#include "SynGraph.h"

using namespace stringconst;

SynGraph::SynGraph(void) {
	unordered_map<string, EdgeList> graphTable = unordered_map<string, EdgeList>();
	vector<int> componentWeightList = vector<int>();
}

SynGraph::~SynGraph(void) {
}

void SynGraph::reorderQuery(Query* q) {
	/*
	populateGraphTable(q);
	findDisjointedComponents();
	calculateComponentsWeight();
	structureClauseWithinComponent();
	setClause //to be done in this function
	
	*/
}

bool SynGraph::isSelectSyn(vector<StringPair> selectList) {
	if (selectList.at(0).getSecond() != ARG_BOOLEAN) {
		return true;
	} else {
		return false;
	}
}

vector<string> SynGraph::getSynFromSuchThatClause(SuchThatClause* stc) {
	vector<string> synList = vector<string>();
	bool isNotFixS1 = (stc->getFirstArgFixed() == false);
	bool isNotFixS2 = (stc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (stc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (stc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(stc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(stc->getSecondArg());
	} 
	return synList;
}

vector<string> SynGraph::getSynFromPatternClause(PatternClause* pc) {
	vector<string> synList = vector<string>();
	bool isNotFixS2 = (pc->isVarFixed() == false);
	bool isNotUnderscoreS2 = (pc->getVarType() != ARG_GENERIC);
	synList.push_back(pc->getSyn());
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(pc->getVar());
	}
	return synList;
}

vector<string> SynGraph::getSynonyms(Clause* c) {
	vector<string> synList = vector<string>();
	ClauseType cType = c->getClauseType();
	if ((cType != WITH_) && 
		(cType != PATTERNASSG_) &&
		(cType != PATTERNIF_) &&
		(cType != PATTERNWHILE_)) {
		SuchThatClause* stc = (SuchThatClause*) c;
		synList = getSynFromSuchThatClause(stc);
	
	} else if (cType != WITH_) { // If it is a Pattern clause
		PatternClause* pc = (PatternClause*) c;
		synList = getSynFromPatternClause(pc);

	} else { // If is a With clause
		// get synonym from with clause
	}
	return synList;
}

void SynGraph::createSelectSynNodes(Query* q) {
	if (isSelectSyn(q->getSelectList())) {
		vector<StringPair> selectList = q->getSelectList();
		BOOST_FOREACH(auto i, selectList) {
			// Select synonyms have a weight of 5
			string syn = i.getFirst();
			SynNode synNode = SynNode(syn);
			synNode.setWeight(5);
			graphTable[syn] = EdgeList();
		}
	} 
}

void SynGraph::createFixedSynNode(Clause* c, int clauseIndex) {
	// A boolean SynNode has an "integer" synonym name
	// and it will have a weight of 0, unless it is an affects
	string syn = boost::lexical_cast<string>(fixedCount);
	SynNode synNode = SynNode(syn);
	synNode.setComponentNum(fixedCount);
	synNode.setWeight(0);

	SynEdge edge = SynEdge(c->getWeight(), clauseIndex);
	EdgeList list = EdgeList();
	pair<SynEdge, SynNode> p = make_pair(edge, synNode); // Loop to itself
	list.push_back(p);

	graphTable[syn] = list;
	fixedCount++;
}

void SynGraph::createUnfixedSynNode(Clause* c, int clauseIndex, vector<string> synList) {

}

void SynGraph::createClauseSynNodes(Query* q) {
	vector<Clause*> clauseList = q->getClauseList();
	int clauseIndex = 0;
	BOOST_FOREACH(auto i, clauseList) {
		Clause* c = i;
		ClauseType cType = i->getClauseType();
		vector<string> synList = getSynonyms(c);

		// Clause returns a boolean
		if (synList.size() == 0) {
			createFixedSynNode(c, clauseIndex);

		} else { // Clause needs to populate a synonym
			createUnfixedSynNode(c, clauseIndex, synList);
		}
		clauseIndex++;
	}
}

void SynGraph::populateGraphTable(Query* q) {
	createSelectSynNodes(q);
	createClauseSynNodes(q);

	// Creating clause synNodes
	vector<Clause*> clauseList = q->getClauseList();
	int clauseIndex = 0;
	BOOST_FOREACH(auto i, clauseList) {
		Clause* c = i;
		ClauseType cType = i->getClauseType();
		vector<string> synList = getSynonyms(c);

		// Clause needs to evaluate 1 synonym
		// ordinary synonyms have weight 1
		if (synList.size() == 1) {
			string syn = synList.at(0);
			SynNode synNode = SynNode(syn);

			// check if clause's synonym is the same as select synonym
			if (isSameAsSelectSyn(syn, selectSyn)) {

			} else {

			}
			synNode.setWeight(1);

			SynEdge edge = SynEdge(c->getWeight(), clauseIndex);
			EdgeList list = EdgeList();
			pair<SynEdge, SynNode> p = make_pair(edge, synNode); // Loop to itself
			list.push_back(p);

			graphTable[syn] = list;
		}

		// Clause needs to evaluate 2 synonyms
		if (synList.size() == 2) {
			if (isSameAsSelectSyn(synList, selectSyn)) {

			}
			string syn1 = synList.at(0);
			SynNode synNode1 = SynNode(syn1);
			
		}
		// if clause needs to return a boolean = fixed
		// if clause needs to return synonyms = not fixed

		// set edge weight of clauses with "fixed" synonym
		// put all fixed clauses in 1 component

		//FollowsClause* fc = (FollowsClause*) c;
		//fc->getS
		// cast it to the correct clause type
		// find synonym from unordered_map
		clauseIndex++;
	}
}

void SynGraph::findDisjointedComponents() {

}

/*
vector<string> SynGraph::getSelectSyn(Query q) {
	vector<string> list = vector<string>();
	vector<StringPair> selectList = q.getSelectList();
	BOOST_FOREACH(auto i, selectList) {
		string syn = i.getFirst();
		list.push_back(syn);
	}
	return list;
}

void SynGraph::createSynNodes(vector<string> selectList, vector<Clause*> clauseList) {
	bool isSelectBool = selectList.at(0).
	BOOST_FOREACH(auto i, selectList) {
		
	}
}

void SynGraph::buildGraph(vector<string> selectList, vector<Clause*> clauseList) {
	createSynNodes(selectList, clauseList);
}
*/