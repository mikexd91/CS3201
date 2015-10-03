#pragma once
#include <string>
#include <list>
#include <vector>

using namespace std;

//cos we will need to access a particular position quite often (depending on which syn)
//and we will not be adding to the middle of a row often
typedef vector<string> Row;
struct ResultTable {
	vector<string> synList;
	//for cheap insertion and erasure
	list<Row> rows;
};