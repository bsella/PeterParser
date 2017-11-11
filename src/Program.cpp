#include "Program.h"
#include "parser.h"

Program::Program(const std::vector<std::string> str){
	for(auto s : str){
		if(s.empty()) continue;
		instructions.push_back(Instruction(s));
	}
}
Program::Program(const std::string *str){
	for(int i = 0; i< sizeof(str)/sizeof(std::string); i++){
		if(str[i].empty()) continue;
		instructions.push_back(Instruction(str[i]));
	}
}
Program::Program(){}

Program::Instruction::Instruction(const std::string t){
	//vérifier s'il y a un point virgule à la fin de l'instruction
	semi=semiAtEnd(t);
	//vérifier s'il s'agit d'une affection de valeur
	if(!defVar(t,lvalue,expr))
		expr=t;
}

void Program::execute(Instruction inst) throw (UndefinedVar){
	//remplacer chaque variable par sa valeur
	if(!replaceVars(inst.expr,envVar)) throw UndefinedVar();

	//std::cout << inst.expr << std::endl;
	ExprTree expr(inst.expr.c_str());
	float value= expr.eval();
	//mettre à jour la variable
	if(inst.lvalue!="")
		envVar[inst.lvalue]=value;
	//afficher le résultat
	if(!inst.semi)
		std::cout<< value << std::endl;
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
