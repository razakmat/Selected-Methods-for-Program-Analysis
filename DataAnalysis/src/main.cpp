#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "JSONToAST.h"
#include "AST.h"
#include "Normalization.hpp"

using namespace std;


int main(int argc, char** argv)
{
    if (argc != 2)
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

    ostream & os = cout;
    now->PrintOut(os);

    delete now;
    delete program;
    return 0;
}
