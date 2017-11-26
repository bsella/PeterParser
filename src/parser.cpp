#include "parser.h"
#include <iostream>

std::regex integer("[0-9]+");
std::regex op("[\\+\\*\\/\\-]");
std::regex f1("sin|cos|tan|sqrt|exp|log");
std::regex f2("pow|hypot");
std::regex f3("lerp");

std::smatch match;

bool is_number(const std::string& text){
	return std::regex_search(text,std::regex("\\-?"+number));
}

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
	}return false;//sinon l'expression n'est pas valide
}

bool is_valid(const std::string& text){
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

bool semiAtEnd(const std::string& text){
	return std::regex_search(text,std::regex(".*;"+space));
}

bool defVar(const std::string& text, std::string& lvalue, std::string& expr){
	if(std::regex_search(text,match,std::regex(space+"("+var+")"+space+"="))){
		lvalue=match[1];
		expr=match.suffix();
		return true;
	}
	return false;
}

bool defFun(const std::string& text, std::vector<std::string>& params, std::string& expr){
	std::string vars;
	if(std::regex_search(text,match,std::regex(space+"(.+)"+space+"="+space+"(.*)"))){
		vars=match[1];
		expr=match[2];
	}else return false;
	for(const auto s : separate(vars))
		if(std::regex_search(s,std::regex(var)))
			params.push_back(s);
		else return false;
	return true;
}

static bool envContains(const std::string& s,
	const std::map<const std::string, std::pair<std::vector<std::string>,std::string>>& envF){
	return envF.find(s)!=envF.end();
}

bool exprIsFunc(const std::string& expr, int& params,
	const std::map<const std::string, std::pair<std::vector<std::string>,std::string>>& envF){
	unsigned i;
	const std::vector<std::string> tokens = separate(expr);
	if(std::regex_search(tokens[0],std::regex("sin|cos|tan|sqrt|exp|log"))){
		if(tokens.size()>1) return false;
		params=1;//2-tokens.size();
	} 
	else if(std::regex_search(tokens[0],std::regex("pow|hypot"))){
		if(tokens.size()>2) return false;
		params=3-tokens.size();
	}
	else if(tokens[0]=="lerp"){
		if(tokens.size()>3) return false;
		params=4-tokens.size();
	}
	else if(tokens[0]=="polynome"){
		if(tokens.size()<2) return false;
		if(tokens.size()>(unsigned)std::stoi(tokens[1])+2) return false;
		params=std::stoi(tokens[1])+3 - tokens.size();
	}else if(envContains(tokens[0],envF)){
		if(tokens.size()>envF.at(tokens[0]).first.size()) return false;
		params= envF.at(tokens[0]).first.size()+1-tokens.size();
	}else return false;
	for(i= 1; i<tokens.size(); i++){
		if(std::regex_search(tokens[i],std::regex(func)))
			return false;
		if(envContains(tokens[i],envF))
			return false;
	}
	return true;
}