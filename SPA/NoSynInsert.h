#pragma once
#include <string>
#include <set>
#include <vector>
#include "InsertType.h"
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;

class NoSynInsert : public InsertType
{
public:
	NoSynInsert();
	//inserts the rows into the results table
	bool execute(ResultTable&);
	// for clauses with 2 or more synonyms
	// the values follows the same order as that dictacted by the synList
	void setPass(bool);
	bool isPass();

private:
	bool pass;	
};