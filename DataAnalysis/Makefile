CXX=g++
CXXFLAGS= -std=c++17 -Wall -pedantic -Wno-long-long -g
CXXLIBS=
OBJECTS= ./obj/main.o ./obj/AST.o ./obj/JSONToAST.o ./obj/Normalization.o ./obj/ASTToCFG.o ./obj/CFG.o ./obj/SignLattice.o ./obj/SignAnalysis.o ./obj/ConstLattice.o ./obj/ConstAnalysis.o ./obj/LiveVarLattice.o ./obj/LiveVarAnalysis.o ./obj/AvailableExprAnalysis.o ./obj/AvailableExprLattice.o ./obj/VeryBusyExprAnalysis.o ./obj/ReachingDefAnalysis.o ./obj/ReachingDefLattice.o

all: compile
	
compile: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o ./DataAnalysis $(CXXLIBS)

./obj/%.o: ./src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./obj/%.o: ./src/*/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

./obj/main.o: ./src/main.cpp ./src/AST.h ./src/JSONToAST.h ./src/Normalization.hpp ./src/CFG.h ./src/ASTToCFG.h ./src/MonotoneAnalysis.h ./src/Sign/SignAnalysis.h ./src/Const/ConstAnalysis.h ./src/Live/LiveVarAnalysis.h ./src/Available/AvailableExprAnalysis.h ./src/VeryBusy/VeryBusyExprAnalysis.h ./src/Reaching/ReachingDefAnalysis.h
./obj/AST.o: ./src/AST.cpp ./src/Visitor.h
./obj/JSONToAST.o: ./src/JSONToAST.cpp ./src/AST.h
./obj/Normalization.o: ./src/Normalization.cpp ./src/AST.h ./src/Visitor.h
./obj/ASTToCFG.o: ./src/ASTToCFG.cpp ./src/AST.h
./obj/CFG.o: ./src/CFG.cpp ./src/AST.h
./obj/MonotoneAnalysis.o: ./src/MonotoneAnalysis.cpp ./src/CFG.h ./src/AST.h ./src/Sign/SignAnalysis.h ./src/Const/ConstAnalysis.h ./src/Live/LiveVarAnalysis.h ./src/Available/AvailableExprAnalysis.h ./src/VeryBusy/VeryBusyExprAnalysis.h ./src/Reaching/ReachingDefAnalysis.h
./obj/SignLattice.o: ./src/Sign/SignLattice.cpp
./obj/SignAnalysis.o: ./src/Sign/SignAnalysis.cpp ./src/Sign/SignLattice.h ./src/CFG.h ./src/AST.h
./obj/ConstLattice.o: ./src/Const/ConstLattice.cpp
./obj/ConstAnalysis.o: ./src/Const/ConstAnalysis.cpp ./src/Const/ConstLattice.h ./src/CFG.h ./src/AST.h
./obj/LiveVarLattice.o: ./src/Live/LiveVarLattice.cpp
./obj/LiveVarAnalysis.o: ./src/Live/LiveVarAnalysis.cpp ./src/Live/LiveVarLattice.h ./src/CFG.h ./src/AST.h
./obj/AvailableExprLattice.o: ./src/Available/AvailableExprLattice.cpp
./obj/AvailableExprAnalysis.o: ./src/Available/AvailableExprAnalysis.cpp ./src/Available/AvailableExprLattice.h ./src/CFG.h ./src/AST.h
./obj/VeryBusyExprAnalysis.o: ./src/VeryBusy/VeryBusyExprAnalysis.cpp ./src/Available/AvailableExprLattice.h ./src/CFG.h ./src/AST.h
./obj/ReachingDefLattice.o: ./src/Reaching/ReachingDefLattice.cpp
./obj/ReachingDefAnalysis.o: ./src/Reaching/ReachingDefAnalysis.cpp ./src/Reaching/ReachingDefLattice.h ./src/CFG.h ./src/AST.h

clean:	
	rm ./obj/*.o 
	rm DataAnalysis


