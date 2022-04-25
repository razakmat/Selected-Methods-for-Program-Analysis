#include "AvailableExprAnalysis.h"
AvailableExprAnalysis::AvailableExprAnalysis()
{
    
}

void AvailableExprAnalysis::PrevNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_prev)
        v.push_back(x);
}

void AvailableExprAnalysis::NextNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_succ)
        v.push_back(x);
}

void AvailableExprAnalysis::TransferFun(CFGNode * node, AvailableExprAnalysis * lat)
{
    if (CFGAssign * n = dynamic_cast<CFGAssign*>(node))
    {
        lat->Eval(n->m_right);
        lat->m_lat.Rem(n->m_left->m_name);
    }
    else if (CFGExpr * n = dynamic_cast<CFGExpr*>(node))
    {
        lat->Eval(n->m_right);
    }
    else if (CFGOutput * n = dynamic_cast<CFGOutput*>(node))
    {
        lat->Eval(n->m_right);
    }
    else if (CFGReturn * n = dynamic_cast<CFGReturn*>(node))
    {
        lat->Eval(n->m_right);
    }
    else if (dynamic_cast<CFGStart*>(node))
    {
        lat->m_lat.Top();
    }
}

void AvailableExprAnalysis::SaveToStr(CFGNode * node)
{
    string str = m_lat.GetName();
    node->m_analysis = str;
}

void AvailableExprAnalysis::Bot()
{
    m_lat.Bot();
}

void AvailableExprAnalysis::Lub(AvailableExprAnalysis * a)
{
    m_lat.Lub(a->m_lat);
}

bool AvailableExprAnalysis::operator==(const AvailableExprAnalysis& a) const
{
    return m_lat == a.m_lat;
}

void AvailableExprAnalysis::Eval(Expr * e)
{
    m_input = false;
    if (BinaryOp * opr = dynamic_cast<BinaryOp*>(e))
    {
        Eval(opr->m_left);
        bool input = m_input;
        Eval(opr->m_right);
        if (m_input || input)
            m_input = true;
        else
            m_lat.Add(opr);
    }
    else if(dynamic_cast<Input*>(e))
    {
        m_input = true;
    }
}
