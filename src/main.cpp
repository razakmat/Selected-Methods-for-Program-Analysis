#include <iostream>
#include <fstream>
#include "JSONToAST.h"
#include "AST.h"
#include "Interpreter.h"
#include "SemanticAnalysis.h"

using namespace std;




int main(int argc, char** argv)
{
    bool ascii = false;
    if (argc != 2 && argc != 3)
    {
        cout << "Wrong number of arguments" << endl;
        return 0;
    }
    else if (argc == 3)
    {
        if (argv[2][0] == '0')
            ascii = true;
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

    try{
        SemanticAnalysis semantic;
        semantic.visit(program);
    }catch(SemanticException& e){
        cout << e.what() << endl;
        delete program;
        return 1;
    }


    Interpreter Eval(ascii);
    try{
        Eval.visit(program);
    }catch(InterpreterException & e){
        cout << e.what() << endl;
    }

    delete program;
    return 0;
}
