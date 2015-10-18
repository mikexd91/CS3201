#include "WithClause.h"
#include "Utils.h"
#include "boost/foreach.hpp"
#include "boost/lexical_cast.hpp"

using namespace boost;

WithClause::WithClause(ClauseType) {
}

WithClause::~WithClause(void)
{
}

bool WithClause::isValid(void){
	WithClauseRef leftSideRef = this->getLeftRef();
	WithClauseRef rightSideRef = this->getLeftRef();
	if (leftSideRef.getAttrType() == SYNONYM_){
		//(return false if !=progline)
	}
	if (rightSideRef.getAttrType() == SYNONYM_){
		//(return false if !=progline)
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
		return false;
	}

	WithClauseRef leftEntityRef = this->getLeftRef();
	WithClauseRef rightEntityRef = this->getRightRef();
	
	RefType LeftEntityRefType = leftEntityRef.getRefType();
	RefType RightEntityRefType = rightEntityRef.getRefType();

	//int = int
	if (LeftEntityRefType == INTEGER_ && RightEntityRefType == INTEGER_){
		return (leftEntityRef.getEntity() == rightEntityRef.getEntity());
	}

	//string = int
	if (LeftEntityRefType == IDENT_ && RightEntityRefType == INTEGER_){
		return (leftEntityRef.getEntity() == rightEntityRef.getEntity());
	}

	//syn = int
	if (LeftEntityRefType == SYNONYM_ && RightEntityRefType == INTEGER_){
		return false;
		//to discuss
	}

	//attrRef = int
	if (LeftEntityRefType == ATTRREF_ && RightEntityRefType == INTEGER_){

		string rightEntity = rightEntityRef.getEntity();
		int leftEntityCond = leftEntityRef.getAttrType();
		string leftEntity = leftEntityRef.getEntity();

		if (leftEntityCond == PROCNAME_ || leftEntityCond == VARNAME_){
			return false;
		} else if (leftEntityCond == CONSTVALUE_ || leftEntityCond == STMTNUM_){
			SingleSynInsert insert = SingleSynInsert();
			insert.setSyn(leftEntity);
			if (leftEntityCond == STMTNUM_){
				StmtTable* stmttable = StmtTable::getInstance();
				unordered_set<Statement*> stmtList = stmttable->getAllStmts();
				int casted = lexical_cast<int>(rightEntity);
				BOOST_FOREACH(Statement* stmt, stmtList){
					if (casted == stmt->getStmtNum()){
						insert.insertValue(rightEntity);
						return res->push(insert);
					}
				}
			} else if (leftEntityCond == CONSTVALUE_){
				ConstTable* consttable = ConstTable::getInstance();
				if (consttable->contains(rightEntity)){
					insert.insertValue(rightEntity);
					return res->push(insert);
				}
			}
		}
		return false;
	}
	
	//int = string
	if (LeftEntityRefType == INTEGER_ && RightEntityRefType == IDENT_){
		return (leftEntityRef.getEntity() == rightEntityRef.getEntity());
	}

	//string = string
	if (LeftEntityRefType == IDENT_ && RightEntityRefType == IDENT_){
		return (leftEntityRef.getEntity() == rightEntityRef.getEntity());
	}

	//syn = string
	if (LeftEntityRefType == SYNONYM_ && RightEntityRefType == IDENT_){
		return false;
		//to discuss
	}

	//attrRef = string
	if (LeftEntityRefType == ATTRREF_ && RightEntityRefType == IDENT_){
		
		int leftEntityCond = leftEntityRef.getAttrType();
		string rightEntity = rightEntityRef.getEntity();
		string leftEntity = leftEntityRef.getEntity();

		if (leftEntityCond == CONSTVALUE_ || leftEntityCond == STMTNUM_){
			return false;
		} else if (leftEntityCond == PROCNAME_){
			ProcTable* proctable = ProcTable::getInstance();
			
		} else if (leftEntityCond == VARNAME_){
			VarTable* vartable = VarTable::getInstance();
		
		}
		return false;
	}
	
	//int = syn
	if (LeftEntityRefType == INTEGER_ && RightEntityRefType == SYNONYM_){
		return false;
		//to discuss
	}

	//string = syn
	if (LeftEntityRefType == IDENT_ && RightEntityRefType == SYNONYM_){
		return false;
		//to discuss
	}

	//syn = syn
	if (LeftEntityRefType == SYNONYM_ && RightEntityRefType == SYNONYM_){
		//TODO
	}

	//attrRef = syn
	if (LeftEntityRefType == ATTRREF_ && RightEntityRefType == SYNONYM_){
		return false;
		//to discuss
	}
	
	//int = attrRef
	if (LeftEntityRefType == INTEGER_ && RightEntityRefType == ATTRREF_){
		//TODO
	}

	//string = attrRef
	if (LeftEntityRefType == IDENT_ && RightEntityRefType == ATTRREF_){
		int rightEntityCond = rightEntityRef.getAttrType();
		if (rightEntityCond == CONSTVALUE_ || rightEntityCond == STMTNUM_){
			return false;
		} else if (rightEntityCond == PROCNAME_){
			//todo
			return false;
		} else if (rightEntityCond == VARNAME_){
			//todo
			return false;
		}
	}

	//syn = attrRef
	if (LeftEntityRefType == SYNONYM_ && RightEntityRefType == ATTRREF_){
		return false;
		//to discuss
	}

	//attrRef = attrRef
	if (LeftEntityRefType == ATTRREF_ && RightEntityRefType == ATTRREF_){
		//TODO
	}
	return false;
}

