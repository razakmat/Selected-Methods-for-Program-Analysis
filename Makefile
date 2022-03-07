CXX=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wno-long-long -O3
CXXLIBS=

all: compile
	
compile: ./src/main.o ./src/AST.o ./src/JSONToAST.o ./src/Interpreter.o ./src/Memory.o ./src/SemanticAnalysis.o
	$(CXX) $(CXXFLAGS) $^ -o interpreter $(CXXLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./src/main.o: ./src/main.cpp ./src/AST.h ./src/JSONToAST.h ./src/SemanticAnalysis.h
./src/AST.o: ./src/AST.cpp ./src/Visitor.h
./src/JSONToAST.o: ./src/JSONToAST.cpp ./src/AST.h
./src/Interpreter.o: ./src/Interpreter.cpp ./src/Visitor.h ./src/AST.h
./src/Memory.o: ./src/Memory.cpp ./src/AST.h
./src/SemanticAnalysis.o: ./src/SemanticAnalysis.cpp ./src/AST.h ./src/Visitor.h
clean:	
	rm ./src/*.o 
	rm interpreter


