#include "Interpreter.h"
#include <iostream>
#include <string>
#include <vector>
#include <typeinfo>
#include <map>

using namespace std;

Interpreter::Interpreter(bool ascii)
:m_ascii(ascii)
{
}

Object * Interpreter::visit ( Number * integer)
{
    return new ObjNum(integer->m_value);
}

Object * Interpreter::visit ( Null * null)
{
    return new ObjNull(null);
}

Object * Interpreter::visit ( Identifier * id)
{
    return m_memory.GetValue(id->m_name);
}

Object * Interpreter::visit ( BinaryOp * Binop)
{
    Object * objLeft = Binop->m_left->Accept(*this);
    Object * objRight = Binop->m_right->Accept(*this);

    if (typeid(*objLeft) == typeid(ObjFun) ||
        typeid(*objRight) == typeid(ObjFun))
        throw InterpreterException("Error: Cannot compare functions.",Binop->m_loc);


    if (typeid(*Binop->m_operator) == typeid(Equal))
    {
        if (typeid(*objLeft) != typeid(*objRight))
            return new ObjNum(0);
        if (typeid(*objLeft) == typeid(ObjNull))
            return new ObjNum(1);
        if (typeid(*objLeft) == typeid(ObjRef))
        {
            return new ObjNum(Binop->m_operator->Apply(
                reinterpret_cast<ObjRef*>(objLeft)->m_ref,
                reinterpret_cast<ObjRef*>(objRight)->m_ref));
        }
    }
    ObjNum * left = reinterpret_cast<ObjNum*>(objLeft);
    ObjNum * right = reinterpret_cast<ObjNum*>(objRight);
    return new ObjNum(Binop->m_operator->Apply(left->m_value,right->m_value));
}

Object * Interpreter::visit ( CallFuncExpr * call)
{

    ObjFun * objfun = reinterpret_cast<ObjFun*>(call->m_targetFun->Accept(*this));
    FunDecl * fun = objfun->m_fun;

    vector<Object*> args;
    for (uint i = 0; i < call->m_arg.size(); i++)
    {
        args.push_back(call->m_arg[i]->Accept(*this));
    }
    m_memory.PushStack();

    for (uint i = 0; i < fun->m_params.size(); i++)
    {
        m_memory.CreateVar(fun->m_params[i]->m_name,args[i]);
    }

    Object * obj = fun->Accept(*this);

    m_memory.PopStack();

    return obj;
}

Object * Interpreter::visit ( Input * input)
{
    if (m_ascii == true)
    {
        char c = -1;
        if (cin.peek() != EOF)
            cin >> c;
        return new ObjNum((int)c);
    }

    int number = -1;
    if (cin.peek() != EOF && cin.peek() != '\n' )
        cin >> number;

    return new ObjNum(number);
}

Object * Interpreter::visit ( Alloc * alloc)
{
    Object * value = alloc->m_expr->Accept(*this);
    ObjRef * ref = m_memory.Alloc(value);
    return ref;
}

Object * Interpreter::visit ( VarRef * varRef)
{
    ObjRef * ref = m_memory.Ref(varRef->m_id->m_name);
    return ref;
}

Object * Interpreter::visit ( Deref * deref)
{
    Object * obj = nullptr;
    try{
        obj = m_memory.Deref(deref->m_pointer->Accept(*this));
    }catch(const string & e){
        throw InterpreterException(e, deref->m_pointer->m_loc);
    }
    return obj;
}


Object * Interpreter::visit ( RecordField * recordField)
{
    return recordField->m_expr->Accept(*this);
}


Object * Interpreter::visit ( Record * record)
{
    vector<pair<string,Object*>> fies;

    for (uint i = 0; i < record->m_fields.size(); i++)
    {
        Object * field = record->m_fields[i]->Accept(*this);
        if (typeid(*field) == typeid(ObjRec))
            throw InterpreterException("Error : Nested records are not allowed.",record->m_fields[i]->m_loc);
        fies.push_back({record->m_fields[i]->m_name,field});   
    }
    return m_memory.CreateRecord(fies);
}


Object * Interpreter::visit ( FieldAccess * fieldAccess)
{
    ObjRec * objRec = reinterpret_cast<ObjRec*>(fieldAccess->m_record->Accept(*this));
    
    Object * obj = nullptr;
    try{
        obj = m_memory.GetRecValue(objRec->m_fields,fieldAccess->m_field);
    }catch(const string & e){
        throw InterpreterException(e,fieldAccess->m_loc);
    }
    return obj;
}


