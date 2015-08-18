#include "ExpressionParser.h"
#include "InvalidExpressionException.h"
#include "Utils.h"

ExpressionParser::ExpressionParser() {
}

queue<string> ExpressionParser::getRPN(queue<string> expr) {
	originalExpression = expr;
	//using Shunting-yard algorithm
	int count = 0;
	word = getWordAndPop(originalExpression);
	if (isValidFactor(word)) {
		parseFactor();
	} else if (word == "(") {
		parseOpenBracket();
	} else {
		throw InvalidExpressionException("Invalid Expression!");
	}
	while (!operationStack.empty()) {
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	return expressionQueue;
}

//Needed for generating RPN. Checks and adds the factor to the expression queue.
void ExpressionParser::parseFactor() {
	expressionQueue.push(word);
	if (!originalExpression.empty()) {
		word = getWordAndPop(originalExpression);
		if (isValidOperator(word)) {
			parseSymbol();
		} else if (word == ")") {
			parseCloseBracket();
		} else {
			throw InvalidExpressionException("Invalid Expression!");
		}
	}
}

//Needed for generating RPN. Checks and adds the factor to the expression queue.
void ExpressionParser::parseSymbol() {
	//while there is an operator token, o2, at the top of the operator stack and the current operator o1 has precedence less than that of o2,
	while (!operationStack.empty() && Utils::isValidOperator(operationStack.top()) && ExpressionParserConstants::OPERATOR_PRIORITIES.at(word) <= ExpressionParserConstants::OPERATOR_PRIORITIES.at(operationStack.top())) {
		//then pop o2 off the operator stack, onto the output queue;
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	//push o1 onto the operator stack.
	operationStack.push(word);
	word = getWordAndPop(originalExpression);
	if (isValidFactor(word)) {
		parseFactor();
	} else if (word == "(") {
		parseOpenBracket();
	} else {
		throw InvalidExpressionException("Invalid Expression!");
	}

}

void ExpressionParser::parseOpenBracket() {
	//push open bracket onto the operator stack.
	operationStack.push(word);
	word = getWordAndPop(originalExpression);
	if (isValidFactor(word)) {
		parseFactor();
	} else {
		throw InvalidExpressionException("Invalid Expression!");
	}
}

//checks that the queue is not empty, then gets the next element of the queue and pop the head
string ExpressionParser::getWordAndPop(queue<string> &originalExpression) {
	if(originalExpression.empty()) {
		throw InvalidExpressionException("Invalid Expression!");
	} else {
		string word = originalExpression.front();
		originalExpression.pop();
		return word;
	}
}

/**
	If the token is a right parenthesis (i.e. ")"):
    Until the token at the top of the stack is a left parenthesis, pop operators off the stack onto the output queue.
    Pop the left parenthesis from the stack, but not onto the output queue.
    If the stack runs out without finding a left parenthesis, then there are mismatched parentheses.
**/
void ExpressionParser::parseCloseBracket() {
	while (!operationStack.empty()) {
		if (operationStack.top() == "(") {
			operationStack.pop();
			word = getWordAndPop(originalExpression);
			if (ExpressionParser::isValidOperator(word)) {
				parseSymbol();
				return;
			} else {
				throw InvalidExpressionException("Invalid Expression!");
			}
		} else {
			expressionQueue.push(operationStack.top());
			operationStack.pop();
		}
	}
	throw InvalidExpressionException("Invalid Expression!");
}

//Check if a string is a valid symbol
bool ExpressionParser::isValidOperator(string word) {
	return ExpressionParserConstants::OPERATOR_PRIORITIES.find(word) != ExpressionParserConstants::OPERATOR_PRIORITIES.end();
}

//Check if a string is a valid factor (constant or name)
bool ExpressionParser::isValidFactor(string word) {
	return Utils::isValidConstant(word) || Utils::isValidName(word);
}
