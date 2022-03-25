#include "TypeAnalysis.h"
#include "TypeException.h"
#include <typeinfo>
#include <iostream>
#include <set>
#include <string>
#include <memory>

TypeAnalysis::TypeAnalysis(set<string> & allFields)
: m_allFields(allFields)
{
    m_int = make_shared<TInt>(TInt{});
    m_AbsentField = make_shared<TAbsentField>(TAbsentField{});
}

shared_ptr<Type> TypeAnalysis::visit(Number * integer)
{
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(integer));
    m_uni.Unify(ptr,m_int);
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(Null * null)
{
    shared_ptr<Type> ptr = make_shared<TFresh>(TFresh{});
    shared_ptr<Type> ptrNull = make_shared<TTerm>(TTerm(null));
    m_uni.Unify(ptrNull,make_shared<TPointer>(ptr));
    return ptrNull;
}

shared_ptr<Type> TypeAnalysis::visit(Identifier * id)
{
    auto it = m_varMap.find(id->m_name);
    return it->second;
}

shared_ptr<Type> TypeAnalysis::visit(BinaryOp * Binop)
{
    shared_ptr<Type> exprl = Binop->m_left->Accept(*this);
    shared_ptr<Type> exprr = Binop->m_right->Accept(*this);
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(Binop));
    if (typeid(*Binop->m_operator) == typeid(Equal))
    {
        m_uni.Unify(exprl,exprr);
        m_uni.Unify(ptr,m_int);
    }
    else
    {
        m_uni.Unify(exprl,m_int);
        m_uni.Unify(exprr,m_int);
        m_uni.Unify(ptr,m_int);
    }
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(CallFuncExpr * call)
{
    shared_ptr<Type> target = call->m_targetFun->Accept(*this);
    shared_ptr<TFunction> fun = make_shared<TFunction>();
    for (uint16_t i = 0; i < call->m_arg.size(); i++)
    {
         fun->m_params.push_back(call->m_arg[i]->Accept(*this));
    }
    fun->m_ret = make_shared<TTerm>(call);
    m_uni.Unify(target,fun);
    return fun->m_ret;
}

