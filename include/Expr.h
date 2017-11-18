#ifndef EXPR_H
#define EXPR_H value

#include <string>
#include <vector>
#include <deque>
#include <stack>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <regex>

/**Exception renvoyée par les méthodes de la classe Expr en cas
de problèmes:
	-Aucune chaine de caractères "utile" est utilisée pour la
	construction de Expr
	-Un caractère qui ne représente ni un opérateur ni un nombre
	est utilisé pour initialiser un token*/
class InvalidExpr{};

/**La classe Expr représente une expression mathématique qui peut
être constituée d'opérateurs et de nombres (réels)*/
class Expr{
public:
	/**Construit une expression a partir de texte*/
	Expr(const char * str);
	Expr();
	/**Retourne la valeur de l'expression*/
	float eval() const;
	/**Affiche la representation interne*/
	void print()const;
protected:
	/**Une structure resprésentant un token d'expression qui contient:
		-Un boolean pour déterminer si l'expression est un nombre
		-Un entier représentant la priorité de l'expression
		-Une chaine de caractères qui représente l'expression*/
	struct ExprToken{
		ExprToken(const std::string& t);
		std::string strToken;
		bool number;
		int prio;
		int nbParam;
	};

	/**Cette méthode prend une liste de chaines de caractères et
	la rêarange selon la notation polonaise inversée*/
	std::deque<ExprToken> RPN(std::string str)const;
	
	/**Environnement qui associe les fonctions définies par
	défaut dans la classe Program*/
	static const std::map<std::string, std::function<float(std::vector<float>)>> envFunDef;
private:	
	/**Cette fonction découpe et filtre une chaine de caractère
	en plusieurs chaines selon s'il s'agit d'un nombre ou un 
	opérateur*/
	const std::vector<std::string> split(std::string s)const;
	
	/**Une file doublement "chainée" retournée par RPN*/
	std::deque<ExprToken> rpnDeque;
	
	/*Le texte utilisé pour "construire" l'expression*/
	const std::string texteExpr;
};

#endif
