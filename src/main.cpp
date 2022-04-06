#include <iostream>
#include <fstream>
#include <set>
#include <string>
#include "JSONToAST.h"
#include "AST.h"
#include "Interpreter.h"
#include "SemanticAnalysis.h"
#include "TypeAnalysis.h"
#include "TypeException.h"

using namespace std;




int main(int argc, char** argv)
{
    bool ascii = false;
    if (argc != 3 && argc != 4)
    {
        cout << "Wrong number of arguments" << endl;
        return 0;
    }
    else if (argc == 4)
    {
        if (argv[3][0] == '0')
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

    SemanticAnalysis semantic;
    try{
        semantic.visit(program);
    }catch(SemanticException& e){
        cout << e.what() << endl;
        delete program;
        return 1;
    }
    set<string> & fields = semantic.GetAllFields();

    if (strcmp(argv[2],"type") == 0)
    {
        try{
            TypeAnalysis types(fields);
            types.StartAnalysis(program);
        }catch(TypeException& e){
            cout << e.what() << endl;
            return 1;
        }
    }
    else if (strcmp(argv[2],"run") == 0)
    {
        Interpreter Eval(ascii);
        try{
            Eval.visit(program);
        }catch(InterpreterException & e){
            cout << e.what() << endl;
        }
    }

    delete program;
    return 0;
}
