#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "../../DataAnalysis/src/JSONToAST.h"
#include "../../DataAnalysis/src/AST.h"
#include "../../DataAnalysis/src/Normalization.hpp"
#include "../../DataAnalysis/src/ASTToCFG.h"
#include "../../DataAnalysis/src/CFG.h"
#include "../../DataAnalysis/src/MonotoneAnalysis.h"
#include "../../Pointers/src/Andersen.hpp"
#include "../../Pointers/src/Steensgaard.hpp"
#include "./NullAnalysis.h"

using namespace std;


int main(int argc, char** argv)
{
    if (argc != 2 && argc != 3)
    {
        cout << "Wrong number of arguments" << endl;
        return 0;
    }

    ifstream file(argv[1]);

    JSONToAST converter;
    Program * program = nullptr;

    try{
        program = converter.convert(file);
    }catch(ParserException& e){
        cout << e.what() << endl;
        return 1;
    }

    Normalization normal;
    Program * now = normal.visit(program);
    delete program;
    program = now;

    ASTToCFG cfg;
    cfg.CreateCFG(program);
    vector<CFGNode*> & code = cfg.GetVectorNodes();


    map<string,vector<string>> res;

    if (argc == 3)
    {
        Steensgaard analysis(program);
        analysis.Start();
        analysis.GetResult(res);
    }
    else
    {
        Andersen analysis(program);
        analysis.Start();
        analysis.GetResult(res);
    }


    NullAnalysis::Prepare(res,code);
    MonotoneAnalysis<NullAnalysis> mon(code);
    mon.Worklist();

    for (auto & x : code)
        x->PrintOut(cout);

    delete program;
    return 0;
}
