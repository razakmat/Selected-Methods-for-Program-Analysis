#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <string>
#include "Visitor.h"
#include "AST.h"
#include <unordered_map>
#include <set>


using namespace std;


class SemanticException : public exception
{
    public:
        SemanticException(const string & msg,const Loc & loc)
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


class SemanticAnalysis : public Visitor
{
    public:
        SemanticAnalysis();
        set<string> & GetAllFields();
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
        unordered_map<string,FunDecl*> m_fun;
        unordered_map<string,IdentifierDecl*> m_var;
        set<string> m_allFields;

};

#endif