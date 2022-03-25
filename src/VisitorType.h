#ifndef __VISITORTYPE_H__
#define __VISITORTYPE_H__

#include "AST.h"
#include "Type.h"
#include <memory>

using namespace std;

class VisitorType
{
    public:
        virtual shared_ptr<Type> visit ( Number * integer) = 0;
        virtual shared_ptr<Type> visit ( Null * null) = 0;
        virtual shared_ptr<Type> visit ( Identifier * id) = 0;
        virtual shared_ptr<Type> visit ( BinaryOp * Binop) = 0;
        virtual shared_ptr<Type> visit ( CallFuncExpr * call) = 0;
        virtual shared_ptr<Type> visit ( Input * input) = 0;
        virtual shared_ptr<Type> visit ( Alloc * alloc) = 0;
        virtual shared_ptr<Type> visit ( VarRef * varRef) = 0;
        virtual shared_ptr<Type> visit ( Deref * deref) = 0;
        virtual shared_ptr<Type> visit ( RecordField * recordField) = 0;
        virtual shared_ptr<Type> visit ( Record * record) = 0;
        virtual shared_ptr<Type> visit ( FieldAccess * fieldAccess) = 0;
        virtual shared_ptr<Type> visit ( AssignStmt * assignStmt) = 0;
        virtual shared_ptr<Type> visit ( NestedBlockStmt * nestedBlockStmt) = 0;
        virtual shared_ptr<Type> visit ( IdentifierDecl * idDecl) = 0;
        virtual shared_ptr<Type> visit ( VarStmt * varStmt) = 0;
        virtual shared_ptr<Type> visit ( ReturnStmt * returnStmt) = 0;
        virtual shared_ptr<Type> visit ( FunBlockStmt * funBlockStmt) = 0;
        virtual shared_ptr<Type> visit ( IfStmt * ifStmt) = 0;
        virtual shared_ptr<Type> visit ( WhileStmt * whileStmt) = 0;
        virtual shared_ptr<Type> visit ( OutputStmt * output) = 0;
        virtual shared_ptr<Type> visit ( FunDecl * funDecl) = 0;
        virtual shared_ptr<Type> visit ( Program * Program) = 0;
};




#endif // __VISITORTYPE_H__