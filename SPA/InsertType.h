#pragma once
#include "Result.h"

using namespace std;
using boost::unordered_set;

class InsertType
{
public:
	//result class to pass its result table inside here to mutate
	virtual void execute(Result::ResultTable&)=0;

}