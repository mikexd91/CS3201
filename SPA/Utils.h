#pragma once
#include <string>
#include <vector>
#include <regex>

using std::string;
using std::vector;
class Utils
{
public:
	Utils(void);
	~Utils(void);
	
	string sanitise(string str);
	vector<string> explode(const string &str);
	
};

namespace UtilsConstants {
	const string DELIM_STRING = " ;={}";
	const char DELIMITERS[] = { ';', '=', '{', '}' };
	const char SANITISED_CHARS[] = { '\t', '\n' };
};