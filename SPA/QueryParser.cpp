#include "QueryParser.h"
#include "Query.h"
#include "StringPair.h"
#include "Utils.h"
#include "PQLExceptions.h"
#include "boost/algorithm/string.hpp"
#include "Clause.h"
#include "FollowsClause.h"
#include "FollowsStarClause.h"
#include "ModifiesClause.h"
#include "ParentClause.h"
#include "ParentStarClause.h"
#include "PatternClause.h"
#include "UsesClause.h"
#include "PatternAssgClause.h"
#include "boost/unordered_map.hpp"
#include "ExpressionParser.h"
#include <queue>
#include <string>
#include <vector>
#include <sstream>
#include <exception>

using namespace std;
using boost::unordered_map;
using namespace boost;	

string QueryParser::removeSpace(string s){
	std::string::iterator end_pos = std::remove(s.begin(), s.end(), ' ');
	s.erase(end_pos, s.end());
	return s;
}

string QueryParser::removeFrontSpaces(string s){
	while (s.at(0) == ' '){
		s.erase(0);
	} 
	return s;
}

vector<string> QueryParser::split(string s, char delim, vector<string>* elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems->push_back(item);
    }
    return *elems;
}

//TODO: CHECK COMPATIBILITY WITH PATTERN AND CLAUSE PARSING
queue<string> QueryParser::queueBuilder(string in){
	queue<string> out;
	vector<string> temp;
	vector<string> outHolder;
	temp = QueryParser::tokeniser(in, ' ');
	outHolder = QueryParser::splitByDelims(temp, "(");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, ")");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, ",");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "\"");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "+");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "-");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "*");
	temp = outHolder;
	outHolder = QueryParser::splitByDelims(temp, "-");
	for (size_t i=0; i<outHolder.size(); i++){
		out.push(outHolder.at(i));
	}
	return out;
}

string QueryParser::queueToString(queue<string> input){
	string item = Utils::getWordAndPop(input);
	stringstream ss("");
	ss << item;
	while (!input.empty()){
		item = Utils::getWordAndPop(input);
		ss << " " << item;
	}
	return ss.str();
}

vector<string> QueryParser::tokeniser(string input, char delim){
	vector<string> elems;
	size_t pos = input.find_first_of(delim);
	while (pos != string::npos){
		if (pos == 0){
			string temp = input.substr(1);
			input = temp;
			pos = temp.find_first_of(delim);
		} else if (pos == input.length()){
			elems.push_back(input.substr(0, pos - 1));
			pos = string::npos;
		} else {
			string s = input.substr(0, pos);
			elems.push_back(s);
			input = input.substr(pos + 1);
			pos = input.find_first_of(delim);
		}
	}
	if (pos == string::npos && !input.empty()){
		elems.push_back(input);
	}
	return elems;
}

bool QueryParser::containsOperator(string s){
	vector<string> opVector;
	opVector.push_back("+");
	opVector.push_back("/");
	opVector.push_back("-");
	opVector.push_back("*");
	return containsAny(s, opVector);
}

string QueryParser::getFirstOperator(string s){
	for (size_t i=0; i<s.size(); i++){
		char current = s.at(i);
		if (current == '+'){
			return "+";
		} else if (current == '-'){
			return "-";
		} else if (current == '/'){
			return "/";
		} else if (current == '*'){
			return "*";
		}
	}
	return stringconst::STRING_EMPTY;
}

int QueryParser::getOperatorIndex(string s){
	for (size_t i=0; i<s.size(); i++){
		char current = s.at(i);
		if ((current == '+') || (current == '-') || (current == '*') || (current == '/')){
			return i;
		}
	}
	return -1;
}

queue<string> QueryParser::exprBuilder(string input){
	vector<string> elems;
	string duplicate = input;
	while (containsOperator(duplicate)){
		int index = getOperatorIndex(duplicate);
		string op = getFirstOperator(duplicate);
		string left = duplicate.substr(0, index);
		string right = duplicate.substr(index+1, duplicate.size() - index);
		elems.push_back(left);
		elems.push_back(op);
		duplicate = right;
	}
	elems.push_back(duplicate);
	queue<string> toReturn;
	for (size_t i = 0; i<elems.size(); i++){
		string current = elems.at(i);
		string toReplace = removeSpace(current);
		toReturn.push(toReplace);
	}
	return toReturn;
}

