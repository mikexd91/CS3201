#pragma once
#include "boost/unordered_set.hpp"

using namespace std;
using boost::unordered_set;

class PQLController {

private:

public:
	PQLController(void);
	~PQLController(void);

	unordered_set<string> parse(string query); // runs PQL controller on this string
};
