CPP=g++
SRC=src
INCLUDE=-I include
CCFLAGS=-std=c++11 -Wall
EXE=peterParser
objects=Expr.o ExprTree.o Program.o parser.o main.o

all: $(EXE) testValid

%.o: $(SRC)/%.cpp include/%.h
	$(CPP) -c $(CCFLAGS) $(INCLUDE) $< -o $@

%.o: $(SRC)/%.cpp
	$(CPP) -c $(CCFLAGS) $(INCLUDE) $< -o $@

$(EXE): $(objects)
	$(CPP) $(CCFLAGS) $(objects) -o $(EXE)

testValid: parser.o testValid.o
	$(CPP) $(CCFLAGS) parser.o testValid.o -o $@

clean:
	rm *.o $(EXE) testValid
