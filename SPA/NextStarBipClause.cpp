/*
 * NextStarBipClause.cpp
 *
 *  Created on: 2 Nov, 2015
 *      Author: Leon
 */

#include "NextStarBipClause.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

using namespace stringconst;
using namespace boost;

NextStarBipClause::NextStarBipClause()
	: SuchThatClause(NEXTSTARBIP_) {
	stmtTable = StmtTable::getInstance();
	cfg = CFGbip::getInstance();
	match = false;
}

NextStarBipClause::~NextStarBipClause() {

}

bool NextStarBipClause::isValid(void) {
	bool firstArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_WHILE)
			|| (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_IF)
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN)
			|| (firstArgType == ARG_CALL);
	bool secondArg = (firstArgType == ARG_GENERIC) || (firstArgType == ARG_WHILE) 
			|| (firstArgType == ARG_PROGLINE) || (firstArgType == ARG_IF) 
			|| (firstArgType == ARG_STATEMENT) || (firstArgType == ARG_ASSIGN)
			|| (firstArgType == ARG_CALL);

	return firstArg && secondArg;
}

// NextBip*(4, 2)
bool NextStarBipClause::evaluateS1FixedS2Fixed(string s1, string s2) {
	Statement* stmt1 = stmtTable->getStmtObj(atoi(s1.c_str()));
	match = false;
	
	unordered_set<int> next = stmt1->getNextBip();
	BOOST_FOREACH(auto s, next) {
		Statement* child = stmtTable->getStmtObj(s);
		vector<string> visited;
		stack<int> entrance;

		if(stmt1->getType() == CALL_STMT_) {
			GNode* ref = stmt1->getGBipNodeRef();
			ProcGNode* procChild = (ProcGNode*) ref->getChildren().at(0);
			int indexOfEntry = getNodePosition(procChild->getParents(), ref);
			entrance.push(indexOfEntry);
		}

		dfsFind(child, s2, visited, entrance);
	}

	return match;
}

// NextBip*(_, _)
bool NextStarBipClause::evaluateS1GenericS2Generic() {
	unordered_set<Statement*> stmts = stmtTable->getAllStmts();

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getNextBip().empty()) {
			return true;
		}
	}

	return false;
}

// NextBip*(_, 2)
bool NextStarBipClause::evaluateS1GenericS2Fixed(string s2) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s2.c_str()));

	if(!stmt->getPrevBip().empty()) {
		return true;
	}

	return false;
}

// NextBip*(2, _)
bool NextStarBipClause::evaluateS1FixedS2Generic(string s1) {
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	if(!stmt->getNextBip().empty()) {
		return true;
	}

	return false;
}

// NextBip*(3, s) || NextBip*(3, a) || NextBip*(3, c) || NextBip*(3, if) || NextBip*(3, w)
unordered_set<string> NextStarBipClause::getAllS2WithS1Fixed(string s1) {
	results.clear();
	Statement* stmt = stmtTable->getStmtObj(atoi(s1.c_str()));

	unordered_set<int> stmtNext = stmt->getNextBip();

	BOOST_FOREACH(auto s, stmtNext) {
		Statement* child = stmtTable->getStmtObj(s);
		vector<string> visited;
		stack<int> entrance;

		if(stmt->getType() == CALL_STMT_) {
			GNode* ref = stmt->getGBipNodeRef();
			ProcGNode* procChild = (ProcGNode*) ref->getChildren().at(0);
			int indexOfEntry = getNodePosition(procChild->getParents(), ref);
			entrance.push(indexOfEntry);
		}

		dfsFindNext(child, visited, entrance, secondArgType);
	}

	return results;
}

// NextBip*(_, s) || NextBip*(_, a) || NextBip*(_, c) || NextBip*(_, if) || NextBip*(_, w)
unordered_set<string> NextStarBipClause::getAllS2() {
	results.clear();
	unordered_set<Statement*> stmts = getNeededStmts(secondArgType);

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getPrevBip().empty()) {
			results.insert(lexical_cast<string>(s->getStmtNum()));
		}
	}

	return results;
}

