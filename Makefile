CXX=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wno-long-long -O3
CXXLIBS=
OBJECTS= ./obj/main.o ./obj/AST.o ./obj/JSONToAST.o ./obj/Interpreter.o ./obj/Memory.o ./obj/SemanticAnalysis.o ./obj/Type.o  ./obj/Unification.o ./obj/TypeAnalysis.o

all: compile
	
compile: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o APR $(CXXLIBS)

./obj/%.o: ./src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./obj/main.o: ./src/main.cpp ./src/AST.h ./src/JSONToAST.h ./src/SemanticAnalysis.h ./src/Interpreter.h ./src/TypeAnalysis.h ./src/TypeException.h
./obj/AST.o: ./src/AST.cpp ./src/Visitor.h ./src/VisitorType.h ./src/Memory.h
./obj/JSONToAST.o: ./src/JSONToAST.cpp ./src/AST.h
./obj/Interpreter.o: ./src/Interpreter.cpp ./src/Visitor.h ./src/AST.h ./src/Memory.h
./obj/Memory.o: ./src/Memory.cpp ./src/AST.h
./obj/SemanticAnalysis.o: ./src/SemanticAnalysis.cpp ./src/AST.h ./src/Visitor.h
./obj/Type.o: ./src/Type.cpp ./src/AST.h
./obj/Unification.o: ./src/Unification.cpp ./src/TypeException.h ./src/Type.h
./obj/TypeAnalysis.o: ./src/TypeAnalysis.cpp ./src/TypeException.h ./src/VisitorType.h ./src/Type.h ./src/Unification.h ./src/AST.h

clean:	
	rm ./obj/*.o 
	rm APR


