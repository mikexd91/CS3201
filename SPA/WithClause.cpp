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
			if (leftSideRef.getEntityType() != stringconst::ARG_PROCEDURE){
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
			if (leftSideRef.getEntityType() != stringconst::ARG_STATEMENT){
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
			if (rightSideRef.getEntityType() != stringconst::ARG_STATEMENT){
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
			//WIP
			return evalSynSyn(leftEntityRef, rightEntityRef, res);
			return false;
		} else {
			if (leftEntityAttr == PROCNAME_){
				//WIP
				return evalPNamePName(leftEntityRef, rightEntityRef, res);
			} else if (leftEntityAttr == VARNAME_){
				//WIP
				return evalVNameVName(leftEntityRef, rightEntityRef, res);
			} else if (leftEntityAttr == CONSTVALUE_){
				//WIP
				return evalValueValue(leftEntityRef, rightEntityRef, res);
			} else if (leftEntityAttr == STMTNUM_){
				//WIP
				return evalStmtStmt(leftEntityRef, rightEntityRef, res);
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
			//proc.name = str
			return evalPNameString(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == IDENT_ && rightEntityAttr == PROCNAME_){
			//str = proc.name
			return evalPNameString(rightEntityRef, leftEntityRef, res);

		} else if (leftEntityAttr == VARNAME_ && rightEntityRefType == IDENT_){
			//var.name = str
			return evalVNameString(leftEntityRef, rightEntityRef, res);

		} else if (leftEntityRefType == IDENT_ && rightEntityAttr == VARNAME_){
			//str = var.name
			return evalVNameString(rightEntityRef, leftEntityRef, res);

		}
		return false;
		// false for the rest of the cases
	}
}

bool WithClause::evalSynSyn(WithClauseRef syn1, WithClauseRef syn2, Result* result){
	if (syn1.getEntity() == syn2.getEntity()){
		return true;
	} else {
		//todo
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

bool WithClause::evalPNamePName(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){return false;}
bool WithClause::evalVNameVName(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){return false;}
bool WithClause::evalValueValue(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){return false;}
bool WithClause::evalStmtStmt(WithClauseRef leftEnt, WithClauseRef rightEnt, Result* result){return false;}