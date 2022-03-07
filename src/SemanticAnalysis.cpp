#include "SemanticAnalysis.h"
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <map>
#include <set>

using namespace std;

SemanticAnalysis::SemanticAnalysis()
{
}


Object * SemanticAnalysis::visit ( Number * integer)
{
    return nullptr;
}
Object * SemanticAnalysis::visit ( Null * null)
{
    return nullptr;
}
Object * SemanticAnalysis::visit ( Identifier * id)
{
    auto it = m_var.find(id->m_name);
    if (it == m_var.end())
    {
        auto it1 = m_fun.find(id->m_name);
        if (it1 == m_fun.end())
            throw SemanticException("Error : Using undeclared variable \"" + 
                                    id->m_name + "\".",id->m_loc);
    }
    return nullptr;
}
Object * SemanticAnalysis::visit ( BinaryOp * Binop)
{
    if (typeid(*Binop->m_left) == typeid(Identifier))
    {
        Identifier * id = reinterpret_cast<Identifier*>(Binop->m_left);
        auto it = m_fun.find(id->m_name);
        if (it != m_fun.end())
            throw SemanticException("Error: Cannot compare functions.",id->m_loc);
    }

    if (typeid(*Binop->m_right) == typeid(Identifier))
    {
        Identifier * id = reinterpret_cast<Identifier*>(Binop->m_right);
        auto it = m_fun.find(id->m_name);
        if (it != m_fun.end())
            throw SemanticException("Error: Cannot compare functions.",id->m_loc);
    }
    Binop->m_left->Accept(*this);
    Binop->m_right->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( CallFuncExpr * call)
{
    if (typeid(*call->m_targetFun) == typeid(Identifier))
    {
        Identifier * id = reinterpret_cast<Identifier*>(call->m_targetFun);
        auto it = m_fun.find(id->m_name);
        if (it == m_fun.end())
            throw SemanticException("Error : Using undeclared function name \"" + 
                                      id->m_name + "\".",id->m_loc);
        if (it->second->m_params.size() != call->m_arg.size())
            throw SemanticException("Error : Wrong number of arguments in call of function \"" + 
                                      id->m_name + "\".",id->m_loc);
    }
    for (uint i = 0; i < call->m_arg.size(); i++)
    {
        call->m_arg[i]->Accept(*this);
    }


    return nullptr;
}
Object * SemanticAnalysis::visit ( Input * input)
{
    return nullptr;
}
Object * SemanticAnalysis::visit ( Alloc * alloc)
{
    alloc->m_expr->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( VarRef * varRef)
{
    auto it = m_fun.find(varRef->m_id->m_name);
    if (it != m_fun.end())
        throw SemanticException("Error : Cannot take address of a function( \"" + varRef->m_id->m_name + "\" ).",varRef->m_loc);
    varRef->m_id->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( Deref * deref)
{
    if (typeid(*deref->m_pointer) == typeid(Identifier))
    {
        Identifier * id = reinterpret_cast<Identifier*>(deref->m_pointer);
        auto it = m_fun.find(id->m_name);
        if (it != m_fun.end())
            throw SemanticException("Error: Function is not a pointer.",id->m_loc);
    }
    deref->m_pointer->Accept(*this);
    return nullptr;
}

Object * SemanticAnalysis::visit ( RecordField * recordField)
{
    return nullptr;
}
Object * SemanticAnalysis::visit ( Record * record)
{
    set<string> fiel; 
    for (uint i = 0; i < record->m_fields.size(); i++)
    {
        auto it = fiel.insert(record->m_fields[i]->m_name);
        if (!it.second)
            throw SemanticException("Error : Duplicate field name \"" + 
                                    record->m_fields[i]->m_name +"\".",record->m_fields[i]->m_loc);
        record->m_fields[i]->Accept(*this);
    }
    return nullptr;
}
Object * SemanticAnalysis::visit ( FieldAccess * fieldAccess)
{
    return nullptr;
}
Object * SemanticAnalysis::visit ( AssignStmt * assignStmt)
{
    if (typeid(*assignStmt->m_left) != typeid(Identifier) &&
        typeid(*assignStmt->m_left) != typeid(Deref) &&
        typeid(*assignStmt->m_left) != typeid(FieldAccess))
        throw SemanticException("Error : Invalid assignment.",assignStmt->m_loc);

    if (typeid(*assignStmt->m_left) == typeid(Identifier))
    {
        Identifier * id = reinterpret_cast<Identifier*>(assignStmt->m_left);
        auto it = m_fun.find(id->m_name);
        if (it != m_fun.end())
            throw SemanticException("Error : Cannot assign to the function.",id->m_loc);
    }
    
    assignStmt->m_left->Accept(*this);
    assignStmt->m_right->Accept(*this);
    return nullptr;
}



Object * SemanticAnalysis::visit ( NestedBlockStmt * nestedBlockStmt)
{
    for (uint i = 0; i < nestedBlockStmt->m_body.size(); i++)
    {
        nestedBlockStmt->m_body[i]->Accept(*this);
    }
    return nullptr;
}
Object * SemanticAnalysis::visit ( IdentifierDecl * idDecl)
{
    return nullptr;
}
Object * SemanticAnalysis::visit ( VarStmt * varStmt)
{
    for (uint i = 0; i < varStmt->m_decls.size(); i++)
    {
        auto it = m_var.insert({varStmt->m_decls[i]->m_name,varStmt->m_decls[i]});
        if (!it.second)
            throw SemanticException("Error : Redeclaration of variable \"" + 
                                    varStmt->m_decls[i]->m_name + "\".",varStmt->m_decls[i]->m_loc);
        auto it1 = m_fun.find(varStmt->m_decls[i]->m_name);
        if (it1 != m_fun.end())
            throw SemanticException("Error : Declaration of variable with same name as function. \"" + 
                                    varStmt->m_decls[i]->m_name + "\".",varStmt->m_decls[i]->m_loc);
    }
    return nullptr;
}
Object * SemanticAnalysis::visit ( ReturnStmt * returnStmt)
{
    returnStmt->m_expr->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( FunBlockStmt * funBlockStmt)
{
    for (uint i = 0; i < funBlockStmt->m_vars.size(); i++)
    {
        funBlockStmt->m_vars[i]->Accept(*this);
    }
    for (uint i = 0; i < funBlockStmt->m_stmts.size(); i++)
    {
        funBlockStmt->m_stmts[i]->Accept(*this);
    }
    funBlockStmt->m_ret->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( IfStmt * ifStmt)
{
    ifStmt->m_guard->Accept(*this);
    ifStmt->m_thenBranch->Accept(*this);
    if (ifStmt->m_elseBranch != nullptr)
        ifStmt->m_elseBranch->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( WhileStmt * whileStmt)
{
    whileStmt->m_guard->Accept(*this);
    whileStmt->m_block->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( OutputStmt * output)
{
    output->m_expr->Accept(*this);
    return nullptr;
}
Object * SemanticAnalysis::visit ( FunDecl * funDecl)
{
    for (uint i = 0; i < funDecl->m_params.size(); i++)
    {
        auto it = m_var.insert({funDecl->m_params[i]->m_name,funDecl->m_params[i]});
        if (!it.second)
            throw SemanticException("Error : Redeclaration of variable \"" + 
                                    funDecl->m_params[i]->m_name + "\".",funDecl->m_params[i]->m_loc);
        auto it1 = m_fun.find(funDecl->m_params[i]->m_name);
        if (it1 != m_fun.end())
            throw SemanticException("Error : Declaration of variable with same name as function. \"" + 
                                    funDecl->m_params[i]->m_name + "\".",funDecl->m_params[i]->m_loc);
    }
    funDecl->m_block->Accept(*this);

    m_var.clear();
    return nullptr;
}
Object * SemanticAnalysis::visit ( Program * Program)
{
    for (uint i = 0; i < Program->m_funs.size(); i++)
    {
        auto it = m_fun.insert({Program->m_funs[i]->m_name,Program->m_funs[i]});
        if (!it.second)
            throw SemanticException("Error : Redefinition of function \"" + 
                                    Program->m_funs[i]->m_name + "\".",Program->m_funs[i]->m_loc);
    }

    for (uint i = 0; i < Program->m_funs.size(); i++)
    {
        Program->m_funs[i]->Accept(*this);
    }
    return nullptr;
}
