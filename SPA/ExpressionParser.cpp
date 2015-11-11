#include "ExpressionParser.h"
#include "InvalidExpressionException.h"
#include "Utils.h"

ExpressionParser::ExpressionParser() {
}

queue<string> ExpressionParser::getRPN(queue<string> expr) {
	originalExpression = expr;
	//using Shunting-yard algorithm
	int count = 0;
	while (!originalExpression.empty()) {
		word = getWordAndPop(originalExpression);
		//parse word if it follows the mathematical rule
		if (Utils::isValidFactor(word) && (previousWord.empty() || Utils::isOpenBracket(previousWord) || Utils::isValidOperator(previousWord))) {
			//factor either appears at the start of the expression, or it follows an open bracket or operator
			parseFactor();
		} else if (Utils::isOpenBracket(word) && (previousWord.empty() || Utils::isValidOperator(previousWord) || Utils::isOpenBracket(previousWord))) {
			//open bracket either appears at the start of the expression, or it follows an operator
			parseOpenBracket();
		} else if (Utils::isCloseBracket(word) && (Utils::isValidFactor(previousWord) || Utils::isCloseBracket(previousWord))) {
			//close bracket follows a valid factor
			parseCloseBracket();
		} else if (Utils::isValidOperator(word) && (Utils::isValidFactor(previousWord) || Utils::isCloseBracket(previousWord))) {
			//operator follows a valid factor or close bracket
			parseOperator();
		} else {
			throw InvalidExpressionException("Invalid Expression!");
		}
		previousWord = word;
	}

	//ensure that the last element in the expression is a close bracket or a factor
	if (!(Utils::isCloseBracket(previousWord) || Utils::isValidFactor(previousWord))) {
		throw InvalidExpressionException("Invalid Expression!");
	}

	//pop the remaining operators into the expression, as according to the Shunting-yard algorithm
	while (!operationStack.empty()) {
		if (!Utils::isValidOperator(operationStack.top())) {
			throw InvalidExpressionException("Invalid Expression!");
		}
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	return expressionQueue;
}

void ExpressionParser::parseFactor() {
	expressionQueue.push(word);
}

void ExpressionParser::parseOperator() {
	//while there is an operator token, o2, at the top of the operator stack and the current operator o1 has precedence less than that of o2,
	while (!operationStack.empty() && Utils::isValidOperator(operationStack.top()) && UtilsConstants::OPERATOR_PRIORITIES.at(word) <= UtilsConstants::OPERATOR_PRIORITIES.at(operationStack.top())) {
		//then pop o2 off the operator stack, onto the output queue;
		expressionQueue.push(operationStack.top());
		operationStack.pop();
	}
	//push o1 onto the operator stack.
	operationStack.push(word);
}

void ExpressionParser::parseOpenBracket() {
	//push open bracket onto the operator stack.
	operationStack.push(word);
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
		if (Utils::isOpenBracket(operationStack.top())) {
			operationStack.pop();
			return;
		} else {
			expressionQueue.push(operationStack.top());
			operationStack.pop();
		}
	}
	throw InvalidExpressionException("Invalid Expression!");
}
