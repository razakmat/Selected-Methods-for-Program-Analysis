#include "MonotoneAnalysis.h"
#include <set>

template<typename T>
MonotoneAnalysis<T>::MonotoneAnalysis(vector<CFGNode*> & cfg)
: m_nodes(cfg)
{
    for (auto & x : cfg)
    {
        m_map3.insert({x,new T()});
    }
}

template<typename T>
MonotoneAnalysis<T>::~MonotoneAnalysis()
{
    for (auto & x : m_map3)
    {
        delete x.second;
    }
}

template<typename T>
T * MonotoneAnalysis<T>::Join(CFGNode * node)
{
    vector<CFGNode*> prev;
    T::PrevNodes(node,prev);
    T * j = new T();
    j->Bot();

    for (auto & x : prev)
    {
        auto it = m_map3.find(x);
        j->Lub(it->second);
    }
    return j;
}

template<typename T>
T * MonotoneAnalysis<T>::Fun(CFGNode * node)
{
    vector<CFGNode*> prev;
    T * j = Join(node);
    T::TransferFun(node,j);
    return j;
}

template<typename T>
void MonotoneAnalysis<T>::Worklist()
{
    for (auto & x : m_map3)
        x.second->Bot();

    set<CFGNode*,typename T::cmp> w;
    for (auto & x : m_nodes)
        w.insert(x);
    

    while (!w.empty())
    {
        auto it1 = w.begin();
        CFGNode * v = *it1;
        w.erase(it1);

        auto it = m_map3.find(v);

        T * x = it->second;
        T * y = Fun(v);

        if (!(*y == *x))
        {
            vector<CFGNode*> succ;
            T::NextNodes(v,succ);
            for (auto & z : succ)
                w.insert(z);
            delete x;
            it->second = y;
        }
    }
    Save();
}

template<typename T>
void MonotoneAnalysis<T>::Save()
{
    for (auto & x : m_map3)
    {
        x.second->SaveToStr(x.first);
    }
}