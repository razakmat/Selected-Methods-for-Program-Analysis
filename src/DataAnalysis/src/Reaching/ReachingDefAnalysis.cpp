#include "ReachingDefAnalysis.h"

ReachingDefAnalysis::ReachingDefAnalysis()
{
    
}

void ReachingDefAnalysis::PrevNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_prev)
        v.push_back(x);
}

void ReachingDefAnalysis::NextNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_succ)
        v.push_back(x);
}

void ReachingDefAnalysis::TransferFun(CFGNode * node, ReachingDefAnalysis * lat)
{
    if (CFGAssign * n = dynamic_cast<CFGAssign*>(node))
    {
        lat->m_lat.Rem(n->m_left->m_name);
        lat->m_lat.Add(n);
    }
}

void ReachingDefAnalysis::SaveToStr(CFGNode * node)
{
    string str = m_lat.GetName();
    node->m_analysis = str;
}

void ReachingDefAnalysis::Bot()
{
    m_lat.Bot();
}

void ReachingDefAnalysis::Lub(ReachingDefAnalysis * a)
{
    m_lat.Lub(a->m_lat);
}

bool ReachingDefAnalysis::operator==(const ReachingDefAnalysis& a) const
{
    return m_lat == a.m_lat;
}