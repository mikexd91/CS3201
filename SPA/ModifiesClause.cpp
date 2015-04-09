#include "ModifiesClause.h"
#include "StmtTable.h"
#include "VarTable.h"
#include "Utils.h"

ModifiesClause::ModifiesClause(void):Clause(MODIFIES_){
}

ModifiesClause::~ModifiesClause(void){

}

bool ModifiesClause::isStmtFixed() {
	return firstArgFixed;
}

bool ModifiesClause::isVarFixed() {
	return secondArgFixed;
}

string ModifiesClause::getStmt() {
	return firstArg;
}

int ModifiesClause::getStmtNum() {
	if (isStmtFixed()) {
		return atoi(firstArg.c_str());
	} else {
		return 0;
	}
}

string ModifiesClause::getVar() {
	return secondArg;
}

string ModifiesClause::getStmtType() {
	return "";
}


bool ModifiesClause::isValid(void){
	string firstType = this->getFirstArgType();
	string secondType = this->getSecondArgType();
	bool firstArg = (firstType == stringconst::ARG_STATEMENT) || (firstType == stringconst::ARG_ASSIGN) || (firstType == stringconst::ARG_WHILE);
	bool secondArg = (secondType == stringconst::ARG_VARIABLE);
	return (firstArg && secondArg);
}

Results ModifiesClause::evaluate(void) {

	/*

	stmt fixed
		var fixed == set the result to true if true
		var syn == return all var that mod by stmt

	stmt syn - can be assg, while, plainstmt
		var fixed == return all stmt that mods this var
		var syn == return all pair (stmt, var) that mod holds

	*/

	if (isStmtFixed()) {
		if (isVarFixed()) {
			return evaluateFixedFixed(getStmtNum(), getVar());
		} else {
			return evaluateFixedSyn(getStmtNum(), getVar());
		}
	} else {
		if (isVarFixed()) {
			return evaluateSynFixed(getStmt(), getVar());
		} else {
			return evaluateSynSyn(getStmt(), getVar());
		}
	}
}

Results ModifiesClause::evaluateFixedFixed(int stmtNum, string varName) {
	Results* res = new Results();
	res->setClausePassed(isParent(stmtNum, varName));

	return *res;
}

Results ModifiesClause::evaluateFixedSyn(int stmtNum, string var) {
	Results* res = new Results();
	res->setFirstClauseSyn(var);
	
	VarTable* vtable = VarTable::getInstance();
	vector<string>* allVarNames = vtable->getAllVarNames();

	for (int i = 0; i < allVarNames->size(); i++) {
		if (isParent(stmtNum, allVarNames->at(i))) {
			res->addSingleResult(allVarNames->at(i));
		}
	}

	res->setClausePassed(res->getSinglesResults().size() > 0);

	return *res;
}

Results ModifiesClause::evaluateSynFixed(string stmt, string varName) {
	return *new Results();
}

Results ModifiesClause::evaluateSynSyn(string stmt, string var) {
	return *new Results();
}

bool ModifiesClause::isParent(int stmtNum, string varName) {
	StmtTable* stable = StmtTable::getInstance();
	Statement* stmt = stable->getStmtObj(stmtNum);

	set<string> mods = stmt->getModifies();

	return mods.find(varName) != mods.end();
}
