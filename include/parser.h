#ifndef PARSER_H
#define PARSER_H value
#include <regex>
#include <string>


const std::string operators= "[\\(\\)\\+\\*\\/\\-]";
const std::string func= "sin|cos|tan|sqrt|exp|log|pow|hypot|lerp|polynome";
const std::string number= "[0-9]*\\.[0-9]+|[0-9]+";
const std::string space= "[ 	]*";
const std::string var= "[a-zA-Z_][a-zA-Z_0-9]*";
const std::string fun= var+space+"\\(.*\\)";

bool is_number(const std::string text);
bool is_valid(std::string text);
std::vector<std::string> separate(const std::string text);
bool semiAtEnd(const std::string text);
bool defVar(const std::string text, std::string& lvalue, std::string& expr);
bool defFun(const std::string text, std::vector<std::string>& inst);
bool replaceVars(std::string& expr, const std::map<const std::string, float> env);
std::vector<std::string> getVars(std::string func);
std::vector<float> _resolve(std::vector<std::string> vars,
			std::map<std::string, float> env,
			std::map<std::string, std::function<float(std::vector<float>)>> envFun);
float resolve(std::string func,
			std::map<std::string, float> env,
			std::map<std::string, std::function<float(std::vector<float>)>> envFun);


#endif