shared_ptr<Type> TypeAnalysis::visit(Input * input)
{
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(input));
    m_uni.Unify(ptr,m_int);
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(Alloc * alloc)
{
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(alloc));
    shared_ptr<Type> ret = alloc->m_expr->Accept(*this);
    m_uni.Unify(ptr,make_shared<TPointer>(TPointer(ret)));
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(VarRef * varRef)
{
    shared_ptr<Type> ret = varRef->m_id->Accept(*this);
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(varRef));
    m_uni.Unify(ptr,make_shared<TPointer>(TPointer(ret)));
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(Deref * deref)
{
    shared_ptr<Type> ret = deref->m_pointer->Accept(*this);
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(deref));
    m_uni.Unify(ret,make_shared<TPointer>(TPointer(ptr)));
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(RecordField * recordField)
{
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(Record * record)
{
    shared_ptr<Type> ptr = make_shared<TTerm>(TTerm(record));
    shared_ptr<TRecord> rec = make_shared<TRecord>();
    for (uint32_t i = 0; i < record->m_fields.size(); i++)
    {
        rec->m_fields.insert({record->m_fields[i]->m_name,record->m_fields[i]->m_expr->Accept(*this)});
    }
    for (auto & it : m_allFields)
        rec->m_fields.insert({it,m_AbsentField});
    m_uni.Unify(ptr,rec);
    return ptr;
}

shared_ptr<Type> TypeAnalysis::visit(FieldAccess * fieldAccess)
{
    shared_ptr<Type> ptr = fieldAccess->m_record->Accept(*this);
    shared_ptr<TRecord> rec = make_shared<TRecord>();
    for (auto & it : m_allFields)
        rec->m_fields.insert({it,make_shared<TFresh>()});
    auto it = rec->m_fields.find(fieldAccess->m_field);
    shared_ptr<TTerm> acc = make_shared<TTerm>(fieldAccess);
    it->second = acc;
    m_uni.Unify(ptr,rec);
    m_allAccessFields.push_back(acc);
    return acc;
}

shared_ptr<Type> TypeAnalysis::visit(AssignStmt * assignStmt)
{

    if (typeid(*assignStmt->m_left) == typeid(Identifier))
    {
        shared_ptr<Type> left = assignStmt->m_left->Accept(*this);
        shared_ptr<Type> right = assignStmt->m_right->Accept(*this);
        m_uni.Unify(left,right);
    }
    else if (typeid(*assignStmt->m_left) == typeid(FieldAccess))
    {
        FieldAccess * acc = dynamic_cast<FieldAccess*>(assignStmt->m_left);
        shared_ptr<TRecord> rec = make_shared<TRecord>();
        for (auto & it : m_allFields)
            rec->m_fields.insert({it,make_shared<TFresh>()});
        auto it = rec->m_fields.find(acc->m_field);
        it->second = assignStmt->m_right->Accept(*this);
        m_allAccessFields.push_back(it->second);
        if (typeid(*acc->m_record) == typeid(Identifier))
        {
            m_uni.Unify(acc->m_record->Accept(*this),rec);
        }
        else
        {
            Deref * ref = dynamic_cast<Deref*>(acc->m_record);
            m_uni.Unify(ref->m_pointer->Accept(*this),make_shared<TPointer>(rec));
        }
    }
    else if (typeid(*assignStmt->m_left) == typeid(Deref))
    {
        shared_ptr<Type> left = (dynamic_cast<Deref*>(assignStmt->m_left))->m_pointer->Accept(*this);
        shared_ptr<Type> right = assignStmt->m_right->Accept(*this);
        m_uni.Unify(left,make_shared<TPointer>(TPointer(right)));
    }
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(NestedBlockStmt * nestedBlockStmt)
{
    for (uint16_t i = 0; i < nestedBlockStmt->m_body.size(); i++)
    {
        nestedBlockStmt->m_body[i]->Accept(*this);
    }
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(IdentifierDecl * idDecl)
{
    auto it = m_varMap.find(idDecl->m_name);
    if (it != m_varMap.end()){
        m_typeMap.insert({it->second,idDecl});
        return it->second;
    }
    else{
        shared_ptr<Type> ptr = make_shared<TVar>(TVar(idDecl->m_name));
        m_varMap.insert({idDecl->m_name,ptr});
        m_typeMap.insert({ptr,idDecl});
        return ptr;
    }
}

shared_ptr<Type> TypeAnalysis::visit(VarStmt * varStmt)
{
    for (uint16_t i = 0; i < varStmt->m_decls.size(); i++)
    {
        varStmt->m_decls[i]->Accept(*this);
    }
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(ReturnStmt * returnStmt)
{
    return returnStmt->m_expr->Accept(*this);
}

shared_ptr<Type> TypeAnalysis::visit(FunBlockStmt * funBlockStmt)
{
    for (uint16_t i = 0; i < funBlockStmt->m_vars.size(); i++)
    {
        funBlockStmt->m_vars[i]->Accept(*this);
    }
    for (uint16_t i = 0; i < funBlockStmt->m_stmts.size(); i++)
    {
        funBlockStmt->m_stmts[i]->Accept(*this);
    }
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(IfStmt * ifStmt)
{
    shared_ptr<Type> expr = ifStmt->m_guard->Accept(*this);
    m_uni.Unify(expr,m_int);
    ifStmt->m_thenBranch->Accept(*this);
    if (ifStmt->m_elseBranch != nullptr)
        ifStmt->m_elseBranch->Accept(*this);
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(WhileStmt * whileStmt)
{
    shared_ptr<Type> expr = whileStmt->m_guard->Accept(*this);
    m_uni.Unify(expr,m_int);
    whileStmt->m_block->Accept(*this);
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(OutputStmt * output)
{
    shared_ptr<Type> expr = output->m_expr->Accept(*this);
    m_uni.Unify(expr,m_int);
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visitParam(IdentifierDecl * idDecl)
{
    auto it = m_varMap.find(idDecl->m_name);
    if (it != m_varMap.end())
        return it->second;
    shared_ptr<Type> ptr = make_shared<TVar>(TVar(idDecl->m_name));
    m_varMap.insert({idDecl->m_name,ptr});
    return ptr;
}


shared_ptr<Type> TypeAnalysis::visit(FunDecl * funDecl)
{
    shared_ptr<TFunction> ptr = make_shared<TFunction>();
    for (uint16_t i = 0; i < funDecl->m_params.size(); i++)
    {
         ptr->m_params.push_back(visitParam(funDecl->m_params[i]));
         if (funDecl->m_name == "main")
            m_uni.Unify(ptr->m_params[i],m_int);
    }
    funDecl->m_block->Accept(*this);
    ptr->m_ret = funDecl->m_block->m_ret->Accept(*this);
    auto it = m_varMap.find(funDecl->m_name);
    if (funDecl->m_name == "main")
        m_uni.Unify(ptr->m_ret,m_int);
    m_uni.Unify(it->second,ptr);
    return nullptr;
}

shared_ptr<Type> TypeAnalysis::visit(Program * Program)
{
    for (uint16_t i = 0; i < Program->m_funs.size(); i++)
    {
        shared_ptr<Type> ptrVar = make_shared<TVar>(TVar(Program->m_funs[i]->m_name));
        if (m_varMap.find(Program->m_funs[i]->m_name) == m_varMap.end())
        {
            m_varMap.insert({Program->m_funs[i]->m_name,ptrVar});
            m_typeMap.insert({ptrVar,Program->m_funs[i]});
        }
        Program->m_funs[i]->Accept(*this);
    }
    return nullptr;
}

void TypeAnalysis::StartAnalysis(Program * Program)
{
    visit(Program);

    for (const auto & it : m_allAccessFields)
    {
        if (dynamic_cast<TAbsentField*>(m_uni.AbsentCheck(it).get()))
        {
            FieldAccess * acc = dynamic_cast<FieldAccess*>(dynamic_cast<TTerm*>(it.get())->m_expr);
            throw TypeException("Trying to access Absent field (" + acc->m_field + ")",acc->m_loc);
        }
    }

    for (auto & it: m_typeMap)
    {
        shared_ptr<Type> ptr = m_uni.Close(it.first);
        cout << it.second->m_name  << "[" << it.second->m_loc.m_line 
                << "," << it.second->m_loc.m_col << "]" << " = ";
        cout << ptr->Print() << endl;
    }
}