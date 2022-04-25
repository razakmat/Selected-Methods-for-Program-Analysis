#include "SignAnalysis.h"
#include <algorithm>

using namespace std;

void SignAnalysis::PrevNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_prev)
        v.push_back(x);
}

void SignAnalysis::NextNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_succ)
        v.push_back(x);
}

void SignAnalysis::TransferFun(CFGNode * node, SignAnalysis * lat)
{
    if (CFGStart * n = dynamic_cast<CFGStart*>(node))
    {
        for (auto & x : n->m_vars)
            lat->m_map.find(x->m_name)->second.Top();           
    }
    else if (CFGAssign * n = dynamic_cast<CFGAssign*>(node))
    {
        lat->m_map.find(n->m_left->m_name)->second = lat->Eval(n->m_right);
    }
    else if (CFGVar * n = dynamic_cast<CFGVar*>(node))
    {
        for (auto & x : n->m_vars)
        {
            lat->m_map.find(x->m_name)->second.Bot();
        }
    }
}

void SignAnalysis::SaveToStr(CFGNode * node)
{
    if (CFGStart * s = dynamic_cast<CFGStart*>(node))
    {
        for (auto & x :s->m_vars)
        m_names.push_back(x->m_name);
        sort(m_names.begin(),m_names.end());
    }
    else if (CFGVar * var = dynamic_cast<CFGVar*>(node))
    {
        for (auto & x :var->m_vars)
        m_names.push_back(x->m_name);
        sort(m_names.begin(),m_names.end());
    }

    string str = "{ ";
    for (auto & x : m_names)
    {
        auto it2 = m_map.find(x);
        str += it2->first;
        str += ": ";
        str += it2->second.GetName();
        if (x != m_names.back())
            str += ", ";
    }
    str += " }";

    node->m_analysis = move(str);
}

SignLattice SignAnalysis::Eval(Expr * e)
{
    SignLattice s;
    if (Number * n = dynamic_cast<Number*>(e))
    {
        return SignLattice::num(n->m_value);
    }
    else if (Identifier * n = dynamic_cast<Identifier*>(e))
    {
        return m_map.find(n->m_name)->second;
    }
    else if (BinaryOp * n = dynamic_cast<BinaryOp*>(e))
    {
        SignLattice left = Eval(n->m_left);
        SignLattice right = Eval(n->m_right);
        if (dynamic_cast<Plus*>(n->m_operator))
            return SignLattice::plus(left,right);
        else if (dynamic_cast<Minus*>(n->m_operator))
            return SignLattice::minus(left,right);
        else if (dynamic_cast<Times*>(n->m_operator))
            return SignLattice::times(left,right);
        else if (dynamic_cast<Divide*>(n->m_operator))
            return SignLattice::div(left,right);
        else if (dynamic_cast<Equal*>(n->m_operator))
            return SignLattice::eqq(left,right);
        else if (dynamic_cast<GreaterThan*>(n->m_operator))
            return SignLattice::gt(left,right);            
    }
    throw "Unsupported expr";
}

void SignAnalysis::Bot()
{
    for (auto & x : m_map)
        x.second.Bot();
}

void SignAnalysis::Lub(SignAnalysis * a)
{
    for(auto it1 = m_map.begin(), end1 = m_map.end(),
        it2 = a->m_map.begin(), end2 = a->m_map.end();
        it1 != end1 || it2 != end2; it1++,it2++)
    {
        it1->second.Lub(it1->second,it2->second);
    }
}

bool SignAnalysis::operator==(const SignAnalysis& a) const
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

SignAnalysis::SignAnalysis()
{
    for (auto & x : m_decl)
        m_map.insert({x,SignLattice()});
}
