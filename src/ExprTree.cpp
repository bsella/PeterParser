#include "ExprTree.h"
#include "parser.h"

ExprTree::ExprTree(const char* str){
	if(!is_valid(str)) throw InvalidExpr();
	auto rpnDeque=RPN(str);
	if(rpnDeque.empty())
		throw InvalidExpr();
	token= new ExprToken(rpnDeque.back());
	rpnDeque.pop_back();
	if(!token->number)
		for(int i=0; i< token->nbParam; i++)
			next.push_back(new ExprTree(rpnDeque));
}
ExprTree::ExprTree(std::deque<ExprToken>& rpn){
	if(rpn.empty())
		throw InvalidExpr();
	token= new ExprToken(rpn.back());
	rpn.pop_back();
	if(!token->number)
		for(int i=0; i< token->nbParam; i++)
			next.push_back(new ExprTree(rpn));
}

ExprTree::~ExprTree(){
	delete token;
	for(auto n: next)
		delete n;
}
float ExprTree::eval()const{
	if(token->number)
		return std::atof(token->strToken.c_str());
	std::vector<float> vars;
	for (auto n : next)
		vars.insert(vars.begin(),n->eval());
	return envFunDef.at(token->strToken)(vars);
}
