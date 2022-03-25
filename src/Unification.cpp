#include "Unification.h"
#include "TypeException.h"
#include <typeinfo>
#include <iostream>
#include <map>

void Unification::Unify(shared_ptr<Type> t1, shared_ptr<Type> t2)
{
    MakeSet(t1);
    shared_ptr<Type> & ptr_t1 = Find(t1);
    MakeSet(t2);
    shared_ptr<Type> & ptr_t2 = Find(t2);

    if (ptr_t1 != ptr_t2)
    {
        if (dynamic_cast<TypeVar*>(ptr_t1.get()) &&
            dynamic_cast<TypeVar*>(ptr_t2.get()))
        {
            Union(ptr_t1,ptr_t2);
        }
        else if (dynamic_cast<TypeVar*>(ptr_t1.get()) &&
            dynamic_cast<ProperType*>(ptr_t2.get()))
        {
            Union(ptr_t1,ptr_t2);
        }
        else if (dynamic_cast<TypeVar*>(ptr_t2.get()) &&
            dynamic_cast<ProperType*>(ptr_t1.get()))
        {
            Union(ptr_t2,ptr_t1);
        }
        else if (dynamic_cast<TPointer*>(ptr_t1.get()) &&
            dynamic_cast<TPointer*>(ptr_t2.get()))
        {
            Union(ptr_t1,ptr_t2);
            Unify(dynamic_cast<TPointer*>(ptr_t1.get())->m_ref,
                  dynamic_cast<TPointer*>(ptr_t2.get())->m_ref);
        }
        else if (dynamic_cast<TFunction*>(ptr_t1.get()) &&
            dynamic_cast<TFunction*>(ptr_t2.get()))
        {
            Union(ptr_t1,ptr_t2);
            vector<shared_ptr<Type>> & m_arg_ptr_t1 = dynamic_cast<TFunction*>(ptr_t1.get())->m_params;
            vector<shared_ptr<Type>> & m_arg_ptr_t2 = dynamic_cast<TFunction*>(ptr_t2.get())->m_params;
            for (uint32_t i = 0; i < m_arg_ptr_t1.size(); i++)
                Unify(m_arg_ptr_t1[i],m_arg_ptr_t2[i]);
            Unify(dynamic_cast<TFunction*>(ptr_t1.get())->m_ret,
                    dynamic_cast<TFunction*>(ptr_t2.get())->m_ret);
        }
        else if (dynamic_cast<TRecord*>(ptr_t1.get()) &&
            dynamic_cast<TRecord*>(ptr_t2.get()))
        {
            Union(ptr_t1,ptr_t2);
            map<string,shared_ptr<Type>> & fields1 = dynamic_cast<TRecord*>(ptr_t1.get())->m_fields;
            map<string,shared_ptr<Type>> & fields2 = dynamic_cast<TRecord*>(ptr_t2.get())->m_fields;
            for (const auto & it1 : fields1)
            {
                const auto & it2 = fields2.find(it1.first);
                Unify(it1.second,it2->second);
            }
        }
        else
        {
            string str = "Cannot unify '" + t1->Print() + "' and '" + 
                            t2->Print() + "' (with canonicals '" + 
                            ptr_t1->Print() + "' and '" + ptr_t2->Print() + "')";
            throw TypeException(str);
        }
    }
}

void Unification::MakeSet(shared_ptr<Type> & t)
{
    m_map.insert({t,t});
}

shared_ptr<Type> &  Unification::Find(shared_ptr<Type> & t)
{
    auto it = m_map.find(t);
    if (it->first != it->second)
        it->second = Find(it->second);
    return it->second;
}

void Unification::Union(shared_ptr<Type> & t1, shared_ptr<Type> & t2)
{
    shared_ptr<Type> & ptr1 = Find(t1);
    shared_ptr<Type> & ptr2 = Find(t2);
    if (ptr1 != ptr2)
        m_map.find(ptr1)->second = ptr2;
}

shared_ptr<Type> Unification::Close(shared_ptr<Type> t)
{
    auto it = m_map.find(t);

    if (it == m_map.end())
    {
        auto ptr = m_map.insert({t,make_shared<TFresh>()});
        return ptr.first->second;
    }

    auto it_rec = m_recMap.insert({it->second,nullptr});
    if (!it_rec.second)
    {
        if (it_rec.first->second != nullptr)
            return it_rec.first->second;
        auto it_program = m_FreshMap.find(it->second);
        if (it_program != m_FreshMap.end())
            it_rec.first->second = it_program->second;
        else{
            it_rec.first->second = make_shared<TFresh>();
            m_FreshMap.insert({it->second,it_rec.first->second});
        }
        return it_rec.first->second;
    }

    shared_ptr<Type> ptr;
    if (typeid(*it->second) == typeid(TFunction))
    {
        TFunction * f = dynamic_cast<TFunction*>(it->second.get());
        shared_ptr<TFunction> fun = make_shared<TFunction>();
        for (uint32_t i = 0; i < f->m_params.size(); i++)
            fun->m_params.push_back(Close(f->m_params[i]));
        fun->m_ret = Close(f->m_ret);
        ptr = fun;
    }
    else if (typeid(*it->second) == typeid(TPointer))
    {
        TPointer * p = dynamic_cast<TPointer*>(it->second.get());
        ptr = make_shared<TPointer>(Close(p->m_ref));
    }
    else if (typeid(*it->second) == typeid(TRecord))
    {
        TRecord * r = dynamic_cast<TRecord*>(it->second.get());
        shared_ptr<TRecord> rec = make_shared<TRecord>();
        for (auto & i : r->m_fields)
        {
            if (typeid(*i.second) != typeid(TAbsentField))
                rec->m_fields.insert({i.first,Close(i.second)});
            else
                rec->m_fields.insert({i.first,i.second});
        }
        ptr = rec;
    }
    else if (typeid(*it->second) == typeid(TVar))
    {
        it->second = make_shared<TFresh>();
        ptr = it->second;
    }
    else if (typeid(*it->second) == typeid(TTerm))
    {
        shared_ptr<Type> fresh = make_shared<TFresh>();
        it->second = fresh;
        ptr = fresh;
    }
    else 
        ptr = it->second;
    
    if (it_rec.first->second != nullptr)
    {
        shared_ptr<TRecursive> TRec = make_shared<TRecursive>();
        TRec->m_fresh = it_rec.first->second;
        TRec->m_inner = ptr;
        ptr = TRec;    
    }
    m_recMap.erase(it_rec.first);
    return ptr;
}

shared_ptr<Type> Unification::AbsentCheck(shared_ptr<Type> t) 
{
    return Find(t);
}