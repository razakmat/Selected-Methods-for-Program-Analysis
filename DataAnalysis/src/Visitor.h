#ifndef VISITOR_H
#define VISITOR_H

#include "AST.h"

using namespace std;

class Visitor
{
    public:
        virtual Number * visit ( Number * integer) = 0;
        virtual Null * visit ( Null * null) = 0;
        virtual Identifier * visit ( Identifier * id) = 0;
        virtual BinaryOp * visit ( BinaryOp * Binop) = 0;
        virtual CallFuncExpr * visit ( CallFuncExpr * call) = 0;
        virtual Input * visit ( Input * input) = 0;
        virtual Alloc * visit ( Alloc * alloc) = 0;
        virtual VarRef * visit ( VarRef * varRef) = 0;
        virtual Deref * visit ( Deref * deref) = 0;
        virtual RecordField * visit ( RecordField * recordField) = 0;
        virtual Record * visit ( Record * record) = 0;
        virtual FieldAccess * visit ( FieldAccess * fieldAccess) = 0;
        virtual AssignStmt * visit ( AssignStmt * assignStmt) = 0;
        virtual NestedBlockStmt * visit ( NestedBlockStmt * nestedBlockStmt) = 0;
        virtual IdentifierDecl * visit ( IdentifierDecl * idDecl) = 0;
        virtual VarStmt * visit ( VarStmt * varStmt) = 0;
        virtual ReturnStmt * visit ( ReturnStmt * returnStmt) = 0;
        virtual FunBlockStmt * visit ( FunBlockStmt * funBlockStmt) = 0;
        virtual IfStmt * visit ( IfStmt * ifStmt) = 0;
        virtual WhileStmt * visit ( WhileStmt * whileStmt) = 0;
        virtual OutputStmt * visit ( OutputStmt * output) = 0;
        virtual FunDecl * visit ( FunDecl * funDecl) = 0;
        virtual Program * visit ( Program * Program) = 0;
};







#endif