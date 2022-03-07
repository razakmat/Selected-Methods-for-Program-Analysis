#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include "Visitor.h"
#include "AST.h"
#include "Memory.h"


using namespace std;


class InterpreterException : public exception
{
    public:
        InterpreterException(const string & msg,const Loc & loc)
        {
           m_msg = msg + " [" + to_string(loc.m_line) + "," + to_string(loc.m_col) + "] ";
        }
    
        virtual const char* what() const noexcept override
        {
            return m_msg.c_str();
        }
    private:
        string m_msg;
}; 


class Interpreter : public Visitor
{
    public:
        Interpreter(bool ascii);
        Object * visit ( Number * integer);
        Object * visit ( Null * null);
        Object * visit ( Identifier * id);
        Object * visit ( BinaryOp * Binop);
        Object * visit ( CallFuncExpr * call);
        Object * visit ( Input * input);
        Object * visit ( Alloc * alloc);
        Object * visit ( VarRef * varRef);
        Object * visit ( Deref * deref);
        Object * visit ( RecordField * recordField);
        Object * visit ( Record * record);
        Object * visit ( FieldAccess * fieldAccess);
        Object * visit ( AssignStmt * assignStmt);
        Object * visit ( NestedBlockStmt * nestedBlockStmt);
        Object * visit ( IdentifierDecl * idDecl);
        Object * visit ( VarStmt * varStmt);
        Object * visit ( ReturnStmt * returnStmt);
        Object * visit ( FunBlockStmt * funBlockStmt);
        Object * visit ( IfStmt * ifStmt);
        Object * visit ( WhileStmt * whileStmt);
        Object * visit ( OutputStmt * output);
        Object * visit ( FunDecl * funDecl);
        Object * visit ( Program * Program);
    private:
        Env m_memory;
        bool m_ascii = false;
};

#endif