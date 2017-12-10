#include "Program.h"

int main(int argc, char *argv[]){
	Program p;
	switch(argc){
	case 0:
		std::cerr << "Usage: "<<argv[0]<<" [programName]" << std::endl;
		return 1;
	case 1:
		std::cout << "Calculette_V3.0" << std::endl;
		std::cout << "try ./peterParser -h for help" << std::endl;
		std::cout << std::endl;
		std::cin >> p;
		p.run();
		break;
	case 2:
		if(!strcmp(argv[1],"-h")){
			std::cout << "\033[1mUSAGE\033[0m" << std::endl;
			std::cout << "	./peterParser [-h] [file ...]" << std::endl << std::endl;
			std::cout << "\033[1mINSTRUCTIONS\033[0m" << std::endl;
			std::cout << "	[var [p ...] =][expr][;]" << std::endl << std::endl;
			std::cout << "\033[1mEXPRESSIONS\033[0m" << std::endl;
			std::cout << "	2 +(3+1) * 5/1" << std::endl;
			std::cout << "	sin 3.1415926" <<std::endl;
			std::cout << "	a = 15" << std::endl;
			std::cout << "	f x = pow 2 x;" << std::endl;
			std::cout << "	square = pow 2" << std::endl;
			std::cout << "	middle a b = lerp a b .5;" << std::endl;
			return 0;
		}
	default:
		for(int i=1; i<argc; i++){
			p=Program();
			std::ifstream testFile;

			std::cout << "\033[1;32m"<<argv[i]<<":\033[0m" <<std::endl;
			testFile.open(argv[i]);
			std::string line;
			while(std::getline(testFile,line))
				std::cout << line << std::endl;
			std::cout << std::endl;
			testFile.close();

			testFile.open(argv[i]);
			testFile >> p;
			p.run();
			testFile.close();
		}
	}
	return 0;
}
