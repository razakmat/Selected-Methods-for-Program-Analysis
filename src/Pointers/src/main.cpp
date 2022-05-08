#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "../../DataAnalysis/src/JSONToAST.h"
#include "../../DataAnalysis/src/AST.h"
#include "../../DataAnalysis/src/Normalization.hpp"
#include "../../DataAnalysis/src/ASTToCFG.h"
#include "../../DataAnalysis/src/CFG.h"
#include "Andersen.hpp"
#include "Steensgaard.hpp"
#include <map>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 3)
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


    program->PrintOut(cout);


    map<string,vector<string>> res;
    if (strcmp(argv[2],"Steensgaard") == 0)
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

    stringstream str;
    str << endl << "Result:" << endl;
    for (auto & x : res)
    {
        str << "pt(" << x.first << ") = {";
        for (auto & y : x.second)
        {
            str << y;
            if (y != x.second.back())
                str << ",";
        }
        str << "}" << endl;
    }
    cout << str.str();

    delete program;
    return 0;
}