// NextBip*(s, 3) || NextBip*(a, 3) || NextBip*(c, 3) || NextBip*(if, 3) || NextBip*(w, 3)
unordered_set<string> NextStarBipClause::getAllS1WithS2Fixed(string s2) {
	results.clear();
	return results;
}

// NextBip*(s, _) || NextBip*(a, _) || NextBip*(c, _) || NextBip*(if, _) || NextBip*(w, 3)
unordered_set<string> NextStarBipClause::getAllS1() {
	results.clear();
	unordered_set<Statement*> stmts = getNeededStmts(firstArgType);

	BOOST_FOREACH(auto s, stmts) {
		if(!s->getNextBip().empty()) {
			results.insert(lexical_cast<string>(s->getStmtNum()));
		}
	}

	return results;
}

unordered_set<vector<string>> NextStarBipClause::getAllS1AndS2() {
	resultsPair.clear();
	return resultsPair;
}

void NextStarBipClause::dfsFind(Statement* stmt, string str, vector<string> visited, stack<int> entrance) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());
	GNode* bipNode = stmt->getGBipNodeRef();

	if(match) {
		return;
	}

	if(currStmt == str) {
		match = true;
		return;
	}

	if(!contains(visited, currStmt)) {
		visited.push_back(currStmt);

		if(bipNode->getNodeType() == CALL_) {
			ProcGNode* procChild = (ProcGNode*) bipNode->getChildren().at(0);
			int indexOfEntrance = getNodePosition(procChild->getParents(), bipNode);
			entrance.push(indexOfEntrance);
			unordered_set<int> nextSet = stmt->getNextBip();
			BOOST_FOREACH(auto next, nextSet) {
				Statement* nextStmt = stmtTable->getStmtObj(next);
				dfsFind(nextStmt, str, visited, entrance);
			}
		} else if(bipNode->getNodeType() == WHILE_) {
			if(entrance.empty()) {
				unordered_set<int> nextSet = stmt->getNextBip();
				BOOST_FOREACH(auto next, nextSet) {
					Statement* nextStmt = stmtTable->getStmtObj(next);
					dfsFind(nextStmt, str, visited, entrance);
				}
			} else /* There is a flow that i need to follow back */ {
				//end
				if(getChildNodeType(bipNode) == 1) {
					dfsFind(stmtTable->getStmtObj(bipNode->getChildren().at(0)->getStartStmt()), str, visited, entrance);
					GNode* exit = bipNode->getChildren().at(1);
					bool flag = true;

					while(flag) {
						int indexOfExit = entrance.top();
						entrance.pop();
						exit = exit->getChildren().at(indexOfExit);

						if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
							dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
							flag = false;
							break;
						}

						// means i need to explore all children of this exit
						if(exit->getNodeType() == END_ && entrance.empty()) {
							dfsFind(exit, str, visited, entrance);
							flag = false;
							break;
						}

						if(exit->getNodeType() == END_ && !entrance.empty()) {
							continue;
						}

						if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
							exit = traverseDummyToGetAnything(exit);
							if(exit->getNodeType() == END_) {
								dfsFind(exit, str, visited, entrance);
							} else {
								dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
							}
							flag = false;
							break;
						}

						if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
							exit = traverseDummyToGetAnything(exit);
							if(exit->getNodeType() == END_) {
								continue;
							} else {
								dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
								flag = false;
								break;
							}
						}
					}
				}

				//dummy
				if(getChildNodeType(bipNode) == 2) {
					dfsFind(stmtTable->getStmtObj(bipNode->getChildren().at(0)->getStartStmt()), str, visited, entrance);
					
					GNode* last = traverseDummyToGetAnything(bipNode->getChildren().at(1));
					
					if(last->getNodeType() != END_) {
						dfsFind(stmtTable->getStmtObj(last->getStartStmt()), str, visited, entrance);
					}

					if(last->getNodeType() == END_) {
						bool flag = true;
						GNode* exit = last;

						while(flag) {
							exit = exit->getChildren().at(entrance.top());
							entrance.pop();

							if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
								dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
								flag = false;
								break;
							}

							// means i need to explore all children of this exit
							if(exit->getNodeType() == END_ && entrance.empty()) {
								dfsFind(exit, str, visited, entrance);
								flag = false;
								break;
							}

							if(exit->getNodeType() == END_ && !entrance.empty()) {
								continue;
							}

							if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									dfsFind(exit, str, visited, entrance);
								} else {
									dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
								}
								flag = false;
								break;
							}

							if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									continue;
								} else {
									dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
									flag = false;
									break;
								}
							}
						}
					}
				}
				
				//others
				if(getChildNodeType(bipNode) == 3) {
					unordered_set<int> nextSet = stmt->getNextBip();
					BOOST_FOREACH(auto next, nextSet) {
						Statement* nextStmt = stmtTable->getStmtObj(next);
						dfsFind(nextStmt, str, visited, entrance);
					}
				}
			}
		} else /*not while or call*/ {
			if(stmt->getStmtNum() == bipNode->getEndStmt()) {
				if(entrance.empty()) {
					unordered_set<int> nextSet = stmt->getNextBip();
					BOOST_FOREACH(auto next, nextSet) {
						Statement* nextStmt = stmtTable->getStmtObj(next);
						dfsFind(nextStmt, str, visited, entrance);
					}
				} else /* There is a flow that i need to follow back */ {
					//end
					if(getChildNodeType(bipNode) == 1) {
						GNode* exit = bipNode->getChildren().at(0);
						bool flag = true;

						while(flag) {
							exit = exit->getChildren().at(entrance.top());
							entrance.pop();

							if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
								dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
								flag = false;
								break;
							}

							if(exit->getNodeType() == END_ && entrance.empty()) {
								dfsFind(exit, str, visited, entrance);
								flag = false;
								break;
							}

							if(exit->getNodeType() == END_ && !entrance.empty()) {
								continue;
							}

							if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									dfsFind(exit, str, visited, entrance);
								} else {
									dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
								}
								flag = false;
								break;
							}

							if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									continue;
								} else {
									dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
									flag = false;
									break;
								}
							}
						}
					}

					//dummy
					if(getChildNodeType(bipNode) == 2) {
						GNode* last = traverseDummyToGetAnything(bipNode->getChildren().at(0));
						
						if(last->getNodeType() != END_) {
							dfsFind(stmtTable->getStmtObj(last->getStartStmt()), str, visited, entrance);
						}

						if(last->getNodeType() == END_) {
							bool flag = true;
							GNode* exit = last;

							while(flag) {
								exit = exit->getChildren().at(entrance.top());
								entrance.pop();

								if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
									dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
									flag = false;
									break;
								}

								// means i need to explore all children of this exit
								if(exit->getNodeType() == END_ && entrance.empty()) {
									dfsFind(exit, str, visited, entrance);
									flag = false;
									break;
								}

								if(exit->getNodeType() == END_ && !entrance.empty()) {
									continue;
								}

								if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
									exit = traverseDummyToGetAnything(exit);
									if(exit->getNodeType() == END_) {
										dfsFind(exit, str, visited, entrance);
									} else {
										dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
									}
									flag = false;
									break;
								}

								if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
									exit = traverseDummyToGetAnything(exit);
									if(exit->getNodeType() == END_) {
										continue;
									} else {
										dfsFind(stmtTable->getStmtObj(exit->getStartStmt()), str, visited, entrance);
										flag = false;
										break;
									}
								}
							}
						}
					}
				
					//others
					if(getChildNodeType(bipNode) == 3) {
						unordered_set<int> nextSet = stmt->getNextBip();
						BOOST_FOREACH(auto next, nextSet) {
							Statement* nextStmt = stmtTable->getStmtObj(next);
							dfsFind(nextStmt, str, visited, entrance);
						}
					}
				}
			} else {
				unordered_set<int> nextSet = stmt->getNextBip();
				BOOST_FOREACH(auto next, nextSet) {
					Statement* nextStmt = stmtTable->getStmtObj(next);
					dfsFind(nextStmt, str, visited, entrance);
				}
			}
		}
	}
}

