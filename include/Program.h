#ifndef PROGRAM_H
#define PROGRAM_H value

#include "Expr.h"
#include "ExprTree.h"
#include <iostream>
#include <fstream>
#include <regex>

/**Exception renvoyée lors de l'execution au cas ou une
variable a été utilisée sans initialisation*/
class UndefinedVar{};
class TooFewArguments{};

/**Programme composé d'une suite d'instructions*/
class Program{
public:
	/**Constucteurs*/
	Program(const std::vector<std::string> str);
	Program(const std::string str[]);
	Program();

	/**Execution du programme*/
	void run();
private:
	/**Structure d'une instruction ou l'utilisateur peut
	affecter une valeur à une variable et/ou afficher le
	resultat*/
	struct Instruction{
		Instruction(const std::string& t);
		bool semi=false;
		bool fDef=false;
		std::vector<std::string> lvalue;
		std::string expr;
	};
	/**Liste d'instructions à executer*/
	std::vector<Instruction> instructions;

	/**Execution d'une instruction*/
	void execute(Instruction inst);

	/**Environnement qui associe le nom d'une variable à
	un nombre (réel)*/
	std::map<const std::string, float> envVar;

	void replaceFunc(std::string& expr);
	bool replaceVars(std::string& expr,const std::vector<std::string>& ignore);

	std::map<const std::string,
			 std::pair<std::vector<std::string>,std::string>> envFun;
};
/**Recevoir des chaines de caractères et les transormer
en instructions du programme. Ces chaines de caractères
peuvent être saisies manuellement ou d'un fichier texte.
La saisie s'arrête quand une ligne vide est saisie*/
std::istream& operator>>(std::istream& in, Program& p);

#endif
