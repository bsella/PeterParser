#ifndef EXPR_TREE_H
#define EXPR_TREE_H value

#include "Expr.h"

/**La structure représentant l'arbre binaire qui contient les Tokens
qui constitue l'expression*/
class ExprTree: public Expr{
public:
	/**Construire un arbre de Tokens en utilisant une chaine
	de characteres. Ce constructeur appelle un constructeur
	récursif au cas ou la profondeur de l'arbre est surperieur
	à 0*/
	ExprTree(const char* str);
	virtual ~ExprTree();
	
	/**Retourne la valeur de l'expression*/
	float eval()const;
	// void print();
private:
	/**Constructeur récurcif*/
	ExprTree(std::deque<ExprToken>& rpnDeque) throw (InvalidExpr);
	
	/**Retoune la valeur de l'expression (récursif)*/
	float evalTree()const;
	ExprToken* token = nullptr;
	std::vector<ExprTree *> next;
};

#endif