void NextStarBipClause::dfsFind(GNode* node, string str, vector<string> visited, stack<int> entrance) {
	vector<GNode*> children = node->getChildren();

	BOOST_FOREACH(auto child, children) {
		if(child->getNodeType() == END_) {
			dfsFind(child, str, visited, entrance);
		}

		if(child->getNodeType() == DUMMY_) {
			GNode* ultimateChild = traverseDummyToGetAnything(child);
			if(ultimateChild->getNodeType() == END_) {
				dfsFind(ultimateChild, str, visited, entrance);
			} else {
				dfsFind(stmtTable->getStmtObj(ultimateChild->getStartStmt()), str, visited, entrance);
			}
		}

		if(child->getNodeType() != END_ && child->getNodeType() != DUMMY_) {
			dfsFind(stmtTable->getStmtObj(child->getStartStmt()), str, visited, entrance);
		}
	}
}

void NextStarBipClause::dfsFindNext(Statement* stmt, vector<string> visited, stack<int> entrance, string type) {
	string currStmt = lexical_cast<string>(stmt->getStmtNum());
	GNode* bipNode = stmt->getGBipNodeRef();

	if(!contains(visited, currStmt)) {
		if(isNeededArgType(type, stmt->getStmtNum())) {
			results.insert(currStmt);
		}

		visited.push_back(currStmt);

		if(bipNode->getNodeType() == CALL_) { 
			ProcGNode* procChild = (ProcGNode*) bipNode->getChildren().at(0);
			int indexOfEntrance = getNodePosition(procChild->getParents(), bipNode);
			entrance.push(indexOfEntrance);
			unordered_set<int> nextSet = stmt->getNextBip();
			BOOST_FOREACH(auto next, nextSet) {
				Statement* nextStmt = stmtTable->getStmtObj(next);
				dfsFindNext(nextStmt, visited, entrance, type);
			}
		} else if(bipNode->getNodeType() == WHILE_) {
			if(entrance.empty()) {
				unordered_set<int> nextSet = stmt->getNextBip();
				BOOST_FOREACH(auto next, nextSet) {
					Statement* nextStmt = stmtTable->getStmtObj(next);
					dfsFindNext(nextStmt, visited, entrance, type);
				}
			} else /* There is a flow that i need to follow back */ {
				//end
				if(getChildNodeType(bipNode) == 1) {
					dfsFindNext(stmtTable->getStmtObj(bipNode->getChildren().at(0)->getStartStmt()), visited, entrance, type);
					GNode* exit = bipNode->getChildren().at(1);
					bool flag = true;

					while(flag) {
						exit = exit->getChildren().at(entrance.top());
						entrance.pop();

						if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
							dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
							flag = false;
							break;
						}
						if(exit->getNodeType() == END_ && entrance.empty()) {
							dfsFindNext(exit, visited, entrance, type);
							flag = false;
							break;
						}
						if(exit->getNodeType() == END_ && !entrance.empty()) {
							continue;
						}
						if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
							exit = traverseDummyToGetAnything(exit);
							if(exit->getNodeType() == END_) {
								dfsFindNext(exit, visited, entrance, type);
							} else {
								dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
							}
							flag = false;
							break;
						}
						if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
							exit = traverseDummyToGetAnything(exit);
							if(exit->getNodeType() == END_) {
								continue;
							} else {
								dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
								flag = false;
								break;
							}
						}
					}
				}

				//dummy
				if(getChildNodeType(bipNode) == 2) {
					dfsFindNext(stmtTable->getStmtObj(bipNode->getChildren().at(0)->getStartStmt()), visited, entrance, type);
					GNode* last = traverseDummyToGetAnything(bipNode->getChildren().at(1));

					if(last->getNodeType() != END_) {
						dfsFindNext(stmtTable->getStmtObj(last->getStartStmt()), visited, entrance, type);
					}
					if(last->getNodeType() == END_) {
						bool flag = true;
						GNode* exit = last;

						while(flag) {
							exit = exit->getChildren().at(entrance.top());
							entrance.pop();

							if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
								dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
								flag = false;
								break;
							}
							// means i need to explore all children of this exit
							if(exit->getNodeType() == END_ && entrance.empty()) {
								dfsFindNext(exit, visited, entrance, type);
								flag = false;
								break;
							}
							if(exit->getNodeType() == END_ && !entrance.empty()) {
								continue;
							}
							if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									dfsFindNext(exit, visited, entrance, type);
								} else {
									dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
								}
								flag = false;
								break;
							}
							if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									continue;
								} else {
									dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
									flag = false;
									break;
								}
							}
						}
					}
				}

				//not dummy and end
				if(getChildNodeType(bipNode) == 3) {
					unordered_set<int> nextSet = stmt->getNextBip();
					BOOST_FOREACH(auto next, nextSet) {
						Statement* nextStmt = stmtTable->getStmtObj(next);
						dfsFindNext(nextStmt, visited, entrance, type);
					}
				}
			}
		} else /*not while or call*/ {
			if(stmt->getStmtNum() == bipNode->getEndStmt()) {
				if(entrance.empty()) {
					unordered_set<int> nextSet = stmt->getNextBip();
					BOOST_FOREACH(auto next, nextSet) {
						Statement* nextStmt = stmtTable->getStmtObj(next);
						dfsFindNext(nextStmt, visited, entrance, type);
					}
				} else /* There is a flow that i need to follow back */ {
					//end
					if(getChildNodeType(bipNode) == 1) {
						GNode* exit = bipNode->getChildren().at(0);
						bool flag = true;

						while(flag) {
							exit = exit->getChildren().at(entrance.top());
							entrance.pop();

							if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
								dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
								flag = false;
								break;
							}
							if(exit->getNodeType() == END_ && entrance.empty()) {
								dfsFindNext(exit, visited, entrance, type);
								flag = false;
								break;
							}
							if(exit->getNodeType() == END_ && !entrance.empty()) {
								continue;
							}
							if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									dfsFindNext(exit, visited, entrance, type);
								} else {
									dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
								}
								flag = false;
								break;
							}
							if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
								exit = traverseDummyToGetAnything(exit);
								if(exit->getNodeType() == END_) {
									continue;
								} else{
									dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
									flag = false;
									break;
								}
							}
						}
					}

					//dummy
					if(getChildNodeType(bipNode) == 2) {
						GNode* last = traverseDummyToGetAnything(bipNode->getChildren().at(0));
						
						if(last->getNodeType() != END_) {
							dfsFindNext(stmtTable->getStmtObj(last->getStartStmt()), visited, entrance, type);
						}

						if(last->getNodeType() == END_) {
							bool flag = true;
							GNode* exit = last;

							while(flag) {
								exit = exit->getChildren().at(entrance.top());
								entrance.pop();

								if(exit->getNodeType() != END_ && exit->getNodeType() != DUMMY_) {
									dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
									flag = false;
									break;
								}

								// means i need to explore all children of this exit
								if(exit->getNodeType() == END_ && entrance.empty()) {
									dfsFindNext(exit, visited, entrance, type);
									flag = false;
									break;
								}

								if(exit->getNodeType() == END_ && !entrance.empty()) {
									continue;
								}

								if(exit->getNodeType() == DUMMY_ && entrance.empty()) {
									exit = traverseDummyToGetAnything(exit);
									if(exit->getNodeType() == END_) {
										dfsFindNext(exit, visited, entrance, type);
									} else {
										dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
									}
									flag = false;
									break;
								}

								if(exit->getNodeType() == DUMMY_ && !entrance.empty()) {
									exit = traverseDummyToGetAnything(exit);
									if(exit->getNodeType() == END_) {
										continue;
									} else {
										dfsFindNext(stmtTable->getStmtObj(exit->getStartStmt()), visited, entrance, type);
										flag = false;
										break;
									}
								}
							}
						}
					}

					//others
					if(getChildNodeType(bipNode) == 3) {
						unordered_set<int> nextSet = stmt->getNextBip();
						BOOST_FOREACH(auto next, nextSet) {
							Statement* nextStmt = stmtTable->getStmtObj(next);
							dfsFindNext(nextStmt, visited, entrance, type);
						}
					}
				}
			} else {
				unordered_set<int> nextSet = stmt->getNextBip();
				BOOST_FOREACH(auto next, nextSet) {
					Statement* nextStmt = stmtTable->getStmtObj(next);
					dfsFindNext(nextStmt, visited, entrance, type);
				}
			}
		}
	}
}

