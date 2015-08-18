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
	void parseOperator();
	void parseOpenBracket();
	void parseCloseBracket();
};
