#include "parser.h"
#include <iostream>

int main(int argc, char *argv[]){
	for(auto text: {"","()","1","+","abc","sin","(",")",
					"1+","+1","++","(+","(1","1)","(1)",
					"sin 1", "1+1","1 f 1","sin 1+1","(++)",
					"(1+)","pow 2 3", "pow 2+3 2", "pow (2+3) 2",
					"pow 2 3+2"," 2+ 4*(sin 3 *(8+2)/ 7)",
					"polynome 2 1 2 3 15", "polynome 1 1 2 3 8"}){
		std::cout << text <<" ->";
		if(is_valid(text))
			std::cout << "true";
		else
			std::cout << "false";
		std::cout << std::endl;
	}
	return 0;
}
