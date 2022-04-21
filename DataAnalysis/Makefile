CXX=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wno-long-long -O3 -g
CXXLIBS=
OBJECTS= ./src/main.o ./src/AST.o ./src/JSONToAST.o ./src/Normalization.o ./src/ASTToCFG.o ./src/CFG.o

all: compile
	
compile: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o ./DataAnalysis $(CXXLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./src/main.o: ./src/main.cpp ./src/AST.h ./src/JSONToAST.h ./src/Normalization.hpp ./src/ASTToCFG.h
./src/AST.o: ./src/AST.cpp ./src/Visitor.h
./src/JSONToAST.o: ./src/JSONToAST.cpp ./src/AST.h
./src/Normalization.o: ./src/Normalization.cpp ./src/AST.h ./src/Visitor.h
./src/ASTToCFG.o: ./src/ASTToCFG.cpp ./src/AST.h
./src/CFG.o: ./src/CFG.cpp ./src/AST.h

clean:	
	rm ./src/*.o 
	rm DataAnalysis


