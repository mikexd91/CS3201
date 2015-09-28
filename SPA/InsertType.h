#pragma once
#include "ResultTable.h"
using namespace std;

class InsertType
{
public:
	//result class to pass its result table inside here to mutate
	virtual bool execute(ResultTable&)=0;

};