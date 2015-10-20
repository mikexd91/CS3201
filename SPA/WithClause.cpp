#include "WithClause.h"
#include "Utils.h"
#include "ConstTable.h"
#include "Constant.h"
#include "VarTable.h"
#include "Variable.h"
#include "StmtTable.h"
#include "Statement.h"
#include "ProcTable.h"
#include "Procedure.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

#include <iostream>
using namespace std;
using namespace boost;

WithClause::WithClause(ClauseType) {
}

WithClause::~WithClause(void)
{
}

bool WithClause::isValid(void){
	WithClauseRef leftSideRef = this->getLeftRef();
	WithClauseRef rightSideRef = this->getLeftRef();
	if (leftSideRef.getRefType() == SYNONYM_){
		if (leftSideRef.getEntityType() != stringconst::ARG_PROGLINE ){
			return false;
		}
	} else if (leftSideRef.getRefType() == ATTRREF_){
		if (leftSideRef.getAttrType() == PROCNAME_){
			if (leftSideRef.getEntityType() != stringconst::ARG_PROCEDURE || leftSideRef.getEntityType() != stringconst::ARG_CALL){
				return false;
			} 
		} else if (leftSideRef.getAttrType() == VARNAME_){
			if (leftSideRef.getEntityType() != stringconst::ARG_VARIABLE){
				return false;
			} 
		} else if (leftSideRef.getAttrType() == CONSTVALUE_){
			if (leftSideRef.getEntityType() != stringconst::ARG_CONSTANT){
				return false;
			} 
		} else if (leftSideRef.getAttrType() == STMTNUM_){
			if (leftSideRef.getEntityType() != stringconst::ARG_STATEMENT
				&& leftSideRef.getEntityType() != stringconst::ARG_ASSIGN
				&& leftSideRef.getEntityType() != stringconst::ARG_WHILE
				&& leftSideRef.getEntityType() != stringconst::ARG_IF
				&& leftSideRef.getEntityType() != stringconst::ARG_CALL){
				return false;
			} 
		}
	}

	if (rightSideRef.getRefType() == SYNONYM_){
		if (rightSideRef.getEntityType() != stringconst::ARG_PROGLINE){
			return false;
		}
	} else if (rightSideRef.getRefType() == ATTRREF_){
		if (rightSideRef.getAttrType() == PROCNAME_){
			if (rightSideRef.getEntityType() != stringconst::ARG_PROCEDURE){
				return false;
			} 
		} else if (rightSideRef.getAttrType() == VARNAME_){
			if (rightSideRef.getEntityType() != stringconst::ARG_VARIABLE){
				return false;
			} 
		} else if (rightSideRef.getAttrType() == CONSTVALUE_){
			if (rightSideRef.getEntityType() != stringconst::ARG_CONSTANT){
				return false;
			} 
		} else if (rightSideRef.getAttrType() == STMTNUM_){
			if (rightSideRef.getEntityType() != stringconst::ARG_STATEMENT
				&& leftSideRef.getEntityType() != stringconst::ARG_ASSIGN
				&& leftSideRef.getEntityType() != stringconst::ARG_WHILE
				&& leftSideRef.getEntityType() != stringconst::ARG_IF){
				return false;
			} 
		}
	}
	 
	if (leftSideRef.getRefType() == ATTRREF_ && rightSideRef.getAttrType() == ATTRREF_){
		if (leftSideRef.getEntityType() != rightSideRef.getEntityType()){
			return false;
		} else if (leftSideRef.getAttrType() != rightSideRef.getAttrType()){
			return false;
		}
	}

	bool returnTypesMatch = (leftSideRef.getReturnType() == rightSideRef.getReturnType());
	return returnTypesMatch;
}

WithClauseRef WithClause::getLeftRef()
{
	return this->leftRef;
}

WithClauseRef WithClause::getRightRef()
{
	return this->rightRef;
}

void WithClause::setLeftRef(WithClauseRef leftRef)
{
	this->leftRef = leftRef;
}

void WithClause::setRightRef(WithClauseRef rightRef)
{
	this->rightRef = rightRef;
}

