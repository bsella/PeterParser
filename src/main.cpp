#include "Program.h"

int main(int argc, char *argv[]){
	Program p;
	switch(argc){
	case 0:
		std::cerr << "Usage: "<<argv[0]<<" [programName]" << std::endl;
		return 1;
	case 1:
		std::cin >> p;
		p.run();
		break;
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