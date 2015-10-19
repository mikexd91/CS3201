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

void SynGraph::populateGraphTable(Query* q) {
	// Creating select synNodes
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

	// Creating clause synNodes
	vector<Clause*> clauseList = q->getClauseList();
	int clauseIndex = 0;
	BOOST_FOREACH(auto i, clauseList) {
		Clause* c = i;
		ClauseType cType = i->getClauseType();
		vector<string> synList = getSynonyms(c);
		if (synList.size() > 0) {
			BOOST_FOREACH(auto j, synList) {
				string syn = j;
				graphTable[j] = EdgeList();
			}
		}
		// if clause needs to return a boolean = fixed
		// if clause needs to return synonyms = not fixed

		// set edge weight of clauses with "fixed" synonym
		// put all fixed clauses in 1 component

		//FollowsClause* fc = (FollowsClause*) c;
		//fc->getS
		// cast it to the correct clause type
		// find synonym from unordered_map
	}
}

// REFACTOR THIS CHUNK TO USE TEMPLATE IF GOT TIME //

vector<string> SynGraph::getFollowsSyn(Clause* c) {
	vector<string> synList = vector<string>();
	FollowsClause* fc = (FollowsClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getFollowsStarSyn(Clause* c) {
	vector<string> synList = vector<string>();
	FollowsStarClause* fc = (FollowsStarClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getParentSyn(Clause* c) {
	vector<string> synList = vector<string>();
	ParentClause* fc = (ParentClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getParentStarSyn(Clause* c) {
	vector<string> synList = vector<string>();
	ParentStarClause* fc = (ParentStarClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getUsesSyn(Clause* c) {
	vector<string> synList = vector<string>();
	UsesClause* fc = (UsesClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getModifiesSyn(Clause* c) {
	vector<string> synList = vector<string>();
	ModifiesClause* fc = (ModifiesClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getCallsSyn(Clause* c) {
	vector<string> synList = vector<string>();
	CallsClause* fc = (CallsClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getCallsStarSyn(Clause* c) {
	vector<string> synList = vector<string>();
	CallsStarClause* fc = (CallsStarClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getCallsStarSyn(Clause* c) {
	vector<string> synList = vector<string>();
	CallsStarClause* fc = (CallsStarClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getCallsStarSyn(Clause* c) {
	vector<string> synList = vector<string>();
	CallsStarClause* fc = (CallsStarClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getNextSyn(Clause* c) {
	vector<string> synList = vector<string>();
	NextClause* fc = (NextClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}

vector<string> SynGraph::getNextStarSyn(Clause* c) {
	vector<string> synList = vector<string>();
	NextStarClause* fc = (NextStarClause*) c;
	bool isNotFixS1 = (fc->getFirstArgFixed() == false);
	bool isNotFixS2 = (fc->getSecondArgFixed() == false);
	bool isNotUnderscoreS1 = (fc->getFirstArgType() != ARG_GENERIC);
	bool isNotUnderscoreS2 = (fc->getSecondArgType() != ARG_GENERIC);
		
	if (isNotFixS1 && isNotUnderscoreS1) {
		// S1 is a synonym
		synList.push_back(fc->getFirstArg());
	} 
		
	if (isNotFixS2 && isNotUnderscoreS2) {
		// S2 is a synonym
		synList.push_back(fc->getSecondArg());

	} else { // is a clause that returns a BOOLEAN
		synList.push_back(boost::lexical_cast<string>(fixedCount));
		fixedCount++;
	}
	return synList;
}


vector<string> SynGraph::getSynonyms(Clause* c) {
	vector<string> synList = vector<string>();
	ClauseType cType = c->getClauseType();
	if (cType == FOLLOWS_) {
		synList = getFollowsSyn(c);
	} 
	if (cType == FOLLOWSSTAR_) {
		synList = getFollowsStarSyn(c);
	} 
	if (cType == PARENT_) {
		synList = getParentSyn(c);
	} 
	if (cType == PARENTSTAR_) {
		synList = getParentStarSyn(c);
	} 
	if (cType == USES_) {
		synList = getUsesSyn(c);
	} 
	if (cType == MODIFIES_) {
		synList = getModifiesSyn(c);
	} 
	if (cType == CALLS_) {
		synList = getCallsSyn(c);
	} 
	if (cType == CALLSSTAR_) {
		synList = getCallsStarSyn(c);
	} 
	if (cType == NEXT_) {
		synList = getNextSyn(c);
	}
	
	if (cType == NEXTSTAR_) {
		//synList = getNextStarSyn(c);
	}
	/*
	if (cType == AFFECTS_) {
		synList = getAffectSyn(c);
	}
	if (cType == AFFECTSSTAR_) {
		synList = getAffectsStarSyn(c);
	}
	
	if (cType == PATTERNASSG_) {
		synList = getPatternAssignSyn(c);
	}
	if (cType == PATTERNIF_) {
		synList = getPatternIfSyn(c);
	}
	if (cType == PATTERNWHILE_) {
		synList = getPatternWhileSyn(c);
	}
	if (cType == WITH_) {
		synList = getWithSyn(c);
	}
	*/
	return synList;
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