bool QueryParser::containsAny(string s, vector<string> list){
	for (size_t i=0; i<list.size(); i++){
		string current = list.at(i);
		if (contains(s, current)){
			return true;
		}
	}
	return false;
}

bool QueryParser::containsDeclarationType(string s){
	vector<string> decVector;
	decVector.push_back(stringconst::ARG_ASSIGN);
	decVector.push_back(stringconst::ARG_STATEMENT);
	decVector.push_back(stringconst::ARG_WHILE);
	decVector.push_back(stringconst::ARG_VARIABLE);
	decVector.push_back(stringconst::ARG_PROGLINE);
	decVector.push_back(stringconst::ARG_CONSTANT);
	return containsAny(s, decVector);
}

bool QueryParser::containsClauseType(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
	return containsAny(s, clauseVector);
}

bool QueryParser::containsKeyword(string s){
	vector<string> wordVector;
	wordVector.push_back(stringconst::STRING_SUCH);
	wordVector.push_back(stringconst::STRING_THAT);
	wordVector.push_back(stringconst::STRING_WITH);
	wordVector.push_back(stringconst::STRING_AND);
	return containsAny(s, wordVector);
}

string QueryParser::getClauseString(string s){
	vector<string> clauseVector;
	clauseVector.push_back(stringconst::TYPE_FOLLOWS);
	clauseVector.push_back(stringconst::TYPE_PARENT);
	clauseVector.push_back(stringconst::TYPE_MODIFIES);
	clauseVector.push_back(stringconst::TYPE_USES);
	clauseVector.push_back(stringconst::TYPE_FOLLOWS_STAR);
	clauseVector.push_back(stringconst::TYPE_PARENT_STAR);
	for (size_t i=0; i<clauseVector.size(); i++){
		string current = clauseVector.at(i);
		if (contains(s, current)){
			return current;
		}
	}
	return stringconst::STRING_EMPTY;
}

Clause* QueryParser::createCorrectClause(string type){
	if (type == stringconst::TYPE_FOLLOWS_STAR){
		FollowsStarClause* clause = new FollowsStarClause();
		return clause;		
	} else if (type == stringconst::TYPE_PARENT_STAR){
		ParentStarClause* clause = new ParentStarClause();
		return clause;			
	} else if (type == stringconst::TYPE_FOLLOWS){
		FollowsClause* clause = new FollowsClause();
		return clause;		
	} else if (type == stringconst::TYPE_PARENT){
		ParentClause* clause = new ParentClause();
		return clause;		
	} else if (type == stringconst::TYPE_MODIFIES){
		ModifiesClause* clause = new ModifiesClause();
		return clause;		
	} else if (type == stringconst::TYPE_USES){
		UsesClause* clause = new UsesClause();
		return clause;
	/*INSERT WHEN CLAUSES ARE DONE
	} else if (type == stringconst::TYPE_CALLS){
		CallsClause* clause = new CallsClause();
		return clause;
	} else if (type == stringconst::TYPE_NEXT){
		NextClause* clause = new NextClause();
		return clause;
	} else if (type == stringconst::TYPE_AFFECTS){
		AffectsClause* clause = new AffectsClause();
		return clause;
	} else if (type == stringconst::TYPE_NEXT_STAR){
		NextStarClause* clause = new NextStarClause();
		return clause;
	} else if (type == stringconst::TYPE_AFFECTS_STAR){
		AffectsStarClause* clause = new AffectsStarClause();
		return clause;
	}
	*/ 
	} else {
		throw UnexpectedClauseException();
	}
}

void QueryParser::parseDeclarations(Query* query, vector<string> list){
	for (size_t i=0; i<list.size(); i++){
		string current = list.at(i);
		boost::algorithm::trim(current);
		vector<string> tokens = tokeniser(current, ',');
		string first = tokens.at(0);
		vector<string> split = tokeniser(first, ' ');
		string decType = split.at(0); 
		boost::trim(decType);
		if (!containsDeclarationType(decType)){
			throw InvalidDeclarationException();
		}
		if (decType == stringconst::ARG_PROGLINE){
			decType = stringconst::ARG_STATEMENT;
		}
		StringPair* newPair = new StringPair();
		newPair->setFirst(split.at(1));
		newPair->setSecond(decType);
		query->addDeclaration(*newPair);
		if (tokens.size() > 1){
			for (size_t i=1; i<tokens.size(); i++){
				string here = tokens.at(i);
				here = removeSpace(here);
				StringPair* newPair = new StringPair();
				newPair->setFirst(here);
				newPair->setSecond(decType);
				query->addDeclaration(*newPair);
			}
		}
	}
}

