#include "ReachingDefLattice.h"
#include <sstream>

ReachingDefLattice::ReachingDefLattice()
{
    
}

void ReachingDefLattice::Lub(ReachingDefLattice & b)
{
    for (auto & x : b.m_assign)
        m_assign.insert(x);
}

bool ReachingDefLattice::operator==(const ReachingDefLattice& a) const
{
    if (m_assign.size() != a.m_assign.size())
        return false;
    
    for(auto it1 = m_assign.begin(), end1 = m_assign.end(),
        it2 = a.m_assign.begin(), end2 = a.m_assign.end();
        it1 != end1 || it2 != end2; it1++,it2++)
    {
        if (it1->first != it2->first)
            return false;
    }
    return true;
}

string ReachingDefLattice::GetName()
{
    string str = "{";
    for (auto & x : m_assign)
    {
        str += x.first;
        str += ", ";
    }
    if (!m_assign.empty()){
        str.pop_back();
        str.pop_back();
    }
    str += "}";
    return str;
}

void ReachingDefLattice::Top()
{
    
}

void ReachingDefLattice::Bot()
{
    m_assign.clear();
}

void ReachingDefLattice::Add(CFGAssign * assign)
{
    std::stringstream ss;

    ss << assign->m_left->m_name;
    ss << " = ";
    assign->m_right->PrintOut(ss);
    m_assign.insert({ss.str(),assign});
}

void ReachingDefLattice::Rem(const string & name)
{
    auto it = m_assign.begin();
    while (it != m_assign.end())
    {
        if (it->second->m_left->m_name == name)
            it = m_assign.erase(it);
        else
            it++;
    }
}
