#include "Program.h"

int main(int argc, char *argv[]){
	Program p;
	if(argc==1)
		std::cin >> p;
	else if(argc==2){
		std::ifstream testFile;
		testFile.open(argv[1]);
		testFile >> p;
	} else{
		std::cerr << "Usage: "<<argv[0]<<" [programName]" << std::endl;
		return 1;
	}
	p.run();
	return 0;
}