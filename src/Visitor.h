#ifndef VISITOR_H
#define VISITOR_H

#include "AST.h"

using namespace std;

class Object;

class Visitor
{
    public:
        virtual Object * visit ( Number * integer) = 0;
        virtual Object * visit ( Null * null) = 0;
        virtual Object * visit ( Identifier * id) = 0;
        virtual Object * visit ( BinaryOp * Binop) = 0;
        virtual Object * visit ( CallFuncExpr * call) = 0;
        virtual Object * visit ( Input * input) = 0;
        virtual Object * visit ( Alloc * alloc) = 0;
        virtual Object * visit ( VarRef * varRef) = 0;
        virtual Object * visit ( Deref * deref) = 0;
        virtual Object * visit ( RecordField * recordField) = 0;
        virtual Object * visit ( Record * record) = 0;
        virtual Object * visit ( FieldAccess * fieldAccess) = 0;
        virtual Object * visit ( AssignStmt * assignStmt) = 0;
        virtual Object * visit ( NestedBlockStmt * nestedBlockStmt) = 0;
        virtual Object * visit ( IdentifierDecl * idDecl) = 0;
        virtual Object * visit ( VarStmt * varStmt) = 0;
        virtual Object * visit ( ReturnStmt * returnStmt) = 0;
        virtual Object * visit ( FunBlockStmt * funBlockStmt) = 0;
        virtual Object * visit ( IfStmt * ifStmt) = 0;
        virtual Object * visit ( WhileStmt * whileStmt) = 0;
        virtual Object * visit ( OutputStmt * output) = 0;
        virtual Object * visit ( FunDecl * funDecl) = 0;
        virtual Object * visit ( Program * Program) = 0;
};







#endif