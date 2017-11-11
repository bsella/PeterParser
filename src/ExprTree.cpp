#include "ExprTree.h"
#include "parser.h"

ExprTree::ExprTree(const char* str){
	if(!is_valid(str)) throw InvalidExpr();
	auto rpnDeque=RPN(str);
	if(rpnDeque.empty())
		throw InvalidExpr();
	int v = rpnDeque.back().variables; //Je sais, c'est pas bien de faire ça
	token= new ExprToken(rpnDeque.back().strToken);
	token->variables=v;
	rpnDeque.pop_back();
	if(!token->number)
		for(int i=0; i< token->variables; i++)
			next.push_back( new ExprTree(rpnDeque));
}
ExprTree::ExprTree(std::deque<ExprToken>& rpn) throw (InvalidExpr){
	if(rpn.empty())
		throw InvalidExpr();
	int v= rpn.back().variables;
	token= new ExprToken(rpn.back().strToken);
	token->variables=v;
	rpn.pop_back();
	if(!token->number)
		for(int i=0; i< token->variables; i++)
			next.push_back( new ExprTree(rpn));
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
