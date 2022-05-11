#include "NullAnalysis.h"

NullAnalysis::NullAnalysis()
{
    for (auto & x : m_res)
        m_map.insert({x.first,NullLattice()});
    for (auto & x : m_allocs)
        m_map.insert({x.second,NullLattice()});
}

void NullAnalysis::Prepare(map<string,vector<string>> & res, vector<CFGNode*> & code)
{
    m_res = res;
    int num = 1;
    for (auto & x : code)
    {
        if (CFGAssign * n = dynamic_cast<CFGAssign*>(x))
        {
            if (dynamic_cast<Alloc*>(n->m_right))
                m_allocs.insert({x->m_nodeNumber,"alloc-"+ to_string(num++)});
        }
        else if (CFGAssignP * n = dynamic_cast<CFGAssignP*>(x))
        {
            if (dynamic_cast<Alloc*>(n->m_right))
                m_allocs.insert({x->m_nodeNumber,"alloc-"+ to_string(num++)});
        }
    }
}

void NullAnalysis::PrevNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_prev)
        v.push_back(x);
}

void NullAnalysis::NextNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_succ)
        v.push_back(x);
}

void NullAnalysis::TransferFun(CFGNode * node, NullAnalysis * lat)
{
    if (CFGAssign * n = dynamic_cast<CFGAssign*>(node))
    {
        if (dynamic_cast<Alloc*>(n->m_right))
        {
            lat->m_map.find(n->m_left->m_name)->second.Bot();
            lat->m_map.find(m_allocs.find(node->m_nodeNumber)->second)->second.Top();
        }
        else if (dynamic_cast<VarRef*>(n->m_right))
        {
            lat->m_map.find(n->m_left->m_name)->second.Bot();
        }
        else if (Identifier * id = dynamic_cast<Identifier*>(n->m_right))
        {
            lat->m_map.find(n->m_left->m_name)->second = lat->m_map.find(id->m_name)->second;
        }
        else if (Deref * ref = dynamic_cast<Deref*>(n->m_right))
        {
            Identifier * id2 = dynamic_cast<Identifier*>(ref->m_pointer);
            if (id2 == nullptr)
                return;
            NullLattice & b = lat->m_map.find(n->m_left->m_name)->second;
            auto it = m_res.find(id2->m_name);
            for (auto & x : it->second)
            {
                b.Lub(b,lat->m_map.find(x)->second);
            }
                
        }
        else if (dynamic_cast<Null*>(n->m_right))
        {
            lat->m_map.find(n->m_left->m_name)->second.Top();
        }
        
    }
    else if (CFGAssignP * n = dynamic_cast<CFGAssignP*>(node))
    {
        Deref * p = dynamic_cast<Deref*>(n->m_left);
        if (p == nullptr)
            return;
        Identifier * id1 = dynamic_cast<Identifier*>(p->m_pointer);
        if (id1 == nullptr)
            return;
        Identifier * id2 = dynamic_cast<Identifier*>(n->m_right);
        if (id2 == nullptr)
            return;
        auto x2 = lat->m_map.find(id2->m_name);
        auto it = m_res.find(id1->m_name);
        for (auto & x : it->second)
        {
            NullLattice & l = lat->m_map.find(x)->second;
            l.Lub(l,x2->second);
        }
    }
}

void NullAnalysis::SaveToStr(CFGNode * node)
{
    string str = "{ ";
    for (auto & x : m_map)
    {
        str += x.first;
        str += ": ";
        str += x.second.GetName();
        str += ", ";
    }
    if (!m_map.empty()){
        str.pop_back();
        str.pop_back();
    }
    str += " }";
    node->m_analysis = move(str);
}

void NullAnalysis::Bot()
{
    for (auto & x : m_map)
        x.second.Bot();
}

void NullAnalysis::Lub(NullAnalysis * a)
{
    for(auto it1 = m_map.begin(), end1 = m_map.end(),
        it2 = a->m_map.begin(), end2 = a->m_map.end();
        it1 != end1 || it2 != end2; it1++,it2++)
    {
        it1->second.Lub(it1->second,it2->second);
    }
}

bool NullAnalysis::operator==(const NullAnalysis& a) const
{
    for(auto it1 = m_map.cbegin(), end1 = m_map.cend(),
        it2 = a.m_map.cbegin(), end2 = a.m_map.cend();
        it1 != end1 || it2 != end2; it1++,it2++)
    {
        if (!(it1->second == it2->second))
            return false;
    }
    return true;
}
