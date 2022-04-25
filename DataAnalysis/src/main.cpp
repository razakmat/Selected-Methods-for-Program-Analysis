#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "JSONToAST.h"
#include "AST.h"
#include "Normalization.hpp"
#include "ASTToCFG.h"
#include "CFG.h"
#include "MonotoneAnalysis.h"
#include "Sign/SignAnalysis.h"
#include "Const/ConstAnalysis.h"
#include "Live/LiveVarAnalysis.h"
#include "Available/AvailableExprAnalysis.h"
#include "VeryBusy/VeryBusyExprAnalysis.h"
#include "Reaching/ReachingDefAnalysis.h"

using namespace std;


int main(int argc, char** argv)
{
    if (argc != 4)
    {
        cout << "Wrong number of arguments" << endl;
        return 0;
    }

    ifstream file(argv[1]);
    if (!file.good())
    {
        cout << "Something wrong with file - " << argv[1] << endl;
        return 0; 
    }


    JSONToAST converter;
    Program * program = nullptr;

    try{
        program = converter.convert(file);
    }catch(ParserException& e){
        cout << e.what() << endl;
        return 1;
    }

    if (strcmp(argv[2],"norm") == 0)
    {
        Normalization normal;
        Program * now = normal.visit(program);
        delete program;
        program = now;
    }

    ASTToCFG cfg;
    cfg.CreateCFG(program);
    vector<CFGNode*> & code = cfg.GetVectorNodes();
    vector<string> & names = cfg.GetVectorNames();

    if (strcmp(argv[3],"sign") == 0)
    {
        SignAnalysis::m_decl = names;
        MonotoneAnalysis<SignAnalysis> mon(code);
        mon.Worklist();
    }
    else if (strcmp(argv[3],"const") == 0)
    {
        ConstAnalysis::m_decl = names;
        MonotoneAnalysis<ConstAnalysis> mon(code);
        mon.Worklist();
    }
    else if (strcmp(argv[3],"live") == 0)
    {
        MonotoneAnalysis<LiveVarAnalysis> mon(code);
        mon.Worklist();
    }
    else if (strcmp(argv[3],"available") == 0)
    {
        MonotoneAnalysis<AvailableExprAnalysis> mon(code);
        mon.Worklist();
    }
    else if (strcmp(argv[3],"busy") == 0)
    {
        MonotoneAnalysis<VeryBusyExprAnalysis> mon(code);
        mon.Worklist();
    }
    else if (strcmp(argv[3],"reaching") == 0)
    {
        MonotoneAnalysis<ReachingDefAnalysis> mon(code);
        mon.Worklist();
    }

    for (auto & x : code)
        x->PrintOut(cout);

    delete program;
    return 0;
}