bool WithClause::evaluate(Result* res){
	
	if(!isValid()){
		cout << "failed valid";
		return false;
	}

	WithClauseRef leftEntityRef = this->getLeftRef();
	WithClauseRef rightEntityRef = this->getRightRef();
	
	string leftEntity = leftEntityRef.getEntity();
	string rightEntity = rightEntityRef.getEntity();
	string leftEntityType = leftEntityRef.getEntityType();
	string rightEntityType = rightEntityRef.getEntityType();
	AttrType leftEntityAttr = leftEntityRef.getAttrType();
	AttrType rightEntityAttr = rightEntityRef.getAttrType();
	RefType leftEntityRefType = leftEntityRef.getRefType();
	RefType rightEntityRefType = rightEntityRef.getRefType();

	if (leftEntityRefType == rightEntityRefType){
		if (leftEntityRefType == IDENT_ || leftEntityRefType == INTEGER_){
			return leftEntity == rightEntity;
		} else if (leftEntityRefType == SYNONYM_){
			return evalSynSyn(leftEntityRef, rightEntityRef, res);
			return false;
		} else {
			if (leftEntityAttr == rightEntityAttr){
				if (leftEntityAttr == PROCNAME_){
					//needs testing
					return evalPNamePName(leftEntityRef, rightEntityRef, res);
				} else if (leftEntityAttr == VARNAME_){
					//WIP
					return evalVNameVName(leftEntityRef, rightEntityRef, res);
				} else if (leftEntityAttr == CONSTVALUE_){
					//WIP
					return evalValueValue(leftEntityRef, rightEntityRef, res);
				} else if (leftEntityAttr == STMTNUM_){
					//needs testing
					return evalSynSyn(leftEntityRef, rightEntityRef, res);
				}
			} else {
				if (leftEntityAttr == PROCNAME_ && rightEntityAttr == VARNAME_){
					//WIP
					return evalPNameVName(leftEntityRef, rightEntityRef, res);
				} else if (rightEntityAttr == PROCNAME_ && leftEntityAttr == VARNAME_){
					//WIP
					return evalPNameVName(rightEntityRef, leftEntityRef, res);
				} else if (leftEntityAttr == STMTNUM_ && rightEntityAttr == CONSTVALUE_){
					//WIP
					return evalStmtValue(leftEntityRef, rightEntityRef, res);
				} else if (rightEntityAttr == STMTNUM_ && leftEntityAttr == CONSTVALUE_){
					//WIP
					return evalStmtValue(rightEntityRef, leftEntityRef, res);
				}
			}
			return false;
		}
	} else {
		// add switch case
		if (leftEntityAttr == CONSTVALUE_ && rightEntityRefType == INTEGER_){
			//c.value = 1
			return evalValueInt(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == INTEGER_ && rightEntityAttr == CONSTVALUE_){
			//1 = c.value
			return evalValueInt(rightEntityRef, leftEntityRef, res);

		} else if (leftEntityRefType == SYNONYM_ && rightEntityRefType == INTEGER_){
			//pline = 1
			return evalSynInt(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == INTEGER_ && rightEntityRefType == SYNONYM_){
			//1 = pline
			return evalSynInt(rightEntityRef, leftEntityRef, res);

		} else if (leftEntityAttr == STMTNUM_ && rightEntityRefType == INTEGER_){
			//stmt# = 1
			return evalStmtInt(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == INTEGER_ && rightEntityAttr == STMTNUM_){
			//1 = stmt#
			return evalStmtInt(rightEntityRef, leftEntityRef, res);

		} else if (leftEntityAttr == PROCNAME_ && rightEntityRefType == IDENT_){

			if (leftEntityType == stringconst::ARG_PROCEDURE){
				//proc.name = str
				return evalPNameString(leftEntityRef, rightEntityRef, res);
			} else if (leftEntityType == stringconst::ARG_CALL){
				//call.name = str (needs testing)
				return evalCallString(leftEntityRef, rightEntityRef, res);
			}

		} else if (leftEntityRefType == IDENT_ && rightEntityAttr == PROCNAME_){
			
			if (rightEntityType == stringconst::ARG_PROCEDURE){
				//str = proc.name
				return evalPNameString(rightEntityRef, leftEntityRef, res);
			} else if (rightEntityType == stringconst::ARG_CALL){
				//str = call.name (needs testing)
				return evalCallString(rightEntityRef, leftEntityRef, res);
			}

		} else if (leftEntityAttr == VARNAME_ && rightEntityRefType == IDENT_){
			//var.name = str
			return evalVNameString(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == IDENT_ && rightEntityAttr == VARNAME_){
			//str = var.name
			return evalVNameString(rightEntityRef, leftEntityRef, res);

		} else if (leftEntityRefType == SYNONYM_ && rightEntityAttr == CONSTVALUE_){
			//p = c.v
			return evalSynValue(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == CONSTVALUE_ && rightEntityAttr == SYNONYM_){
			//c.v = p
			return evalSynValue(rightEntityRef, leftEntityRef, res);

		} else if (leftEntityRefType == SYNONYM_ && rightEntityAttr == STMTNUM_){
			//p = s.#
			return evalSynStmt(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == STMTNUM_ && rightEntityAttr == SYNONYM_){
			//s.# = p
			return evalSynStmt(rightEntityRef, leftEntityRef, res);
		}
		return false;
		// false for the rest of the cases
	}
}

bool WithClause::evalSynSyn(WithClauseRef syn1, WithClauseRef syn2, Result* result){
	//two proglines
	if (syn1.getEntity() == syn2.getEntity()){
		//put a = a in reults
		MultiSynInsert insert = MultiSynInsert();
		std::string s1e = syn1.getEntity();
		std::string s2e = syn2.getEntity();
		vector<string> syns = vector<string>();
		syns.push_back(s1e);
		syns.push_back(s2e);
		insert.setSyns(syns);
		StmtTable* stable = StmtTable::getInstance();
		unordered_set<Statement*> allS = stable->getAllStmts();
		BOOST_FOREACH(Statement* s, allS){
			string num = std::to_string((long long)s->getStmtNum());
			vector<string> newRes = vector<string>();
			newRes.push_back(num);
			newRes.push_back(num);
			insert.insertValues(newRes);
		}
		result->push(insert);
		return true;
	} else {
		if (result->isSynPresent(syn1.getEntity())){
			if (result->isSynPresent(syn2.getEntity())){
				//both in result
				MultiSynInsert insert = MultiSynInsert();
				string s1e = syn1.getEntity();
				string s2e = syn2.getEntity();
				vector<string> syns = vector<string>();
				syns.push_back(s1e);
				syns.push_back(s2e);
				insert.setSyns(syns);
				unordered_set<vector<string>> resultPairs = result->getMultiSyn(syns);
				bool found;
				BOOST_FOREACH(vector<string> pair, resultPairs){
					if (pair.at(0) == pair.at(1)){
						string p1 = pair.at(0);
						string p2 = pair.at(1);
						vector<string> newPair = vector<string>();
						newPair.push_back(p1);
						newPair.push_back(p2);
						insert.insertValues(newPair);
						found = true;
					}
					if (found){
						result->push(insert);
					}
					return found;
				}
			} else {
				//only left in result
				MultiSynInsert insert = MultiSynInsert();
				string s1e = syn1.getEntity();
				string s2e = syn2.getEntity();
				vector<string> syns = vector<string>();
				syns.push_back(s1e);
				syns.push_back(s2e);
				insert.setSyns(syns);
				unordered_set<string> syn1res = result->getSyn(syn1.getEntity());
				bool found;
				StmtTable* stable = StmtTable::getInstance();
				unordered_set<Statement*> allS = stable->getAllStmts();
				BOOST_FOREACH(string syn1Val, syn1res){
					BOOST_FOREACH(Statement* s, allS){
						if (s->getStmtNum() == stoi(syn1Val)){
							vector<string> newRes = vector<string>();
							newRes.push_back(syn1Val);
							newRes.push_back(syn1Val);
							insert.insertValues(newRes);
							found = true;
						}
					}
				}
				if (found){
					result->push(insert);
				}
				return found;
			}
		} else if (result->isSynPresent(syn2.getEntity())){
			//only right syn
			MultiSynInsert insert = MultiSynInsert();
			string s1e = syn1.getEntity();
			string s2e = syn2.getEntity();
			vector<string> syns = vector<string>();
			syns.push_back(s1e);
			syns.push_back(s2e);
			insert.setSyns(syns);
			unordered_set<string> syn2res = result->getSyn(syn2.getEntity());
			bool found;
			StmtTable* stable = StmtTable::getInstance();
			unordered_set<Statement*> allS = stable->getAllStmts();
			BOOST_FOREACH(string syn2Val, syn2res){
				BOOST_FOREACH(Statement* s, allS){
					if (s->getStmtNum() == stoi(syn2Val)){
						vector<string> newRes = vector<string>();
						newRes.push_back(syn2Val);
						newRes.push_back(syn2Val);
						insert.insertValues(newRes);
						found = true;
					}
				}
			}
			if (found){
				result->push(insert);
			}
			return found;
		} else {
			return true;
		}
	}
	return false;
}

bool WithClause::evalValueInt(WithClauseRef constEnt, WithClauseRef intEnt, Result* result){
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(constEnt.getEntity());
	if (result->isSynPresent(constEnt.getEntity())){
		unordered_set<string> synValues = result->getSyn(constEnt.getEntity());
		bool found = false;
		BOOST_FOREACH(string s, synValues){
			if (s == intEnt.getEntity()){
				found = true;
				insert.insertValue(s);
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	} else {
		ConstTable* ctable = ConstTable::getInstance();
		vector<int> allC = ctable->getAllConstValues();
		bool found = false;
		BOOST_FOREACH(int i, allC){
			if (i == stoi(intEnt.getEntity())){
				found = true;
				insert.insertValue(intEnt.getEntity());
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	}
	return false;
}

bool WithClause::evalSynInt(WithClauseRef progEnt, WithClauseRef intEnt, Result* result){
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(progEnt.getEntity());
	if (result->isSynPresent(progEnt.getEntity())){
		unordered_set<string> synValues = result->getSyn(progEnt.getEntity());
		bool found = false;
		BOOST_FOREACH(string s, synValues){
			if (s == intEnt.getEntity()){
				found = true;
				insert.insertValue(s);
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	} else {
		StmtTable* stable = StmtTable::getInstance();
		unordered_set<Statement*> allS = stable->getAllStmts();
		bool found = false;
		BOOST_FOREACH(Statement* s, allS){
			if (s->getStmtNum() == stoi(intEnt.getEntity())){
				found = true;
				insert.insertValue(intEnt.getEntity());
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	}
	return false;
}

bool WithClause::evalStmtInt(WithClauseRef stmtEnt, WithClauseRef intEnt, Result* result){
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(stmtEnt.getEntity());
	if (result->isSynPresent(stmtEnt.getEntity())){
		unordered_set<string> synValues = result->getSyn(stmtEnt.getEntity());
		bool found = false;
		BOOST_FOREACH(string s, synValues){
			if (s == intEnt.getEntity()){
				found = true;
				insert.insertValue(s);
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	} else {
		StmtTable* stable = StmtTable::getInstance();
		unordered_set<Statement*> allS = stable->getAllStmts();
		bool found = false;
		BOOST_FOREACH(Statement* s, allS){
			if (s->getStmtNum() == stoi(intEnt.getEntity())){
				found = true;
				insert.insertValue(intEnt.getEntity());
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	}
	return false;
}

bool WithClause::evalVNameString(WithClauseRef varEnt, WithClauseRef strEnt, Result* result){
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(varEnt.getEntity());
	if (result->isSynPresent(varEnt.getEntity())){
		unordered_set<string> synValues = result->getSyn(varEnt.getEntity());
		bool found = false;
		BOOST_FOREACH(string s, synValues){
			if (s == strEnt.getEntity()){
				found = true;
				insert.insertValue(s);
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	} else {
		VarTable* vtable = VarTable::getInstance();
		unordered_set<Variable*> allV = vtable->getAllVariables();
		bool found = false;
		BOOST_FOREACH(Variable* v, allV){
			if (v->getName() == strEnt.getEntity()){
				found = true;
				insert.insertValue(strEnt.getEntity());
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	}
	return false;
}

bool WithClause::evalPNameString(WithClauseRef procEnt, WithClauseRef strEnt, Result* result){
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(procEnt.getEntity());
	if (result->isSynPresent(procEnt.getEntity())){
		unordered_set<string> synValues = result->getSyn(procEnt.getEntity());
		bool found = false;
		BOOST_FOREACH(string s, synValues){
			if (s == strEnt.getEntity()){
				found = true;
				insert.insertValue(s);
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	} else {
		ProcTable* ptable = ProcTable::getInstance();
		unordered_set<Procedure*> allP = ptable->getAllProcs();
		bool found = false;
		BOOST_FOREACH(Procedure* p, allP){
			if (p->getProcName() == strEnt.getEntity()){
				found = true;
				insert.insertValue(strEnt.getEntity());
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	}
	return false;
}

bool WithClause::evalCallString(WithClauseRef callEnt, WithClauseRef strEnt, Result* result){
	SingleSynInsert insert = SingleSynInsert();
	insert.setSyn(callEnt.getEntity());
	if (result->isSynPresent(callEnt.getEntity())){
		unordered_set<string> synValues = result->getSyn(callEnt.getEntity());
		bool found = false;
		BOOST_FOREACH(string s, synValues){
			if (s == strEnt.getEntity()){
				found = true;
				insert.insertValue(s);
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	} else {
		StmtTable* stable = StmtTable::getInstance();
		unordered_set<Statement*> allC = stable->getCallStmts();
		bool found = false;
		BOOST_FOREACH(Statement* s, allC){
			if (s->getProc()->getProcName() == strEnt.getEntity()){
				insert.insertValue(strEnt.getEntity());
				found = true;
			}
		}
		if (found){
			result->push(insert);
		}
		return found;
	}
	return false;
}

bool WithClause::evalPNamePName(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){
	//two pnames
	MultiSynInsert insert = MultiSynInsert();
	string le = leftEnt.getEntity();
	string re = rightEnt.getEntity();
	vector<string> syns = vector<string>();
	syns.push_back(le);
	syns.push_back(re);
	insert.setSyns(syns);
	if (leftEnt.getEntity() == rightEnt.getEntity()){
		//put a = a in reults
		ProcTable* ptable = ProcTable::getInstance();
		unordered_set<Procedure*> allP = ptable->getAllProcs();
		BOOST_FOREACH(Procedure* p, allP){
			string name = p->getProcName();
			vector<string> newRes = vector<string>();
			newRes.push_back(name);
			newRes.push_back(name);
			insert.insertValues(newRes);
		}
		result->push(insert);
		return true;
	} else {
		if (result->isSynPresent(leftEnt.getEntity())){
			if (result->isSynPresent(rightEnt.getEntity())){
				//both in result
				unordered_set<vector<string>> resultPairs = result->getMultiSyn(syns);
				bool found;
				BOOST_FOREACH(vector<string> pair, resultPairs){
					if (pair.at(0) == pair.at(1)){
						string p1 = pair.at(0);
						string p2 = pair.at(1);
						vector<string> newPair = vector<string>();
						newPair.push_back(p1);
						newPair.push_back(p2);
						insert.insertValues(newPair);
						found = true;
					}
					if (found){
						result->push(insert);
					}
					return found;
				}
			} else {
				//only left in result
				unordered_set<string> leRes = result->getSyn(leftEnt.getEntity());
				bool found;
				BOOST_FOREACH(string leftEntRes, leRes){
					vector<string> newRes = vector<string>();
					newRes.push_back(leftEntRes);
					newRes.push_back(leftEntRes);
					insert.insertValues(newRes);
					found = true;
				}
				if (found){
					result->push(insert);
				}
				return found;
			}
		} else if (result->isSynPresent(rightEnt.getEntity())){
			//only right syn
			unordered_set<string> reRes = result->getSyn(rightEnt.getEntity());
			bool found;
			BOOST_FOREACH(string rightEntRes, reRes){
				vector<string> newRes = vector<string>();
				newRes.push_back(rightEntRes);
				newRes.push_back(rightEntRes);
				insert.insertValues(newRes);
				found = true;
			}
			if (found){
				result->push(insert);
			}
			return found;
		} else {
			return true;
		}
	}
	return false;
}

bool WithClause::evalVNameVName(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){
	//two vnames
	MultiSynInsert insert = MultiSynInsert();
	string le = leftEnt.getEntity();
	string re = rightEnt.getEntity();
	vector<string> syns = vector<string>();
	syns.push_back(le);
	syns.push_back(re);
	insert.setSyns(syns);
	if (leftEnt.getEntity() == rightEnt.getEntity()){
		//put a = a in reults
		VarTable* vtable = VarTable::getInstance();
		unordered_set<Variable*> allV = vtable->getAllVariables();
		BOOST_FOREACH(Variable* v, allV){
			string name = v->getName();
			vector<string> newRes = vector<string>();
			newRes.push_back(name);
			newRes.push_back(name);
			insert.insertValues(newRes);
		}
		result->push(insert);
		return true;
	} else {
		if (result->isSynPresent(leftEnt.getEntity())){
			if (result->isSynPresent(rightEnt.getEntity())){
				//both in result
				unordered_set<vector<string>> resultPairs = result->getMultiSyn(syns);
				bool found;
				BOOST_FOREACH(vector<string> pair, resultPairs){
					if (pair.at(0) == pair.at(1)){
						string p1 = pair.at(0);
						string p2 = pair.at(1);
						vector<string> newPair = vector<string>();
						newPair.push_back(p1);
						newPair.push_back(p2);
						insert.insertValues(newPair);
						found = true;
					}
					if (found){
						result->push(insert);
					}
					return found;
				}
			} else {
				//only left in result
				unordered_set<string> leRes = result->getSyn(leftEnt.getEntity());
				bool found;
				BOOST_FOREACH(string leftEntRes, leRes){
					vector<string> newRes = vector<string>();
					newRes.push_back(leftEntRes);
					newRes.push_back(leftEntRes);
					insert.insertValues(newRes);
					found = true;
				}
				if (found){
					result->push(insert);
				}
				return found;
			}
		} else if (result->isSynPresent(rightEnt.getEntity())){
			//only right syn
			unordered_set<string> reRes = result->getSyn(rightEnt.getEntity());
			bool found;
			BOOST_FOREACH(string rightEntRes, reRes){
				vector<string> newRes = vector<string>();
				newRes.push_back(rightEntRes);
				newRes.push_back(rightEntRes);
				insert.insertValues(newRes);
				found = true;
			}
			if (found){
				result->push(insert);
			}
			return found;
		} else {
			return true;
		}
	}
	return false;
}

bool WithClause::evalValueValue(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){
	// c.v = c.v
	MultiSynInsert insert = MultiSynInsert();
	string le = leftEnt.getEntity();
	string re = rightEnt.getEntity();
	vector<string> syns = vector<string>();
	syns.push_back(le);
	syns.push_back(re);
	insert.setSyns(syns);
	if (leftEnt.getEntity() == rightEnt.getEntity()){
		//put a = a in reults
		ConstTable* ctable = ConstTable::getInstance();
		vector<Constant*> allC = ctable->getAllConst();
		BOOST_FOREACH(Constant* c, allC){
			int value = c->getValue();
			string name = std::to_string((long long)value);
			vector<string> newRes = vector<string>();
			newRes.push_back(name);
			newRes.push_back(name);
			insert.insertValues(newRes);
		}
		result->push(insert);
		return true;
	} else {
		if (result->isSynPresent(leftEnt.getEntity())){
			if (result->isSynPresent(rightEnt.getEntity())){
				//both in result
				unordered_set<vector<string>> resultPairs = result->getMultiSyn(syns);
				bool found;
				BOOST_FOREACH(vector<string> pair, resultPairs){
					if (pair.at(0) == pair.at(1)){
						string p1 = pair.at(0);
						string p2 = pair.at(1);
						vector<string> newPair = vector<string>();
						newPair.push_back(p1);
						newPair.push_back(p2);
						insert.insertValues(newPair);
						found = true;
					}
					if (found){
						result->push(insert);
					}
					return found;
				}
			} else {
				//only left in result
				unordered_set<string> leRes = result->getSyn(leftEnt.getEntity());
				bool found;
				BOOST_FOREACH(string leftEntRes, leRes){
					vector<string> newRes = vector<string>();
					newRes.push_back(leftEntRes);
					newRes.push_back(leftEntRes);
					insert.insertValues(newRes);
					found = true;
				}
				if (found){
					result->push(insert);
				}
				return found;
			}
		} else if (result->isSynPresent(rightEnt.getEntity())){
			//only right syn
			unordered_set<string> reRes = result->getSyn(rightEnt.getEntity());
			bool found;
			BOOST_FOREACH(string rightEntRes, reRes){
				vector<string> newRes = vector<string>();
				newRes.push_back(rightEntRes);
				newRes.push_back(rightEntRes);
				insert.insertValues(newRes);
				found = true;
			}
			if (found){
				result->push(insert);
			}
			return found;
		} else {
			return true;
		}
	}
	return false;
}

//deprecated
bool WithClause::evalStmtStmt(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){return false;}

bool evalStmtValue(WithClauseRef, WithClauseRef, Result*){return false;}
bool evalPNameVName(WithClauseRef, WithClauseRef, Result*){return false;}
bool evalSynValue(WithClauseRef, WithClauseRef, Result*){return false;}
bool evalSynStmt(WithClauseRef, WithClauseRef, Result*){return false;}
