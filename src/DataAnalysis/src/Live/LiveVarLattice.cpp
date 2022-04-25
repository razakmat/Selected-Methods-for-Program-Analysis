#include "LiveVarLattice.h"


LiveVarLattice::LiveVarLattice()
{
    
}

void LiveVarLattice::Lub(LiveVarLattice & b)
{
    for (auto & x : b.m_var)
        m_var.insert(x);
}

bool LiveVarLattice::operator==(const LiveVarLattice& a) const
{
    return m_var == a.m_var;
}

string LiveVarLattice::GetName()
{
    string str = "{";
    for (auto & x : m_var)
    {
        str += x;
        str +=",";
    }
    if (!m_var.empty())
        str.pop_back();
    str += "}";
    return str;
}

void LiveVarLattice::Top()
{
}

void LiveVarLattice::Bot()
{
    m_var.clear();
}

void LiveVarLattice::Add(const string & name)
{
    m_var.insert(name);
}

void LiveVarLattice::Rem(const string & name)
{
    m_var.erase(name);
}