#include "parser.h"

bool is_number(const std::string text){
	std::regex rgx("\\-?"+number);
	return std::regex_search(text,rgx);
}

std::regex op("[\\+\\*\\/\\-]");
std::regex f1("sin|cos|tan|sqrt|exp|log");
std::regex f2("pow|hypot");
std::regex f3("lerp");
std::regex integer("[0-9]+");
std::smatch match;

//vérifier si l'expression est valide
static bool is_valid(std::vector<std::string>& text, bool parenthensis, int lookingFor){
	if(text.empty()) return !parenthensis && lookingFor==0;//à la fin de l'expression, on est pas dans des parenthèses et on a trouvé toutes les valeurs
	auto top = text.front(); text.erase(text.begin());//on prend le premier token
	if(top=="(") return is_valid(text, true, 1) && is_valid(text,parenthensis,lookingFor-1);//chercher une valeur dans les parenthèses et puis vérifier la suite
	else if(top==")") return parenthensis && lookingFor==0;//c'est la fin de la "sous-expression" entre parenthèses, on est pas dans des parenthèses et on a trouvé toutes les valeurs
	else if(regex_search(top,f1)) return is_valid(text,parenthensis,lookingFor);//on cherche toujours le meme nombre de paramètres
	else if(regex_search(top,f2)) return is_valid(text,parenthensis,lookingFor+1);//on cherche un paramètre de plus après
	else if(regex_search(top,f3)) return is_valid(text,parenthensis,lookingFor+2);//on cherche deux paramètres de plus après
	else if(is_number(top)) return is_valid(text,parenthensis,lookingFor-1);//on a trouvé un nombre, on continue
	else if(regex_search(top,op) && lookingFor==0) return is_valid(text,parenthensis,1);//si on trouve un opérateur et qu'on ne cherche plus rien, on continue
	else if(top=="polynome"){
		if(text.empty() || !regex_search(text.front(),integer)) return false;//n=le paramètre qui suit "polynome"
		return is_valid(text,parenthensis,lookingFor+std::stoi(text.front())+2);//on cherche n+2 paramètres de plus après
	}
	return false;//sinon l'expression n'est pas valide
}

bool is_valid(std::string text){
	std::vector<std::string> tokens = separate(text);
	return is_valid(tokens, false, 1);
}
std::vector<std::string> separate(std::string text){
	std::vector<std::string> ret;
	std::regex rgx(space+"("+number+"|"+operators+"|"+func+"|"+var+")");
	while(std::regex_search(text, match, rgx)){
		ret.push_back(match[1]);
		text=match.suffix();
	}
	return ret;
}

bool semiAtEnd(const std::string text){
	std::regex rgx(".*;"+space);
	return std::regex_search(text,rgx);
}

bool defVar(const std::string text, std::string& lvalue, std::string& expr){
	std::regex rgx(space+"("+var+")"+space+"=(.*)");
	if(std::regex_search(text,match,rgx)){
		lvalue=match[1];
		expr=match[2];
		return true;
	}
	return false;
}

bool defFun(const std::string text, std::vector<std::string>& inst){
	std::regex rgxFun(space+"("+var+")"+space+"\\((.*)\\)"+space+"="+space+"(.*)");
	std::string vars, expr;
	if(std::regex_search(text,match,rgxFun)){
		inst.push_back(match[1]);
		vars=match[2];
		expr=match[3];
	}else return false;
	std::regex rgxVars(space+"("+var+")"+space+",(.*)");
	while(std::regex_search(vars,match,rgxVars)){
		inst.push_back(match[1]);
		vars=match[2];
	}
	std::regex rgxVar(space+"("+var+")"+space);
	if(std::regex_search(vars,match,rgxVar)) inst.push_back(match[1]);
	else return false;
	inst.push_back(expr);
	return true;
}

bool replaceVars(std::string& expr, const std::map<const std::string, float> env){
	std::vector<std::string> tokens=separate(expr);
	std::regex rgx(var), rgxF(func);
	expr.clear();
	for(auto t : tokens){
		if(!std::regex_search(t,rgx) || std::regex_search(t,rgxF)){
			expr+=" "+t;
			continue;
		}
		if(env.find(t)!=env.end())
			expr+=" "+std::to_string(env.at(t));
		else return false;
	}
	return true;
}
