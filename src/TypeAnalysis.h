#ifndef __TYPEANALYSIS_H__
#define __TYPEANALYSIS_H__

#include <string>
#include "VisitorType.h"
#include "Type.h"
#include "Unification.h"
#include "AST.h"
#include <unordered_map>
#include <memory>
#include <set>

using namespace std;

class TypeAnalysis : public VisitorType
{
    public:
        TypeAnalysis(set<string> & allFields);
        void StartAnalysis(Program * Program);
        shared_ptr<Type> visit ( Number * integer);
        shared_ptr<Type> visit ( Null * null);
        shared_ptr<Type> visit ( Identifier * id);
        shared_ptr<Type> visit ( BinaryOp * Binop);
        shared_ptr<Type> visit ( CallFuncExpr * call);
        shared_ptr<Type> visit ( Input * input);
        shared_ptr<Type> visit ( Alloc * alloc);
        shared_ptr<Type> visit ( VarRef * varRef);
        shared_ptr<Type> visit ( Deref * deref);
        shared_ptr<Type> visit ( RecordField * recordField);
        shared_ptr<Type> visit ( Record * record);
        shared_ptr<Type> visit ( FieldAccess * fieldAccess);
        shared_ptr<Type> visit ( AssignStmt * assignStmt);
        shared_ptr<Type> visit ( NestedBlockStmt * nestedBlockStmt);
        shared_ptr<Type> visit ( IdentifierDecl * idDecl);
        shared_ptr<Type> visit ( VarStmt * varStmt);
        shared_ptr<Type> visit ( ReturnStmt * returnStmt);
        shared_ptr<Type> visit ( FunBlockStmt * funBlockStmt);
        shared_ptr<Type> visit ( IfStmt * ifStmt);
        shared_ptr<Type> visit ( WhileStmt * whileStmt);
        shared_ptr<Type> visit ( OutputStmt * output);
        shared_ptr<Type> visitParam(IdentifierDecl * idDecl);
        shared_ptr<Type> visit ( FunDecl * funDecl);
        shared_ptr<Type> visit ( Program * Program);
    private:
        Unification m_uni;
        unordered_map<string,shared_ptr<Type>> m_varMap;
        unordered_map<shared_ptr<Type>,Decl*> m_typeMap;
        unordered_map<string,shared_ptr<Type>> m_fields;
        shared_ptr<Type> m_int;
        shared_ptr<Type> m_AbsentField;
        set<string> m_allFields;
        vector<shared_ptr<Type>> m_allAccessFields;
};




#endif // __TYPEANALYSIS_H__