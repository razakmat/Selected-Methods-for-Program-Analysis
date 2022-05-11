CXX=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wno-long-long -O3
CXXLIBS=
OBJECTS= ./obj/main.o ./obj/AST.o ./obj/JSONToAST.o ./obj/Normalization.o ./obj/ASTToCFG.o ./obj/CFG.o ./obj/Andersen.o ./obj/Steensgaard.o

all: compile
	
compile: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o ./Pointers $(CXXLIBS)

./obj/%.o: ./src/%.cpp 
	$(CXX) $(CXXFLAGS) -c $< -o $@

./obj/%.o: ../DataAnalysis/src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./obj/main.o: ./src/main.cpp ../DataAnalysis/src/AST.h ../DataAnalysis/src/JSONToAST.h ../DataAnalysis/src/Normalization.hpp ../DataAnalysis/src/CFG.h ../DataAnalysis/src/ASTToCFG.h ./src/Andersen.hpp ./src/Steensgaard.hpp
./obj/AST.o: ../DataAnalysis/src/AST.cpp ../DataAnalysis/src/Visitor.h
./obj/JSONToAST.o: ../DataAnalysis/src/JSONToAST.cpp ../DataAnalysis/src/AST.h
./obj/Normalization.o: ../DataAnalysis/src/Normalization.cpp ../DataAnalysis/src/AST.h ../DataAnalysis/src/Visitor.h
./obj/ASTToCFG.o: ../DataAnalysis/src/ASTToCFG.cpp ../DataAnalysis/src/AST.h
./obj/CFG.o: ../DataAnalysis/src/CFG.cpp ../DataAnalysis/src/AST.h
./obj/Andersen.o: ./src/Andersen.cpp ../DataAnalysis/src/AST.h
./obj/Steensgaard.o: ./src/Steensgaard.cpp ../DataAnalysis/src/AST.h

clean:	
	rm ./obj/*.o 
	rm Pointers

