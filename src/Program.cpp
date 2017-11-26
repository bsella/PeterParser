#include "Program.h"
#include "parser.h"

Program::Program(const std::vector<std::string> str){
	for(auto s : str){
		if(s.empty()) continue;
		instructions.push_back(Instruction(s));
	}
}
Program::Program(const std::string *str){
	for(unsigned i = 0; i< sizeof(str)/sizeof(std::string); i++){
		if(str[i].empty()) continue;
		instructions.push_back(Instruction(str[i]));
	}
}
Program::Program(){}

Program::Instruction::Instruction(const std::string& t){
	//vérifier s'il y a un point virgule à la fin de l'instruction
	semi=semiAtEnd(t);
	
	//vérifier s'il s'agit d'une affectation de valeur
	if(!defFun(t,lvalue,expr)){
		expr=t;
		return;
	}
	if(lvalue.size()>1)
		fDef=true;
}

void Program::execute(Instruction inst){
	std::vector<std::string> vars=inst.lvalue; 
	if(!vars.empty())
		vars.erase(vars.begin());
	std::vector<std::string> ignore=vars;
	for(auto it = envFun.begin(); it!=envFun.end(); ++it)
  		ignore.push_back(it->first);
	//remplacer chaque variable par sa valeur
	if(!replaceVars(inst.expr,ignore)) throw UndefinedVar();

	if(inst.lvalue.size()>=1){
		int params;
		if(exprIsFunc(inst.expr,params,envFun)){
			for(int i=0; i<params; ++i){
				inst.lvalue.push_back("_"+std::to_string(i));
				vars.push_back("_"+std::to_string(i));
				inst.expr+= " _"+std::to_string(i);
			}
			inst.fDef=true;
		}
	}

	// std::cout <<"	"+ inst.expr << std::endl;
	//mettre à jour la variable
	if(!inst.lvalue.empty()){
		if(inst.fDef){
			replaceFunc(inst.expr);
			envFun[inst.lvalue[0]]={vars,inst.expr};
			if(!inst.semi){
				std::cout<<"	"+inst.lvalue[0]+"(";
				for(unsigned i=1; i<inst.lvalue.size(); i++){
					if(i!=1) std::cout<< ", ";
					std::cout<< inst.lvalue[i];
				}
				std::cout<< ") ="+inst.expr << std::endl;
			}
		}
		else{
			replaceFunc(inst.expr);
			float value= ExprTree(inst.expr.c_str()).eval();
			envVar[inst.lvalue[0]]=value;
			if(!inst.semi)
				std::cout<< "	" << value << std::endl;
		}
	}else if(!inst.semi){
		replaceFunc(inst.expr);
		std::cout<<"	"<< ExprTree(inst.expr.c_str()).eval() << std::endl; 
	}
}

bool Program::replaceVars(std::string& expr, const std::vector<std::string>& ignore){
	const auto tokens=separate(expr);
	expr.clear();
	for(const auto t : tokens){
		if( std::find(ignore.begin(), ignore.end(), t) != ignore.end() ||
			!std::regex_search(t,std::regex(var)) ||
			std::regex_search(t,std::regex(func))){
			expr+=" "+t;
			continue;
		}
		if(envVar.find(t)!=envVar.end())
			expr+=" "+std::to_string(envVar.at(t));
		else return false;
	}
	return true;
}

//f x y = y + x
//f 2 3 = 3 + 2
//expr [y + x]
void Program::replaceFunc(std::string& expr){
	const auto tokens=separate(expr);
	expr.clear();
	for(unsigned i=0; i<tokens.size(); i++){
		if(envFun.find(tokens[i])==envFun.end()){
			expr+=" "+tokens[i];
			continue;
		}
		auto newExpr= separate(envFun[tokens[i]].second);
		for(unsigned j=0; j<envFun[tokens[i]].first.size(); j++)
			for(unsigned k=0; k<newExpr.size(); k++){
				if(i+j+1>=tokens.size())
					throw TooFewArguments();
				if(newExpr[k]==envFun[tokens[i]].first[j])
					newExpr[k]=tokens[i+j+1];
			}
		for(auto v: newExpr)
			expr+=" "+ v;
		i+=envFun[tokens[i]].first.size();
	}
}

void Program::run(){
	for(auto i : instructions)
		execute(i);
}

std::istream& operator>>(std::istream& in, Program& p){
	std::string input;
	std::getline(in,input);
	std::vector<std::string> v;
	while(!input.empty()){
		v.push_back(input);
		std::getline(in,input);
	}
	p=Program(v);
	return in;
}
