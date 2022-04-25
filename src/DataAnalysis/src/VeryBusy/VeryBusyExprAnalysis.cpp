#include "VeryBusyExprAnalysis.h"
VeryBusyExprAnalysis::VeryBusyExprAnalysis()
{
    
}

void VeryBusyExprAnalysis::PrevNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_succ)
        v.push_back(x);
}

void VeryBusyExprAnalysis::NextNodes(CFGNode * node, vector<CFGNode*> & v)
{
    for (auto & x : node->m_prev)
        v.push_back(x);
}

void VeryBusyExprAnalysis::TransferFun(CFGNode * node, VeryBusyExprAnalysis * lat)
{
    if (CFGAssign * n = dynamic_cast<CFGAssign*>(node))
    {
        lat->m_lat.Rem(n->m_left->m_name);
        lat->Eval(n->m_right);
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
    else if (dynamic_cast<CFGStart*>(node) || dynamic_cast<CFGEnd*>(node))
    {
        lat->m_lat.Top();
    }
}

void VeryBusyExprAnalysis::SaveToStr(CFGNode * node)
{
    string str = m_lat.GetName();
    node->m_analysis = str;
}

void VeryBusyExprAnalysis::Bot()
{
    m_lat.Bot();
}

void VeryBusyExprAnalysis::Lub(VeryBusyExprAnalysis * a)
{
    m_lat.Lub(a->m_lat);
}

bool VeryBusyExprAnalysis::operator==(const VeryBusyExprAnalysis& a) const
{
    return m_lat == a.m_lat;
}

void VeryBusyExprAnalysis::Eval(Expr * e)
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
