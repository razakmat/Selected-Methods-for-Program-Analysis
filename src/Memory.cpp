#include "Memory.h"
#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>


using namespace std;


ObjNull::ObjNull(Null * null)
: m_null(null)
{
    m_is_null = true;
}

ObjNum::ObjNum(int value)
:m_value(value)
{
    m_zero = (value == 0);
}

ObjRef::ObjRef(int ref)
: m_ref(ref)
{
    m_is_ref = true;
}

ObjFun::ObjFun(FunDecl * fun)
:m_fun(fun)
{
}

ObjRec::ObjRec(Stack * fields)
:m_fields(fields)
{ 
}


void Heap::AssignToHeap(Object * arg)
{
    m_data.push_back(arg);
    m_current++;
}

void Heap::AssignToHeap(Object * arg,int ref)
{
    m_data[ref] = arg;
}


Object * Heap::GetFromHeap(int ref)
{
    return m_data[ref];
}

int Heap::GetCurrent()
{
    return m_current;
}



Env::Env()
:m_heap(new Heap())
{
}


Object * Env::GetValue(const string & name)
{
    auto ref = m_stack->m_ref.find(name);
    if (ref == m_stack->m_ref.end())
        ref = m_refFun.find(name);

    return m_heap->GetFromHeap(ref->second);;
}

void Env::CreateVar(const string & name,Object * arg)
{

    m_stack->m_ref.insert({name,m_heap->GetCurrent()});
    m_heap->AssignToHeap(arg);
}

ObjRec * Env::CreateRecord(vector<pair<string,Object*>> & fields)
{
    Stack * now = new Stack();
    for (uint i = 0; i < fields.size(); i++)
    {
        now->m_ref.insert({fields[i].first,m_heap->GetCurrent()});
        m_heap->AssignToHeap(fields[i].second);
    }
    return new ObjRec(now);
}


Object * Env::GetRecValue(Stack * fields,const string & field)
{
    auto ref = fields->m_ref.find(field);

    if (ref == fields->m_ref.end())
        throw "Error : trying to access unknown field \"" + field + "\".";

    return m_heap->GetFromHeap(ref->second);
}

void Env::CreateFun(FunDecl * fun)
{
    m_refFun.insert({fun->m_name,m_heap->GetCurrent()});
    m_heap->AssignToHeap(new ObjFun(fun));
}

ObjRef * Env::Alloc(Object * expr)
{
    m_heap->AssignToHeap(expr);
    return new ObjRef(m_heap->GetCurrent() - 1);
}

ObjRef * Env::Ref(const string & name)
{
    auto ref = m_stack->m_ref.find(name);
    if (ref == m_stack->m_ref.end())
        ref = m_refFun.find(name);

    return new ObjRef(ref->second);
}

Object * Env::Deref(Object * obj)
{
    if (obj->m_is_ref)
    {
        ObjRef * objref = reinterpret_cast<ObjRef*>(obj);
        return m_heap->GetFromHeap(objref->m_ref);
    }
    else
        throw "Error : Not a pointer.";
    return obj;
}

void Env::AssignDW(const string & name,Object * right)
{
    auto ref = m_stack->m_ref.find(name);

    m_heap->AssignToHeap(right,ref->second);
}

void Env::AssignDFW(Stack * fields,const string & name,Object * right)
{
    auto ref = fields->m_ref.find(name);
    if (ref == fields->m_ref.end())
        throw "Error : Trying to write into non-existing field \""+ name + "\".";

    m_heap->AssignToHeap(right,ref->second);
}

void Env::AssignIW(ObjRef * objref,Object * right)
{
    m_heap->AssignToHeap(right,objref->m_ref);
}


void Env::PushStack()
{
    Stack * now = new Stack();
    now->m_prev = m_stack;
    m_stack = now;

}

void Env::PopStack()
{
    Stack * now = m_stack->m_prev;
    delete m_stack;
    m_stack = now;
}

