#include "AvailableExprLattice.h"
#include <sstream>

AvailableExprLattice::AvailableExprLattice()
{
    
}

void AvailableExprLattice::Lub(AvailableExprLattice & b)
{
    if (m_all){
        m_var = b.m_var;
        m_all = false;
        return;
    }
    if (b.m_all){
        return;
    }


    map<string,Expr*> res;
    for(auto it1 = m_var.begin(), end1 = m_var.end(),
        it2 = b.m_var.begin(), end2 = b.m_var.end();
        it1 != end1 && it2 != end2;)
    {
        if (it1->first == it2->first){
            res.insert({it1->first,it1->second});
            it1++;it2++;
        }
        else{
            if (it1->first < it2->first)
                it1++;
            else 
                it2++;
        }
    }
    m_all = false;
    m_var = move(res);
}

bool AvailableExprLattice::operator==(const AvailableExprLattice& a) const
{
    if (m_all && a.m_all)
        return true;
    if (m_var.size() != a.m_var.size())
        return false;

    for(auto it1 = m_var.begin(), end1 = m_var.end(),
        it2 = a.m_var.begin(), end2 = a.m_var.end();
        it1 != end1 || it2 != end2; it1++,it2++)
    {
        if (it1->first != it2->first)
            return false;
    }
    return true;
}

string AvailableExprLattice::GetName()
{
    string str = "{";
    for (auto & x : m_var)
    {
        str += x.first;
        str += ", ";
    }
    if (!m_var.empty()){
        str.pop_back();
        str.pop_back();
    }
    str += "}";
    return str;
}

void AvailableExprLattice::Top()
{
    m_var.clear();
    m_all = false;
}

void AvailableExprLattice::Bot()
{
    m_var.clear();
    m_all = true;
}

void AvailableExprLattice::Add(Expr * expr)
{
    if (m_all)
        return;
    std::stringstream ss;
    expr->PrintOut(ss);
    m_var.insert({ss.str(),expr});
}

void AvailableExprLattice::Rem(const string & name)
{
    if (m_all)
        return;

    auto it = m_var.begin();
    while (it != m_var.end())
    {
        if (FindName(name,it->second))
            it = m_var.erase(it);
        else
            it++;
    }
}

bool AvailableExprLattice::FindName(const string & name, Expr * expr)
{
    if (BinaryOp * opr = dynamic_cast<BinaryOp*>(expr))
    {
        if (FindName(name,opr->m_left))
            return true;
        if (FindName(name,opr->m_right))
            return true;
    }
    else if (Identifier * id = dynamic_cast<Identifier*>(expr))
    {
        return id->m_name == name;
    }
    return false;
}
