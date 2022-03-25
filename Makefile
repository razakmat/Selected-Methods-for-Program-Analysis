CXX=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wno-long-long -O3
CXXLIBS=

all: compile
	
compile: ./src/main.o ./src/AST.o ./src/JSONToAST.o ./src/Interpreter.o ./src/Memory.o ./src/SemanticAnalysis.o ./src/Type.o  ./src/Unification.o ./src/TypeAnalysis.o
	$(CXX) $(CXXFLAGS) $^ -o APR $(CXXLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./src/main.o: ./src/main.cpp ./src/AST.h ./src/JSONToAST.h ./src/SemanticAnalysis.h ./src/Interpreter.h ./src/TypeAnalysis.h ./src/TypeException.h
./src/AST.o: ./src/AST.cpp ./src/Visitor.h ./src/VisitorType.h ./src/Memory.h
./src/JSONToAST.o: ./src/JSONToAST.cpp ./src/AST.h
./src/Interpreter.o: ./src/Interpreter.cpp ./src/Visitor.h ./src/AST.h ./src/Memory.h
./src/Memory.o: ./src/Memory.cpp ./src/AST.h
./src/SemanticAnalysis.o: ./src/SemanticAnalysis.cpp ./src/AST.h ./src/Visitor.h
./src/Type.o: ./src/Type.cpp ./src/AST.h
./src/Unification.o: ./src/Unification.cpp ./src/TypeException.h ./src/Type.h
./src/TypeAnalysis.o: ./src/TypeAnalysis.cpp ./src/TypeException.h ./src/VisitorType.h ./src/Type.h ./src/Unification.h ./src/AST.h
clean:	
	rm ./src/*.o 
	rm APR