void QueryParser::unexpectedEndCheck(queue<string> in){
	if (in.empty()){
		throw UnexpectedEndException();
	}
}

void QueryParser::parseSelectSynonyms(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string first = Utils::getWordAndPop(line);
	if (first != stringconst::STRING_SELECT){
		throw InvalidSelectException();
	} else {
		bool expectSelect = true;
		while (expectSelect){
			string current = Utils::getWordAndPop(line);
			if (decList.find(current) == decList.end()){
				throw MissingDeclarationException();
			} else {
				string type = decList.at(current);
				StringPair* newPair = new StringPair();
				newPair->setFirst(current);
				newPair->setSecond(type);
				query->addSelectSynonym(*newPair);
			}
			if (line.empty()){
				expectSelect = false;
			} else {
				string next = line.front();
				if (containsKeyword(next) || contains(next, stringconst::TYPE_PATTERN)){
					expectSelect = false;
				}
			}
		}
	}
}

//TODO: UPDATE PARSE CLAUSE WITH NEW QUEUE (DONE, UNIT TESTING)
void QueryParser::parseClause(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	string clauseType = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	Clause* newClause;
	newClause = createCorrectClause(clauseType);
	string openParen = Utils::getWordAndPop(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);

	string firstArg = Utils::getWordAndPop(line);
	if (decList.find(firstArg) == decList.end()){
		if (!Utils::isValidConstant(firstArg)){
			if (!contains(firstArg, "\"")){
				if (firstArg != stringconst::STRING_EMPTY){
					throw MissingDeclarationException();
				} else {
					newClause->setFirstArg(firstArg);
					newClause->setFirstArgFixed(false);
					newClause->setFirstArgType(stringconst::ARG_GENERIC);
				}
			} else {
				int start = firstArg.find_first_of("\"");
				int end = firstArg.find_last_of("\"");
				string first = firstArg.substr(start+1, end-start-1);
				newClause->setFirstArg(first);
				newClause->setFirstArgFixed(true);
				newClause->setFirstArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setFirstArg(firstArg);
			newClause->setFirstArgFixed(true);
			newClause->setFirstArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string firstArgType = decList.at(firstArg);
		newClause->setFirstArg(firstArg);
		newClause->setFirstArgFixed(false);
		newClause->setFirstArgType(firstArgType);
	}
	unexpectedEndCheck(line);

	string comma = Utils::getWordAndPop(line);
	if (comma != ","){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);
	
	string secondArg = Utils::getWordAndPop(line);
	cout << "secondArg=" << secondArg << "|";
	if (decList.find(secondArg) == decList.end()){
		if (!Utils::isValidConstant(secondArg)){
			if (!contains(secondArg, "\"")){
				if (secondArg != stringconst::STRING_EMPTY){
					throw MissingDeclarationException();
				} else {
					newClause->setSecondArg(secondArg);
					newClause->setSecondArgFixed(false);
					newClause->setSecondArgType(stringconst::ARG_GENERIC);
				}
			} else {
				int start = secondArg.find_first_of("\"");
				int end = secondArg.find_last_of("\"");
				string second = secondArg.substr(start+1, end-start-1);
				newClause->setSecondArg(second);
				newClause->setSecondArgFixed(true);
				newClause->setSecondArgType(stringconst::ARG_VARIABLE);
			}
		} else {
			newClause->setSecondArg(secondArg);
			newClause->setSecondArgFixed(true);
			newClause->setSecondArgType(stringconst::ARG_STATEMENT);
		}
	} else {
		string secondArgType = decList.at(secondArg);
		newClause->setSecondArg(secondArg);
		newClause->setSecondArgFixed(false);
		newClause->setSecondArgType(secondArgType);
	}
	unexpectedEndCheck(line);

	//error here, check the queue
	string closeParen = Utils::getWordAndPop(line);
	cout << "[" << closeParen << "]";
	if (closeParen != ")"){
		throw InvalidSyntaxException();
	}

	query->addClause(newClause);
}

//TODO UPDATE WITH NEW QUEUE
//PARSE BRACKETS, COMMAS, OPERATORS, UNDERSCORE AND INVERTED COMMAS AS INDIVIDUAL TOKENS
void QueryParser::parsePattern(Query* query, queue<string> line){
	unordered_map<string, string> decList = query->getDeclarationList();
	
	string wordPattern = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);

	string synonym = Utils::getWordAndPop(line);
	if (decList.find(synonym) == decList.end() || decList.at(synonym) != stringconst::ARG_ASSIGN){
		throw InvalidDeclarationException();
	}
	unexpectedEndCheck(line);

	string openParen = Utils::getWordAndPop(line);
	if (openParen != "("){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);

	string var;
	string underscore1 = line.front();
	if (underscore1 == "_"){
		var = underscore1;
	} else {
		while (line.front() != ","){
			string varParts = line.front();
			if (varParts == "_"){
				throw InvalidSyntaxException();
			} else {
				var = varParts;
				Utils::getWordAndPop(line);
			}
		}
	}

	string comma = Utils::getWordAndPop(line);
	if (comma != ","){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);

	string underscoreFirst = Utils::getWordAndPop(line);
	if (underscoreFirst != "_"){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);

	string openExpr = Utils::getWordAndPop(line);
	if (openExpr != "\""){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);

	string current = Utils::getWordAndPop(line);
	unexpectedEndCheck(line);
	queue<string> asdf;
	if (current == "\""){
		throw InvalidClauseException();
	} else {
		while (current != "\"" && !line.empty()){
			asdf.push(current);
			current = Utils::getWordAndPop(line);
		}
		if (current != "\""){
			throw InvalidSyntaxException();
		}
		if (line.empty()){
			throw UnexpectedEndException();
		}
		unexpectedEndCheck(line);
	}

	string underscoreLast = Utils::getWordAndPop(line);
	if (underscoreLast != "_"){
		throw InvalidSyntaxException();
	}
	unexpectedEndCheck(line);

	string closeExpr = Utils::getWordAndPop(line);
	if (closeExpr != ")"){
		throw InvalidSyntaxException();
	}

	ExpressionParser exprP;
	queue<string> postASDF = exprP.getRPN(asdf);
	string expression = "_\"" + queueToString(postASDF) + "\"_";

	PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expression);
	query->addClause(newClause);
	/*
	if (line.empty()){
		throw UnexpectedEndException();
	}
	string next = line.front();
	size_t pos = next.find_first_of("(");
	string synonym = next.substr(0, pos);
	if (decList.find(synonym) == decList.end() || decList.at(synonym) != stringconst::ARG_ASSIGN){
		throw InvalidDeclarationException();
	}
	size_t npos = next.find_first_of(",");
	string var = next.substr(pos+1, npos-pos-1);
	bool varFixed = false;
	if (decList.find(var) == decList.end()){
		if (contains(var, "\"")){
			size_t spos = var.find_first_of("\"");
			size_t fpos = var.find_last_of("\"");
			if (spos == fpos){
				throw InvalidArgumentException();
			} else {
				string arg = var.substr(spos+1, fpos-spos-1);
				var = arg;
				varFixed = true;
			}
		}
	}
	std::stringstream ss = stringstream();
	bool endToken = false;
	bool readBegin = false;
	bool readEnd = false;
	Utils::getWordAndPop(line);
	string subsequent = line.front();
	endToken = contains(subsequent, ")");
	while (!endToken){
		if (!readBegin){
			//TO READ : _"
			if (!(contains(subsequent, "_") || contains(subsequent, "\""))){
				throw InvalidArgumentException();
			}
			readBegin = true;
			size_t bpos = subsequent.find_first_of("_");
			size_t epos = subsequent.find_first_of("\"");
			if (bpos != epos - 1 || bpos != 0){
				throw InvalidArgumentException();
			}
			size_t len = subsequent.size();
			if (len > 2){
				string exprPart = subsequent.substr(2);
				ss << exprPart;
			}
		} else {
			//TO READ (EXPR || EXPR"_)
			if (!contains(subsequent, "_") && !contains(subsequent, "\"")){
				ss << " " << subsequent;
			} else {
				if (contains(subsequent, "_") && contains(subsequent, "\"")){
					size_t epos = subsequent.find_first_of("\"");
					size_t fpos = subsequent.find_first_of("_");
					size_t len = subsequent.size();
					if ((epos != fpos -1) && fpos != len - 1){
						throw InvalidArgumentException();
					}
					string exprPart = subsequent.substr(0, epos);
					ss << " " << exprPart;
					readEnd = true;
				} else {
					throw InvalidArgumentException();
				}
			}
		}
		Utils::getWordAndPop(line);
		subsequent = line.front();
		endToken = contains(subsequent, ")");
	}
	if (!readBegin){
		//ONLY ONE TOKEN FOR WHOLE EXPRESSION
		size_t bpos = subsequent.find_first_of("_");
		size_t fpos = subsequent.find_last_of("_");
		if (bpos == fpos){
			if (bpos == 0 && !contains(subsequent, "\"")){
				PatternAssgClause* newClause = new PatternAssgClause(synonym, var, stringconst::STRING_EMPTY);
				newClause->setVarFixed(varFixed);
				query->addClause(newClause);
			} else {
				throw InvalidArgumentException();
			}
		} else {
			size_t spos = subsequent.find_first_of("\"");
			size_t epos = subsequent.find_last_of("\"");
			if (spos == epos){
				throw InvalidArgumentException();
			} else {
				string exprPart = subsequent.substr(spos + 1, epos - spos - 1);
				queue<string> expression = exprBuilder(exprPart);
				ExpressionParser expressionParser;
				queue<string> exprRPN = expressionParser.getRPN(expression);
				string expr = "_\"" + queueToString(exprRPN) + "\"_";
				PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expr);
				newClause->setVarFixed(varFixed);
				query->addClause(newClause);
			}
		}
	} else if (!readEnd){
		//TOKEN CONTAINS END OF EXPRESSION
		size_t epos = subsequent.find_first_of("\"");
		size_t fpos = subsequent.find_first_of("_");
		if (fpos != epos +1){
			throw InvalidArgumentException();
		} else {
			string exprPart = subsequent.substr(0, epos);
			ss << " " << exprPart;
			string expressionS = ss.str();
			queue<string> expressionQ = exprBuilder(expressionS);
			ExpressionParser expressionParser;
			queue<string> exprRPN = expressionParser.getRPN(expressionQ);
			string expr = "_\"" + queueToString(exprRPN) + "\"_";
			PatternAssgClause* newClause = new PatternAssgClause(synonym, var, expr);
			newClause->setVarFixed(varFixed);
			query->addClause(newClause);
		}
	} else {
		//TOKEN CONTAINS ONLY )
		Utils::getWordAndPop(line);
	}
	*/
} 

