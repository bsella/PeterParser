#ifndef PARSER_H
#define PARSER_H value
#include <regex>

const std::string operators= "[\\(\\)\\+\\*\\/\\-]";
const std::string func= "sin|cos|tan|sqrt|exp|log|pow|hypot|lerp|polynome";
const std::string number= "[0-9]*\\.[0-9]+|[0-9]+";
const std::string space= "[ 	]*";
const std::string var= "[a-zA-Z_][a-zA-Z_0-9]*";
const std::string fun= var+space+"\\(.*\\)";

bool is_number(const std::string& text);
std::vector<std::string> separate(std::string text);
bool is_valid(const std::string& text);
bool semiAtEnd(const std::string& text);
//cette fonction est devenu obsolette apres l'implementation de defFun 
bool defVar(const std::string& text, std::string& lvalue, std::string& expr);
bool defFun(const std::string& text, std::vector<std::string>& params, std::string& expr);
bool exprIsFunc(const std::string& expr, int& params,
				const std::map<const std::string, std::pair<std::vector<std::string>,std::string>>& envF);

#endif