void NextStarBipClause::dfsFindNext(GNode* node, vector<string> visited, stack<int> entrance, string type) {
	vector<GNode*> children = node->getChildren();

	BOOST_FOREACH(auto child, children) {
		if(child->getNodeType() == END_) {
			dfsFindNext(child, visited, entrance, type);
		}

		if(child->getNodeType() == DUMMY_) {
			GNode* ultimateChild = traverseDummyToGetAnything(child);
			if(ultimateChild->getNodeType() == END_) {
				dfsFindNext(ultimateChild, visited, entrance, type);
			} else {
				dfsFindNext(stmtTable->getStmtObj(ultimateChild->getStartStmt()), visited, entrance, type);
			}
		}

		if(child->getNodeType() != END_ && child->getNodeType() != DUMMY_) { 
			dfsFindNext(stmtTable->getStmtObj(child->getStartStmt()), visited, entrance, type);
		}
	}
}

GNode* NextStarBipClause::traverseDummyToGetAnything(GNode* ref) {
	GNode* child = ref;

	while(true) {
		child = child->getChildren().at(0);

		if(child->getNodeType() == DUMMY_) {
			continue;
		} else {
			break;
		}
	}

	return child;
}

bool NextStarBipClause::contains(vector<string> arr, string item) {
	if(find(arr.begin(), arr.end(), item) != arr.end()) {
		return true;
	}

	return false;
}

