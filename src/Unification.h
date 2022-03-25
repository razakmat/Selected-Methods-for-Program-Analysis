#ifndef __UNIFICATION_H__
#define __UNIFICATION_H__

#include "Type.h"
#include <memory>
#include <unordered_map>

using namespace std;

class Unification
{
    public:
        void Unify(shared_ptr<Type> t1, shared_ptr<Type> t2);
        shared_ptr<Type> Close(shared_ptr<Type> t);
        shared_ptr<Type> AbsentCheck(shared_ptr<Type> t);
    private:
        void MakeSet(shared_ptr<Type> & t);
        shared_ptr<Type> & Find(shared_ptr<Type> & t);
        void Union(shared_ptr<Type> & t1, shared_ptr<Type> & t2);
        unordered_map<shared_ptr<Type>,shared_ptr<Type>> m_map;
        unordered_map<shared_ptr<Type>,shared_ptr<Type>> m_recMap;
        unordered_map<shared_ptr<Type>,shared_ptr<Type>> m_FreshMap;

};





#endif // __UNIFICATION_H__