Query QueryParser::parseQuery(string input){
	Query* output = new Query();
	vector<string> splitBySC = tokeniser(input, ';');
	int numDeclarations = splitBySC.size() - 1;
	string selectStatement = splitBySC.at(splitBySC.size()-1);
	queue<string> selectQueue = queueBuilder(selectStatement);
	splitBySC.pop_back();
	parseDeclarations(output, splitBySC);
	while(!selectQueue.empty()){
		string current = selectQueue.front();
		if (current == stringconst::STRING_SELECT){
			parseSelectSynonyms(output, selectQueue);
		} else if (containsClauseType(current)){
			parseClause(output, selectQueue);
		} else if (contains(current, stringconst::TYPE_PATTERN)){
			parsePattern(output, selectQueue);
		} else if (containsKeyword(current)){
			//selectQueue.pop();
		}
		selectQueue.pop();
	}
	vector<Clause*> clauseList = output->getClauseList();
	for (size_t i=0; i<clauseList.size(); i++){
		Clause* current = clauseList.at(i);
		//if (!current->isValid()){
			//throw InvalidClauseException();
		//}
	}
	return *output;
}

vector<string> QueryParser::splitByDelims(vector<string> in, string delim){
	vector<string> out;
	for (size_t i = 0; i < in.size(); i++){
		string current = in.at(i);
		size_t pos = current.find_first_of(delim);
		while (pos != std::string::npos){
			string before = current.substr(0, pos);
			out.push_back(before);
			out.push_back(delim);
			string after = current.substr(pos+1);
			current = after;
			pos = current.find_first_of(delim);			
		}
		if (!current.empty()){
			out.push_back(current);
		}
	}
	return out;
}