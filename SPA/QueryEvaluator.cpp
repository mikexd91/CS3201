#include "QueryEvaluator.h"
#include "Utils.h"
#include "InsertType.h"
#include "boost/foreach.hpp"
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <iostream>
#include <string>

QueryEvaluator::QueryEvaluator(void)
{
	stmtTable = StmtTable::getInstance();
	varTable = VarTable::getInstance();
	constTable = ConstTable::getInstance();
	procTable = ProcTable::getInstance();
	bool validResults = false;
}

QueryEvaluator::~QueryEvaluator(void)
{
}

string QueryEvaluator::boolToString(bool b) {
	if (b) {
		return "true";
	} else {
		return "false";
	}
}

unordered_set<string> QueryEvaluator::printInvalid(Query* q) {
	unordered_set<string> print = unordered_set<string>();
	selectList = q->getSelectList();
	bool isSelectBool = selectList.at(0).getFirst() == "BOOLEAN";
	if (isSelectBool) {
		string strFalse = boolToString(false);
		print.insert(strFalse);
	} 
	return print;
}

unordered_set<string> QueryEvaluator::printValues(Result* finalRes, vector<StringPair> selList){
	unordered_set<string> autotesterPrintouts = unordered_set<string>();
	if (!validResults && selList.at(0).getFirst() == "BOOLEAN"){
		autotesterPrintouts.clear();
		autotesterPrintouts.insert("false");
	} else if (!validResults){
		autotesterPrintouts.clear();
	} else {
		if (selList.size() == 1){
			autotesterPrintouts = printSingleSynValues(*finalRes, selList.at(0));
		} else if (selList.size() > 1){
			autotesterPrintouts = printTupleSynValues(*finalRes, selList);
		}
	}
	if (selList.at(0).getFirst() == "BOOLEAN" && autotesterPrintouts.empty()){
		autotesterPrintouts.insert("false");
	}
	return autotesterPrintouts;
}

unordered_set<string> QueryEvaluator::printSingleSynValues(Result &result, StringPair syn) {
	string synonym = syn.getFirst();
	unordered_set<string> resultSet = unordered_set<string>();
	if (syn.getSecond() == stringconst::ARG_CALL && syn.getAttribute() == stringconst::ATTR_COND_PROCNAME){
		unordered_set<string> callNums = result.getSyn(synonym);
		StmtTable* stable = StmtTable::getInstance();
		BOOST_FOREACH(string n, callNums){
			int num = stoi(n);
			Statement* stmt = stable->getStmtObj(num);
			string cname = stmt->getCalls();
			resultSet.insert(cname);
		}
	} else {
		resultSet = result.getSyn(synonym);
	}
	return resultSet;
}

unordered_set<string> QueryEvaluator::printTupleSynValues(Result &result, vector<StringPair> selectList) {
	unordered_set<string> resultSet;
	vector<size_t> callProcName = vector<size_t>();
	for (size_t i=0; i<selectList.size(); i++){
		StringPair sp = selectList.at(i);
		if (sp.getSecond() == stringconst::ARG_CALL && sp.getAttribute() == stringconst::ATTR_COND_PROCNAME){
			callProcName.push_back(i);
		}
	}

	vector<string> synList;
	for (auto iter = selectList.begin(); iter != selectList.end(); ++iter) {
		synList.push_back(iter->getFirst());
	}
	if (callProcName.empty()){
		unordered_set<vector<string>> resSet = result.getMultiSyn(synList);
		for (auto rowIter = resSet.begin(); rowIter != resSet.end(); ++rowIter) {
			string tuple = "";
			vector<string> tupleList = *rowIter;
			for (auto valueIter = tupleList.begin(); valueIter != tupleList.end(); ++valueIter) {
				if (valueIter != tupleList.end() - 1) {
					tuple.append(*valueIter);
					tuple.append(" ");
				} else {
					tuple.append(*valueIter);
				}
			}
			resultSet.insert(tuple);
		}
	} else {
		StmtTable* stable = StmtTable::getInstance();
		string whitespace = " ";
		unordered_set<vector<string>> resSet = result.getMultiSyn(synList);
		BOOST_FOREACH(vector<string> tupleRes, resSet){
			for (size_t index=0; index < tupleRes.size(); index++){
				if (std::find(callProcName.begin(), callProcName.end(), index) != callProcName.end()){
					//what a long .contains statement
					string callNum = tupleRes.at(index);
					string callName = stable->getStmtObj(stoi(callNum))->getCalls();
					tupleRes.at(index) = callName;
				}
			}
			string tuple = "";
			tuple.append(tupleRes.at(0));
			for (size_t index=1; index<tupleRes.size(); index++){
				tuple.append(whitespace);
				tuple.append(tupleRes.at(index));
			}
			resultSet.insert(tuple);
		}
	}
	return resultSet;
}

