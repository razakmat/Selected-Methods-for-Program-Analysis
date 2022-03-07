#ifndef MEMORY_H
#define MEMORY_H


#include <unordered_map>
#include <string>
#include <vector>
#include <stack>

#include "AST.h"

using namespace std;

class Stack
{
    public:
        unordered_map<string, int> m_ref;
        Stack * m_prev = nullptr;
};

class Object
{
public:
    virtual ~Object() = default;
    bool m_zero = false;
    bool m_is_ref = false;
    bool m_is_null = false;
};

class ObjNull : public Object
{
public:
    ObjNull(Null * null = nullptr);
    Null * m_null;
};

class ObjNum : public Object
{
public:
    ObjNum(int value);
    int m_value;
};

class ObjRef : public Object
{
public:
    ObjRef(int ref);
    int m_ref;
};

class ObjFun : public Object
{
public:
    ObjFun(FunDecl * fun);
    FunDecl * m_fun;
};

class ObjRec : public Object
{
public:
    ObjRec(Stack * fields);
    Stack * m_fields;
};


class Heap
{
    public:
    Object * GetFromHeap(int ref);
    int GetCurrent();
    void AssignToHeap(Object * arg);
    void AssignToHeap(Object * arg,int ref);
    vector<Object*> m_data;
    int m_current = 0;

};

class Env
{
    public:
        Env();
        Object * GetValue(const string & name);
        void CreateVar(const string & name,Object * arg = new ObjNull());
        Object * GetRecValue(Stack * fields,const string & field);
        void CreateFun(FunDecl * fun);
        ObjRec * CreateRecord(vector<pair<string,Object*>> & fields);
        ObjRef * Alloc(Object * expr);
        ObjRef * Ref(const string & name);
        Object * Deref(Object * obj);
        void AssignDW(const string & name,Object * right);
        void AssignDFW(Stack * fields,const string & name,Object * right);
        void AssignIW(ObjRef * objref,Object * right);
        void PushStack();
        void PopStack();
    private:
        unordered_map<string, int> m_refFun;
        Stack * m_stack;
        Heap * m_heap;

};



#endif