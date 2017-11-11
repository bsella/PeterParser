#include "Expr.h"
#include "parser.h"
#include <cmath>

Expr::Expr(const char* str):texteExpr(str){
	if(!is_valid(str))
		throw InvalidExpr();
	rpnDeque=RPN(str);
}
Expr::Expr(){}
Expr::ExprToken::ExprToken(const std::string& t):strToken(t){
	bool is_operator=false;
	prio=0;
	for(auto i : {"-","+","/","*","(",")"})
		if (strToken==i){
			variables=2;
			is_operator=true;
			break;
		}
	if(!is_operator)
		for(auto it : {"sin","cos","tan","sqrt","exp","log"})
			if (strToken==it){
				variables=1;
				is_operator=true;
				prio=3;
				break;
			}
	if(!is_operator)
		for(auto it : {"pow","hypot"})
			if (strToken==it){
				variables=2;
				is_operator=true;
				prio=3;
				break;
			}
	if(!is_operator)
		if (strToken=="lerp"){
			variables=3;
			is_operator=true;
			prio=3;
		}
		else if(strToken=="polynome"){
			variables=-1;
			is_operator=true;
			prio=3;
		}


	number= is_number(strToken);
	//renvoyer une exception si le token n'est ni nombre ni opérateur
	if(!number && !is_operator) throw InvalidExpr();
	
	if(strToken=="*" || strToken=="/")
		prio=2;
	else if(strToken=="+" || strToken=="-")
		prio=1;
}

float Expr::eval()const{
	std::stack<Expr::ExprToken> evalStack;
	auto rpnDequeCopy=rpnDeque;
	while(!rpnDequeCopy.empty()){
		if(rpnDequeCopy.front().number)
			evalStack.push(rpnDequeCopy.front());
		else{
			std::vector<float> vars;
			for(int i=0; i<rpnDequeCopy.front().variables; i++){
				vars.insert(vars.begin(),std::atof(evalStack.top().strToken.c_str()));
				evalStack.pop();
			}
			evalStack.push(ExprToken(std::to_string(envFunDef.at(rpnDequeCopy.front().strToken)(vars))));
		}
		rpnDequeCopy.pop_front();
	}
	return std::atof(evalStack.top().strToken.c_str());
}

const std::vector<std::string> Expr::split(std::string s)const{
	return separate(s);
}

std::deque<Expr::ExprToken> Expr::RPN(std::string str)const{
	std::stack<Expr::ExprToken> opStack;
	std::deque<Expr::ExprToken> ret;
	auto tokens=split(str);
	for (auto s = tokens.begin(); s!=tokens.end(); ++s){
		ExprToken exprToken=ExprToken(*s);
		if (exprToken.number)
			ret.push_back(exprToken);
		else{
			if(*s=="(")
				opStack.push(exprToken);
			else if (*s==")"){
				while(!opStack.empty() && opStack.top().strToken!="("){
					ret.push_back(opStack.top());
					opStack.pop();
				}
				opStack.pop();
			}else if (*s=="polynome"){
				s++;
				exprToken.variables=std::stoi(*s)+2;
				opStack.push(exprToken);
			}else{
				while(!opStack.empty() && opStack.top().prio>exprToken.prio){
					ret.push_back(opStack.top());
					opStack.pop();
				}
				opStack.push(exprToken);
			}
		}
	}
	while(!opStack.empty()){
		ret.push_back(opStack.top());
		opStack.pop();
	}
	return ret;
}

void Expr::print()const{
	auto q=RPN(texteExpr);
	std::cout << " RPN( ";
	while(!q.empty()){
		std::cout << q.front().strToken << " ";
		q.pop_front();
	}
	std::cout<<")" <<std::endl;
	std::cout << texteExpr << " = " << eval() << std::endl; 
}

const std::map<std::string, std::function<float(std::vector<float>)>> Expr::envFunDef={
	{"+",[](std::vector<float> v){if(v.size()!=2)throw InvalidExpr(); return v[0]+v[1];}},
	{"-",[](std::vector<float> v){if(v.size()!=2)throw InvalidExpr(); return v[0]-v[1];}},
	{"*",[](std::vector<float> v){if(v.size()!=2)throw InvalidExpr(); return v[0]*v[1];}},
	{"/",[](std::vector<float> v){if(v.size()!=2)throw InvalidExpr(); return v[0]/v[1];}},
	{"sin",[](std::vector<float> v){if(v.size()!=1)throw InvalidExpr(); return std::sin(v[0]);}},
	{"cos",[](std::vector<float> v){if(v.size()!=1)throw InvalidExpr(); return std::cos(v[0]);}},
	{"tan",[](std::vector<float> v){if(v.size()!=1)throw InvalidExpr(); return std::tan(v[0]);}},
	{"sqrt",[](std::vector<float> v){if(v.size()!=1)throw InvalidExpr(); return std::sqrt(v[0]);}},
	{"log",[](std::vector<float> v){if(v.size()!=1)throw InvalidExpr(); return std::log(v[0]);}},
	{"exp",[](std::vector<float> v){if(v.size()!=1)throw InvalidExpr(); return std::exp(v[0]);}},
	{"pow",[](std::vector<float> v){if(v.size()!=2)throw InvalidExpr(); return std::pow(v[1],v[0]);}},
	{"hypot",[](std::vector<float> v){if(v.size()!=2)throw InvalidExpr(); return std::sqrt(v[1]*v[1]+v[0]*v[0]);}},
	{"lerp",[](std::vector<float> v){
		if(v.size()!=3 ||v[2]>1||v[2]<0) throw InvalidExpr();
		return v[2]*v[1]+(1-v[2])*v[0];
	}},
	{"polynome",[](std::vector<float> v){
		if(v.size()<2)throw InvalidExpr();
		float sum=0;
		for(int i=0; i<v.size()-1; i++)
			sum+=v[i]*std::pow(v[v.size()-1],i);
		return sum;}}
};