void QueryEvaluator::setClauseList(vector<Clause*> clauseList) {
	this->clauseList = clauseList;
}

void QueryEvaluator::setSelectList(vector<StringPair> selectList) {
	this->selectList = selectList;
}

Result* QueryEvaluator::evalOptimisedQuery(Query* query, vector<int>* componentIndices){
	Result* finalResult = new Result();
	vector<Clause*> clauseList = query->getClauseList();
	vector<StringPair> selList = query->getSelectList();
	bool selectTuple = (selList.size() > 1);
	bool productFlag = false;
	bool selectBoolean = false;
	bool singleNotFound = true;
	bool tupleNotFound = true;

	string synonym;
	vector<string> tuple;
	if (selectTuple){
		BOOST_FOREACH(StringPair sp, selList){
			tuple.push_back(sp.getFirst());
		}
	} else {
		synonym = selList.at(0).getFirst();
		selectBoolean = (synonym == "BOOLEAN");
	}

	if (clauseList.empty()){
		bool done = evalNoClause(query, finalResult);
		this->validResults = done;
	} else {
		int start = 0;
		Result* currentRes = new Result();
		BOOST_FOREACH(int end, *componentIndices){
			int limit = start + end;
			this->validResults = evalNumClauses(query, start, limit, currentRes);
			if (!validResults){
				break;
			}
			if (selectTuple){
				if (isTuplePresent(currentRes, tuple)){
					tupleNotFound = false;
					extractTupleSynonymResults(currentRes, finalResult, tuple);
				} else {
					BOOST_FOREACH(string s, tuple){
						if (currentRes->isSynPresent(s)){
							extractSingleSynonymResults(currentRes, finalResult, synonym);
						}
					}
				}
			} else {
				if (selectBoolean){
					singleNotFound = false;
					SingleSynInsert insert = SingleSynInsert();
					insert.setSyn(synonym);
					insert.insertValue("true");
					finalResult->push(insert);
				}
				if (currentRes->isSynPresent(synonym)){
					singleNotFound = false;
					extractSingleSynonymResults(currentRes, finalResult, synonym);
				}
			}
			start = end;
		}
	}
	if (!validResults){
		finalResult = new Result();
	} else {
		if (!selectTuple && singleNotFound){
			evalNoClause(query, finalResult);
		} else if (selectTuple && tupleNotFound){
			evalNoClause(query, finalResult);
		}
	}
	return finalResult;
}

bool QueryEvaluator::evalNumClauses(Query* query, int start, int end, Result* output){
	//eval start inclusive to end non inclusive
	vector<Clause*> clauseList = query->getClauseList();
	for (int i=start; i<end; i++){
		Clause* currentClause = clauseList.at(i);
		bool a = currentClause->evaluate(output);
		if (!a){
			this->validResults = false;
			return false;
		}
	}
	return true;
}

