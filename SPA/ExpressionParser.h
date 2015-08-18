#include <string>
#include <queue>
#include <stack>
#include "boost/assign.hpp"
#include "boost/unordered/unordered_map.hpp"

using namespace std;

class ExpressionParser {
public: 
	ExpressionParser();
	queue<string> getRPN(queue<string>);

private:
	stack<string> operationStack;
	queue<string> expressionQueue;
	queue<string> originalExpression;
	string word;
	string getWordAndPop(queue<string>&);
	void parseFactor();
	void parseSymbol();
	void parseOpenBracket();
	void parseCloseBracket();
	bool isValidFactor(string);
	bool isValidOperator(string);
};

namespace ExpressionParserConstants {
	const boost::unordered_map<std::string, int> OPERATOR_PRIORITIES = boost::assign::map_list_of ("+", 2) ("-", 2) ("*", 3);
}