Object * Interpreter::visit ( AssignStmt * assignStmt)
{
    Object * right = assignStmt->m_right->Accept(*this);
    if (typeid(*assignStmt->m_left) == typeid(Identifier) )
    {
        Identifier * id = reinterpret_cast<Identifier*>(assignStmt->m_left);
        m_memory.AssignDW(id->m_name,right);
    }
    else if (typeid(*assignStmt->m_left) == typeid(FieldAccess))
    {
        FieldAccess * acc = reinterpret_cast<FieldAccess*>(assignStmt->m_left);
        ObjRec * objRec = reinterpret_cast<ObjRec*>(acc->m_record->Accept(*this));
        try{
            m_memory.AssignDFW(objRec->m_fields,acc->m_field,right);
        }catch(const string & e){
            throw InterpreterException(e,acc->m_loc);
        }
    }
    else if (typeid(*assignStmt->m_left) == typeid(Deref))
    {
        Deref * deref = reinterpret_cast<Deref*>(assignStmt->m_left);
        Object * obj = deref->m_pointer->Accept(*this);
        if (!obj->m_is_ref)
            throw InterpreterException("Error: Not a pointer.",deref->m_pointer->m_loc);
        m_memory.AssignIW(reinterpret_cast<ObjRef*>(obj),right);
    }
    return nullptr;
}

Object * Interpreter::visit ( NestedBlockStmt * nestedBlockStmt)
{
    for (uint i = 0; i < nestedBlockStmt->m_body.size(); i++)
    {
        nestedBlockStmt->m_body[i]->Accept(*this);
    }
    return nullptr;
}

Object * Interpreter::visit ( IdentifierDecl * idDecl)
{
    m_memory.CreateVar(idDecl->m_name);
    return nullptr;
}

Object * Interpreter::visit ( VarStmt * varStmt)
{
    for (uint i = 0; i < varStmt->m_decls.size(); i++)
    {
        varStmt->m_decls[i]->Accept(*this);
    }
    return nullptr;
}

Object * Interpreter::visit ( ReturnStmt * returnStmt)
{
    return returnStmt->m_expr->Accept(*this);;
}

Object * Interpreter::visit ( FunBlockStmt * funBlockStmt)
{
    for (uint i = 0; i < funBlockStmt->m_vars.size(); i++)
    {
        funBlockStmt->m_vars[i]->Accept(*this);
    }
    for (uint i = 0; i < funBlockStmt->m_stmts.size(); i++)
    {
        funBlockStmt->m_stmts[i]->Accept(*this);
    }
    return funBlockStmt->m_ret->Accept(*this);
}

Object * Interpreter::visit ( IfStmt * ifStmt)
{
    Object * cond = ifStmt->m_guard->Accept(*this);

    if (!cond->m_zero)
        ifStmt->m_thenBranch->Accept(*this);
    else
    {
        if (ifStmt->m_elseBranch != nullptr)
            ifStmt->m_elseBranch->Accept(*this);
    }
    return nullptr;
}

Object * Interpreter::visit ( WhileStmt * whileStmt)
{
    while (true)
    {
        Object * cond = whileStmt->m_guard->Accept(*this);
        if (cond->m_zero)
            return nullptr;
        whileStmt->m_block->Accept(*this);
    }
}

Object * Interpreter::visit ( OutputStmt * output)
{

    ObjNum * num = reinterpret_cast<ObjNum*>(output->m_expr->Accept(*this));
    if (m_ascii == true)
    {
        unsigned char c = (unsigned char)num->m_value;
        cout << c;
        return nullptr;
    }
    cout << num->m_value << endl;
    return nullptr;
}

Object * Interpreter::visit ( FunDecl * funDecl)
{
    return funDecl->m_block->Accept(*this);
}

Object * Interpreter::visit ( Program * Program)
{
    FunDecl * top = nullptr;
    for (uint i = 0; i < Program->m_funs.size(); i++)
    {
        if(Program->m_funs[i]->m_name == "main")
            top = Program->m_funs[i];
        else
            m_memory.CreateFun(Program->m_funs[i]);
    }
    m_memory.PushStack();
    top->Accept(*this);
    m_memory.PopStack();
    return nullptr;
}