bool QueryEvaluator::evalNoClause(Query* query, Result* output){
	StmtTable* stable = StmtTable::getInstance();
	ConstTable* ctable = ConstTable::getInstance();
	VarTable* vtable = VarTable::getInstance();
	ProcTable* ptable = ProcTable::getInstance();
	vector<StringPair> selectList = query->getSelectList();
	BOOST_FOREACH(StringPair sp, selectList){
		SingleSynInsert insert = SingleSynInsert();
		string synName = sp.getFirst();
		string synonymType = sp.getSecond();
		insert.setSyn(synName);
		if (synonymType == stringconst::ARG_BOOLEAN){
			return false;
		} else if (synonymType == stringconst::ARG_ASSIGN){
			unordered_set<Statement*> allA = stable->getAssgStmts();
			BOOST_FOREACH(Statement* s, allA){
				insert.insertValue(to_string((long long)s->getStmtNum()));
			}
		} else if (synonymType == stringconst::ARG_CALL){
			unordered_set<Statement*> allC = stable->getCallStmts();
			BOOST_FOREACH(Statement* s, allC){
				insert.insertValue(to_string((long long)s->getStmtNum()));
			}
		} else if (synonymType == stringconst::ARG_IF){
			unordered_set<Statement*> allI = stable->getIfStmts();
			BOOST_FOREACH(Statement* s, allI){
				insert.insertValue(to_string((long long)s->getStmtNum()));
			}
		} else if (synonymType == stringconst::ARG_STATEMENT || synonymType == stringconst::ARG_PROGLINE){
			unordered_set<Statement*> allS = stable->getAllStmts();
			BOOST_FOREACH(Statement* s, allS){
				insert.insertValue(to_string((long long)s->getStmtNum()));
			}
		} else if (synonymType == stringconst::ARG_WHILE){
			unordered_set<Statement*> allW = stable->getWhileStmts();
			BOOST_FOREACH(Statement* s, allW){
				insert.insertValue(to_string((long long)s->getStmtNum()));
			}
		} else if (synonymType == stringconst::ARG_CONSTANT){
			if (sp.getAttribute() == stringconst::ATTR_COND_VALUE){
				vector<int> allC = ctable->getAllConstValues();
				BOOST_FOREACH(int i, allC){
					insert.insertValue(to_string((long long)i));
				}
			} else {
				vector<string> allC = ctable->getAllConstNames();
				BOOST_FOREACH(string s, allC){
					insert.insertValue(s);
				}
			}
		} else if (synonymType == stringconst::ARG_PROCEDURE){
			unordered_set<Procedure*> allP = ptable->getAllProcs();
			BOOST_FOREACH(Procedure* p, allP){
				string name = p->getProcName();
				insert.insertValue(name);
			}
		} else if (synonymType == stringconst::ARG_VARIABLE){
			unordered_set<Variable*> allV = vtable->getAllVariables();
			BOOST_FOREACH(Variable* v, allV){
				string name = v->getName();
				insert.insertValue(name);
			}
		}
		output->push(insert);
	}
	return true;
}

void QueryEvaluator::extractSingleSynonymResults(Result* small, Result* big, string synonym){
	unordered_set<string> smallRes = small->getSyn(synonym);
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(synonym);
	BOOST_FOREACH(string s, smallRes){
		insert.insertValue(s);
	}
	big->push(insert);
}

void QueryEvaluator::extractTupleSynonymResults(Result* small, Result* big, vector<string> tuple){
	unordered_set<vector<string>> smallRes = small->getMultiSyn(tuple);
	MultiSynInsert insert = MultiSynInsert();
	insert.setSyns(tuple);
	BOOST_FOREACH(vector<string> v, smallRes){
		insert.insertValues(v);
	}
	big->push(insert);
}

bool QueryEvaluator::isTuplePresent(Result* res, vector<string> tuple){
	BOOST_FOREACH(string s, tuple){
		if (!res->isSynPresent(s)){
			return false;
		}
	}
	return true;
}

void QueryEvaluator::copyResult(Result* small, Result* big, string syn){
	extractSingleSynonymResults(small, big, syn);
}

void QueryEvaluator::mergeMultiResult(Result* small, Result* big, vector<string> syns){
	extractTupleSynonymResults(small, big, syns);
}