int NextStarBipClause::getNodePosition(vector<GNode*> arr, GNode* item) {
	int pos = find(arr.begin(), arr.end(), item) - arr.begin();

	return pos;
}

unordered_set<Statement*> NextStarBipClause::getNeededStmts(string type) {
	unordered_set<Statement*> stmts;
	
	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		return stmtTable->getAllStmts();
	}

	if(type == ARG_IF) {
		return stmtTable->getIfStmts();
	}

	if(type == ARG_WHILE) {
		return stmtTable->getWhileStmts();
	}

	if(type == ARG_CALL) {
		return stmtTable->getCallStmts();
	}

	if(type == ARG_ASSIGN) {
		return stmtTable->getAssgStmts();
	}

	return stmts;
}

bool NextStarBipClause::isNeededArgType(string type, int stmtNum) {
	Statement* stmt = stmtTable->getStmtObj(stmtNum);

	if(type == ARG_STATEMENT || type == ARG_PROGLINE) {
		return true;
	}
	
	if(stmt->getType() == ASSIGN_STMT_ && type == ARG_ASSIGN) {
		return true;
	}

	if(stmt->getType() == WHILE_STMT_ && type == ARG_WHILE) {
		return true;
	}

	if(stmt->getType() == CALL_STMT_ && type == ARG_CALL) {
		return true;
	}

	if(stmt->getType() == IF_STMT_ && type == ARG_IF) {
		return true;
	}

	return false;
}

int NextStarBipClause::getChildNodeType(GNode* ref) {
	switch(ref->getNodeType()) {
		case WHILE_: {
			switch(ref->getChildren().at(1)->getNodeType()) {
				case END_:
					return 1;
				case DUMMY_:
					return 2;
				default:
					return 3;
			}
			break;
		}
		default: {
			switch(ref->getChildren().at(0)->getNodeType()) {
				case END_:
					return 1;
				case DUMMY_:
					return 2;
				default:
					return 3;
			}
			break;
		}
	}
}