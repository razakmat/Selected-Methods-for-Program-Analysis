#ifndef __NORMALIZATION_H__
#define __NORMALIZATION_H__

#include "AST.h"
#include "Visitor.h"
#include <vector>
#include <string>

using namespace std;

class Normalization : public Visitor
{
    public:
        Normalization();
        Number * visit ( Number * integer);
        Null * visit ( Null * null);
        Identifier * visit ( Identifier * id);
        BinaryOp * visit ( BinaryOp * Binop);
        CallFuncExpr * visit ( CallFuncExpr * call);
        Input * visit ( Input * input);
        Alloc * visit ( Alloc * alloc);
        VarRef * visit ( VarRef * varRef);
        Deref * visit ( Deref * deref);
        RecordField * visit ( RecordField * recordField);
        Record * visit ( Record * record);
        FieldAccess * visit ( FieldAccess * fieldAccess);
        AssignStmt * visit ( AssignStmt * assignStmt);
        NestedBlockStmt * visit ( NestedBlockStmt * nestedBlockStmt);
        IdentifierDecl * visit ( IdentifierDecl * idDecl);
        VarStmt * visit ( VarStmt * varStmt);
        ReturnStmt * visit ( ReturnStmt * returnStmt);
        FunBlockStmt * visit ( FunBlockStmt * funBlockStmt);
        IfStmt * visit ( IfStmt * ifStmt);
        WhileStmt * visit ( WhileStmt * whileStmt);
        OutputStmt * visit ( OutputStmt * output);
        FunDecl * visit ( FunDecl * funDecl);
        Program * visit ( Program * program);
    protected:
        void CheckAndCreateNewExpr(Expr *& e);
        void AddNewExprs(vector<StmtInNestedBlock*> & stmts);
        vector<string> m_newVar;
        vector<Expr*> m_newExpr;
        bool m_SExp;
        int m_idName;
        int m_currentName;
        int m_col;
        static const int m_tab = 3;

};


#endif // __NORMALIZATION_H__