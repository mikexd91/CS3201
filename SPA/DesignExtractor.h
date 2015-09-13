#pragma once
#include "Utils.h"
#include "AST.h"
#include <string>
#include <vector>

using std::string;
using boost::unordered_set;

class DesignExtractor {
public: 
	DesignExtractor();
	void populateParentStar();
private:
	void recurse(StmtNode*, vector<int>&);

};
